#include "Button.h"
__weak void Btn_Pressing_Callback(Button *btn){}
__weak void Btn_Pressed_Short_Callback(Button *btn){}
__weak void Btn_Press_Timeout_Callback(Button *btn){}
__weak void Btn_Release_Callback(Button *btn){}
__weak void Btn_Press_Timeout_Repeat_Callback(Button *btn){};
__weak void	Btn_Release_Timeout_Callback(Button *btn){};
	
	
void Button_Handle(Button *btn)
{
	//--------------------xu li nhieu------------------------- 
	uint8_t sta = HAL_GPIO_ReadPin(btn->port,btn->pin);
	if(sta != btn->btn_filter)
	{
		btn->is_debouncing = 1;
		btn->btn_filter = sta;
		btn->time_debounce = HAL_GetTick();
	}
	//-----------------tin hieu xac lap------------------------
	if(btn->is_debouncing && (HAL_GetTick() - btn->time_debounce >= 15))
	{
		btn->btn_current = btn->btn_filter;
		//----------------xu li nhan nha---------------------
		if(btn->btn_current != btn->btn_last)
		{
			if(btn->btn_current == 0)//nhan phim
			{
				btn->long_press = 1;
				btn->flag_repeat = 1;
				btn->t_start_press = HAL_GetTick();
				btn->t_start_repeat = btn->t_start_press;
				Btn_Pressing_Callback(btn);
			}
			else //nha phim
			{
				if(HAL_GetTick() - btn->t_start_press <= 1000)
				{
					Btn_Pressed_Short_Callback(btn);
				}
				btn->long_press = 0;
				Btn_Release_Callback(btn);
				btn->flag_repeat = 0;
				btn->t_start_release = HAL_GetTick();
			}
			btn->btn_last = btn->btn_current;
		}
		btn->is_debouncing = 0;
	}
//-----------------------xu li nhan giu--------------------------------	
	if((btn->long_press)&& (HAL_GetTick() - btn->t_start_press >= 3000))
	{
		Btn_Press_Timeout_Callback(btn);
		btn->long_press = 0;
	}
//-----------------------xu li nhan giu goi lien tuc------------------- 
	if(( btn->flag_repeat) && HAL_GetTick() - btn->t_start_repeat >= 1000)
	{
		Btn_Press_Timeout_Repeat_Callback(btn); 
		btn->t_start_repeat = HAL_GetTick(); 
	}	
//-----------------------xu li nha phim goi lien tuc----------------------
 	if((btn->flag_repeat == 0)&& HAL_GetTick() - btn->t_start_release >= 2000)
	{
		Btn_Release_Timeout_Callback(btn);
		btn->t_start_release = HAL_GetTick();
	}
}
void Button_Init(Button *btn,GPIO_TypeDef *port,uint16_t pin)
{
	btn->btn_current = 1;
	btn->btn_last = 1;
	btn->btn_filter = 1;
	btn->time_debounce = 0;
	btn->is_debouncing = 0;
	btn->t_start_press = 0;
	btn->long_press = 0;
	btn->port = port;
	btn->pin = pin;
}

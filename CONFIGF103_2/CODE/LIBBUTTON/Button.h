#ifndef __BUTTON_H
#define __BUTTON_H

#include "main.h"
typedef struct 
{
	uint32_t time_debounce;
	uint32_t t_start_press;
	uint32_t t_start_repeat;
	uint32_t t_start_release;
	GPIO_TypeDef *port;
	uint16_t pin;
	uint8_t flag_repeat;
	uint8_t btn_current : 1;
	uint8_t btn_last : 1;
	uint8_t btn_filter : 1;
	uint8_t is_debouncing : 1;
	uint8_t long_press : 1;
}Button;

void Button_Handle(Button *btn);
void Button_Init(Button *btn,GPIO_TypeDef *port,uint16_t pin);

#endif

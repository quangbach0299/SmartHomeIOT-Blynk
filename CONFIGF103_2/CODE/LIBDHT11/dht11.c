#include "dht11.h"

#define DHT11_PIN_SET   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET)                                          
#define DHT11_PIN_RESET HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)                                          
#define DHT11_READ_IO   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)    

unsigned int ND = 0;
unsigned int DA = 0;
void DHT11(void)
{
    if(DHT11_READ_DATA() == 1)
    {
       //printf("11111\n");
    }
    else if(DHT11_READ_DATA() == 0)
    {
        // printf("2222\n");
    }
    else
    {
        //printf("33333\n");
    }
    HAL_Delay(10);                               
}


void DHT11_START(void)
{
    DHT11_GPIO_MODE_SET(0);                        

    DHT11_PIN_RESET;                                
    
    HAL_Delay(20);                                
    
    DHT11_GPIO_MODE_SET(1);  
	
}                                                  

unsigned char DHT11_READ_BIT(void)
{
    while(!DHT11_READ_IO);                          
    
    Tims_delay_us(40);                             
    
    if(DHT11_READ_IO)                              
    {
        while(DHT11_READ_IO);                      
        return 1;
    }   
    else                                           
    {
        return 0;
    }
}

unsigned char DHT11_READ_BYTE(void)
{
    uint8_t i,temp = 0;                            
    
    for(i=0; i<8 ;i++)
    {
        temp <<= 1;                                 
        if(DHT11_READ_BIT())                       
        {
            temp |= 1;                            
        }
    }
    return temp;
}

unsigned char DHT11_READ_DATA(void)
{
    uint8_t i;
    uint8_t data[5] = {0};
    
    DHT11_START();                                 
    
    if(DHT11_Check())                               
    {  
        while(!DHT11_READ_IO);                     
        while(DHT11_READ_IO);                      
        
        for(i=0; i<5; i++)
        {                        
            data[i] = DHT11_READ_BYTE();            
        }
        
        if(data[0] + data[1] + data[2] + data[3] == data[4])
        {
           
					printf("Humidity：%d.%d%%RH - Temperature：%d.%d°C\n",data[0],data[1],data[2],data[3]);
					ND = data[2] ;
					DA = data[0] ;
					//printf("ND:%d\n",ND);
					//printf("DA:%d\n",DA);
            return 1;                               
        }
        else
        {
            return 0;                             
        }
    }
    else                                          
    {
        return 2;
    }
}

unsigned char DHT11_Check(void)
{
    Tims_delay_us(40);
    if(DHT11_READ_IO == 0)                         
    {
        return 1;
    }
    else                                            
    {
        return 0;
    }
}


static void DHT11_GPIO_MODE_SET(uint8_t mode)
{
    if(mode)
    {
        
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = GPIO_PIN_8;                  
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;            
        GPIO_InitStruct.Pull = GPIO_PULLUP;                
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    else 
    {
        
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.Pin = GPIO_PIN_8;                
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;     
        GPIO_InitStructure.Pull = GPIO_PULLUP;             
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;    
        HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
    }
}


void Tims_delay_us(uint16_t nus)
{
	__HAL_TIM_SET_COUNTER(DLY_TIM_Handle, 0);
	__HAL_TIM_ENABLE(DLY_TIM_Handle);
	while (__HAL_TIM_GET_COUNTER(DLY_TIM_Handle) < nus)
	{
	}
	__HAL_TIM_DISABLE(DLY_TIM_Handle);
}


void Coarse_delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}

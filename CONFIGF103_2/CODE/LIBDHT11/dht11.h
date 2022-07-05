#ifndef __DHT11_H__
#define __DHT11_H__
#include "main.h"
#include "gpio.h"
#include "stdio.h"
#include "tim.h"
#include "stm32f1xx_hal.h"
#include "usart.h"


                                                      

#define DLY_TIM_Handle (&htim1)                                                                                     



void DHT11(void);
void DHT11_START(void);
unsigned char DHT11_READ_BIT(void);
unsigned char DHT11_READ_BYTE(void);
unsigned char DHT11_READ_DATA(void);
unsigned char DHT11_Check(void);
static void DHT11_GPIO_MODE_SET(uint8_t mode);
void Tims_delay_us(uint16_t nus);
void delay_us(uint16_t nus);
    
#endif

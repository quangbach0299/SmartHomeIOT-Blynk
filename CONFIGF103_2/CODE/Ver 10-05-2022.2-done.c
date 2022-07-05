#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "rc522.h"
#include "dht11.h"
#include "i2c-lcd.h"
#include "eeprom.h"
#include "Button.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

void SystemClock_Config(void);

//PRITN DEBUG=======================================
#ifdef __GNUC__
 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 
 
PUTCHAR_PROTOTYPE 
{

	 HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);
	//HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
	 return ch;
}
//==================================================
// RC522
extern uint8_t MFRC522_Check(uint8_t* id);
extern uint8_t MFRC522_Compare(uint8_t* CardID, uint8_t* CompareID);
extern void MFRC522_WriteRegister(uint8_t addr, uint8_t val);
extern uint8_t MFRC522_ReadRegister(uint8_t addr);
extern void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);
extern void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
extern uint8_t MFRC522_Request(uint8_t reqMode, uint8_t* TagType);
extern uint8_t MFRC522_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen);
extern uint8_t MFRC522_Anticoll(uint8_t* serNum);
extern void MFRC522_CalulateCRC(uint8_t* pIndata, uint8_t len, uint8_t* pOutData);
extern uint8_t MFRC522_SelectTag(uint8_t* serNum);
extern uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum);
extern uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t* recvData);
extern uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t* writeData);
extern void MFRC522_Init(void);
extern void MFRC522_Reset(void);
extern void MFRC522_AntennaOn(void);
extern void MFRC522_AntennaOff(void);
extern void MFRC522_Halt(void);

uint8_t str[MFRC522_MAX_LEN];

static uint8_t newCard[5] = {0xff,0xff,0xff,0xff,0xff};
static uint8_t rfid_entering;//check de vao mode ghi the duy nhat 1 lan
static uint32_t buttonPressCounter = 0;
static uint8_t EVENT;
uint8_t rfid_status;
uint8_t rfid_status;
uint8_t pressing_exit;
long last_rfid_start;

#define MODE_GHI_THE  1
#define MODE_XOA_THE  2
#define MODE_RESET    3
#define MODE_EXIT     4
Button btn_rfid;
uint16_t VirtAddVarTab[31] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30}; //key(n) ((n-1)*5)+0 ((n-1)*5)+1 ((n-1)*5)+2 ((n-1)*5)+3 ((n-1)*5)+4 
uint16_t new_id = 0;
	// 0 1 2 3 4 key1
	
	uint16_t key[5][5] = { 
		{0xFF,0xFF,0xFF,0xFF,0xFF},
		{0xFF,0xFF,0xFF,0xFF,0xFF},
		{0xFF,0xFF,0xFF,0xFF,0xFF},
		{0xFF,0xFF,0xFF,0xFF,0xFF} ,
		{0xFF,0xFF,0xFF,0xFF,0xFF} 	
	};
	
//===================================================
// DHT11
extern unsigned int ND ;
extern unsigned int DA;
//===================================================

//===================================================
// Biên trang thai 4 den , 4 quat
unsigned long last =  0;
unsigned int TT_DEN1 = 0;
unsigned int TT_DEN2 = 0;
unsigned int TT_DEN3 = 0;
unsigned int TT_DEN4 = 0;
unsigned int TT_QUAT1 = 0;
unsigned int TT_QUAT2 = 0;
unsigned int TT_QUAT3 = 0;
unsigned int TT_COI = 0; // GA
unsigned int TT_BUZZER = 0;
unsigned int TT_RFID = 0;
	
	
#define DEN1_SANG HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET)
#define DEN1_TAT HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET)

#define DEN2_SANG HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)
#define DEN2_TAT HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)

#define DEN3_SANG HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)
#define DEN3_TAT HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)

#define DEN4_SANG HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET)
#define DEN4_TAT HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)

#define QUAT1_CHAY HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)
#define QUAT1_TAT HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)


#define QUAT2_CHAY HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)
#define QUAT2_TAT HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

#define QUAT3_CHAY HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)
#define QUAT3_TAT HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)


#define COI_CHAY HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET)
#define COI_TAT HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET)

#define BUZZER_CHAY HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET)
#define BUZZER_TAT HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET)

//===================================================
// define 8 nút nhân và 1 cam bien chuyen dong
#define READ_NUT_DEN1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)
#define READ_NUT_DEN2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)
#define READ_NUT_DEN3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)
#define READ_NUT_DEN4 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)
#define READ_NUT_QUAT1 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)
#define READ_NUT_QUAT2 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)
#define READ_NUT_QUAT3 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)


#define READ_NUT_HONGNGOAI_CUA HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)
#define READ_NUT_HONGNGOAI_TOLET HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)
//===================================================
//MQ2

unsigned int Data_MQ2 = 0;
unsigned int Nguong_MQ2 = 100;
unsigned int DOOR = 0;
//===================================================
// servo
bool servo_start = false;
unsigned long lastServo = 0;
unsigned long i = 0;
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}	
 
//===================================================
//JSON
#define SizeJson 200
char JSON[200];
char Str_ND[200];
char Str_DA[200];
char Str_DEN1[200];
char Str_DEN2[200];
char Str_DEN3[200];
char Str_DEN4[200];
char Str_QUAT1[200];
char Str_QUAT2[200];
char Str_QUAT3[200];
char Str_COI[200];
char Str_BUZZER[200];





cJSON *str_json;
void SendDataJson(unsigned int ND , unsigned int DA , 
									unsigned int TT_DEN1 , unsigned int TT_DEN2 , 
									unsigned int TT_DEN3 , unsigned int TT_DEN4 , 
									unsigned int TT_QUAT1 , unsigned int TT_QUAT2 , 
									unsigned int TT_QUAT3,unsigned int TT_COI ,
									unsigned int TT_BUZZER);
//===================================================
// UART1 ESP8266
#define SizeBuf 200
char rx_buffer[SizeBuf];
uint8_t rx_data;
unsigned int rx_index = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Received(void);
void clearbufferEnd(void);
void XuLyJson(char *DataJson);
//===================================================
// LCD I2C
// LCD hang 1 0x00
// LCD hang 2 0x40									
void Display_Temp_Humidity(unsigned int Temp, unsigned int Humidity , unsigned int Data_MQ2);
void HienThiLCD(void);
//===================================================
// hàm function

void Read_RFID_RC522(void);
void Read_DHT11(void);
void SoSanh_ND(void);
void Read_DHT11(void);
void Read_Nut_Den1(void);
void Read_Nut_Den2(void);
void Read_Nut_Den3(void);
void Read_Nut_Den4(void);
void Read_Nut_Quat1(void);
void Read_Nut_Quat2(void);
void Read_Nut_Quat3(void);

void DieuKhienDen1(void);
void DieuKhienDen2(void);
void DieuKhienDen3(void);
void DieuKhienDen4(void);
void DieuKhienQuat1(void);
void DieuKhienQuat2(void);
void DieuKhienQuat3(void);

void Read_HongNgoai_Tolet(void);
void Read_HongNgoai_Door(void);
void Read_All_Button(void);
void Read_MQ2(void);
void Read_SenSor(void);
void SoSanh_MQ2(void);
void ServoStart(void);
void ServoStop(void);
void Start_Servo_RFID(void);

uint8_t MFRC522_CompareEEROM(uint8_t * CardID,uint16_t key_id);
void Write_Key(uint8_t* id_card);
void Delete_Key(void);
void Reset_All_Key(void);
 
void BuzzerControl(void);
void BuzzerSetLoop(uint32_t numberLoop);


//====================================================================================
//Dieu khien tieng buzzer
static uint32_t buzzerTime = 0;
static uint32_t buzzerON = 10;  // 200ms
static uint32_t buzzerOff = 300;  // 200ms
static uint32_t buzzerLoop = 0;  // 200ms


void BuzzerSetLoop(uint32_t numberLoop)
{
	 buzzerLoop = numberLoop;
}


void BuzzerControl(void)
{
	if(buzzerLoop >0)
	{
		buzzerTime++;
		if(buzzerTime < buzzerON)
		{
			BUZZER_CHAY;		
		}
		else if(buzzerTime > buzzerON && buzzerTime < (buzzerON + buzzerOff))
		{
			BUZZER_TAT;
		}
		else if(buzzerTime >(buzzerON + buzzerOff))	
		{
			buzzerLoop--;
			buzzerTime = 0;
		}		 
			else;
	}
}



 

 

	


int main(void)
{
  HAL_Init();

	
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
	
	DEN1_TAT;
	DEN2_TAT;
	DEN3_TAT;
	DEN4_TAT;
	QUAT1_TAT;
	QUAT2_TAT;
	QUAT3_TAT;
	COI_TAT;
	BUZZER_TAT;
	
	HAL_UART_Receive_IT(&huart1,&rx_data,1);
	HAL_Delay(100);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_Delay(100);
//	ServoStop();
//	HAL_Delay(1000);
	MFRC522_Init();
	HAL_Delay(1000);
	lcd_init();
	HAL_Delay(1000);
	lcd_send_string("SETTING STM32");
	HAL_Delay(1000);
	printf("Start STM32F103\n");
	HAL_Delay(2000);
	lcd_clear ();
	htim2.Instance -> CCR2 = map(50,0,180,600,2300);
	HAL_Delay(1000);
  BuzzerSetLoop(5);
	last = HAL_GetTick();
//-----------------------------------------------		
	HAL_FLASH_Unlock();
	EE_Init();
	for(uint8_t n = 1; n <6; n++ )
	{
		for(uint8_t j = 0; j <5; j++ )
		EE_ReadVariable((n-1)*5+j,&key[n-1][j]);
	}
	EE_ReadVariable(31,&new_id);  //ban dau se bang 0
	Button_Init(&btn_rfid,GPIOB,GPIO_PIN_4);
//-----------------------------------------------	
  while (1)
  {
		
 
		Button_Handle(&btn_rfid);
		Read_SenSor();
		Read_All_Button();
  	Read_RFID_RC522(); 
		Start_Servo_RFID();
		
  }
  
}
//====================================================================================
// hong ngoai toled
void Read_HongNgoai_Tolet(void)
{
	if(READ_NUT_HONGNGOAI_TOLET == 0)
	{
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_HONGNGOAI_TOLET == 1)
			{
				DieuKhienDen4();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
//===================================================================================
// hong ngoai cua
void Read_HongNgoai_Door(void)
{
	
	if(READ_NUT_HONGNGOAI_CUA == 0)
	{
		TT_BUZZER = 1;
		BUZZER_CHAY;
		HAL_Delay(200);
		BUZZER_TAT;
		HAL_Delay(200);
	}
}
//===================================================================================
// UART STM32 - ESP
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart -> Instance == USART1)
	{
		last = HAL_GetTick();
		Received();
		HAL_UART_Receive_IT(&huart1,&rx_data,1);
	}
}
void Received(void)
{
	//rx_buffer
	if(rx_data != '\n')
	{
		rx_buffer[rx_index++] = rx_data;
		
	}
	else
	{
		printf("Data Nhan duoc la: %s\n",rx_buffer);
		//printf("Data Nhan duoc ESP: %s\n",rx_buffer);
		XuLyJson(rx_buffer);
		clearbufferEnd();
	}
	
}
void clearbufferEnd(void)
{
	rx_index = 0;
	for(int i = 0 ; i < SizeBuf ; i++)
	{
		rx_buffer[i] = 0;
	}
	last = HAL_GetTick();
}
void XuLyJson(char *DataJson)
{
	str_json = cJSON_Parse(DataJson);
	if(!str_json)
	{
		printf("JSON ERROR!\r\n");
		return;
	}
	else
	{
		printf("JSON OK!\r\n");
		if(cJSON_GetObjectItem(str_json, "S"))
		{
			printf("SEND Data\n");
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
		}
		else if(cJSON_GetObjectItem(str_json, "D1"))
		{
			
			DieuKhienDen1();
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
			
		}
		else if(cJSON_GetObjectItem(str_json, "D2"))
		{
			DieuKhienDen2();
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
		}
		else if(cJSON_GetObjectItem(str_json, "D3"))
		{
			DieuKhienDen3();
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
		}
		else if(cJSON_GetObjectItem(str_json, "D4"))
		{
			DieuKhienDen4();
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
		}
		else if(cJSON_GetObjectItem(str_json, "Q1"))
		{
			DieuKhienQuat1();
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
		}
		else if(cJSON_GetObjectItem(str_json, "Q2"))
		{
			DieuKhienQuat2();
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
		}
		else if(cJSON_GetObjectItem(str_json, "Q3"))
		{
			DieuKhienQuat3();
			SendDataJson(ND , DA , 
									TT_DEN1 , TT_DEN2 , 
									TT_DEN3 , TT_DEN4 , 
									TT_QUAT1,TT_QUAT2 , 
									TT_QUAT3, TT_COI ,
									TT_BUZZER);
			if(TT_BUZZER == 1)
			{
				TT_BUZZER = 0;
			}
			last = HAL_GetTick();
		}	
		else if(cJSON_GetObjectItem(str_json, "MQ2"))
		{
			Nguong_MQ2 = atoi(cJSON_GetObjectItem(str_json, "MQ2")->valuestring);
			printf("Nguong_MQ2 thay doi la: %d\r\n",Nguong_MQ2);
		}
		
		else if(cJSON_GetObjectItem(str_json, "DOOR"))
		{
			DOOR = atoi(cJSON_GetObjectItem(str_json, "DOOR")->valuestring);
			printf("DOOR: %d\r\n",DOOR);
			if(DOOR == 1)
			{
				
			}
			
			
			
//			 if(strstr(cJSON_GetObjectItem(str_json,"COI")->valuestring,"0") != NULL)  
//			{
//				uint8_t off[] = "Coi off";
//				HAL_UART_Transmit(&huart2,(uint8_t *)off,sizeof(off),100);
//				servo_close();
//	  		TT_COI = 0;
//			}
//			else if(strstr(cJSON_GetObjectItem(str_json,"COI")->valuestring,"1") != NULL) //bien dang o dang string
//			{
// 			  uint8_t on[] = "Coi On";
// 				HAL_UART_Transmit(&huart2,(uint8_t *)on,sizeof(on),100);
//				servo_open();
//				TT_COI = 1;
//		 
//			}
		}
	
		last = HAL_GetTick();
		cJSON_Delete(str_json);
	}
}
//===================================================================================
// SEND JSON
void SendDataJson(unsigned int ND , unsigned int DA , 
									unsigned int TT_DEN1 , unsigned int TT_DEN2 , 
									unsigned int TT_DEN3 , unsigned int TT_DEN4 , 
									unsigned int TT_QUAT1 , unsigned int TT_QUAT2 , 
									unsigned int TT_QUAT3,unsigned int TT_COI  ,
									unsigned int TT_BUZZER)
{
	
	for(int i = 0 ; i < 200 ; i++)
	{
		
		JSON[i] = 0;
		Str_ND[i] = 0;
		Str_DA[i] = 0;
		Str_DEN1[i] = 0;
		Str_DEN2[i] = 0;
		Str_DEN3[i] = 0;
		Str_DEN4[i] = 0;
		Str_QUAT1[i] = 0;
		Str_QUAT2[i] = 0;
		Str_QUAT3[i] = 0;
		Str_COI[i]= 0;
		Str_BUZZER[i] = 0;
	}
	// chuyen du lieu truyen vào ve char
	sprintf(Str_ND, "%d", ND);
	sprintf(Str_DA, "%d", DA);
	sprintf(Str_DEN1, "%d", TT_DEN1);
	sprintf(Str_DEN2, "%d", TT_DEN2);
	sprintf(Str_DEN3, "%d", TT_DEN3);
	sprintf(Str_DEN4, "%d", TT_DEN4);
	sprintf(Str_QUAT1, "%d", TT_QUAT1);
	sprintf(Str_QUAT2, "%d", TT_QUAT2);
	sprintf(Str_QUAT3, "%d", TT_QUAT3);
	sprintf(Str_COI, "%d", TT_COI);
	sprintf(Str_BUZZER, "%d", TT_BUZZER);
	//{"ND":"123","DA":"456"}
	
	
	strcat(JSON,"{\"ND\":\"");
	strcat(JSON,Str_ND);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"DA\":\"");
	strcat(JSON,Str_DA);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"D1\":\"");
	strcat(JSON,Str_DEN1);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"D2\":\"");
	strcat(JSON,Str_DEN2);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"D3\":\"");
	strcat(JSON,Str_DEN3);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"D4\":\"");
	strcat(JSON,Str_DEN4);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"Q1\":\"");
	strcat(JSON,Str_QUAT1);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"Q2\":\"");
	strcat(JSON,Str_QUAT2);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"Q3\":\"");
	strcat(JSON,Str_QUAT3);
	strcat(JSON,"\",");
	
	strcat(JSON,"\"COI\":\"");
	strcat(JSON,Str_COI);
	strcat(JSON,"\",");
  
	strcat(JSON,"\"BUZ\":\"");
	strcat(JSON,Str_BUZZER);
	strcat(JSON,"\"}\n");
	
	printf("%s",JSON); //  gui du lieu ve PC
	//printf("SEND ESP OK");
	HAL_UART_Transmit(&huart1,(uint8_t *)&JSON,strlen(JSON),1000);
	//HAL_UART_Transmit(&huart1,(uint8_t *)&"{\"STM32\":\"OK\"}",strlen("{\"STM32\":\"OK\"}"),1000);
	
}
//===================================================================================
void Read_All_Button(void)
{
	Read_Nut_Den1();
	Read_Nut_Den2();
	Read_Nut_Den3();
	Read_Nut_Den4();
	Read_Nut_Quat1();
	Read_Nut_Quat2();
	Read_Nut_Quat3();
	Read_HongNgoai_Tolet();
	Read_HongNgoai_Door();
	
}
//===================================================================================
// function doc nut nhan 
void Read_Nut_Den1(void)
{
	if(READ_NUT_DEN1 == 1)
	{
		printf("Nut 1 dc nhan!\n");
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_DEN1 == 0)
			{
				DieuKhienDen1();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
void Read_Nut_Den2(void)
{
	if(READ_NUT_DEN2 == 1)
	{
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_DEN2 == 0)
			{
				DieuKhienDen2();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
void Read_Nut_Den3(void)
{
	if(READ_NUT_DEN3 == 1)
	{
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_DEN3 == 0)
			{
				DieuKhienDen3();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
void Read_Nut_Den4(void)
{
	if(READ_NUT_DEN4 == 1)
	{
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_DEN4 == 0)
			{
				DieuKhienDen4();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
void Read_Nut_Quat1(void)
{
	if(READ_NUT_QUAT1 == 1)
	{
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_QUAT1 == 0)
			{
				DieuKhienQuat1();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
void Read_Nut_Quat2(void)
{
	if(READ_NUT_QUAT2 == 1)
	{
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_QUAT2 == 0)
			{
				DieuKhienQuat2();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
void Read_Nut_Quat3(void)
{
	if(READ_NUT_QUAT3 == 1)
	{
		HAL_Delay(200);
		while(1)
		{
			if(READ_NUT_QUAT3 == 0)
			{
				DieuKhienQuat3();
				HAL_Delay(200);
				last = HAL_GetTick();
				break;
			}
		}
	}
}
//=======================================================================
// function dieu khien den va quat
void DieuKhienDen1(void)
{
	if(TT_DEN1 == 0)
	{
		printf("DEN 1 ON\n");
		DEN1_SANG;
		TT_DEN1 = 1;
	}
	else if(TT_DEN1 == 1)
	{
		printf("DEN 1 OFF\n");
		DEN1_TAT;
		TT_DEN1 = 0;
	}
}
void DieuKhienDen2(void)
{
	if(TT_DEN2 == 0)
	{
		printf("DEN 2 ON\n");
		DEN2_SANG;
		TT_DEN2 = 1;
	}
	else if(TT_DEN2 == 1)
	{
		printf("DEN 2 OFF\n");
		DEN2_TAT;
		TT_DEN2 = 0;
	}
}
void DieuKhienDen3(void)
{
	if(TT_DEN3 == 0)
	{
		printf("DEN 3 ON\n");
		DEN3_SANG;
		TT_DEN3 = 1;
	}
	else if(TT_DEN3 == 1)
	{
		printf("DEN 3 OFF\n");
		DEN3_TAT;
		TT_DEN3 = 0;
	}
}
void DieuKhienDen4(void)
{
	if(TT_DEN4 == 0)
	{
		printf("DEN 4 ON\n");
		DEN4_SANG;
		TT_DEN4 = 1;
	}
	else if(TT_DEN4 == 1)
	{
		printf("DEN 4 OFF\n");
		DEN4_TAT;
		TT_DEN4 = 0;
	}
}
void DieuKhienQuat1(void)
{
	if(TT_QUAT1 == 0)
	{
		printf("QUAT 1 ON\n");
		QUAT1_CHAY;
		TT_QUAT1 = 1;
	}
	else if(TT_QUAT1 == 1)
	{
		printf("QUAT 1 OFF\n");
		QUAT1_TAT;
		TT_QUAT1 = 0;
	}
}
void DieuKhienQuat2(void)
{
	if(TT_QUAT2 == 0)
	{
		printf("QUAT 2 ON\n");
		QUAT2_CHAY;
		TT_QUAT2 = 1;
	}
	else if(TT_QUAT2 == 1)
	{
		printf("QUAT 2 OFF\n");
		QUAT2_TAT;
		TT_QUAT2 = 0;
	}
}
void DieuKhienQuat3(void)
{
	if(TT_QUAT3 == 0)
	{
		printf("QUAT 3 ON\n");
		QUAT3_CHAY;
		TT_QUAT3 = 1;
	}
	else if(TT_QUAT3 == 1)
	{
		printf("QUAT 3 OFF\n");
		QUAT3_TAT;
		TT_QUAT3 = 0;
	}
}
//===================================================================================
//function kiem tra cam bien
void Read_SenSor(void)
{
	if(HAL_GetTick() - last >= 1500)
		{
			DHT11();
			printf("ND:%d\n",ND);
			printf("DA:%d\n",DA);
			Read_MQ2();
			SoSanh_MQ2();
			HienThiLCD();
			printf("door:%d\n",READ_NUT_HONGNGOAI_CUA);
			printf("toilet:%d\n",READ_NUT_HONGNGOAI_TOLET);
			printf("=======================\n");

			last = HAL_GetTick();
		}
}
void SoSanh_MQ2(void)
{
	if(Data_MQ2 >= Nguong_MQ2)
	{
		printf("Coi Bao Start!!!\n");
		TT_COI = 1;
		COI_CHAY;
	}
	else
	{
		printf("Coi Bao Stop!!!\n");
		TT_COI = 0;
		COI_TAT;
	}
}
void Read_MQ2(void)
{
	if(HAL_ADC_Start(&hadc1) == HAL_OK)
		{
			if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
			{
				Data_MQ2 = 0;
				Data_MQ2 = HAL_ADC_GetValue(&hadc1);
				HAL_ADC_Stop(&hadc1);
				printf("ADC1-MQ2-PA0:%d\n",Data_MQ2);
			}
		}	
}
void HienThiLCD(void)
{
	lcd_clear();
	Display_Temp_Humidity(ND, DA,Data_MQ2);
}

void Display_Temp_Humidity(unsigned int Temp, unsigned int Humidity , unsigned int Data_MQ2)
{
	if(EVENT == MODE_GHI_THE   )
	{ 
	lcd_send_cmd (0x80|0x00);
	lcd_send_string("MODE GHI THE");	
	lcd_send_cmd (0x80|0x09);	
	 	
	}
	else if(EVENT == MODE_XOA_THE)
	{
	 lcd_send_cmd (0x80|0x00);
   lcd_send_string("MODE XOA THE");	
	 
		}
	else 
	{
	lcd_send_cmd (0x80|0x00);
	lcd_send_string("SMART HOME");
	}	
	
	lcd_send_cmd (0x80|0x40);
	
	char str[20] = {0};
	sprintf (str, "N:%d",Temp);
	lcd_send_string(str);
	
	char str1[20] = {0};
	sprintf (str1, " D:%d",Humidity);
	lcd_send_string(str1);
	
	char str2[20] = {0};
	sprintf (str2, " G:%d",Data_MQ2);
	lcd_send_string(str2);
	

}
//========================================================================================
//function dieu khien RFID
void ServoStart(void)
{
	htim2.Instance -> CCR2 = map(180,0,180,600,2200);
}
void Start_Servo_RFID(void)
{
	//lastServo servo_start
	
	if(servo_start)
	{
		if(HAL_GetTick() - lastServo >= 5000)
		{
			i = 0;
			printf("%ld.Servo Stop\n",i);
			ServoStop();
			servo_start = false;
			lastServo = HAL_GetTick();
		}
	}
}

void Read_RFID_RC522(void)
{
	//khi vao mode ma ko thao tac thi tu exit
	 
	if(HAL_GetTick() - last_rfid_start >= 15000 && rfid_entering == 0 && EVENT == MODE_GHI_THE)
	{
			EVENT = MODE_EXIT;
		  BuzzerSetLoop(1);

	}
	else if
		(HAL_GetTick() - last_rfid_start >= 15000 && rfid_entering == 0 && EVENT == MODE_XOA_THE)
	{
		 EVENT = MODE_EXIT;
		 BuzzerSetLoop(1);
	}
		
	uint8_t static match = 0;
 	
	if(!MFRC522_Request(PICC_REQIDL,str))
	 {
		if(!MFRC522_Anticoll(str))
		{
				
			last = HAL_GetTick();
		 
			for (uint8_t i = 0; i< 5; i++)
			{
				if(newCard[i] != str[i])
					match++;
			}		
			if(match > 0)
			{
				printf("lan quet moi \n\r");
				match = 0;
				rfid_entering = 1;
			}
			else
				{
					printf("the dang duoc giu tai cua \n\r");
			  	match = 0;
				}
			 	for (uint8_t i = 0; i< 5; i++)
			{
				newCard[i]= str[i];
			}	

		
			if(rfid_entering == 1)
			{
				if(EVENT == MODE_GHI_THE)
				{
					 Write_Key(str);
					 pressing_exit = 1;	 
				}
				else if ( EVENT == MODE_XOA_THE)
				{
					 Delete_Key();	
					 pressing_exit = 1;	 	
				}
				 rfid_entering = 0;
			 }
					 
			 printf("MaThe:%02x-%02x-%02x-%02x-%02x\r\n", str[0], str[1], str[2], str[3], str[4]);
					
				 
			 for(uint8_t n = 1 ; n < 6; n++)
			 {
				 if (MFRC522_CompareEEROM(str,n) == MI_OK) 
					{
						  i++;
							
							//printf("[%s voi Card ID=%02x-%02x-%02x-%02x-%02x] \r\n", dataUDP_permit, CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);	
							if(i == 1)
							{
								printf("The %d OK!\n\r",n);
								printf("Servo Start\n");
								
								servo_start = true;
								ServoStart();//cho phep i ve 0 sau 5s
							}			
							lastServo = HAL_GetTick();
					} 
			 }
		 }
	 }
}
void ServoStop(void)
{
	//htim2.Instance -> CCR2 = 50;
	htim2.Instance -> CCR2 = map(50,0,180,600,2300);
}
//==========================================================================================
//function ghi xoa the RFID
 
//Ham so sanh FLash va Data the quet vao
uint8_t MFRC522_CompareEEROM(uint8_t * CardID,uint16_t key_id)
{
	uint8_t i;
	uint8_t CompareID[5];

    for(uint8_t i = 0; i < 5; i++ )
    {
    	CompareID[i] = (uint8_t)key[key_id-1][i];
    }

	for (i = 0; i < 5; i++) {
		if (CardID[i] != CompareID[i]) 
			return MI_ERR;
	}
	return MI_OK;
	
};

 
 


void Write_Key(uint8_t* id_card)
{
	new_id++;
	if(new_id > 5)
	{
		new_id = 1;
	}
	
 
	for(uint8_t k = 0; k<5; k ++)
	{
		EE_WriteVariable(((new_id-1)*5)+k,id_card[k]);
		EE_ReadVariable(((new_id-1)*5)+k,&key[new_id-1][k]);
	}
		 
		 
	 
	
	EE_WriteVariable(31,new_id);
	
	lcd_send_cmd (0x80|0x00);
	char str[20] = {0};
	sprintf (str, "ID luu la %d ",new_id);
	lcd_send_string(str);
	 
	printf("Id dc luu vao flash %d la : %02x-%02x-%02x-%02x-%02x ",new_id,key[new_id-1][0],key[new_id-1][1],key[new_id-1][2], key[new_id-1][3],key[new_id-1][4]);
	pressing_exit = 1;
	last_rfid_start = HAL_GetTick();
 
}

void Delete_Key(void)
{
	for(uint8_t n = 1; n < 6; n++)
	{
	if( MFRC522_CompareEEROM(str,n) == MI_OK)
	{
		for (uint8_t j = 0; j <5; j++) 
		{
		EE_WriteVariable((n-1)*5+j,0xff);
		EE_ReadVariable((n-1)*5+j,&key[n-1][j]);
		}
		printf("The duoc xoa la the so : %d \r\n",n); 
		
		lcd_send_cmd (0x80|0x00);
		char str[20] = {0};
		sprintf (str, "ID xoa la %d ",new_id);
		lcd_send_string(str);
		
	}
	}
	last_rfid_start = HAL_GetTick();
}

void Reset_All_Key(void)
{
	for(uint8_t n = 1; n <6; n++ )
     {
     	 for(uint8_t j = 0; j <5; j++ )
			 {
     	 EE_WriteVariable((n-1)*5+j,0xff);
			 EE_ReadVariable((n-1)*5+j,&key[n-1][j]);
			 } 
		printf("Da reset bo nho");
			 
		lcd_send_cmd (0x80|0x00);	 
		lcd_send_string("FACTORY RESET....");
		HAL_Delay(1000);
	  new_id = 0;
		EVENT = MODE_EXIT;
		 } 
}
	
	
	

void Btn_Pressing_Callback(Button *btn)
{
 		if(EVENT == MODE_GHI_THE )//&& pressing_exit == 1)//do la thu vien minh viet no nhay vo check cai pressing truoc nen ko can pressing exit
		{
			EVENT = MODE_EXIT;
			printf("Mode EXIT \n\r");
			pressing_exit = 0;
			BuzzerSetLoop(1);
			
			lcd_send_cmd (0x80|0x00);
			lcd_send_string("EXITING.....");
			HAL_Delay(1000);
			
		}
		else if(EVENT == MODE_XOA_THE )//&& pressing_exit == 1)
		{
			EVENT = MODE_EXIT;
			printf("Mode EXIT \n\r");
			pressing_exit = 0;
			BuzzerSetLoop(1);
			
			lcd_send_cmd (0x80|0x00);
			lcd_send_string("EXITING.....");
			HAL_Delay(1000);
		}
} 

void Btn_Press_Timeout_Repeat_Callback(Button *btn)
{
		printf("Nut dc nhan \n\r");
		buttonPressCounter++;
		if(buttonPressCounter == 2 )
		{
			printf(" Mode Them the moi \n\r");
			BuzzerSetLoop(2);
			EVENT = MODE_GHI_THE;
			
 
			
			last_rfid_start = HAL_GetTick();
			// them ther //
		}
		 if(buttonPressCounter == 5)
		{
			BuzzerSetLoop(4);
			printf("Mode Xoa the \n\r");
			// xoa the //
			EVENT = MODE_XOA_THE;

 
			
			last_rfid_start = HAL_GetTick();
		}	
		if(buttonPressCounter == 10)
		{
			BuzzerSetLoop(10);
			printf(" Mode RESET the \n\r");
			
			Reset_All_Key();
			EVENT = MODE_EXIT;
			 
		}	
}

void Btn_Release_Timeout_Callback(Button *btn)
{
			printf("Nut tha\n\r");
			buttonPressCounter = 0;
			
			if(EVENT ==  MODE_GHI_THE )
			{
				//BuzzerSetLoop(2);
			 	printf("Mode them the moi \n\r");
				 
			} 
			if(EVENT == MODE_XOA_THE)
			{
				//BuzzerSetLoop(3);
				printf("Mode xoa the \n\r");
			}
			if(EVENT == MODE_EXIT)
			{
				//BuzzerSetLoop(3);
				printf("Mode Exit \n\r");
				 
			}
}























//==================================================




void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#ifndef _DHT11_H
#define _DHT11_H

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"	
/*
使用此DHT11驱动时：默认DHT11的数据线连接在单片机的PB1引脚，
                  如需改动则只需改动
									                  DHT11_PIN 与 DHT11_DAT

使用时还需在主函数中使能端口，只需添加以下代码：
                                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx,ENABLE);
																		
使用方式例程：
						uint8_t t=0,t1=0,h=0,h1=0;
            DHT11_Data_TypeDef My_DHT11;
            t = My_DHT11.temp_int;
            t1 = My_DHT11.temp_deci;
            h = My_DHT11.humi_int;
            h1 = My_DHT11.humi_deci;
            DHT11_ReadData(&My_DHT11);
						
						
@by fury
*/
#define   DHT11_PIN     GPIO_Pin_1
#define   DHT11_DAT     GPIOB

#define   DHT11_IN      GPIO_ReadInputDataBit(DHT11_DAT, DHT11_PIN)   //读取PB1总线上的电平
#define   DHT11_OUT_0   GPIO_ResetBits(DHT11_DAT, DHT11_PIN)          //将PB1总线拉低
#define   DHT11_OUT_1   GPIO_SetBits(DHT11_DAT, DHT11_PIN)            //将PB1总线拉高

typedef struct
{
	uint8_t humi_int;				    // 湿度的整数部分
	uint8_t humi_deci;	 		    // 湿度的小数部分
	uint8_t temp_int;	 					// 温度的整数部分
	uint8_t temp_deci;	 		    // 温度的小数部分
	uint8_t check_sum;	 		    // 校验和

} DHT11_Data_TypeDef;

uint8_t DHT11_ReadData(DHT11_Data_TypeDef* DHT11_Data);






#endif


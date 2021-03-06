#ifndef _DHT11_H
#define _DHT11_H

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"	
/*
ʹ�ô�DHT11����ʱ��Ĭ��DHT11�������������ڵ�Ƭ����PB1���ţ�
                  ����Ķ���ֻ��Ķ�
									                  DHT11_PIN �� DHT11_DAT

ʹ��ʱ��������������ʹ�ܶ˿ڣ�ֻ��������´��룺
                                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx,ENABLE);
																		
ʹ�÷�ʽ���̣�
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

#define   DHT11_IN      GPIO_ReadInputDataBit(DHT11_DAT, DHT11_PIN)   //��ȡPB1�����ϵĵ�ƽ
#define   DHT11_OUT_0   GPIO_ResetBits(DHT11_DAT, DHT11_PIN)          //��PB1��������
#define   DHT11_OUT_1   GPIO_SetBits(DHT11_DAT, DHT11_PIN)            //��PB1��������

typedef struct
{
	uint8_t humi_int;				    // ʪ�ȵ���������
	uint8_t humi_deci;	 		    // ʪ�ȵ�С������
	uint8_t temp_int;	 					// �¶ȵ���������
	uint8_t temp_deci;	 		    // �¶ȵ�С������
	uint8_t check_sum;	 		    // У���

} DHT11_Data_TypeDef;

uint8_t DHT11_ReadData(DHT11_Data_TypeDef* DHT11_Data);






#endif


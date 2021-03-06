#include "DHT11.h"
 
			/*DHT11ͨ��˵��
			���߿���Ϊ�ߵ�ƽ��
			�������� ��ʼ�ź� ������������������18ms��Ȼ�����߻ָ����У�����Ϊ�ߵ�ƽ������ʱ�ȴ�20~40us��
			�����л�Ϊ����״̬����ȡDHT11������ ��Ӧ�ź� ����DHT11����������40~80us����
			Ȼ���ж�DHT11�Ƿ񷢳�80us�ĸߵ�ƽ�ź�����ǾͿɲɼ����ݡ�
			
			����DHT11�������0��ʱ��������ȡ��50us�͵�ƽ��֮��26~28us�ߵ�ƽ��
			����DHT11�������1��ʱ��������ȡ��50us�͵�ƽ��֮��70us�ĸߵ�ƽ��
			*/
			
			
//�趨DHT11���Ϊ������
static void DHT11_Mode_OUT_PP(void)
{
	GPIO_InitTypeDef My_GPIO0;

	My_GPIO0.GPIO_Pin = DHT11_PIN;
  My_GPIO0.GPIO_Mode = GPIO_Mode_Out_PP;
  My_GPIO0.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(DHT11_DAT, &My_GPIO0);
}

//�趨DHT11���Ϊ�������
static void DHT11_Mode_IN_NP(void)
{
	GPIO_InitTypeDef My_GPIO1;

	My_GPIO1.GPIO_Pin = DHT11_PIN;
  My_GPIO1.GPIO_Mode = GPIO_Mode_IPU;
  My_GPIO1.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(DHT11_DAT, &My_GPIO1);
}

//��ȡһ���ֽ�
uint8_t DHT11_ReadByte(void)
{
	uint8_t i, temp = 0;

	for (i = 0; i < 8; i++)
	{
		while (DHT11_IN == 0);		// �ȴ��͵�ƽ����
		
		delay_us(40);					//	��ʱ 40 ΢��		�͵�ƽΪ 0 ���ߵ�ƽΪ 1

		if (DHT11_IN == 1)
		{
			while (DHT11_IN == 1);	// �ȴ��ߵ�ƽ����

			temp |= (uint8_t)(0X01 << (7 - i));			// �ȷ��͸�λ MSB
		}
		else
		{
			temp &= (uint8_t)~(0X01 << (7 - i));
		}
	}
	return temp;
}

//��ȡһ������
uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_OUT_PP();		// �����������������
	DHT11_OUT_0;
	delay_ms(18);					// ��ʱ 18 ms
	DHT11_OUT_1;						// �������ߣ���ʱ 30 us
	delay_us(30);

	DHT11_Mode_IN_NP();			// �������룬��ȡ DHT11 ����
	if (DHT11_IN == 0)			// �յ��ӻ�Ӧ��
	{
		while (DHT11_IN == 0);		// �ȴ��ӻ�Ӧ��ĵ͵�ƽ����
		while (DHT11_IN == 1);		// �ȴ��ӻ�Ӧ��ĸߵ�ƽ����

		/*��ʼ��������*/
		DHT11_Data->humi_int  = DHT11_ReadByte();
		DHT11_Data->humi_deci = DHT11_ReadByte();
		DHT11_Data->temp_int  = DHT11_ReadByte();
		DHT11_Data->temp_deci = DHT11_ReadByte();
		DHT11_Data->check_sum = DHT11_ReadByte();

		DHT11_Mode_OUT_PP();		// ��ȡ��������������
		DHT11_OUT_1;

		// ����У��
		if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else		// δ�յ��ӻ�Ӧ��
	{
		return 0;
	}
}
 


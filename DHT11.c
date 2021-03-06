#include "DHT11.h"
 
			/*DHT11通信说明
			总线空闲为高电平。
			主机发送 开始信号 （即将总线拉低至少18ms）然后将总线恢复空闲（即拉为高电平），延时等待20~40us，
			主机切换为输入状态，读取DHT11发出的 响应信号 （即DHT11将总线拉低40~80us），
			然后判断DHT11是否发出80us的高电平信号如果是就可采集数据。
			
			代表DHT11输出数字0的时序：主机读取到50us低电平，之后26~28us高电平。
			代表DHT11输出数字1的时序：主机读取到50us低电平，之后70us的高电平。
			*/
			
			
//设定DHT11针脚为输出针脚
static void DHT11_Mode_OUT_PP(void)
{
	GPIO_InitTypeDef My_GPIO0;

	My_GPIO0.GPIO_Pin = DHT11_PIN;
  My_GPIO0.GPIO_Mode = GPIO_Mode_Out_PP;
  My_GPIO0.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(DHT11_DAT, &My_GPIO0);
}

//设定DHT11针脚为输入针脚
static void DHT11_Mode_IN_NP(void)
{
	GPIO_InitTypeDef My_GPIO1;

	My_GPIO1.GPIO_Pin = DHT11_PIN;
  My_GPIO1.GPIO_Mode = GPIO_Mode_IPU;
  My_GPIO1.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(DHT11_DAT, &My_GPIO1);
}

//读取一个字节
uint8_t DHT11_ReadByte(void)
{
	uint8_t i, temp = 0;

	for (i = 0; i < 8; i++)
	{
		while (DHT11_IN == 0);		// 等待低电平结束
		
		delay_us(40);					//	延时 40 微秒		低电平为 0 ，高电平为 1

		if (DHT11_IN == 1)
		{
			while (DHT11_IN == 1);	// 等待高电平结束

			temp |= (uint8_t)(0X01 << (7 - i));			// 先发送高位 MSB
		}
		else
		{
			temp &= (uint8_t)~(0X01 << (7 - i));
		}
	}
	return temp;
}

//读取一次数据
uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_OUT_PP();		// 主机输出，主机拉低
	DHT11_OUT_0;
	delay_ms(18);					// 延时 18 ms
	DHT11_OUT_1;						// 主机拉高，延时 30 us
	delay_us(30);

	DHT11_Mode_IN_NP();			// 主机输入，获取 DHT11 数据
	if (DHT11_IN == 0)			// 收到从机应答
	{
		while (DHT11_IN == 0);		// 等待从机应答的低电平结束
		while (DHT11_IN == 1);		// 等待从机应答的高电平结束

		/*开始接收数据*/
		DHT11_Data->humi_int  = DHT11_ReadByte();
		DHT11_Data->humi_deci = DHT11_ReadByte();
		DHT11_Data->temp_int  = DHT11_ReadByte();
		DHT11_Data->temp_deci = DHT11_ReadByte();
		DHT11_Data->check_sum = DHT11_ReadByte();

		DHT11_Mode_OUT_PP();		// 读取结束，主机拉高
		DHT11_OUT_1;

		// 数据校验
		if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else		// 未收到从机应答
	{
		return 0;
	}
}
 

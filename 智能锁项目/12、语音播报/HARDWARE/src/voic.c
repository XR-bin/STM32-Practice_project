#include "sys.h"
#include "voic.h"
#include "delay.h"

/*****************************************************
*函数功能  ：对voic语音模块对应的GPIO口进行初始化设置
*函数名    ：Voic_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            VO_BUSY     PB1   //输入
*            VO_DATA     PB0   //输出
********************************************************/
void Voic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx配置结构体
	
	//时钟使能   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //使能PB端口时钟
	
	//GPIO配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;				               //PB0 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //根据设定参数初始化PB0
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				               //PB1 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		       //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //根据设定参数初始化PB0
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

/*****************************************************
*函数功能  ：向voic语音模块发送数据位1
*函数名    ：Voic_SendHight
*函数参数  ：void
*函数返回值：void
*描述      ：
*            VO_BUSY     PB1   //输入
*            VO_DATA     PB0   //输出
********************************************************/
void Voic_SendHight(void)
{
	VO_DATA_H;
	delay_us(1500);
	VO_DATA_L;
	delay_us(500);
}

/*****************************************************
*函数功能  ：向voic语音模块发送数据位0
*函数名    ：Voic_SendLow
*函数参数  ：void
*函数返回值：void
*描述      ：
*            VO_BUSY     PB1   //输入
*            VO_DATA     PB0   //输出
********************************************************/
void Voic_SendLow(void)
{
	VO_DATA_H;
	delay_us(500);
	VO_DATA_L;
	delay_us(1500);
}

/*****************************************************
*函数功能  ：向voic语音模块发送数据
*函数名    ：Voic_SendData
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
*            VO_BUSY     PB1   //输入
*            VO_DATA     PB0   //输出
********************************************************/
void Voic_SendData(u8 data)
{
	u8 i;
	
	VO_DATA_H;
	delay_ms(8);
	VO_DATA_L;
	delay_ms(1);
	
	for(i=0;i<8;i++)
	{
		if(data & 0x80) Voic_SendHight();
		else  Voic_SendLow();
		
		data<<=1;
	}
}





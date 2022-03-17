//#include "sys.h"
#include "stm32f10x.h" 



/*****************************************************
*函数功能  ：对LED对应的GPIO口进行初始化设置
*函数名    ：LED_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            LED     PA1
*            高电平灭，低电平亮-----输出模式
********************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx配置结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				     //PA1 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					       //根据设定参数初始化PA1
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);						         //PA1 输出高
}





#include "sys.h"

/*****************************************************
*函数功能  ：对蓝牙对应的GPIO口进行初始化设置
*函数名    ：Bluetooth_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*           BLUE_PWRC    PB12     1：开  	    0：关
*     			BLUE_KEY		 PA8			1：命令	模式  0：正常通行
********************************************************/
void Bluetooth_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx配置结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //使能PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	       //使能PA端口时钟

	//PB12
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;				               //PB12 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //根据设定参数初始化PB12
	//PA8
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;				               //PA8 端口配置
	GPIO_Init(GPIOA, &GPIO_InitStruct);					                 //根据设定参数初始化PA8
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);						                 //PB1 输出低
}
















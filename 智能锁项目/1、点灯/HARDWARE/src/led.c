#include "sys.h"

/*****************************************************
*函数功能  ：对LED对应的GPIO口进行初始化设置
*函数名    ：LED_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            LED1     PB3
*            LED2     PB2   
*            高电平灭，低电平亮-----输出模式
********************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx配置结构体
	
	/*失能JTAG PB3、PB4*/
  //AFIO时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //重映射SWJ,让JTAG失能，使PB3、PB4变为普通GPIO口
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	//时钟使能   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //使能PB端口时钟
	
	//GPIO配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;				     //PB2/3 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //根据设定参数初始化PA1

	//GPIO输出高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_2);						       //PB2/3 输出高
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
}





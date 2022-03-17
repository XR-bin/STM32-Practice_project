#include "sys.h"
#include "motor.h"

/*****************************************************
*函数功能  ：对步进电机对应的GPIO口进行初始化设置
*函数名    ：Motor_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            M_IN1 ---- PB3
*            M_IN2 ---- PB4
*            M_IN3 ---- PB5
*            M_IN4 ---- PB6
********************************************************/
void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx配置结构体
	
	/*失能JTAG PB3、PB4*/
  //AFIO时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //重映射SWJ,让JTAG失能，使PB3、PB4变为普通GPIO口
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
  //GPIOx时钟使能  GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//GPIOx配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	  //PB3/4/5/6端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		                        //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		                        //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                              //根据设定参数初始化PB3/4/5/6
}



/*****************************************************
*函数功能  ：控制电机转动(正转，出纸)
*函数名    ：Motor_Run
*函数参数  ：void
*函数返回值：void
*描述      ：
*            M_IN1 ---- PB3
*            M_IN2 ---- PB4
*            M_IN3 ---- PB5
*            M_IN4 ---- PB6
********************************************************/
void Motor_Run(void)
{
	static u8 cont = 0;
	switch(cont)
	{
		//第一个节拍
		case 0: M_IN1_H;  M_IN2_L;  M_IN3_L;  M_IN4_H; break;
		//第二个节拍
		case 1: M_IN1_L;  M_IN2_H;  M_IN3_L;  M_IN4_H; break;
		//第三个节拍
		case 2: M_IN1_L;  M_IN2_H;  M_IN3_H;  M_IN4_L; break;  
		//第四个节拍
		case 3: M_IN1_H;  M_IN2_L;  M_IN3_H;  M_IN4_L; break;     
	}
	cont++;
	if(cont >= 4) cont = 0;
}

/*****************************************************
*函数功能  ：控制电机转动(反转，收纸)
*函数名    ：Motor_Run
*函数参数  ：void
*函数返回值：void
*描述      ：
*            M_IN1 ---- PB3
*            M_IN2 ---- PB4
*            M_IN3 ---- PB5
*            M_IN4 ---- PB6
********************************************************/
void Motor_Run2(void)
{
	static u8 cont = 0;
	switch(cont)
	{
		//第一个节拍
		case 0: M_IN1_H;  M_IN2_L;  M_IN3_H;  M_IN4_L; break; 
		//第二个节拍
		case 1: M_IN1_L;  M_IN2_H;  M_IN3_H;  M_IN4_L; break; 
		//第三个节拍
		case 2: M_IN1_L;  M_IN2_H;  M_IN3_L;  M_IN4_H; break;
		//第四个节拍
		case 3: M_IN1_H;  M_IN2_L;  M_IN3_L;  M_IN4_H; break;    
	}
	cont++;
	if(cont >= 4) cont = 0;
}




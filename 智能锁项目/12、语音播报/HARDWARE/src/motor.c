#include "sys.h"
#include "motor.h"
#include "delay.h"

/*****************************************************
*函数功能  ：对motor电机对应的GPIO口进行初始化设置
*函数名    ：Motor_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            LOCK+     PA4
*            LOCK-     PA5   
********************************************************/
void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx配置结构体
	
	//时钟使能   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	       //使能PA端口时钟
	
	//GPIO配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				     //PA4/5 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					                 //根据设定参数初始化PA4/5
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


/*****************************************************
*函数功能  ：motor电机正转反转实现开门和关门
*函数名    ：Motor_DoorSwitch
*函数参数  ：u8 Switch
*函数返回值：void
*描述      ：
*            LOCK+     PA4
*            LOCK-     PA5   
*         
*           MySwitch==0  关门
*           MySwitch==1  开门
********************************************************/
void Motor_DoorSwitch(u8 MySwitch)
{
	switch(MySwitch)
	{
		case 0:LOCK1_L; LOCK2_H; break;   //开
		case 1:LOCK1_H; LOCK2_L; break;   //关
	}
	delay_ms(250);
	LOCK1_L;
  LOCK2_L;
}


/*****************************************************
*函数功能  ：模仿门禁开门
*函数名    ：Motor_DoorOpen
*函数参数  ：u8 Switch
*函数返回值：void
*描述      ：
*            LOCK+     PA4
*            LOCK-     PA5   
*         
*           MySwitch==0  关门
*           MySwitch==1  开门
********************************************************/
void Motor_DoorOpen(u8 s)
{
  Motor_DoorSwitch(1);
	delay_ms(s*1000);
	Motor_DoorSwitch(0);
}












#ifndef _MOTOR_H
#define _MOTOR_H	 
	#include "sys.h"

	#define  M_IN1_L    GPIO_ResetBits(GPIOB,GPIO_Pin_3)    
	#define  M_IN1_H    GPIO_SetBits(GPIOB,GPIO_Pin_3)      

	#define  M_IN2_L    GPIO_ResetBits(GPIOB,GPIO_Pin_4)    
	#define  M_IN2_H    GPIO_SetBits(GPIOB,GPIO_Pin_4)
	
	#define  M_IN3_L    GPIO_ResetBits(GPIOB,GPIO_Pin_5)    
	#define  M_IN3_H    GPIO_SetBits(GPIOB,GPIO_Pin_5)
	
	#define  M_IN4_L    GPIO_ResetBits(GPIOB,GPIO_Pin_6)    
	#define  M_IN4_H    GPIO_SetBits(GPIOB,GPIO_Pin_6)
	
	#define  MOTOR_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6)
	
	void Motor_Init(void);
	void Motor_Run(void);
	void Motor_Run2(void);
	
#endif









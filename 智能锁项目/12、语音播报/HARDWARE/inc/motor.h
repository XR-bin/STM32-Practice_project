#ifndef _MOTOR_H
#define _MOTOR_H

	#include "sys.h"
	
	//LOCK
	#define   LOCK1_H   GPIO_SetBits(GPIOA,GPIO_Pin_4)
	#define   LOCK1_L   GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	#define   LOCK2_H   GPIO_SetBits(GPIOA,GPIO_Pin_5)
	#define   LOCK2_L   GPIO_ResetBits(GPIOA,GPIO_Pin_5);

	void Motor_Init(void);
	void Motor_DoorSwitch(u8 MySwitch);
	void Motor_DoorOpen(u8 s);

#endif







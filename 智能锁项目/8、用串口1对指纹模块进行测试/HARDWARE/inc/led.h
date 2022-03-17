#ifndef __LED_H
#define __LED_H	 

	#include "sys.h"

	#define LED1_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_3)    //开LED1
	#define LED1_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_3)      //关LED1
	#define LED2_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_2)    //开LED2
	#define LED2_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_2)      //关LED2

	void LED_Init(void);//初始化
	void zhiwen(void);
	
#endif



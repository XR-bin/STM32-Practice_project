#ifndef __LED_H
#define __LED_H	 
	#include "sys.h"

	#define LED_ON    GPIO_ResetBits(GPIOA,GPIO_Pin_1)    //开LED
	#define LED_OFF   GPIO_SetBits(GPIOA,GPIO_Pin_1)      //关LED

	void LED_Init(void);//初始化
	
#endif










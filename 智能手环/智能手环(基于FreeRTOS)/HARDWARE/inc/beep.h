#ifndef _BEEP_H
#define _BEEP_H

	#include "stm32f4xx.h"  

	#define BEEP_ON  GPIO_ResetBits(GPIOF,GPIO_Pin_8);
  #define BEEP_OFF GPIO_SetBits(GPIOF,GPIO_Pin_8);
  
  void BEEP_Init(void);

#endif



















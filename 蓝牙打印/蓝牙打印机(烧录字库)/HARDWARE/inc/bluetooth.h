#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

  #include "sys.h"
	
	#define   BLUE_PWR_ON     GPIO_SetBits(GPIOB,GPIO_Pin_1)
	#define   BLUE_PWR_OFF    GPIO_ResetBits(GPIOB,GPIO_Pin_1)
	
	#define   BLUE_KEY_CMD    GPIO_SetBits(GPIOB,GPIO_Pin_9)
	#define   BLUE_KEY_DATA   GPIO_ResetBits(GPIOB,GPIO_Pin_9)
  
  void Bluetooth_Init(void);

#endif





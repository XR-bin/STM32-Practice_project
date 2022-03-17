#ifndef _KEY_H
#define _KEY_H

  #include "stm32f4xx.h"  

  #define KEY GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
  
  void KEY_Init(void);
  u8 KEY_Scan(void);
  
#endif












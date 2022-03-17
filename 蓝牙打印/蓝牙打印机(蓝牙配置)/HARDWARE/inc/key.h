#ifndef _KEY_H
#define _KEY_H

  #include "sys.h"
	
	#define KEY    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
  
  void KEY_Init(void);
  u8 KEY_Scan(void);

#endif









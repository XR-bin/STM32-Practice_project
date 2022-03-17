#ifndef _LED_H
#define _LED_H

	#define  LED_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_7)
	#define  LED_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_7)   

  void LED_Init(void);

#endif








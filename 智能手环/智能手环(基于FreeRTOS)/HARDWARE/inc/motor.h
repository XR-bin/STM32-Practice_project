#ifndef _MOTOR_H
#define _MOTOR_H

	#define  MOTOR_ON   GPIO_SetBits(GPIOB,GPIO_Pin_10)   
	#define  MOTOR_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_10)

  void MOTOR_Init(void);

#endif






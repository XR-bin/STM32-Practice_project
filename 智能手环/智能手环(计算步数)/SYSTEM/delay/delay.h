#ifndef _DELAY_H
#define _DELAY_H
  
  #include "stm32f4xx.h"
  
  void SysTick_Init(void);
  void SysTick1_Init(u16 ms);
  void delay_us(u32 us);
  void delay1_ms(u16 ms);
  void delay_ms(u16 ms);

#endif












#ifndef __DELAY_H
#define __DELAY_H 

  #include "sys.h"  

  void SysTick_Init(void);
  void delay_us(u32 us);
  void delay1_ms(u16 ms);
  void delay_ms(u16 ms);
  void SysTick1_Init(u16 ms);

#endif






























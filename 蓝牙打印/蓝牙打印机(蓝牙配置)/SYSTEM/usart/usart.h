#ifndef _USART_H
#define _USART_H

  #include "sys.h"
  
  void USART1_Init(u32 baud);
  void USART1_Send_Byte(u8 data);
  u8 USART1_Receive_Byte(void);
	void USART1_Send_Str(u8 *str);
  void USART1_Receive_Str(u8 *str);
	void USART2_Init(u32 baud);
	void USART2_Send_Byte(u8 data);
	u8 USART2_Receive_Byte(void);
	void USART2_Send_Str(u8 *str);
	void USART2_Receive_Str(u8 *str);
	
#endif









#ifndef _HP6_H
#define _HP6_H

	#include "stm32f4xx.h"
	
	#define HP6_EN   GPIO_ResetBits(GPIOC,GPIO_Pin_13)   //HP6电源脚使能
	#define HP6_DIS  GPIO_SetBits(GPIOC,GPIO_Pin_13)     //HP6电源脚失能
	
	#define   HP6_WRITE    0xCC      //器件地址  写操作
	#define   HP6_READ     0xCD      //器件地址  读操作
	
	u16 Crc16(u8 *data,u8 len);
	void HP6_Init(void);
	u8 HP6_Send(u8 *cmd);
	u8 HP6_Receive(u8 *buf);
	u8 HP6_Send_Data(u8 *se_buff,u8 *re_buff);
	u8 HP6_OpenRate(void);
	u8 HP6_GetRateResult(u8 *re_buff);
	u8 HP6_CloseRate(void);
	u8 HP6_OpenBlood(void);
	u8 HP6_GetBloodResult(u8 *re_buff);
	u8 HP6_CloseBlood(void);

#endif






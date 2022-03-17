#ifndef _SHT20_H
#define _SHT20_H

  #include "stm32f4xx.h"
	
	typedef struct trh
	{
		float t;
		u16 rh;
	}THR;
	
	#define   SHT20_WRITE    0x80      //器件地址  写操作
	#define   SHT20_READ     0x81      //器件地址  读操作
	#define   SHT20_RST_CMD  0xfe      //软件复位指令
	#define   SHT20_T_CMD    0xf3      //获取温度指令
  #define   SHT20_RH_CMD   0xf5      //获取湿度指令
	
	u8 SHT20_RST(void);
	u8 SHT20_Init(void);
	u16 SHT20_RH_T(u8 cmd);
	THR SHT20_RH_T_data(void);
	
#endif



















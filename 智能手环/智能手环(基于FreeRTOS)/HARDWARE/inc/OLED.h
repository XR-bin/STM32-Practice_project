#ifndef _OLED_H
#define _OLED_H

  #include "stm32f4xx.h"
  
  //DC  数据命令选择线(0，读写命令； 1，读写数据)
  #define  OLED_DC_L   GPIO_ResetBits(GPIOA, GPIO_Pin_15)  //拉低DC，选择命令
  #define  OLED_DC_H   GPIO_SetBits(GPIOA, GPIO_Pin_15)    //拉高DC，选择数据
  //CLK  时钟线
  #define  OLED_CLK_L   GPIO_ResetBits(GPIOB, GPIO_Pin_3)  //拉低CLK
  #define  OLED_CLK_H   GPIO_SetBits(GPIOB, GPIO_Pin_3)    //拉高CLK 
  //RST  硬复位OLED
  #define  OLED_RST_L   GPIO_ResetBits(GPIOB, GPIO_Pin_13)  //拉低RST
  #define  OLED_RST_H   GPIO_SetBits(GPIOB, GPIO_Pin_13)    //拉高RST 
  //MOSI 主输出数据线
  #define  OLED_MOSI_L   GPIO_ResetBits(GPIOB, GPIO_Pin_5)  //拉低MOSI
  #define  OLED_MOSI_H   GPIO_SetBits(GPIOB, GPIO_Pin_5)    //拉高MOSI
	
	//DS  片选
	#define  OLED_DS_L   GPIO_ResetBits(GPIOB, GPIO_Pin_7)  //拉低DS
  #define  OLED_DS_H   GPIO_SetBits(GPIOB, GPIO_Pin_7)    //拉高DS

  //数据和命令的选择宏定义
  #define  OLED_COM     0      //命令
  #define  OLED_DATA    1      //数据
  
  //OLED屏大小
  #define  OLED_LINE       132    //132列像素
  #define  OLED_ROW        8      //8*8行像素(1页8行，共有8页)
   
  void OLED_IO_Init(void);
  void OLED_WR_Byte(u8 data,u8 DC);
  void OLED_Clear(void);
  void OLED_XY(u8 x,u8 y);
  void OLED_Open(void);
  void OLED_Close(void);
  void OLED_Init(void);
  
  void OLED_ShowChar(u16 x,u8 y,u8 size,u8 ch);
  void OLED_ShowString(u8 x,u8 y,u8 size,u8 *str);
  void OLED_ShowHanzi(u16 x,u8 y,u8 size,u8 *hz);
  void OLED_HanziString(u8 x,u8 y,u8 size,u8 *str);
  
  void OLED_Show(u8 x,u8 y,u8 size,u8 *str);
  
  void OLED_ShowPicture(u8 x,u8 y,u8 *tu);
	void OLED_ShowPicture2(u8 x,u8 y,u8 *tu);
	
	void OLED_ShowT(u16 x,u8 y,u8 *hz);
  
#endif















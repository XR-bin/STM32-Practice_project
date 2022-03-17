#ifndef __PRINTER_H
#define __PRINTER_H	 
	#include "sys.h"

	#define   STB1_L    GPIO_ResetBits(GPIOB,GPIO_Pin_15)    
	#define   STB1_H    GPIO_SetBits(GPIOB,GPIO_Pin_15) 
	#define   STB2_L    GPIO_ResetBits(GPIOB,GPIO_Pin_14)    
	#define   STB2_H    GPIO_SetBits(GPIOB,GPIO_Pin_14) 
	#define   STB3_L    GPIO_ResetBits(GPIOB,GPIO_Pin_13)    
	#define   STB3_H    GPIO_SetBits(GPIOB,GPIO_Pin_13)   	
	#define   STB4_L    GPIO_ResetBits(GPIOB,GPIO_Pin_12)    
	#define   STB4_H    GPIO_SetBits(GPIOB,GPIO_Pin_12) 
	#define   STB5_L    GPIO_ResetBits(GPIOB,GPIO_Pin_11)    
	#define   STB5_H    GPIO_SetBits(GPIOB,GPIO_Pin_11)  
	#define   STB6_L    GPIO_ResetBits(GPIOB,GPIO_Pin_10)    
	#define   STB6_H    GPIO_SetBits(GPIOB,GPIO_Pin_10)   

	#define   LAT_L     GPIO_ResetBits(GPIOA,GPIO_Pin_8)    
	#define   LAT_H     GPIO_SetBits(GPIOA,GPIO_Pin_8) 
	#define   CLK_L     GPIO_ResetBits(GPIOA,GPIO_Pin_11)    
	#define   CLK_H     GPIO_SetBits(GPIOA,GPIO_Pin_11) 
	#define   DI_L      GPIO_ResetBits(GPIOA,GPIO_Pin_12)    
	#define   DI_H      GPIO_SetBits(GPIOA,GPIO_Pin_12) 
	
	extern u8 p_flag;
	
	void Printer_Init(void);
	void Printer_Write_Byte(u8 data);
	void Printer_Left(void);
	void Printer_Right(void);
	void Printer_OFF(void);
	void Printer_Stripe(void);
	void Printer_Picture(u8 *tu);
	
#endif











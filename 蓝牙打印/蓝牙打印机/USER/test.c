#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "key.h"
#include "usart.h"
#include "bluetooth.h"
#include "time.h"
#include "motor.h"
#include "printer.h"
#include "w25q64.h"
#include "stdio.h"
#include "matrix.h"
#include "nvic.h"
#include "stdio.h"
#include "string.h"

/**************************************************************
*备注事项：
*         1、W25Q64的0x0b0000~0x1CF82C用于存储字库
*            开始：块：0b   第11块      扇区：0    第0个扇区
*                  页：0    第0页       字节：00   第0个字节
*            结束：块：1C   第28块      扇区：F    第15个扇区
*                  页：8    第8页       字节：2C   第44个字节
***************************************************************/

int main(void)
{			
	u8 key;
	
	SysTick_Init();	  	//延时初始化
	LED_Init();
	KEY_Init();
	USART1_Init(115200);
	USART2_Init(38400);
	Bluetooth_Init();
//	BLUE_PWR_ON;
//	BLUE_KEY_CMD;
	Printer_Init();
	Motor_Init();
	TIM3_Init(8000-1,71);
	W25Q64_Init();
		
	
	while(1)
	{	
		key = KEY_Scan();
		if(key==1)
		{
			GPIOA->ODR ^= 1<<1;
			Printer1_String24(buff);
			MOTOR_OFF;
			Printer_OFF();
		}
	}	 
}





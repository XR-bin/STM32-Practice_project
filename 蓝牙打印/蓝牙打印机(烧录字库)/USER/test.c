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


int main(void)
{			
//	u8 key;
	u16 k;
	
	SysTick_Init();	  	//延时初始化
	LED_Init();
	KEY_Init();
	USART1_Init(115200);
	USART2_Init(38400);
//	Bluetooth_Init();
//	BLUE_PWR_ON;
//	BLUE_KEY_CMD;
//	Printer_Init();
//	Motor_Init();
//	TIM3_Init(800-1,71);
	W25Q64_Init();
	
	k = W25Q64_Read_id();
	printf("ID:%d\r\n",k);
	printf("开始擦除\r\n");
  W25Q64_Blocks_Erase(0x0b0000,21);
  printf("擦除完成\r\n");
	
	while(1)
	{
//		key = KEY_Scan();
//		if(key==1)
//		{
//			GPIOA->ODR ^= 1<<1;
//			TIM_Cmd(TIM3, ENABLE);
//		}
	}	 
}





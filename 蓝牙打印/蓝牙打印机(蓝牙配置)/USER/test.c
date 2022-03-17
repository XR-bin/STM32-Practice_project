#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "key.h"
#include "usart.h"
#include "bluetooth.h"


int main(void)
{			
	u8 key;
	
	SysTick_Init();	  	//延时初始化
	LED_Init();
	KEY_Init();
	USART1_Init(115200);
	USART2_Init(38400);
	Bluetooth_Init();
	BLUE_PWR_ON;
	BLUE_KEY_CMD;
	
	while(1)
	{
		key = KEY_Scan();
		if(key==1)
		{
			GPIOA->ODR ^= 1<<1;
			USART1_Send_Str("你好\r\n");
		}
	}	 
}





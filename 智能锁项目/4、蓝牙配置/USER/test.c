#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"

int main(void)
{			
	SysTick_Init();	  	 //—” ±≥ı ºªØ
	USART1_Init(115200); 
	USART3_Init(115200);
  LED_Init();
	OLED_Init();
	Bluetooth_Init();
	BLUE_PWR_OFF;
	BLUE_KEY_CMD;
	//BLUE_KEY_DATA;
	
	while(1)
	{
		
	}	 
}





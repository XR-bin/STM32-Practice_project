#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"

int main(void)
{			
	SysTick_Init();	  	 //—” ±≥ı ºªØ
	USART1_Init(115200); 
  LED_Init();
	OLED_Init();
	
	while(1)
	{
		
	}	 
}





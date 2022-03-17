#include "sys.h"	
#include "delay.h"	
#include "led.h"

int main(void)
{			
	SysTick_Init();	  	//—” ±≥ı ºªØ
  LED_Init();
	while(1)
	{
		LED1_ON;
		LED2_ON;
		delay_ms(500);
		LED1_OFF;
		LED2_OFF;
		delay_ms(500);
	}	 
}





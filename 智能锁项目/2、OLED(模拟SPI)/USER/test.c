#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"

int main(void)
{			
	SysTick_Init();	  	//延时初始化
  LED_Init();
	OLED_Init();
	
	OLED_HanziString(0,0,16,"普通");
	
	while(1)
	{
		
	}	 
}





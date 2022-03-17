#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"
#include "at24cxx.h"
#include "stdio.h"
#include "rtc.h"
#include "BS8116A.h"
#include "MG200.h"
#include "spi.h"
#include "MFRC522.h"
#include "motor.h"

int main(void)
{		
	
	SysTick_Init();	  	 //—” ±≥ı ºªØ
	USART1_Init(115200);
	LED_Init();
	Motor_Init();
	Motor_DoorOpen(1);
	
	while(1)
	{
	}
}





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
#include "voic.h"

int main(void)
{		
	u8 i;
	
	SysTick_Init();	  	 //—” ±≥ı ºªØ
	USART1_Init(115200);
	Voic_Init();
	
	while(1)
	{
		for(i=1;i<32;i++)
		{
			Voic_SendData(i);
			delay_ms(500);
			while(VO_BUSY_HL);
		}
	}
}





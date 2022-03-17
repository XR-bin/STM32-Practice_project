#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "motor.h"
#include "usart.h"
#include "time.h"
#include "nvic.h"
#include "pwm.h"
#include "OLED.h"
#include "matrix.h"
#include "adc.h"
#include "stdio.h"
#include "sht20.h"
#include "hp6.h"
#include "mpu6050.h"
#include "rtc.h"
#include "interface.h"
#include "function.h"

int main(void)
{
	u8 temp=0;
	
	Watch_Init();
	Watch_Menu();
  while(1)
	{
		if(temp==0) temp = Select_Menu();
		else
		{
			switch(temp)
			{
				case 1:Bp_Function();temp=0;break;
				case 2:Rate_Function();temp=0;break;
				case 3:Temperature_Function();temp=0;break;
				case 4:Humidity_Function();temp=0;break;
				case 5:RTC_Function();temp=0;break;
				case 6:Step_Function();temp=0;break;
			}
		}
		motion_Scan();
  }
}



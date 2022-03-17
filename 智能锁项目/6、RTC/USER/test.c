#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"
#include "at24cxx.h"
#include "stdio.h"
#include "rtc.h"

int main(void)
{		
	RTC_DATES time = {2019,10,10,0,12,20,20};  //星期是无法设置的
  RTC_DATES rtc;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	SysTick_Init();	  	 //延时初始化
	USART1_Init(115200); 
	//USART3_Init(115200);
	OLED_Init();
  MyRTC_Init(time);
	
	OLED_HanziString(0,0,16,"普通");
  
	while(1)
	{
    rtc = RTC_Get();
    printf("%d年%d月%d日 星期%d\r\n",rtc.year,rtc.month,rtc.day,rtc.week);
    printf("%d : %d : %d\r\n",rtc.hour,rtc.min,rtc.sec);
    delay_ms(1000);
  } 
}





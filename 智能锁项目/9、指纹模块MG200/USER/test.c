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

int main(void)
{		
	u8 ID = 0 ,i;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	SysTick_Init();	  	 //延时初始化
	USART1_Init(115200); 
  MG200_Init();
	MG200_ON;
	delay_ms(100);
	MG200_Enroll();    //登记用户指纹信息
	MG200_EraseAll();  //删除所有用户
	for(i=0;i<3;i++)
	MG200_Enroll();    //登记用户指纹信息
	
	i = MG200_GetUserNum();
	printf("指纹用户个数：%d\r\n",i);
	
	while(1)
	{
		ID =MG200_Match1Nl();
		if(ID != 0)
		{
			printf("ID:%d\r\n",ID);
			delay_ms(100);
			MG200_EraseOne(ID);
			
			delay_ms(100);
			i = MG200_GetUserNum();
			printf("指纹用户个数：%d\r\n",i);
			
			ID = 0;
		}
	}	 
}





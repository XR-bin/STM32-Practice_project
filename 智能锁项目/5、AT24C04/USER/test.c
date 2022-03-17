#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"
#include "at24cxx.h"
#include "stdio.h"

int main(void)
{		
	u8 as[] = "885qwertyuiopasd123456789";
  u8 bs[40] ;
  u8 err1,err2;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	SysTick_Init();	  	 //延时初始化
	USART1_Init(115200); 
	//USART3_Init(115200);
  LED_Init();
	OLED_Init();
	AT24CXX_Init();  
	//delay_ms(10000);
  err1 = AT24CXXaddr_Write_Addr(AT24C04,AT24CXX_WRITE,0,sizeof(as),as);
  err2 = AT24CXXaddr_Read_Addr(AT24C04,AT24CXX_WRITE,0,sizeof(as),bs);
	
	while(1)
	{
		printf("err1 : %d    err2 = %d\r\n",err1,err2);
    printf("bs : %s\r\n",bs);
    delay_ms(1000);
	}	 
}





#include "delay.h"
#include "sys.h"

//如果使用OS,则包括下面的头文件即可
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h"
#endif

static u8  fac_us=0;							//us延时倍乘数  提供给微秒级延时用的，不会引起任务调度		   
static u16 fac_ms=0;							//ms延时倍乘数  提供给会引起任务调度的毫秒级延时函数用的 



extern void xPortSysTickHandler(void);
 
/********************************************************
*函数功能  ：滴答定时器的中断服务函数
*函数名    ：SysTick_Handler
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*           该函数为系统提供心跳，起到任务调度的作用  
***********************************************************/
void SysTick_Handler(void)
{	
	//判断系统是否已经运行
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{
		//为系统提供心跳(用于任务调度)
		xPortSysTickHandler();	
	}
}
			   
/***************************************************************************
*函数功能  ：滴答定时器初始化
*函数名    ：delay_init
*函数参数  ：void
*函数返回值：void
*描述      ：
*           configTICK_RATE_HZ = 1000 系统时钟节拍频率为1000，周期就是1ms
*****************************************************************************/
void delay_init(void)
{
	u32 reload;
	u8 SystemCoreClock = 168;   
	
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //选择内部高速时钟HCLK   168M不分频 (SysTick->CTRL寄存器的第2位)
	fac_us = SystemCoreClock;							            //不论是否使用OS,fac_us都需要使用，1个fac_us = 1微秒 = 168次
	reload = SystemCoreClock;							            //每秒钟的计数次数 单位为M	   
	reload*=1000000/configTICK_RATE_HZ;		            //根据configTICK_RATE_HZ设定溢出时间   reload(系统时钟) = 1ms = 168000
	                                                  //reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	  16777216 / 168000000 = 0.0998
	fac_ms=1000/configTICK_RATE_HZ;			              //代表OS可以延时的最少单位(根据系统每1000为1ms) ，是提供给有系统延时的ms级延时，还会引起任务调度	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;          //开启SYSTICK中断
	SysTick->LOAD=reload; 					                  //每0.0998s中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;           //开启SYSTICK     
}								    

/******************************************************************
*函数功能  ：系统滴答定时器微秒延时
*函数名    ：delay_us
*函数参数  ：u32 nus
*函数返回值：void
*描述      ：
*           这里注意一下SYSTICK是一个递减的计数器就可以了.
*           该函数不会引起任务调度
*******************************************************************/      								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload = SysTick->LOAD;				  //当前SysTick->LOAD寄存器的重载值	    	 
	ticks= nus * fac_us; 						    //需要的节拍数(要延时多少微秒)
	told = SysTick->VAL;        				//刚进入时的计数器值(已经计数到多少)
	while(1)
	{
		tnow=SysTick->VAL;	  //获取当前计数器值
		//判断计数器是否已经计数完一圈了，如果刚好一圈就不进行计算
		if(tnow!=told)
		{	    
			//如果当前值比上一次的值(第一次循环是刚进来的值，后来会改变)，说明定时器还没溢出
			if(tnow<told)tcnt+=told-tnow;	  //上一次的值 - 当前值 + 上一次经历的值 = 总经历的值
			//如果当前值比上一次的值大时，说明定时器有溢出过
			else tcnt+=reload-tnow+told;	  //计数器的最大值 - 当前值 + 上一次的值 + 上一次经历的值 = 总经历的值
			told=tnow;     //当前值变为上一次值
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}										    
}  

/******************************************************************
*函数功能  ：系统滴答定时器毫秒延时(有利用到系统的延时函数)
*函数名    ：delay_ms
*函数参数  ：u32 nms
*函数返回值：void
*描述      ：
*            该函数会产生任务阻塞并引起任务调度
*******************************************************************/
void delay_ms(u32 nms)
{	
	//判断系统是否已经运行
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
	{		
		if(nms>=fac_ms)						      //延时的时间大于OS的最少时间周期 
		{ 
				vTaskDelay(nms/fac_ms);	 		//使用FreeRTOS系统延时，会引起任务阻塞
		}
		nms%=fac_ms;						        //OS已经无法提供这么小的延时了,采用普通方式延时补充时差    
	}
	delay_us((u32)(nms*1000));				//普通方式延时
}

/******************************************************************
*函数功能  ：系统滴答定时器毫秒延时
*函数名    ：delay_xms
*函数参数  ：u16 ms
*函数返回值：void
*描述      ：
*            直接循环调用delay_us函数，不会引起任务调度
*******************************************************************/
void delay_xms(u32 nms)
{
	u32 i;

	//不断循环调用微秒级延时函数，达到毫秒级延时
	for(i=0;i<nms;i++) delay_us(1000); 
}

			 




































#include "stm32f4xx.h" 
#include "rtc.h"
#include "stdio.h"

/*****************************************
*函数功能  ：设置RTC的时、分、秒、星期
*函数名    ：RTC_Set_Time
*函数参数  ：u8 hour,u8 min,u8 sec,u8 pm
*函数返回值：void
*描述      ：
*******************************************/
void RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeStruct;     //时间设置结构体
	
	RTC_TimeStruct.RTC_Hours=hour;      //时
	RTC_TimeStruct.RTC_Minutes=min;     //分
	RTC_TimeStruct.RTC_Seconds=sec;     //秒
	RTC_TimeStruct.RTC_H12=ampm;        //am/pm
	
	RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);   //设置时间(RTC_Format_BIN：二进制格式写入)
}

/**********************************************
*函数功能  ：设置RTC的年、星期、月、日
*函数名    ：RTC_Set_Date
*函数参数  ：u8 year,u8 month,u8 day,u8 week
*函数返回值：void
*描述      ：
************************************************/
void RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef  RTC_DateStruct;   //日期设置结构体
	
	RTC_DateStruct.RTC_Date=date;      //日
	RTC_DateStruct.RTC_Month=month;    //月
	RTC_DateStruct.RTC_WeekDay=week;   //星期
	RTC_DateStruct.RTC_Year=year;      //年
	
  RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);   //设置日期(RTC_Format_BIN：二进制格式写入)
}


/************************************
*函数功能  ：对RTC进行初始化设置
*函数名    ：RTC_Init
*函数参数  ：RTC_DATES rtc
*函数返回值：void
*描述      ：
*************************************/
void MYRTC_Init(RTC_DATES rtc)
{
	RTC_InitTypeDef   RTC_InitStruct;
	
	//时钟时能     PWR
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	
	//使能对RTC的写访问
	PWR_BackupAccessCmd(ENABLE);	//备份域写保护：使能后备寄存器访问(PWR->CR寄存器的第8位)
	
	/*时钟源选择*/
	RCC_LSICmd(ENABLE);//开启内部低速晶振LSI
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		            //选择内部低速时钟作为时钟源(RCC->BDCR寄存器的第8-9位)
	//RCC_LSEConfig(RCC_LSE_ON);                          //开启外部低速震荡器(LSE) (RCC->BDCR寄存器的第0位)
	RCC_LSICmd(ENABLE);                                   //开启内部低速晶振LSI(RCC_CSR的第0位)
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);   //内部低速震荡器就绪 (RCC->BDCR寄存器的第1位)
	RCC_RTCCLKCmd(ENABLE);                                //RTC时钟使能(RCC->BDCR寄存器的第15位)
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0xff)//用于避免复位后重新设时间值(没电池时只对复位有用，断电后该位会清零，依然会重新设时间值)
	{
		
		/*RTC配置*/
		RTC_InitStruct.RTC_AsynchPrediv = 0x7F;              //(默认值不要改)RTC异步分频系数：异步预分频系数0x7F+1(RTC->PRER寄存器的第16-22位)
    RTC_InitStruct.RTC_SynchPrediv  = 0xFF;              //(默认值不要改)RTC同步分频系数：同步预分频系数0xFF+1(RTC->PRER寄存器的第0-14位)
    RTC_InitStruct.RTC_HourFormat   = RTC_HourFormat_24;//小时格式：24小时格式(RTC_CR寄存器的第6位)
    RTC_Init(&RTC_InitStruct);                          //初始化RTC配置
		
		/*时间设置*/
    //设置时间
    RTC_Set_Time(rtc.hour,rtc.min,rtc.sec,rtc.pm);
    //设置年月日星期
    RTC_Set_Date(rtc.year,rtc.month,rtc.day,rtc.week);
		
		//避免复位后重新设时间值，但断电重启该位会自动清零。
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xff);  //(RTC->BKP0R寄存器)
	}
}















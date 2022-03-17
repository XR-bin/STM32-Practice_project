#ifndef __RTC_H
#define __RTC_H	 

	#include "stm32f4xx.h" 
  //用于设置时间的结构体
  typedef struct
  {
    u8 year;
    u8 month;
    u8 day;
    u8 week;
    u8 hour;
    u8 min;
    u8 sec;
    u8 pm;
  }RTC_DATES;
	
	typedef	struct
	{
		u8 hour;
		u8 minute;
		u8 second;
		u8 twentyMsCount;   //20毫秒计数
	}timeStamp_t;

  void MYRTC_Init(RTC_DATES rtc);
  void RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);
  void RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);


#endif



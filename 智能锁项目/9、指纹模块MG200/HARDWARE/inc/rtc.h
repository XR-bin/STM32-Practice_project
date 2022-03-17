#ifndef _RTC_H
#define _RTC_H

  #include "sys.h"

  typedef struct
  {
    u16 year;
    u8 month;
    u8 day;
    u8 week;
    u8 hour;
    u8 min;
    u8 sec;
  }RTC_DATES;
  
  void MyRTC_Init(RTC_DATES rtc);
  u8 Leap_Year(u16 year);
  u8 RTC_Set(RTC_DATES rtc);
  RTC_DATES RTC_Get(void);
  u8 RTC_Week(RTC_DATES rtc);

#endif


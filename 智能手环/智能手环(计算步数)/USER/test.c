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
#include "MeterStepAlgorithm.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}


int main(void)
{
	RTC_DATES time = {19,6,27,6,6,6,20,0};
	ADD_SPEED jiao;
	RTC_TimeTypeDef Rtc_Time;
  RTC_DateTypeDef Rtc_Date;
	static sportsInfo_t userSportsInfo;
	static u8 tempSecond;								       //保存秒钟暂态量
	static timeStamp_t timeStamp ={0};         //存时分秒和20毫秒内的计数
	u8 buff[30];
	
	//个人基本信息：身高、体重
	personInfo_t My_info;
	
	My_info.height = (float)170/100;
	My_info.weight = (float)134/2;
	
  LED_Init();
	KEY_Init();
	MOTOR_Init();
	TIM10_Init(9999,16799);
	ADC1_Init_In3();
	//TIM2_PWM_Init(1000,84,pwm);
	OLED_Init();
	ADC1_Init_In3();
	SHT20_Init();
	HP6_Init();
	MPU6050_Init();
	MYRTC_Init(time);//必须用内部低速时钟
	
  while(1)
	{
		jiao = MPU6050_ReadSpeed();
		RTC_GetTime(RTC_Format_BIN,&Rtc_Time);         
		RTC_GetDate(RTC_Format_BIN, &Rtc_Date);
		
		if(tempSecond != timeStamp.second)
		{
			tempSecond = timeStamp.second;
			timeStamp.twentyMsCount = 0 ;//20ms计数变量清零
		}
		else												  //秒不更新，1秒等于50*20ms
		{
			timeStamp.twentyMsCount ++;//20ms计数变量++
		}
		
		timeStamp.hour	 = Rtc_Time.RTC_Hours;
		timeStamp.minute = Rtc_Time.RTC_Minutes;
		timeStamp.second = Rtc_Time.RTC_Seconds;
		
		userSportsInfo = *SensorChanged(&jiao,&timeStamp,&My_info);
		
		sprintf((char *)buff,"Step:%05d",userSportsInfo.stepCount);
		OLED_ShowString(3,1,16,buff);
		sprintf((char *)buff,"kal:%0.1f KAL",userSportsInfo.calories);
		OLED_ShowString(3,3,16,buff);
		sprintf((char *)buff,"dis:%0.1f M ",userSportsInfo.distance);
		OLED_ShowString(3,5,16,buff);
		
		delay_ms(20);
  }
}



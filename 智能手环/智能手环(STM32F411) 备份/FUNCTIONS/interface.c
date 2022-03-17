#include "stm32f4xx.h"  
#include "interface.h"
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
#include "MeterStepAlgorithm.h"

static void delay_ms(u32 ms)
{
	u32 i = 100 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}


/*************************************************************
*函数功能  ：初始化整个智能手环程序
*函数名    ：Watch_Init 
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Watch_Init(void)
{
	RTC_DATES time = {19,6,27,5,6,6,20,0};
	u8 i,t=70,k;
	
  LED_Init();
	KEY_Init();
	MOTOR_Init();
	TIM10_Init(9999,9999);
	ADC1_Init_In3();
	//TIM2_PWM_Init(1000,50,pwm);
	OLED_Init();
	ADC1_Init_In3();
	SHT20_Init();
	HP6_Init();
	MPU6050_Init();
	MYRTC_Init(time);//必须用内部低速时钟
	
	for(i=0;i<14;i++)
  {
		k++;
		if(k==7)k=0;
		OLED_ShowPicture(t,0,&TU[k*512]);
		delay_ms(100);
		OLED_Clear();
		t-=10;
  }
	OLED_ShowPicture(1,0,TU2);
	OLED_ShowString(60,2,16,"welcome");
	OLED_ShowString(63,4,16,"to use");
	
	while(!KEY_Scan());
	OLED_Clear();
	OLED_ShowChar(10,1,16,'*');
}

/*************************************************************
*函数功能  ：智能手环主菜单界面
*函数名    ：Watch_Menu 
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Watch_Menu(void)
{
	OLED_HanziString(20,1,16,"血压");
	OLED_HanziString(75,1,16,"心率");
	OLED_HanziString(20,3,16,"温度");
	OLED_HanziString(75,3,16,"湿度");
	OLED_HanziString(20,5,16,"时间");
	OLED_HanziString(75,5,16,"步数");
}	

/*************************************************************
*函数功能  ：智能手环心率检查界面
*函数名    ：Rate_HMI 
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Rate_HMI(void)
{
	OLED_ShowPicture(1,0,TU3);
	OLED_HanziString(65,1,16,"按下按键");
	OLED_HanziString(65,3,16,"开始心率");
	OLED_HanziString(82,5,16,"检查");
}

/*************************************************************
*函数功能  ：智能手环心率检查界面
*函数名    ：Bp_HMI 
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Bp_HMI(void)
{
	OLED_ShowPicture(1,0,TU4);
	OLED_HanziString(65,1,16,"按下按键");
	OLED_HanziString(65,3,16,"开始血压");
	OLED_HanziString(82,5,16,"检查");
}



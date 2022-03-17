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
#include "math.h"
#include "MeterStepAlgorithm.h"

u8 x = 10;
u8 y = 1;

static void delay_ms(u32 ms)
{
	u32 i = 100 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

/*************************************************************
*函数功能  ：菜单选择光标的移动
*函数名    ：Select_Menu
*函数参数  ：void
*函数返回值：u8
*描述      ：
**************************************************************/
u8 Select_Menu(void)
{
	u8 key=0;
	u16 cont=0;
	static u8 temp=1;
	u8 temp2 = 0;
	
	key = KEY_Scan();
	if(key)
	{
		while(KEY)
		{
			cont++;
			delay_ms(1);
		}
		if(cont>400)key=2;
	}
	
	if(key==1)
	{
		if((x!=65)||(y!=5))
		{
			if(x != 65)
			{
				OLED_ShowChar(x,y,16,' ');
				x+=55;
				OLED_ShowChar(x,y,16,'*');
				cont=0;
			}
			else
			{
				OLED_ShowChar(x,y,16,' ');
				x=10;y+=2;
				OLED_ShowChar(x,y,16,'*');
				cont=0;
			}
			temp++;
		}
		else
		{
			OLED_ShowChar(x,y,16,' ');
			x=10;y=1;
			OLED_ShowChar(x,y,16,'*');
			cont=0;
			temp=1;
		}
	}
	else if(key == 2)
	{
		OLED_Clear();
		switch(temp)
		{
			case 1:Bp_HMI();temp2=1;break;
			case 2:Rate_HMI();temp2=2;break;
			case 3:OLED_ShowPicture(1,0,TU5);temp2=3;break;
			case 4:OLED_ShowPicture(1,0,TU6);temp2=4;;break;
			case 5:temp2=5;break;
			case 6:OLED_ShowPicture(1,0,TU7);temp2=6;break;
		}
		cont=0;
	}
	
	return temp2;
}

/*************************************************************
*函数功能  ：血压检测功能
*函数名    ：Bp_Function
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Bp_Function(void)
{
	u8 key=0;
	u16 cont=0;
	u8 bp_val[24] = {0};
	u8 bp_buff[20] = {0};
	
	while(1)
	{
		key = KEY_Scan();
		if(key)
		{
			while(KEY)
			{
				cont++;
				delay_ms(1);
			}
			if(cont>400)key=2;
		}
		if(key==1)
		{
			key=0;
			cont=0;
			OLED_ShowPicture2(2,0,TU8);
			delay_ms(5000);
			OLED_Clear();
			OLED_ShowPicture(35,0,TU4);
			OLED_ShowString(2,1,12,"test...");
			HP6_OpenBlood();
			while(bp_val[7]==0)HP6_GetBloodResult(bp_val);
			HP6_CloseBlood();
			sprintf((char *)bp_buff,"H:%3d  L:%3d",bp_val[10],bp_val[11]);
			bp_val[7] = 0;
			OLED_Clear();
			OLED_Show(50,1,16,"血压");
			OLED_Show(15,3,16,bp_buff);
			//等待按键按下
			while(!key)key = KEY_Scan();
			//看看按键是否有长按
			while(key)
			{
				key = KEY_Scan();
				if(key)
				{
					while(KEY)
					{
						cont++;
						delay_ms(1);
					}
					if(cont>400)key=2;
				}
			}
			key=3;
		}
		if(key==2)
		{
			OLED_Clear();
			Watch_Menu();
			OLED_ShowChar(x,y,16,'*');
			break;
		}
		else if(key==3)
		{
			OLED_Clear();
			Bp_HMI();
		}
	}
}

/*************************************************************
*函数功能  ：心率检测功能
*函数名    ：Rate_Function
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Rate_Function(void)
{
	u8 key=0;
	u16 cont=0;
	u8 Rate_val[24] = {0};
	u8 Rate_buff[20] = {0};
	static u8 old = 0;
	
	while(1)
	{
		key = KEY_Scan();
		if(key)
		{
			while(KEY)
			{
				cont++;
				delay_ms(1);
			}
			if(cont>400)key=2;
		}
		if(key==1)
		{
			key=0;
			cont=0;
			OLED_ShowPicture2(2,0,TU9);
			delay_ms(4800);
			OLED_Clear();
			OLED_ShowPicture(32,0,TU3);
			OLED_ShowString(82,6,12,"test...");
			HP6_OpenRate();
			while((Rate_val[7]==0)||(Rate_val[7]==old))HP6_GetRateResult(Rate_val);
			HP6_CloseRate();
			old = Rate_val[7];
			sprintf((char *)Rate_buff,"%3d BPM",Rate_val[7]);
			OLED_Clear();
			OLED_Show(50,1,16,"心率");
			OLED_Show(15,3,24,Rate_buff);
			//等待按键按下
			while(!key)key = KEY_Scan();
			//看看按键是否有长按
			while(key)
			{
				key = KEY_Scan();
				if(key)
				{
					while(KEY)
					{
						cont++;
						delay_ms(1);
					}
					if(cont>400)key=2;
				}
			}
			key=3;
		}
		if(key==2)
		{
			OLED_Clear();
			Watch_Menu();
			OLED_ShowChar(x,y,16,'*');
			break;
		}
		else if(key==3)
		{
			OLED_Clear();
			Rate_HMI();
		}
	}
}

/*************************************************************
*函数功能  ：环境温度检测功能
*函数名    ：Temperature_Function
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Temperature_Function(void)
{
	u8 key=0;
	u16 cont=0;
	float T;
	u8 T_buff[20];
	
	OLED_HanziString(55,1,16,"实时温度");
	while(1)
	{
		key = KEY_Scan();
		if(key)
		{
			while(KEY)
			{
				cont++;
				delay_ms(1);
			}
			if(cont>400)
			{
				OLED_Clear();
				Watch_Menu();
				OLED_ShowChar(x,y,16,'*');
				break;
			}
			cont=0;
		}
		
		T = SHT20_RH_T(SHT20_T_CMD);
		T = -46.85 + 175.72 * ( T / pow(2,16));
		sprintf((char *)T_buff,"%2.1f",T);
		OLED_ShowString(55,3,24,T_buff);
		//OLED_ShowString(110,4,16,"C");
		OLED_ShowT(110,4,Tbuff);
	}
}

/*************************************************************
*函数功能  ：环境湿度检测功能
*函数名    ：Humidity_Function
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Humidity_Function(void)
{
	u8 key=0;
	u16 cont=0;
	float RH;
	u8 RH_buff[20];
	
	OLED_HanziString(67,1,16,"实时湿度");
	while(1)
	{
		key = KEY_Scan();
		if(key)
		{
			while(KEY)
			{
				cont++;
				delay_ms(1);
			}
			if(cont>400)
			{
				OLED_Clear();
				Watch_Menu();
				OLED_ShowChar(x,y,16,'*');
				break;
			}
			cont=0;
		}
		
		RH = SHT20_RH_T(SHT20_RH_CMD);
		RH = -6 + 125 * (RH / pow(2,16));
		sprintf((char *)RH_buff,"%2.1f",RH);
		OLED_ShowString(71,3,24,RH_buff);
	}
}

/*************************************************************
*函数功能  ：时间显示功能
*函数名    ：RTC_Function
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void RTC_Function(void)
{
	RTC_TimeTypeDef Rtc_Time;
  RTC_DateTypeDef Rtc_Date;
	u8 Date_buff[25];
	u8 Time_buff[25];
	u8 key=0;
	u16 cont;
	
	while(1)
	{
		//这个RTC_Format_BIN有点特别和设置时间的库函数的RTC_Format_BIN的意思不一样
		//这里的RTC_Format_BIN是指把二进制数据转为十进制数据
		RTC_GetTime(RTC_Format_BIN,&Rtc_Time);         
		RTC_GetDate(RTC_Format_BIN, &Rtc_Date);
		
		sprintf((char *)Date_buff,"20%02d-%02d-%02d",Rtc_Date.RTC_Year,Rtc_Date.RTC_Month,Rtc_Date.RTC_Date);
		sprintf((char *)Time_buff,"%02d:%02d",Rtc_Time.RTC_Hours,Rtc_Time.RTC_Minutes);
		
		switch(Rtc_Date.RTC_WeekDay)
		{
			case 1:OLED_ShowString(45,6,16," Mon ");break;
			case 2:OLED_ShowString(45,6,16,"Tues ");break;
			case 3:OLED_ShowString(45,6,16," Wed ");break;
			case 4:OLED_ShowString(45,6,16,"Thurs");break;
			case 5:OLED_ShowString(45,6,16," Fri ");break;
			case 6:OLED_ShowString(45,6,16," Sat ");break;
			case 7:OLED_ShowString(45,6,16," Sun ");break;
		}
		
		OLED_ShowString(35,1,24,Time_buff);
		OLED_ShowString(25,4,16,Date_buff);
		
		key = KEY_Scan();
		if(key)
		{
			while(KEY)
			{
				cont++;
				delay_ms(1);
			}
			if(cont>400)
			{
				OLED_Clear();
				Watch_Menu();
				OLED_ShowChar(x,y,16,'*');
				break;
			}
		}
	}
}

personInfo_t My_info = {170/100,134/2};    //身体的身高、体重
sportsInfo_t SportsInfo;                   //装步数、卡路里、路程的结构体变量
ADD_SPEED as;                              //存放加速度
RTC_TimeTypeDef Rtc_Time;                  //获取RTC时间
static u8 tempSecond;								       //保存秒钟暂态量
static timeStamp_t timeStamp ={0};         //存时分秒和20毫秒内的计数

/***********************************************************
*函数功能  ：运动扫描
*函数名    ：motion_Scan
*函数参数  ：void
*函数返回值：void
*描述      ：
*************************************************************/
void motion_Scan(void)
{
	as = MPU6050_ReadSpeed();
	RTC_GetTime(RTC_Format_BIN,&Rtc_Time);         
	
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
	
	SportsInfo = *SensorChanged(&as,&timeStamp,&My_info);
	
	delay_ms(20);
}

/*************************************************************
*函数功能  ：查看步数、路程、卡路里功能
*函数名    ：Step_Function
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void Step_Function(void)
{
	u8 key=0;
	u16 cont=0;
	u8 temp=1;
	
	while(1)
	{
		key = KEY_Scan();
		if(key)
		{
			while(KEY)
			{
				cont++;
				delay_ms(1);
			}
			if(cont>400)
			{
				OLED_Clear();
				Watch_Menu();
				OLED_ShowChar(x,y,16,'*');
				break;
			}
			cont=0;
			temp++;
			if(temp==4)temp=1;
			OLED_Clear();
			switch(temp)
			{
				case 1:OLED_ShowPicture(1,0,TU7);StepNumber();break;
				case 2:OLED_ShowPicture(1,0,TU10);CalorieConsume();break;
				case 3:OLED_ShowPicture(1,0,TU11);StepDistance();break;
			}
		}
		
		switch(temp)
		{
			case 1:StepNumber();break;
			case 2:CalorieConsume();break;
			case 3:StepDistance();break;
		}
		motion_Scan();
	}
}

/*************************************************************
*函数功能  ：查看步数
*函数名    ：StepNumber
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void StepNumber(void)
{
	u8 buff[5];
	
	sprintf((char *)buff,"%04d",SportsInfo.stepCount);
	OLED_ShowString(67,1,16,"Step");
	OLED_ShowString(71,3,24,buff);
}

/*************************************************************
*函数功能  ：查看卡路里消耗
*函数名    ：CalorieConsume
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void CalorieConsume(void)
{
	u8 buff[5];
	
	sprintf((char *)buff,"%0.1f",SportsInfo.calories);
	OLED_ShowString(67,1,16,"Calorie");
	OLED_ShowString(69,3,24,buff);
	OLED_ShowString(100,6,12,"cal");
}

/*************************************************************
*函数功能  ：查看步行路程
*函数名    ：StepDistance 
*函数参数  ：void
*函数返回值：void
*描述      ：
**************************************************************/
void StepDistance(void)
{
	u8 buff[5];
	
	sprintf((char *)buff,"%0.1f",SportsInfo.distance);
	OLED_ShowString(59,1,16,"Distance");
	OLED_ShowString(69,3,24,buff);
	OLED_ShowString(115,6,12,"M");
}





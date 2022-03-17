#ifndef _MSA_H
#define _MSA_H

  #include "stm32f4xx.h"
	#include "mpu6050.h"
	#include "rtc.h"
	
	typedef struct 
	{
		float calories; //卡路里
		float distance;//走路路程，单位为m
		u32 stepCount;	//步数
	}sportsInfo_t;
	
	typedef struct 
	{
		float height;  //身高
		float weight;  //体重
	}personInfo_t;
	
	#define 	TRUE 		    1     //表示上升趋势
	#define 	FALSE 	    0     //表示下降趋势
	#define   VALUE_NUM   4     //阈值数组大小
	
	sportsInfo_t *SensorChanged(ADD_SPEED *pAccValue, timeStamp_t *timeStamp_p,personInfo_t *personInfo);
	sportsInfo_t *DetectorNewStep(float values,timeStamp_t *timeStamp_p,personInfo_t *personInfo);
	u8 DetectorPeak(float newValue,float oldValue);
	float Threshold_Value(float value);
	float AverageValue(float value[], int n);

#endif





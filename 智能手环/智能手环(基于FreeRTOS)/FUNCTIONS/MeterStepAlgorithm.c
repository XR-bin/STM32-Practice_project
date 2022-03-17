#include "stm32f4xx.h"
#include "MeterStepAlgorithm.h"
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


//用来装当前加速度经公式处理后的值  
float gravityNew = 0;
//上次的xyz加速度经公式处理后的值 
float gravityOld = 0; 
//当前波峰的时间  
long timeOfThisPeak = 0;  
//上一次波峰的时间  
long timeOfLastPeak = 0; 
//当前的时间  
long timeOfNow = 0; 

//上一点的状态，上升还是下降  
u8 lastStatus = FALSE;       //初始值，我们默认是下降状态
//当前的状态是否上升状态 
u8 isDirectionUp = FALSE;    //初始值，我们默认是下降状态
//持续上升次数  
int continueUpCount = 0;
//记录在下降趋势前持续上升的次数，为了记录波峰的上升次数  
int continueUpFormerCount = 0; 
//波峰值  
float peakOfWave = 0; 
//波谷值  
float valleyOfWave = 0; 

//阈值  
float ThreadValue = (float) 2.0;   //初始化阈值为2.0
//用于存放计算阈值的波峰波谷差值  
float tempValue[VALUE_NUM] ={0};
//当前阈值数组装了多少个元素
int tempCount = 0;
//动态阈值需要动态的数据，这个值用于这些动态数据的阈值  
float initialValue = (float) 1.3; 

//计步缓存(记录当前步数)
static u8 stepTempCount =0;
//行走信息:卡路里、里程、步数
static sportsInfo_t sportsInfo;

/*****************************************************************************************
*函数功能  ：将MPU6050的加速度数据通过算法得到我们要的步数、行走距离、消耗卡路里(这个函数只是个包装)
*函数名    ：SensorChanged 
*函数参数  ：ADD_SPEED *pAccValue, timeStamp_t *timeStamp_p,personInfo_t * personInfo
*函数返回值：sportsInfo_t
*描述      ：
*           参数1：xyz的角速度(结构体)  
*           参数2：RTC时间(结构体)
*           参数3：身体特征：身高、体重(结构体)
******************************************************************************************/
sportsInfo_t *SensorChanged(ADD_SPEED *pAccValue, timeStamp_t *timeStamp_p,personInfo_t *personInfo)
{
//	u8 buff[20];
	
	ADD_SPEED *p = pAccValue;   //间接变量
	
	//将加速度根据公式进行处理
	gravityNew = (float)sqrt(p->x_val*p->x_val + p->y_val*p->y_val + p->z_val*p->z_val);
	
//	sprintf((char *)buff,"Step:%0.2f",gravityNew);
//	OLED_ShowString(3,1,16,buff);
	
	return DetectorNewStep(gravityNew,timeStamp_p,personInfo);
}



/*****************************************************************************************
*函数功能  ：求步数、行走距离、消耗卡路里
*函数名    ：DetectorNewStep
*函数参数  ：float values,timeStamp_t *timeStamp_p,personInfo_t *personInfo
*函数返回值：sportsInfo_t
*描述      ：
*           参数1：经过公式处理后的xyz加速度的值 
*           参数2：RTC时间(结构体)
*           参数3：身体特征：身高、体重(结构体)
******************************************************************************************/
sportsInfo_t *DetectorNewStep(float values,timeStamp_t *timeStamp_p,personInfo_t *personInfo)
{
	personInfo_t *pI = personInfo;     //间接变量，取代我们传进的人体信息
	timeStamp_t *time_p = timeStamp_p; //间接变量，取代我们传进的RTC时间
	static u32 step_2second;           //记录每两秒所走的步数
	static u32 time_old;               //上一次的时间(也就是上一次进这个函数的时间)
	u32 time_now;                      //当前时间(也就是最新的时间)
	float step_lenth,walk_speed,walk_distance,Calories;//步长，步速，行走距离，消耗的卡路里
	
	if(gravityOld==0)
	{
		//获取当前的经过公式处理后的xyz加速度
		gravityOld = values;
	}
	else 
  { 
		if (DetectorPeak(values, gravityOld))//检测到波峰，出现峰值才执行里面的东西
		{
			//当前峰值时间与过去峰值时间进行交接
			timeOfLastPeak = timeOfThisPeak;//更新上次波峰的时间 
			time_now = timeOfNow = (((time_p->hour*60)+time_p->minute)*60+time_p->second)*1000+time_p->twentyMsCount*20; //获取时间 ,并转化为毫秒
			
			//如果检测到了波峰，并且符合时间差以及阈值的条件，则判定为1步
			//当前时间 - 上一次的峰值时间 大于 250         波峰值 - 波谷值 大于等于  阈值
			if((timeOfNow - timeOfLastPeak >= 250 )&&(peakOfWave - valleyOfWave >= ThreadValue))
			{
				timeOfThisPeak = timeOfNow; //更新当前波峰时间 
				stepTempCount++;            //加1为两步
				step_2second ++;            //加1为两步(这是用来计算步长用的)
				
				//判断是否已经两秒了
				if((time_now - time_old) >= 2000 ) 
				{
					//根据步数与步长的关系表得出以下关系
					if(step_2second == 1) step_lenth = pI->height/5;
					else if(step_2second == 2) step_lenth = pI->height/4;
					else if(step_2second == 3) step_lenth = pI->height/3;
					else if(step_2second == 4) step_lenth = pI->height/2;
					else if(step_2second == 5) step_lenth = pI->height/1.2f;
					else if(step_2second == 7) step_lenth = pI->height;
					else if(step_2second == 8) step_lenth = pI->height*1.2f;
					else step_lenth = 0;
					
					//步数 = 步数 * 步长 / 2秒
					walk_speed = step_2second * step_lenth/2;        //求出步数
					//行走距离 = 步数 * 步长
					walk_distance  = step_2second * step_lenth;      //求出行走距离
					//卡路里 = 4.5 * 步行速度 * (体重 / 2 )/1800
					Calories = 4.5f*walk_speed*(pI->weight/2)/1800;  //卡路里
					
					sportsInfo.calories  += Calories;
					sportsInfo.distance  += walk_distance;		
					time_old = time_now;         //更新时间
					step_2second = 0;            //记录每两秒所走的步数的变量清零
				}
				
				/************处理无效运动*************/
				/***********************************************************
				*1.连续记录5才开始计步 
				*2.例如记录的步用户停住超过3秒，则前面的记录失效，下次从头开始 
				*3.连续4记录了步用户还在运动，之前的数据才有效
				*************************************************************/
				//如果3秒多步数少于5，计数失败
				if ((stepTempCount< 5 )&&(timeOfNow - timeOfLastPeak >= 3000))          
				{
					//步数重新计数
					stepTempCount = 0;
				}
				//如果3秒内，步数超过5步，正常计数
				else if((stepTempCount>= 5)&&(timeOfNow - timeOfLastPeak <= 3000))
				{
					//步数计数
					sportsInfo.stepCount += stepTempCount;          
					stepTempCount         = 0; 				
				}
			}
			//判断两个峰值的时差大于250秒，且波峰与波谷差要大于1.3
			if ((timeOfNow - timeOfLastPeak >= 250) && (peakOfWave - valleyOfWave >= initialValue)) 
			{
				//更新时间
				timeOfThisPeak = timeOfNow;
				//获取阈值
				ThreadValue = Threshold_Value(peakOfWave - valleyOfWave);//更新阀值  
			}
		}
	}
	gravityOld = values;  
  return &sportsInfo;
}

/*****************************************************************************************
*函数功能  ：检查波峰
*函数名    ：DetectorPeak
*函数参数  ：float newValue, float oldValue
*函数返回值：u8
*描述      ：
*           参数1：当前经过公式处理后的xyz加速度的值 
*           参数2：上一次经过公式处理后的xyz加速度的值
*
*           以下四个条件判断为波峰：
*					 （1）目前点为下降的趋势：isDirectionUp为FALSE 
*					 （2）之前的点为上升的趋势：lastStatus为TRUE 
*					 （3）到波峰为止，持续上升大于等于2次
*					 （4）波峰值大于20 //Jahol:把这个值修改为15
*           记录波谷值：
*					 （1）观察波形图，可以发现在出现步子的地方，波谷的下一个就是波峰，有比较明显的特征以及差值 
*				   （2）所以要记录每次的波谷值，为了和下次的波峰做对比 	
*   
******************************************************************************************/
u8 DetectorPeak(float newValue,float oldValue)
{
	//当前状态与过去状态进行交接
	lastStatus = isDirectionUp;
	
	//判断当前的状态趋势
	if (newValue >= oldValue) //呈上升趋势
	{
		//当前状态变为上升
		isDirectionUp = TRUE;  
		//上升次数加1
    continueUpCount++; 
	}
	else				              //呈下降趋势
	{
		//当前状态变为下降
		isDirectionUp = FALSE;
		//获取下降趋势前的上升次数
		continueUpFormerCount = continueUpCount;
		//上升次数记录清零
		continueUpCount = 0;
	}
	
	//如果当前是下降趋势，而上一次是上升趋势，且在下降趋势前的上升次数大于2、上次经公式处理的xyz加速度值要大于20
	if ((!isDirectionUp) && lastStatus && ((continueUpFormerCount >= 2) && (oldValue >= 20)))
	{
		//将上一次的经公式处理的xyz加速度值作为波峰值
		peakOfWave = oldValue;
		return 1;
	}
	//如果上一次是下降趋势，而当前是上升趋势，
	else if ((!lastStatus) && isDirectionUp) 
  {  
		//将上一次的经公式处理的xyz加速度值作为波谷值
    valleyOfWave = oldValue;  
    return 0;  
  }
	//如果上一次和当前的状态同为上升或下降
	else
	{
		//不做任何处理
		return 0;
	}
	
}

/*****************************************************************************************
*函数功能  ：阈值的计算
*函数名    ：Threshold_Value
*函数参数  ：float value
*函数返回值：float
*描述      ：
*            参数：波峰与波谷的差
******************************************************************************************/
float Threshold_Value(float value)
{
	u8 i;
	float tempThread = ThreadValue;
	
	//判断数组是否装满
	if (tempCount < VALUE_NUM)
  {  
    tempValue[tempCount] = value;  
    tempCount++;  
  } 
	else
	{
		tempThread = AverageValue(tempValue, VALUE_NUM);//计算阀值
		
		//更新数组
		for ( i = 1;i < VALUE_NUM;i++)//线性移位更新
    {  
      tempValue[i - 1] = tempValue[i];  
    }  
    tempValue[VALUE_NUM - 1] = value; 
	}
	return tempThread;
}

/*****************************************************************************************
*函数功能  ：梯度化阈值
*函数名    ：AverageValue
*函数参数  ：float value[] , int n
*函数返回值：float
*描述      ：
*             参数1：阈值数组
*             参数2：要求平均值的元素个数
*
*             1.计算数组的均值 
*			        2.通过均值将阈值梯度化在一个范围里 
******************************************************************************************/
float AverageValue(float value[], int n)
{
	float ave = 0;  
  u8 i =0;
	
  for (i=0;i<n;i++)
  {  
    ave += value[i];//求和  
  }
	
	ave = ave/VALUE_NUM;//求平均值 
	
	if (ave >= 8)  
    ave = (float) 4.3; 
  else if (ave >= 7 && ave < 8)  
    ave = (float) 3.3;  
  else if (ave >= 4 && ave < 7)  
    ave = (float) 2.3;  
  else if (ave >= 3 && ave < 4)  
    ave = (float) 2.0;  
  else 
    ave = (float) 1.3;  
  
  return ave; 
}





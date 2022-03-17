#include "sys.h"
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
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/**************开始任务******************/
void start_task(void *pvParameters);  //开始任务的函数
#define START_TASK_SIZE 		128       //开始任务的堆栈大小     n*4个字节
#define START_TASK_PRIO		   1        //开始任务的优先级
TaskHandle_t   StartTask_Handler;     //创建开始任务函数句柄

/**************按键选择任务******************/
void pick_task(void *pvParameters);  //按键选择任务函数
#define PICK_TASK_SIZE 		 50        //按键选择任务堆栈大小     n*4个字节
#define PICK_TASK_PRIO		 2         //按键选择任务优先级
TaskHandle_t   PickTask_Handler;     //创建按键选择任务函数句柄

/**************手环功能任务******************/
void func_task(void *pvParameters);  //手环功能任务函数
#define FUNC_TASK_SIZE 		128        //手环功能任务堆栈大小     n*4个字节
#define FUNC_TASK_PRIO		 3         //手环功能任务优先级
TaskHandle_t   FuncTask_Handler;     //创建手环功能任务函数句柄

/**************实时计步任务******************/
void count_task(void *pvParameters);  //实时计步任务函数
#define COUNT_TASK_SIZE 	100         //实时计步任务堆栈大小     n*4个字节
#define COUNT_TASK_PRIO		 4          //实时计步任务优先级
TaskHandle_t   CountTask_Handler;     //创建实时计步任务函数句柄

//消息队列句柄
QueueHandle_t Key_Queue;   		//按键值消息队列句柄

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init();		      //初始化延时函数
	Watch_Init();         //开机界面
	
	//创建开始任务
  xTaskCreate((TaskFunction_t) start_task,            //任务函数
							(char *        ) "start_task",          //任务名
							(uint16_t      ) START_TASK_SIZE,       //任务堆栈大小
							(void *        ) NULL,                  //传递给任务函数的参数
							(UBaseType_t   ) START_TASK_PRIO,       //任务优先级
							(TaskHandle_t *) &StartTask_Handler );  //任务句柄    
							
	vTaskStartScheduler();                             //开启任务调度
}
 
//开始任务任务函数
void start_task(void *pvParameters)
{
	//进入临界区防止以下代码执行时被打断
	taskENTER_CRITICAL();           //进入临界区
	
	//创建消息队列
	//xQueueCreate函数的参数
	//参数1：消息队列长度(也就是消息个数)
	//参数2：消息的长度(大小)，以字节为单位
	Key_Queue=xQueueCreate(1,1);        //创建消息Key_Queue
	
	//按键选择任务
	xTaskCreate((TaskFunction_t) pick_task,            //任务函数
							(char *        ) "pick_task",          //任务名
							(uint16_t      ) PICK_TASK_SIZE,       //任务堆栈大小
							(void *        ) NULL,                 //传递给任务函数的参数
							(UBaseType_t   ) PICK_TASK_PRIO,       //任务优先级
							(TaskHandle_t *) &PickTask_Handler );  //任务句柄   
	//手环功能任务
	xTaskCreate((TaskFunction_t) func_task,            //任务函数
							(char *        ) "func_task",          //任务名
							(uint16_t      ) FUNC_TASK_SIZE,       //任务堆栈大小
							(void *        ) NULL,                 //传递给任务函数的参数
							(UBaseType_t   ) FUNC_TASK_PRIO,       //任务优先级
							(TaskHandle_t *) &FuncTask_Handler );  //任务句柄
	//实时计步任务
	xTaskCreate((TaskFunction_t) count_task,           //任务函数
							(char *        ) "count_task",         //任务名
							(uint16_t      ) COUNT_TASK_SIZE,      //任务堆栈大小
							(void *        ) NULL,                 //传递给任务函数的参数
							(UBaseType_t   ) COUNT_TASK_PRIO,      //任务优先级
							(TaskHandle_t *) &CountTask_Handler ); //任务句柄
	
	vTaskDelete(StartTask_Handler); //删除开始任务
	
	taskEXIT_CRITICAL();            //退出临界区
}


/****************按键选择任务*******************/
void pick_task(void *pvParameters)
{
	u8 temp = 0;
	Watch_Menu();         //界面初始化
	while(1)
	{
		//按键扫描
		temp = Select_Menu();
		
		if((Key_Queue != NULL)&&(temp != 0))
		{
			xQueueSend(Key_Queue,&temp,0);     //消息入队 
			vTaskSuspend( NULL );              //挂起功能选择任务，停止运行 
		}
		else
			vTaskDelay(50);
	}
}

/****************手环功能任务*******************/
void func_task(void *pvParameters)
{
	u8 key = 0;
	
	while(1)
	{
		if((Key_Queue!=NULL) && (xQueueReceive(Key_Queue,&key,1)))
		{
			switch(key)
			{
				case 1:Bp_Function();         key=0;break;
				case 2:Rate_Function();       key=0;break;
				case 3:Temperature_Function();key=0;break;
				case 4:Humidity_Function();   key=0;break;
				case 5:RTC_Function();        key=0;break;
				case 6:Step_Function();       key=0;break;
			}
			vTaskResume(PickTask_Handler);   //恢复功能选择任务，停止运行        
		}
	}
}

/****************实时计步任务*******************/
void count_task(void *pvParameters)
{
	while(1)
	{
		motion_Scan();
		vTaskDelay(10);
	}
}





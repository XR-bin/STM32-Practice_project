#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "motor.h"
#include "printer.h"
#include "w25q64.h"

u8 buff[500] = {'\r','\n',0};

/******************************************串口中断******************************************/
/******************************************************************
*函数功能  ：串口1的中断服务函数
*函数名    ：USART1_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void USART1_IRQHandler(void)
{
  static u8 i;
  
  //判断是由什么形式触发中断
	//接收完成中断    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //中断要执行的任务
    buff[i] = USART1->DR;
		i++;
    LED_ON;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART1->SR;
    USART1->DR;
    
		buff[i]   = '\r';
		buff[i+1] = '\n';
    i = 0;
		LED_OFF;
  }
}

/******************************************************************
*函数功能  ：串口2的中断服务函数(测试蓝牙)
*函数名    ：USART2_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
//void USART2_IRQHandler(void)
//{
//  static u8 i = 0;
//  static u8 str[200];
//  
//  //判断是由什么形式触发中断
//	//接收完成中断    SET=1   RESET=0
//  if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
//  {
//    //中断要执行的任务
//    str[i] = USART2->DR;
//		i++;
//  }
//	//空闲中断        SET=1   RESET=0
//  if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)   
//  {
//		//清除标志
//    USART2->SR;
//    USART2->DR;
//		
//		str[i] = '\0';
//    printf("你发送的数据是：%s\r\n",str);
//    i = 0;
//  }
//}

/******************************************************************
*函数功能  ：串口1的中断服务函数
*函数名    ：USART1_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void USART2_IRQHandler(void)
{
  static u8 i;
  
  //判断是由什么形式触发中断
	//接收完成中断    SET=1   RESET=0
  if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
  {
    //中断要执行的任务
    buff[i] = USART2->DR;
		i++;
    LED_ON;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART2->SR;
    USART2->DR;
    
		buff[i]   = '\r';
		buff[i+1] = '\n';
    i = 0;
		LED_OFF;
  }
}

/******************************************定时器中断******************************************/
/******************************************
*函数功能  ：定时器3的中断服务函数
*函数名    ：TIM3_IRQHandler 
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*********************************************/
void TIM3_IRQHandler(void)
{
	static u32 i = 0;
  //检查TIM3更新中断发生与否
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    //清除TIMx更新中断标志 
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
    Motor_Run();//开启电机转动
		if (i % 2 == 0)
		{
			Printer_Left(); //先打印左边
		}
		else
		{
			Printer_Right(); //后打印右边
		}
		i++;
		if(i >= 3)     //800ms
		{
			i = 0;
			p_flag = 1;
		}
  }
}




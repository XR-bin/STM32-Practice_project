#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "motor.h"
#include "printer.h"
#include "w25q64.h"

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
  u8 data;
  static u32 addr = 0x0b0000;
  
  //判断是由什么形式触发中断
	//接收完成中断    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //中断要执行的任务
    data = USART1->DR;
    W25Q64_Page_Write(addr,1,&data);
    addr++;
    LED_ON;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART1->SR;
    USART1->DR;
    
    LED_OFF;
  }
}

/******************************************************************
*函数功能  ：串口2的中断服务函数
*函数名    ：USART2_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void USART2_IRQHandler(void)
{
  static u8 i = 0;
  static u8 str[200];
  
  //判断是由什么形式触发中断
	//接收完成中断    SET=1   RESET=0
  if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
  {
    //中断要执行的任务
    str[i] = USART2->DR;
		i++;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART2->SR;
    USART2->DR;
		
		str[i] = '\0';
    printf("你发送的数据是：%s\r\n",str);
    i = 0;
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
    //中断事件
    Motor_Run();
		
		if (i  == 1)
		{
			LAT_H;
			LAT_L;
			LAT_H;	//锁存输出数据
			Printer_Left();
			p_flag = 1;//打印标志置1，更新下一行数据
		}
		else if(i == 2)
		{
			Printer_Right();
		}
		i++;
		if(i == 3)
		{
			i =1;
		}
  }
}




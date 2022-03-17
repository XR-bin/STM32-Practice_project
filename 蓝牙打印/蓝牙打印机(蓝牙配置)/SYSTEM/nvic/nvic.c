#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

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
  static u8 i = 0;
  static u8 str[200];
	
  //判断是由什么形式触发中断
	//接收完成中断    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //中断要执行的任务
    str[i] = USART1->DR;
		i++;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART1->SR;
    USART1->DR;
		
		str[i] = '\0';
		
		USART2_Send_Str(str);
		
		i = 0;
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

/******************************************外部中断******************************************/
/******************************************************************
*函数功能  ：外部中断4服务函数
*函数名    ：EXTI0_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void EXTI4_IRQHandler(void)
{
	//判断外部中断线0是否真的触发   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
  {
    delay_ms(10);
		//滤波
    if(!(GPIOE->IDR & (1<<4)))
    {
      EXTI->PR |= (1<<4);
      GPIOB->ODR ^= (1<<5);
    }
  }
}

/******************************************************************
*函数功能  ：软件外部中断发初始化
*函数名    ：EXTI9_5_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
             无需端口
*******************************************************************/
void EXTI9_5_IRQHandler(void)
{
	//判断外部中断线5是否真的触发   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
  {
    EXTI->PR |= (1<<5);
		GPIOB->ODR ^= (1<<5);
  }
}

/******************************************ADC中断******************************************/
/******************************************************************
*函数功能  ：ADC3中断服务函数
*函数名    ：ADC3_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void ADC3_IRQHandler(void)
{
	u16 data;
	
	//是否转换完成
	if(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET)
	{
		//读取转换后的数据(ADC_DR寄存器)
		data = ADC_GetConversionValue(ADC3);
		printf("data:%d\r\n",data);
	}
	//清除标志
	ADC_ClearITPendingBit(ADC3,ADC_IT_EOC);
	//开启规则通道转换开关(ADC_CR2寄存器的22位)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);	
}



/******************************************定时器中断******************************************/
/******************************************
*函数功能  ：定时器6的中断服务函数
*函数名    ：TIM6_DAC_IRQHandler ：
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM6_IRQHandler(void)
{
	//检查TIM6更新中断发生与否
  if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  { 
    //中断事件
    GPIOE->ODR ^= (1<<5);
  }
	//清除TIMx更新中断标志 
  TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}

/******************************************
*函数功能  ：定时器7的中断服务函数
*函数名    ：TIM7_IRQHandler ：
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM7_IRQHandler(void)
{
  static u8 temp = 0;
  
	//判断是否定时器7中断
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		//紧急事件
		if(temp == 0)
    {
      if(TIM3->CCR2 == 0) temp = 1;
      else TIM3->CCR2 -=20;
    }
    if(temp == 1)
    {
      if(TIM3->CCR2 == 1000) temp = 0;
      else TIM3->CCR2 +=20;
    }
	}
	//清除TIMx更新中断标志 
  TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
}

/*********************************************
*函数功能  ：定时器4中断函数
*函数名    ：TIM1_BRK_TIM9_IRQHandler ：
*函数参数  ：void     
*函数返回值：void
*函数描述  ：
* 					 判断是定时9的通道1的重复捕获标志
* 							清标志位
* 							读CCR1
*
* 					 判断是定时9的通道2的捕获标志
* 							清标志位
*               读CCR2
*********************************************/
void TIM4_IRQHandler(void)
{
	static u16 data_2;
	u16 data_1;
	u16 p_val;
	
	if(TIM_GetITStatus(TIM4,TIM_FLAG_CC2) != RESET)
	{
		//清除捕通道IC2的获标志位
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC2);
		//紧急事件
		//CCR2
		data_2 = TIM_GetCapture2(TIM4);	
	}
	
	if(TIM_GetITStatus(TIM4,TIM_FLAG_CC1) != RESET)
	{
		//清除通道1的捕获标志位
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC1);
		//清除通道1重复捕获标志位
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_CC1OF);
		//紧急事件
		//记录值CCR1
		data_1 = TIM_GetCapture1(TIM4);
		//计算周期和占空比
		p_val = (float)data_2/data_1*100;
		
		printf("周期：%d   高电平：%d   占空比：%d%%\r\n",data_1,data_2,p_val);	
	}
}




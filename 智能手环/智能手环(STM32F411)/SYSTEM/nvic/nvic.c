#include "stm32f4xx.h"  
#include "stdio.h"
#include "nvic.h"
#include "led.h"
#include "MeterStepAlgorithm.h"
#include "mpu6050.h"

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
    //执行中断任务
    str[i] = USART_ReceiveData(USART1); //读取一个字节的数据(USART1->DR)
    i++;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART1->SR;
    USART1->DR;
    
    str[i] = '\0';
    printf("你发送的数据是：%s\r\n",str);
    i = 0;
  }
}


/******************************************ADC******************************************/
/******************************************************************
*函数功能  ：ADC中断服务函数
*函数名    ：ADC_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void ADC_IRQHandler(void)
{
  u16 data;
  if(ADC_GetITStatus(ADC1,ADC_IT_EOC) == SET)
  { 
    //读取转换后的数据(ADC1->DR)
		data = ADC_GetConversionValue(ADC1);
    printf("data:%d\r\n",data);
  }
	//清除标志
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
  //开启规则通道转换开关(ADC1->CR2寄存器的第30位)
	ADC_SoftwareStartConv(ADC1);
}

/******************************************TIMx定时器******************************************/
/******************************************
*函数功能  ：定时器9的中断服务函数
*函数名    ：TIM1_BRK_TIM9_IRQHandler
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM1_BRK_TIM9_IRQHandler(void)
{
	//判断是否定时器9中断
	if(TIM_GetITStatus(TIM9,TIM_FLAG_Update)==SET)
	{ 
		//紧急事件
		GPIOA->ODR ^= 1 << 7;
		//清除中断标志位
		TIM_ClearITPendingBit(TIM9,TIM_FLAG_Update);
	}
}

/******************************************
*函数功能  ：定时器10的中断服务函数
*函数名    ：TIM1_UP_TIM10_IRQHandler
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM1_UP_TIM10_IRQHandler(void)
{
	//判断是否定时器10中断
	if(TIM_GetITStatus(TIM10,TIM_FLAG_Update)==SET)
	{ 
		//紧急事件
		GPIOA->ODR ^= 1 << 7;
		//清除中断标志位
		TIM_ClearITPendingBit(TIM10,TIM_FLAG_Update);
	}
}

/******************************************
*函数功能  ：定时器3的中断服务函数
*函数名    ：TIM3_IRQHandler
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM3_IRQHandler(void)
{
	//判断是否定时器3中断
	if(TIM_GetITStatus(TIM3,TIM_FLAG_Update)==SET)
	{ 
		//紧急事件
		GPIOA->ODR ^= 1 << 7;
		//清除中断标志位
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
	}
}

/******************************************
*函数功能  ：定时器4的中断服务函数
*函数名    ：TIM4_IRQHandler
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM4_IRQHandler(void)
{
	//判断是否定时器4中断
	if(TIM_GetITStatus(TIM4,TIM_FLAG_Update)==SET)
	{ 
		//紧急事件
		
		//清除中断标志位
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
	}
}


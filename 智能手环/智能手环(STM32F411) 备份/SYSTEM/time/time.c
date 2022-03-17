#include "stm32f4xx.h"  

/*****************************************************
*函数功能  ：对定时器9进行初始化设置
*函数名    ：TIM9_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM9_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(中断)配置结构体
	
	//时钟使能  TIM9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(基本定时器只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)(注意：基本定时器没有这个)	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStruct);            //初始化TIM9
	//TIMx中断选择并使能
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE); //允许定时器9更新中断
	
	//TIM10中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;    //TIM9中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //抢占优先级3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //子优先级3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                           //根据指定的参数初始化VIC寄存器
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM9, ENABLE);			            // 使能TIM9重载寄存器ARR
	
	TIM_Cmd(TIM9,ENABLE); //使能定时器9
}

/*****************************************************
*函数功能  ：对定时器10进行初始化设置
*函数名    ：TIM10_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM10_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(中断)配置结构体
	
	//时钟使能  TIM9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(基本定时器只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)(注意：基本定时器没有这个)	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);            //初始化TIM10
	//TIMx中断选择并使能
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE); //允许定时器10更新中断
	
	//TIM10中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;    //TIM10中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //抢占优先级2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //子优先级2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                           //根据指定的参数初始化VIC寄存器
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM10, ENABLE);			            // 使能TIM10重载寄存器ARR
	
	TIM_Cmd(TIM10,ENABLE); //使能定时器10
}

/*****************************************************
*函数功能  ：对定时器4进行初始化设置
*函数名    ：TIM4_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(中断)配置结构体
	
	//时钟使能  TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(基本定时器只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)(注意：基本定时器没有这个)	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);            //初始化TIM4
	//TIMx中断选择并使能
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	
	//TIM10中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;             //TIM4中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //抢占优先级2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //子优先级2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                           //根据指定的参数初始化VIC寄存器
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM4, ENABLE);			                 // 使能TIM4重载寄存器ARR
	
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
}

/*****************************************************
*函数功能  ：对定时器4进行初始化设置
*函数名    ：TIM3_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;                  //NVIC(中断)配置结构体
	
	//时钟使能  TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(基本定时器只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)(注意：基本定时器没有这个)	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);            //初始化TIM3
	//TIMx中断选择并使能
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	
	//TIM10中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;             //TIM3中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;     //抢占优先级2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;		       //子优先级2
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			       //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                           //根据指定的参数初始化VIC寄存器
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM3, ENABLE);			                 // 使能TIM3重载寄存器ARR
	
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
}



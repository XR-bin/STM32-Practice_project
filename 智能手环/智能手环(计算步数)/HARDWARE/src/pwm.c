#include "stm32f4xx.h"   

/*****************************************************
*函数功能  ：对通用定时器2进行初始化设置并输出PWM
*函数名    ：TIM2_PWM_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过PWM来控制MOTOR的震动频率
*            PWM输出口 ： PB10   TIM2_CH3  输出通道3
********************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc,u16 ccr3)
{
	GPIO_InitTypeDef   GPIO_InitStruc;                   //GPIOx配置结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;      //TIMx配置结构体
	TIM_OCInitTypeDef  TIM_OCInitStruct;                 //通道配置结构体
	
  //时钟使能   GPIOB   TIM12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  //使能TIM2时钟
	
	/*GPIO口配置*/
	//PB10
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_10;              //选择PB10
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉下拉
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_100MHz;     	//速度100MHz
	GPIO_Init(GPIOB,&GPIO_InitStruc);                   //根据设定参数初始化PB15
	
	//TIM2对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);  //PB10 复用为TIM2
	
	//TIMx配置
  TIM_TimeBaseInitStruct.TIM_Period = arr; 	                 //自动重装载值(TIMx->ARR寄存器)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;                  //定时器分频(TIMx->PSC寄存器)
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式(TIMx->CR1寄存器的第4位)(通用定时器12只有递增计数)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //时钟分频:tDTS=tCK_INT(TIMx->CR1寄存器的第8-9位)
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);            //初始化TIM2
	
	//PWM模式配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //配置为PWM模式1(TIMx_CCMR1寄存器的输出模式的第4-6位)
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //通道选择输出模式(TIMx->CCER寄存器的第0位)
	TIM_OCInitStruct.TIM_Pulse = ccr3;	                       //设置比较寄存器(TIMx->CCR1)
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;      //设置有效电平为低电平(TIMx->CCER寄存器的第1位)
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);                      //通道3初始化配置
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);          //通道3比较寄存器影子寄存器使能
 
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM2, ENABLE);			                  // 使能TIM2重载寄存器ARR
	
	/*使能定时器2函数 */
	TIM_Cmd(TIM2, ENABLE);   //使能定时器2
}











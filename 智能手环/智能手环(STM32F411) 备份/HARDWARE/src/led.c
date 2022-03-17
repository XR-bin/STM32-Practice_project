#include "stm32f4xx.h"  

/*****************************************************
*函数功能  ：对LED对应的GPIO口进行初始化设置
*函数名    ：LED_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*             LED ------ PA7
*            高电平灭，低电平亮-----输出模式
********************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruc;
  
  //CPIOx口时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  /*GPIOx端口配置*/
  //PA7
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_7;         //选择引脚7
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;      //通用输出
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //无上下拉
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_2MHz;    //速度为2MHz
  GPIO_Init(GPIOA,&GPIO_InitStruc);
  
  /*设置输出数据寄存器*/
  //先将LED都关闭
  GPIO_SetBits(GPIOA,GPIO_Pin_7);
}






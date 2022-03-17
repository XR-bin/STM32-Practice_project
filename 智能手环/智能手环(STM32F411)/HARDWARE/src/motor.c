#include "stm32f4xx.h"

/*****************************************************
*函数功能  ：对MOTOR对应的GPIO口进行初始化设置
*函数名    ：MOTOR_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*             MOTOR ------ PB10
*            高电不震，低电震动-----输出模式
********************************************************/
void MOTOR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;
  
  //CPIOx口时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  /*GPIOx端口配置*/
  //PA7
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_10;         //选择引脚7
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;      //通用输出
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //无上下拉
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_2MHz;    //速度为2MHz
  GPIO_Init(GPIOB,&GPIO_InitStruc);
  
  /*设置输出数据寄存器*/
  //先将MOTOR都关闭
  GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}






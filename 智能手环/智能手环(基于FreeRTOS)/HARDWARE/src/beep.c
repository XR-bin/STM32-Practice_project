#include "stm32f4xx.h"     
#include "beep.h"

/*****************************************************
*函数功能  ：对BEEP对应的GPIO口进行初始化设置
*函数名    ：BEEP_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             BEEP  PF8
             高电响，低电不响-----输出模式
********************************************************/
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruc;
  
  //时钟使能  GPIOF 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_8;       //PC0
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_OUT;    //通用输出
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL; //无上下拉
  GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;    //推挽输出
  GPIO_InitStruc.GPIO_Speed = GPIO_Speed_2MHz;  //速度为50MHz
  GPIO_Init(GPIOF,&GPIO_InitStruc);             //根据设定参数初始化PF8
	
	/*设置输出数据寄存器*/
  //先将LED都关闭
  GPIO_SetBits(GPIOF,GPIO_Pin_8);               //低电不响  
}

















#include "sys.h"
#include "delay.h"
#include "key.h"

/*****************************************************
*函数功能  ：对KEY对应的GPIO口进行初始化设置
*函数名    ：KEY_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            KEY     PB8     低电平按下，高电平抬起
*                                 -------------输出模式
********************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx配置结构体
	
	/*GPIOx设置*/
  //GPIOx时钟使能  GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  //GPIOx配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	       //PB8端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 	 //上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化PB8  
}


/*****************************************************
*函数功能  ：按键扫描函数
*函数名    ：KEY_Scan
*函数参数  ：void
*函数返回值：u8
*描述      ：
*            KEY      PB8     低电平按下，高电平抬起
*                                 -------------输出模式
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  if(key_flay && (!KEY))
  {
    delay_ms(10);
    if(!KEY)
    {
      key_flay = 0;
			temp = 1;
    }
  }
	
  if(KEY) key_flay = 1;
	
  return temp;
}













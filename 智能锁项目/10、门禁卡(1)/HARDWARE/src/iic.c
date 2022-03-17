#include "sys.h"
#include "iic.h"
#include "delay.h"

/*****************************************************
*函数功能  ：对模拟IIC所需要的GPIO口进行初始化设置
*函数名    ：IIC_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*          时钟线  IIC_SCL  PB6   推挽输出
*          数据线  IIC_SDA  PB7   开漏输出
*                 高电平处于空闲状态
********************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;   //GPIOx配置结构体	

	//时钟使能   GPIOB   GPIOC 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

	//PB6
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;				     //PB6 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //根据设定参数初始化PB6
	//PB7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;				     //PB6 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStruct);					       //根据设定参数初始化PB7
	
	//端口输出数据寄存器      高电平空闲
	GPIO_SetBits(GPIOB,GPIO_Pin_6);						             //PB6 输出高
	GPIO_SetBits(GPIOE,GPIO_Pin_7); 						           //PB7 输出高 
}

/*****************************************************
*函数功能  ：模拟IIC的起始信号
*函数名    ：IIC_Start
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SCL_H       //时钟线拉高
*            SCL_L       //时钟线拉低
*            SDA_OUT_H   //输出模式下数据线拉高
*            SDA_OUT_L   //输出模式下数据线拉低
*            SDA_IN      //输入模式
*            SDA_INT     //接收到的值
********************************************************/
void IIC_Start(void)
{
 //时钟线拉低
  SCL_L;
 //数据线拉高
  SDA_OUT_H;
 //时钟线拉高
  SCL_H;
 //延时5us
  delay_us(5);
 //数据线拉低
  SDA_OUT_L;
 //延时5us
  delay_us(5);
 //时钟线拉低0
  SCL_L;
}

/******************************************************
*函数功能  ：模拟IIC的停止信号
*函数名    ：IIC_Stop
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SCL_H       //时钟线拉高
*            SCL_L       //时钟线拉低
*            SDA_OUT_H   //输出模式下数据线拉高
*            SDA_OUT_L   //输出模式下数据线拉低
*            SDA_IN      //输入模式
*            SDA_INT     //接收到的值
********************************************************/
void IIC_Stop(void)
{
 //时钟线拉低
  SCL_L;
 //数据线拉低
  SDA_OUT_L;
 //时钟线拉高
  SCL_H;
 //延时5us
  delay_us(5);
 //数据线拉高
  SDA_OUT_H;
 //延时5us
  delay_us(5);
}

/*****************************************************
*函数功能  ：模拟IIC的主机发送应答信号
*函数名    ：IIC_Send_Ack
*函数参数  ：u8 ack
*函数返回值：void
*描述      ：
*            SCL_H       //时钟线拉高
*            SCL_L       //时钟线拉低
*            SDA_OUT_H   //输出模式下数据线拉高
*            SDA_OUT_L   //输出模式下数据线拉低
*            SDA_IN      //输入模式
*            SDA_INT     //接收到的值
*            低电平  0   //应答   
*            高电平  1   //不应答
********************************************************/
void IIC_Send_Ack(u8 ack)
{
 //时钟线拉低
  SCL_L;
 //延时5us
  delay_us(5);
 /*判断是否要发送应答信号*/
 //发送不应答信号
  if(ack == 1)
  {
   //数据线拉高
    SDA_OUT_H;
   //延时1us
    delay_us(1);
   //时钟线拉高
    SCL_H;
  }
 //发送应答信号
  else if(ack == 0)
  {
   //数据线拉低
    SDA_OUT_L;
   //延时1us
    delay_us(1);
   //时钟线拉高
    SCL_H;
  }
  
 //延时5us
  delay_us(5);
 //时钟线拉低
  SCL_L;
}

/*****************************************************
*函数功能  ：模拟IIC的主机接收应答信号
*函数名    ：IIC_Receive_Ack
*函数参数  ：void
*函数返回值：u8
*描述      ：
*            SCL_H       //时钟线拉高
*            SCL_L       //时钟线拉低
*            SDA_OUT_H   //输出模式下数据线拉高
*            SDA_OUT_L   //输出模式下数据线拉低
*            SDA_IN      //输入模式
*            SDA_INT     //接收到的值
*            低电平  0   //应答   
*            高电平  1   //不应答
********************************************************/
u8 IIC_Receive_Ack(void)
{
 //时钟线拉低
  SCL_L;
 //进入输入模式
  SDA_IN;
 //延时5us
  delay_us(5);
 //时钟线拉高
  SCL_H;
 /*判断从机发送的应答信号*/
 //从机不应答
  if(SDA_INT)
  {
   //时钟线拉低
    SCL_L;
   //返回1
    return 1;
  }
 //从机应答
 //时钟线拉低
  SCL_L;
 //返回0
  return 0;
}

/*****************************************************
*函数功能  ：模拟IIC的主机发送一个字节的数据
*函数名    ：IIC_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
*            SCL_H       //时钟线拉高
*            SCL_L       //时钟线拉低
*            SDA_OUT_H   //输出模式下数据线拉高
*            SDA_OUT_L   //输出模式下数据线拉低
*            SDA_IN      //输入模式
*            SDA_INT     //接收到的值
********************************************************/
void IIC_Send_Byte(u8 data)
{
  u8 i;
  
  /*for语句循环发送*/
  for(i=0;i<8;i++)
  {
   //时钟线拉低
    SCL_L;
   //延时5us
    delay_us(5);
   /*判断发送是0还是1*/   //高低电平
   //发送1
    if(data & 0x80)
    {
     //数据线拉高
      SDA_OUT_H;
    }
   //发送0
    else
    {
     //数据线拉低
      SDA_OUT_L;
    }
   //延时1us
    delay_us(1);
   //时钟线拉高
    SCL_H;
   //延时5us
    delay_us(5);
   //data左移一位
    data <<= 1;
  }
  
 //时钟线拉低
  SCL_L;
}

/*****************************************************
*函数功能  ：模拟IIC的主机接收一个字节的数据
*函数名    ：IIC_Receive_Byte
*函数参数  ：void
*函数返回值：u8
*描述      ：
*            SCL_H       //时钟线拉高
*            SCL_L       //时钟线拉低
*            SDA_OUT_H   //输出模式下数据线拉高
*            SDA_OUT_L   //输出模式下数据线拉低
*            SDA_IN      //输入模式
*            SDA_INT     //接收到的值
********************************************************/
u8 IIC_Receive_Byte(void)
{
  u8 i;
  u8 data = 0;
  
 //拉低时钟线
  SCL_L;
 //进入接收模式
  SDA_IN;
 //for语句接收数据
  for(i=0;i<8;i++)
  {
   //时钟线拉低
    SCL_L;
   //data左移一位
    data <<= 1;
   //延时5us
    delay_us(5);
   //时钟线拉高
    SCL_H;
   /*判断接收的是1还是0*/    //高低电平
   //接收的是1
    if(SDA_INT)
    {
     //data | 0x1
      data |= 0x1;
    }
   //接收的是0 不做处理
  }
 //时钟线拉低
  SCL_L;
  
  return data;
}










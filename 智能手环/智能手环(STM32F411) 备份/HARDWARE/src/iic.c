#include "stm32f4xx.h"  
#include "iic.h"

//延时函数微秒
static void delay_us(u32 us)
{
	u32 i = 168 / 4 * us;    //由系统时钟频率决定的
	while(i)
	{
		i--;
	}
}

/*****************************************************
*函数功能  ：对模拟IIC所需要的GPIO口进行初始化设置
*函数名    ：IIC_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*          时钟线  IIC_SCL  PB8   推挽输出
*          数据线  IIC_SDA  PB9   开漏输出
*                 高电平处于空闲状态
********************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;       //GPIOx配置结构体

	//时钟使能  GPIOB 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /*GPIOx端口配置*/
  //PB8
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;             //选择PB8
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;         //普通输出
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;        //推挽输出
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;    //速度为100MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                //根据设定参数初始化PB8
	//PB9
	//因为配置一样，所以一些变量无需改变
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;             //选择PB9
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;        //开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStruct);                //根据设定参数初始化PB9
	
	//先将时钟线和数据线设置空闲
	GPIO_SetBits(GPIOB,GPIO_Pin_8);     //时钟线拉高
	GPIO_SetBits(GPIOB,GPIO_Pin_9);     //数据线拉高
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
 //时钟线拉低
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
 /*判断主机是否发送应答*/
 //时钟线拉低
  SCL_L;
 //延时1us
  delay_us(5);
 if(ack == 1)//主机发送不应答
 {
  //数据线拉高
   SDA_OUT_H;
  //延时1us
   delay_us(1);
  //时钟线拉高
   SCL_H;
 }
 else if(ack == 0)//主机发送应答
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
 //时钟拉低
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
 //接收模式
  SDA_IN;
 //延时5us
  delay_us(5);
 //时钟线拉高
  SCL_H;
 /*判断从机是否应答*/
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
  
  /*for语句*/
  for(i=0;i<8;i++)
  {
   //时钟线拉低
    SCL_L;
   //延时5us
    delay_us(5);
   /*发送数据*/
   //判断数据最高位是1还是0
    if(data & 0x80)
    {
     //是1的话数据线拉高
      SDA_OUT_H;
    }
    else
    {
     //是0的话数据线拉低
      SDA_OUT_L;
    }
   //延时1us
    delay_us(1);
   //时钟线拉高
    SCL_H;
   //延时5us
    delay_us(5);
   //data左移1位
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
  
 //时钟线拉低
  SCL_L;
 //进入输入模式
  SDA_IN;
  /*for语句*/
  for(i=0;i<8;i++)
  {
   //时钟线拉低
    SCL_L;
   //延时5us
    delay_us(5);
   //data向左移1位
    data <<= 1;
   //时钟线拉高
    SCL_H;
   /*判断数据线接收的是1还是0*/
   //如果是1，则data | 0x1
    if(SDA_INT)
    {
      data |= 0x1;
    }
  }
  
  //时钟线拉低
  SCL_L;
  
  return data;
}



/************************************hp6*********************************************/
/*****************************************************
*函数功能  ：对模拟IIC所需要的GPIO口进行初始化设置
*函数名    ：IIC_Hp6_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*          时钟线  IIC_SCL  PA2   推挽输出
*          数据线  IIC_SDA  PA1   开漏输出
*                 高电平处于空闲状态
********************************************************/
void IIC_Hp6_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;       //GPIOx配置结构体

	//时钟使能  GPIOB 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /*GPIOx端口配置*/
  //PA2
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;             //选择PA2
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;         //普通输出
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;        //推挽输出
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;    //速度为100MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                //根据设定参数初始化PA2
	//PA1
	//因为配置一样，所以一些变量无需改变
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;             //选择PA1
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;        //开漏输出
	GPIO_Init(GPIOA, &GPIO_InitStruct);                //根据设定参数初始化PA1
	
	//先将时钟线和数据线设置空闲
	GPIO_SetBits(GPIOA,GPIO_Pin_2);     //时钟线拉高
	GPIO_SetBits(GPIOA,GPIO_Pin_1);     //数据线拉高
}	

/*****************************************************
*函数功能  ：模拟IIC的起始信号
*函数名    ：IIC_Hp6_Start
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SCL_HP6_H       //时钟线拉高
*            SCL_HP6_L       //时钟线拉低
*            SDA_HP6_OUT_H   //输出模式下数据线拉高
*            SDA_HP6_OUT_L   //输出模式下数据线拉低
*            SDA_HP6_IN      //输入模式
*            SDA_HP6_INT     //接收到的值
********************************************************/
void IIC_Hp6_Start(void)
{
 //时钟线拉低
  SCL_HP6_L;
 //数据线拉高
  SDA_HP6_OUT_H;
 //时钟线拉高
  SCL_HP6_H;
 //延时5us
  delay_us(5);
 //数据线拉低
  SDA_HP6_OUT_L;
 //延时5us
  delay_us(5);
 //时钟线拉低
  SCL_HP6_L;
}

/******************************************************
*函数功能  ：模拟IIC的停止信号
*函数名    ：IIC_Hp6_Stop
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SCL_HP6_H       //时钟线拉高
*            SCL_HP6_L       //时钟线拉低
*            SDA_HP6_OUT_H   //输出模式下数据线拉高
*            SDA_HP6_OUT_L   //输出模式下数据线拉低
*            SDA_HP6_IN      //输入模式
*            SDA_HP6_INT     //接收到的值
********************************************************/
void IIC_Hp6_Stop(void)
{
 //时钟线拉低
  SCL_HP6_L;
 //数据线拉低
  SDA_HP6_OUT_L;
 //时钟线拉高
  SCL_HP6_H;
 //延时5us
  delay_us(5);
 //数据线拉高
  SDA_HP6_OUT_H;
 //延时5us
  delay_us(5);
}

/*****************************************************
*函数功能  ：模拟IIC的主机发送应答信号
*函数名    ：IIC_Hp6_Send_Ack
*函数参数  ：u8 ack
*函数返回值：void
*描述      ：
*            SCL_HP6_H       //时钟线拉高
*            SCL_HP6_L       //时钟线拉低
*            SDA_HP6_OUT_H   //输出模式下数据线拉高
*            SDA_HP6_OUT_L   //输出模式下数据线拉低
*            SDA_HP6_IN      //输入模式
*            SDA_HP6_INT     //接收到的值
*            低电平  0   //应答   
*            高电平  1   //不应答
********************************************************/
void IIC_Hp6_Send_Ack(u8 ack)
{
 /*判断主机是否发送应答*/
 //时钟线拉低
  SCL_HP6_L;
 //延时1us
  delay_us(5);
 if(ack == 1)//主机发送不应答
 {
  //数据线拉高
   SDA_HP6_OUT_H;
  //延时1us
   delay_us(1);
  //时钟线拉高
   SCL_HP6_H;
 }
 else if(ack == 0)//主机发送应答
 {
  //数据线拉低
   SDA_HP6_OUT_L;
  //延时1us
  delay_us(1);
  //时钟线拉高
   SCL_HP6_H;
 }
 //延时5us
  delay_us(5);
 //时钟拉低
  SCL_HP6_L;
}

/*****************************************************
*函数功能  ：模拟IIC的主机接收应答信号
*函数名    ：IIC_Hp6_Receive_Ack
*函数参数  ：void
*函数返回值：u8
*描述      ：
*            SCL_HP6_H       //时钟线拉高
*            SC_HP6L_L       //时钟线拉低
*            SDA_HP6_OUT_H   //输出模式下数据线拉高
*            SDA_HP6_OUT_L   //输出模式下数据线拉低
*            SDA_HP6_IN      //输入模式
*            SDA_HP6_INT     //接收到的值
*            低电平  0   //应答   
*            高电平  1   //不应答
********************************************************/
u8 IIC_Hp6_Receive_Ack(void)
{
 //时钟线拉低
  SCL_HP6_L;
 //接收模式
  SDA_HP6_IN;
 //延时5us
  delay_us(5);
 //时钟线拉高
  SCL_HP6_H;
 /*判断从机是否应答*/
 //从机不应答
  if(SDA_HP6_INT)
  {
   //时钟线拉低
    SCL_HP6_L;
   //返回1
    return 1;
  }
 //从机应答
 //时钟线拉低
  SCL_HP6_L;
 //返回0
  return 0;
}

/*****************************************************
*函数功能  ：模拟IIC的主机发送一个字节的数据
*函数名    ：IIC_Hp6_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
*            SCL_HP6_H       //时钟线拉高
*            SCL_HP6_L       //时钟线拉低
*            SDA_HP6_OUT_H   //输出模式下数据线拉高
*            SDA_HP6_OUT_L   //输出模式下数据线拉低
*            SDA_HP6_IN      //输入模式
*            SDA_HP6_INT     //接收到的值
********************************************************/
void IIC_Hp6_Send_Byte(u8 data)
{
  u8 i;
  
  /*for语句*/
  for(i=0;i<8;i++)
  {
   //时钟线拉低
    SCL_HP6_L;
   //延时5us
    delay_us(5);
   /*发送数据*/
   //判断数据最高位是1还是0
    if(data & 0x80)
    {
     //是1的话数据线拉高
      SDA_HP6_OUT_H;
    }
    else
    {
     //是0的话数据线拉低
      SDA_HP6_OUT_L;
    }
   //延时1us
    delay_us(1);
   //时钟线拉高
    SCL_HP6_H;
   //延时5us
    delay_us(5);
   //data左移1位
    data <<= 1;
  }
  
  //时钟线拉低
  SCL_HP6_L;
}

/*****************************************************
*函数功能  ：模拟IIC的主机接收一个字节的数据
*函数名    ：IIC_Hp6_Receive_Byte
*函数参数  ：void
*函数返回值：u8
*描述      ：
*            SCL_HP6_H       //时钟线拉高
*            SCL_HP6_L       //时钟线拉低
*            SDA_HP6_OUT_H   //输出模式下数据线拉高
*            SDA_HP6_OUT_L   //输出模式下数据线拉低
*            SDA_HP6_IN      //输入模式
*            SDA_HP6_INT     //接收到的值
********************************************************/
u8 IIC_Hp6_Receive_Byte(void)
{
  u8 i;
  u8 data = 0;
  
 //时钟线拉低
  SCL_HP6_L;
 //进入输入模式
  SDA_HP6_IN;
  /*for语句*/
  for(i=0;i<8;i++)
  {
   //时钟线拉低
    SCL_HP6_L;
   //延时5us
    delay_us(5);
   //data向左移1位
    data <<= 1;
   //时钟线拉高
    SCL_HP6_H;
   /*判断数据线接收的是1还是0*/
   //如果是1，则data | 0x1
    if(SDA_HP6_INT)
    {
      data |= 0x1;
    }
  }
  
  //时钟线拉低
  SCL_HP6_L;
  
  return data;
}







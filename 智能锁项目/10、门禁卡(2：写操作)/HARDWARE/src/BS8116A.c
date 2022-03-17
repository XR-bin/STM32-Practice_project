#include "sys.h"
#include "BS8116A.h"
#include "iic.h"
#include "stdio.h"

//这个延时用于跳过BS8116A忙阶段用的
void BS81XXWait(void)
{
   u8 t = 150;
   while(t--);
}

/***************************************************************
*函数功能  ：对BS81xx所需要的GPIO口进行初始化设置(无外部中断)
*函数名    ：BS81xx_Init1
*函数参数  ：void
*函数返回值：void
*描述      ：
*          时钟线  IIC_SCL  PB6   推挽输出
*          数据线  IIC_SDA  PB7   开漏输出
*          触发线  TK_IRQ   PB8   下拉输入    可以加入外部中断
*****************************************************************/
void BS81xx_Init1(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx配置结构体
	
	/*GPIOx设置*/
  //GPIOx时钟使能  GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  //GPIOx配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	       //PB8端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 //下拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化PB8
	//下拉
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);                //输入低电平下拉
	
	IIC_Init();
}

/***************************************************************
*函数功能  ：通过IIC通信读取BS8116A的输出寄存器KeyStatus的值
*函数名    ：BS81xx_ReadKey
*函数参数  ：u8 KeyStatus_arrd    //BS8116A的输出寄存器KeyStatus地址
*函数返回值：u8
*描述      ：
*          时钟线  IIC_SCL  PB6   推挽输出
*          数据线  IIC_SDA  PB7   开漏输出
*          触发线  TK_IRQ   PB8   下拉输入    可以加入外部中断
*
*          1       BS81XX_ERR1        //应答失效   器件地址数据传输失败
*          2       BS81XX_ERR2        //应答失效   字节地址数据传输失败
*          0xa0    BS81XX_Addr_Write  //器件地址+写操作 
*          0xa1    BS81XX_Addr_Read   //器件地址+读操作
*
*   特别注意！！！
*      		  这里的IIC通信有一点特别，每次发完8bit数据给BS8116A时，BS8116A
*    		都会自己拉高时钟线，告诉主机它在忙，我们必须等它忙完了才能继续发下
*    		一个8bit的数据，否则会发生通信失败(亲测过)，我们直接给个小延时来跳
*    		过这个忙阶段(我直接写了一个小延时函数BS81XXWait())
*
*   如果需要了解，请上网自己搜索资料，不过BS8116A相关技术支持资料较少
*********************************************************************/
u8 BS81xx_ReadKey(u8 KeyStatus_arrd,u8 *data)
{
	u8 ack=0;
	
	//主机发送起始信号
  IIC_Start();
	//主机发送器件地址(写)
  IIC_Send_Byte(BS81XX_Addr_Write);
	//接收从机应答信号
	ack = IIC_Receive_Ack();
	//判断应答
	if(ack != 0)
	{
		IIC_Stop();
		return BS81XX_ERR1;
	}
	BS81XXWait();
	//发送输出寄存器KeyStatus地址(字地址)
  IIC_Send_Byte(KeyStatus_arrd);
	//接收从机应答信号
	ack = IIC_Receive_Ack();
	//判断应答
	if(ack != 0)
	{
		IIC_Stop();
		return BS81XX_ERR2;
	}
	BS81XXWait();
	//主机发送起始信号
  IIC_Start();
  //主机发送器件地址(读)
  IIC_Send_Byte(BS81XX_Addr_Read);
  //主机接收应答
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return BS81XX_ERR1;
  }
	BS81XXWait();
	//主机开始接收数据
  *data = IIC_Receive_Byte();
  //主机发送应答信号    //不应答信号
  IIC_Send_Ack(1);
  //主机发送停止信号
  IIC_Stop();
  
  return ack;
}

/***************************************************************
*函数功能  ：将BS8116A的输出寄存器KeyStatus的值进行按键值转换
*函数名    ：BS81xx_Key
*函数参数  ：void
*函数返回值：u8
*描述      ：
*          时钟线  IIC_SCL  PB6   推挽输出
*          数据线  IIC_SDA  PB7   开漏输出
*          触发线  TK_IRQ   PB8   下拉输入    可以加入外部中断
*
*          1       BS81XX_ERR1        //应答失效   器件地址数据传输失败
*          2       BS81XX_ERR2        //应答失效   字节地址数据传输失败
*          0xa0    BS81XX_Addr_Write  //器件地址+写操作 
*          0xa1    BS81XX_Addr_Read   //器件地址+读操作
*
*          0x08    BS81XX_KeyStatus0  //BS8116A的输出寄存器KeyStatus0地址
*          0x09    BS81XX_KeyStatus1  //BS8116A的输出寄存器KeyStatus1地址
*********************************************************************/
u8 BS81xx_Key(void)
{
	u8 data_h,data_l;
	static u8 key_flay = 1;
	u16 key = 0;
	u8 temp = 0;
	
	if(key_flay && (!TK_IRQ))
	{
		//获取输出寄存器KeyStatus值(每一个位代表一个按键)
		BS81xx_ReadKey(BS81XX_KeyStatus1,&data_h);
		BS81xx_ReadKey(BS81XX_KeyStatus0,&data_l);
		
		//合成16位数据
		key = ((u16)data_h<<8) + (u16)data_l;
		
		key_flay = 0;
		
		switch(key)
		{
			case 0x0001: temp = '1'; break;
			case 0x0002: temp = '2'; break;
			case 0x0004: temp = '3'; break;
			case 0x0008: temp = '4'; break;
			case 0x0010: temp = '5'; break;
			case 0x0020: temp = '6'; break;
			case 0x0040: temp = '7'; break;
			case 0x0080: temp = '8'; break;
			case 0x0100: temp = '9'; break;
			case 0x0200: temp = '*'; break;
			case 0x0400: temp = '0'; break;
			case 0x0800: temp = '#'; break;
		}
	}
	if((!key_flay)&& TK_IRQ) key_flay = 1;
	
	return temp;
}







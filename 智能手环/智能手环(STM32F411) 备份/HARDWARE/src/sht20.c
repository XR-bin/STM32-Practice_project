#include "stm32f4xx.h"
#include "iic.h"
#include "sht20.h"
#include "math.h"

//延时函数毫秒
static void delay_ms(u32 ms)
{
	u32 i = 100 / 4 * 1000 * ms;   //由系统时钟频率决定的
	while(i)
	{
		i--;
	}
}

/*****************************************************
*函数功能  ：对温湿度传感器进行复位
*函数名    ：SHT20_RST
*函数参数  ：void
*函数返回值：u8
*描述      ：
********************************************************/
u8 SHT20_RST(void)
{
	u8 ack;
	
	//起始信号
	IIC_Start();
	//发送写操作命令
	IIC_Send_Byte(SHT20_WRITE);
	//等待应答
	ack = IIC_Receive_Ack();
	//发送操作指令失败
	if(ack == 1)
	{
		//停止信号
		IIC_Stop();
		return 1;
	}
	//发送软件复位命令
	IIC_Send_Byte(SHT20_RST_CMD);
	//等待应答
	ack = IIC_Receive_Ack();
	//发送复位指令失败
	if(ack == 1)
	{
		//停止信号
		IIC_Stop();
		return 2;
	}
	
	//停止信号
	IIC_Stop();
	
	//复位成功
	return 0;
}

/*****************************************************
*函数功能  ：对温湿度传感器进行初始化
*函数名    ：SHT20_Init
*函数参数  ：void
*函数返回值：u8
*描述      ：
********************************************************/
u8 SHT20_Init(void)
{
	//IIC引脚初始化
	IIC_Init();
	
	//SHT20软件复位
	return SHT20_RST();
}

/*****************************************************
*函数功能  ：获取SHT20的温度或湿度值
*函数名    ：SHT20_RH_T
*函数参数  ：u8 cmd
*函数返回值：u16
*描述      ：
********************************************************/
u16 SHT20_RH_T(u8 cmd)
{
	u8 ack;
	u8 val_1,val_2;
	u16 data;
	
	//起始信号
	IIC_Start();
	//发送写操作命令
	IIC_Send_Byte(SHT20_WRITE);
	//等待应答
	ack = IIC_Receive_Ack();
	//发送操作指令失败
	if(ack == 1)
	{
		//停止信号
		IIC_Stop();
		return 1;
	}
	//发送测量温度或湿度命令
	IIC_Send_Byte(cmd);
	//等待应答
	ack = IIC_Receive_Ack();
	//发送操作指令失败
	if(ack == 1)
	{
		//停止信号
		IIC_Stop();
		return 1;
	}
	
	//等待数据测量完成
	delay_ms(85);       //给他时间去测量
	//起始信号
	IIC_Start();
	//发送写操作命令
	IIC_Send_Byte(SHT20_READ);
	//等待应答
	while(IIC_Receive_Ack());    //直到测量完成
	
	//获取测量数据
	val_1 = IIC_Receive_Byte();      //接收高八位
	IIC_Send_Ack(0);                 //接续接收
  val_2 = IIC_Receive_Byte(); 	   //接收低八位
	IIC_Send_Ack(1);                 //不再接收
	
	//停止信号
	IIC_Stop();
	
	data = (val_1<<8)|val_2;
	
	return data;
}

/*****************************************************
*函数功能  ：获取SHT20的温度和湿度值
*函数名    ：SHT20_RH_T_data
*函数参数  ：viod
*函数返回值：THR
*描述      ：
********************************************************/
THR SHT20_RH_T_data(void)
{
	u16 T_val;
	u16 RH_val;
	THR trh_val;
	
	//执行获取温度
	T_val = SHT20_RH_T(SHT20_T_CMD);	
	//执行获取湿度
	RH_val = SHT20_RH_T(SHT20_RH_CMD);
	
	//转换具体的值
	//温度：
	trh_val.t = -46.85 + 175.72 * ( T_val / pow(2,16));
	//湿度：
	trh_val.rh = -6 + 125 * (RH_val / pow(2,16));
	
	return trh_val;
}









 



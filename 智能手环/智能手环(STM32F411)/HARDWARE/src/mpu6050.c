#include "stm32f4xx.h"
#include "iic.h"
#include "mpu6050.h"
#include "OLED.h"
#include "stdio.h"

//延时函数毫秒
static void delay_ms(u32 ms)
{
	u32 i = 100 / 4 * 1000 * ms;   //由系统时钟频率决定的
	while(i)
	{
		i--;
	}
}

/***************************************************************
*函数功能  ：模拟IIC的主机向某个从机写多个字节数据到某个寄存器里
*函数名    ：IIC_Write
*函数参数  ：u8 addr, u8 reg, u8 len, u8 * data
*函数返回值：u8
*描述      ：
*             addr :  某个从机的器件地址
*             reg  :  从机的某个寄存器地址
***************************************************************/
u8 IIC_Write(u8 addr,u8 reg,u8 len,u8 * data)
{
  u8 ack;
  u8 i;
  
  //主机发送起始信号
  IIC_Start();
  //注：
  //   I2C_Direction_Transmitter 是 写
  //   I2C_Direction_Receiver    是 读
  //主机发送器件地址(写)  
  IIC_Send_Byte((addr<<1)|I2C_Direction_Transmitter);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return 1;    //第1阶段错误：发送器件地址错误
  }
  //发送寄存器地址
  IIC_Send_Byte(reg);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return 2;    //第2阶段错误：发送寄存器地址错误
  }
  //开始写入数据
  for (i = 0; i < len; i++)
  {
    IIC_Send_Byte(data[i]);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return 3;   //第3阶段错误：发送数据过程错误
    }
  }
  //主机发送停止信号
  IIC_Stop();
  
  return 0;  //完成写入
}

/******************************************************************
*函数功能  ：模拟IIC的主机向某个从机的某个寄存器里读取多个字节数据
*函数名    ：IIC_Read
*函数参数  ：u8 addr, u8 reg, u8 len, u8 *buf
*函数返回值：u8
*描述      ：
*             addr :  某个从机的器件地址
*             reg  :  从机的某个寄存器地址
*******************************************************************/
u8 IIC_Read(u8 addr,u8 reg,u8 len,u8 *buf)
{
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  //注：
  //   I2C_Direction_Transmitter 是 写
  //   I2C_Direction_Receiver    是 读
  //主机发送器件地址(写)  
  IIC_Send_Byte((addr<<1)|I2C_Direction_Transmitter);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return 1;    //第1阶段错误：发送器件地址错误
  }
  //发送寄存器地址
  IIC_Send_Byte(reg);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return 2;    //第2阶段错误：发送寄存器地址错误
  }
  //主机发送起始信号
  IIC_Start();
  //注：
  //   I2C_Direction_Transmitter 是 写
  //   I2C_Direction_Receiver    是 读
  //主机发送器件地址(读)
  IIC_Send_Byte((addr<<1)|I2C_Direction_Receiver);
  //主机接收应答
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return 3;    //第3阶段错误：发送器件地址错误
  }
  //开始读取数据
  while (len)
  {
    *buf = IIC_Receive_Byte();
  
    if (len == 1)
        IIC_Send_Ack(1);   //不应答(不接收)
    else
        IIC_Send_Ack(0);   //应答  (继续接收)
    buf++;
    len--;
  }
  
  //主机发送停止信号
  IIC_Stop();
  
  return 0;   //读取完成
}

/***********************************************************************************
*函数功能  ：对MPU6050进行初始化
*函数名    ：MPU6050_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            初始化步骤：
*                      1、初始化 IIC 接口
*                      2、复位 MPU6050
*                      3、设置角速度传感器（陀螺仪）和加速度传感器的满量程范围
*                      4、设置其他参数
*                      5、配置系统时钟源并使能角速度传感器和加速度传感器
*           
*           地址：
*                MPU6050_ADDR     0x68     MPU6050器件地址 
*                SMPLRT_DIV       0x19	   陀螺仪采样率分频寄存器
*                PWR_MGMT_1       0x6B	   电源管理寄存器1
*                CONFIG           0x1A	   配置寄存器
*                GYRO_CONFIG      0x1B	   陀螺仪配置寄存器
*                ACCEL_CONFIG     0x1C	   加速度传感器配置寄存器
***************************************************************************************/
void MPU6050_Init(void)
{
  //要对MPU6050内部寄存器配置的值
  u8 buff[6] = {0x80,0x01,0x07,0x04,0x18,0x00};
  
  //IIC接口初始化
  IIC_Init();
  
  //延时3ms，避免iic电路不稳(可不加)
  delay_ms(3);
  
  //MPU6050复位
  IIC_Write(MPU6050_ADDR,PWR_MGMT_1, 1,&buff[0]);   //复位
  delay_ms(100);//延时5ms，避免MPU6050复位未完成
  
  //MPU6050内部寄存器设置
  IIC_Write(MPU6050_ADDR,PWR_MGMT_1, 1,&buff[1]);   //解除休眠状态  时钟源：PLL，使用 X 轴陀螺作为参考   一般都选用这个
  IIC_Write(MPU6050_ADDR,SMPLRT_DIV, 1,&buff[2]);   //采样频率 ：125Hz
  IIC_Write(MPU6050_ADDR,CONFIG, 1,&buff[3]);       //加速度传感器：带宽21Hz 延迟8.5ms Fs=1Khz 陀螺仪：带宽20Hz  8.3ms Fs=1Khz 
  IIC_Write(MPU6050_ADDR,GYRO_CONFIG, 1,&buff[4]);  //陀螺仪的满量程范围：± 2000° /S
  IIC_Write(MPU6050_ADDR,ACCEL_CONFIG, 1,&buff[5]); //加速度传感器的满量程范围：± 2g
}

/***********************************************************
*函数功能  ：从MPU605里的某个寄存器读取一个16位得到数据
*函数名    ：MPU6050_Receive_Byte
*函数参数  ：u8 REG_Address
*函数返回值：short
*描述      ：
*            这个函数是专门针对获取加速度测量值、角速度测量值
*        和温度测量值(本实验没用)的，因为测量值高位寄存器和测
*        量值低位寄存器地址是连续的，支持寄存器连读
*
*        MPU6050_ADDR   0x68  MPU6050器件地址
***********************************************************/
short MPU6050_Receive_Byte(u8 REG_Address)
{
  u8 Measure[2];   //用于装测量值高位和低位的
  
  //获取测量值高位和低位
  IIC_Read(MPU6050_ADDR,REG_Address,2,Measure);
  
  //将8位数据组合成16位数据返回去
  return ((Measure[0] << 8) | Measure[1]);
}


/***********************************************************
*函数功能  ：从MPU605里的获取当前的加速度
*函数名    ：MPU6050_Receive_Byte
*函数参数  ：u8 REG_Address
*函数返回值：short
*描述      ：
*        MPU6050_ADDR   0x68  MPU6050器件地址
*     	 ACCEL_XOUT_H	  0x3B  //X加速度测量值高位寄存器
*        ACCEL_XOUT_L	  0x3C  //X加速度测量值低位寄存器
*        ACCEL_YOUT_H	  0x3D  //Y加速度测量值高位寄存器
*        ACCEL_YOUT_L	  0x3E  //Y加速度测量值低位寄存器
*        ACCEL_ZOUT_H	  0x3F  //Z加速度测量值高位寄存器
*        ACCEL_ZOUT_L	  0x40  //Z加速度测量值低位寄存器
***********************************************************/
ADD_SPEED MPU6050_ReadSpeed(void)
{
	ADD_SPEED xyz_val;
	
	short x_data;
	short y_data;
	short z_data;
	
	/*X轴方向*/
	x_data  = MPU6050_Receive_Byte(ACCEL_XOUT_H);
	/*Y轴方向*/
	y_data  = MPU6050_Receive_Byte(ACCEL_YOUT_H);
	/*Z轴方向*/
	z_data  = MPU6050_Receive_Byte(ACCEL_ZOUT_H);
	
	/*将轴转换成加速度*/
	xyz_val.x_val = ((float)x_data / (65536/4)) * 10;
	xyz_val.y_val = ((float)y_data / (65536/4)) * 10;
	xyz_val.z_val = ((float)z_data / (65536/4)) * 10;
	
	return xyz_val;
}







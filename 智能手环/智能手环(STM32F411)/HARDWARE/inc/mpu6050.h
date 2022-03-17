#ifndef _MPU6050_H
#define _MPU6050_H

  #include "stm32f4xx.h"
	
	typedef struct
	{
		float	x_val;
		float y_val;
		float z_val;
	}ADD_SPEED;

	//MPU6050器件地址及内部寄存器地址
  //需知更多寄存器地址请查询手册：<<MPU-6000 & MPU-6050 寄存器表及其描述(中文版)>>
  #define  MPU6050_ADDR   0x68  //MPU6050器件地址
  #define	 SMPLRT_DIV		  0x19	//陀螺仪采样率分频寄存器：陀螺仪采样率，典型值：0x07(125Hz)
  #define	 CONFIG			    0x1A	//配置寄存器：低通滤波频率，典型值：0x06(5Hz)
  #define  FIFO_EN        0x23  // FIFO 使能寄存器：一般小工程不用
  #define	 GYRO_CONFIG		0x1B	//陀螺仪配置寄存器：陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
  #define	 ACCEL_CONFIG	  0x1C	//加速度传感器配置寄存器：加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
  #define	 ACCEL_XOUT_H	  0x3B  //X加速度测量值高位寄存器
  #define	 ACCEL_XOUT_L	  0x3C  //X加速度测量值低位寄存器
  #define  ACCEL_YOUT_H	  0x3D  //Y加速度测量值高位寄存器
  #define	 ACCEL_YOUT_L	  0x3E  //Y加速度测量值低位寄存器
  #define	 ACCEL_ZOUT_H	  0x3F  //Z加速度测量值高位寄存器
  #define	 ACCEL_ZOUT_L	  0x40  //Z加速度测量值低位寄存器
  #define	 TEMP_OUT_H		  0x41  //温度测量值高位寄存器
  #define  TEMP_OUT_L		  0x42  //温度测量值低位寄存器
  #define	 GYRO_XOUT_H		0x43  //X角速度测量值高位寄存器
  #define	 GYRO_XOUT_L		0x44	//X角速度测量值低位寄存器
  #define	 GYRO_YOUT_H		0x45  //Y角速度测量值高位寄存器
  #define	 GYRO_YOUT_L		0x46  //Y角速度测量值低位寄存器
  #define	 GYRO_ZOUT_H		0x47  //Z角速度测量值高位寄存器
  #define	 GYRO_ZOUT_L		0x48  //Z角速度测量值低位寄存器
  #define	 PWR_MGMT_1		  0x6B	//电源管理寄存器1：电源管理，典型值：0x00(正常启用)
  #define	 WHO_AM_I		    0x75	//IIC地址寄存器(默认数值0x68，只读)
	
	u8 IIC_Write(u8 addr,u8 reg,u8 len,u8 * data);
	u8 IIC_Read(u8 addr,u8 reg,u8 len,u8 *buf);
	void MPU6050_Init(void);
	short MPU6050_Receive_Byte(u8 REG_Address);
	ADD_SPEED MPU6050_ReadSpeed(void);
	
#endif






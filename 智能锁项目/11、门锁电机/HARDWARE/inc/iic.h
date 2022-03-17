#ifndef _IIC_H
#define _IIC_H

  #include "sys.h"
  //SCL:
  #define SCL_H      GPIO_SetBits(GPIOB,GPIO_Pin_6)    //时钟线拉高
  #define SCL_L      GPIO_ResetBits(GPIOB,GPIO_Pin_6)  //时钟线拉低
  //SDA
  //输出：
  #define SDA_OUT_H  GPIO_SetBits(GPIOB,GPIO_Pin_7)    //输出模式下数据线拉高
  #define SDA_OUT_L  GPIO_ResetBits(GPIOB,GPIO_Pin_7)  //输出模式下数据线拉低
  //输入：
  #define SDA_IN     GPIO_SetBits(GPIOB,GPIO_Pin_7)           //输入模式
  #define SDA_INT    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)  //接收到的值

  void IIC_Init(void);
  void IIC_Start(void);
  void IIC_Stop(void);
  void IIC_Send_Ack(u8 ack);
  u8 IIC_Receive_Ack(void);
  void IIC_Send_Byte(u8 data);
  u8 IIC_Receive_Byte(void);

#endif














#ifndef _MG200_H
#define _MG200_H

	#include "sys.h"
	
	//MCU接收MG200指纹模块的数据包结构体
	typedef struct MG200
	{
		u8 StartCode;   //起始码
		u8 SendAddr;    //发送地址
		u8 ReceiveAddr; //接收地址
		u8 AckCode;     //应答码
		u8 Result;      //返回值
		u8 Parameter;   //参数
		u8 Reserve;     //预留字节
		u8 Checksum;    //校验和
		u8 End;         //接收完成标记
	}MG200DATA;
	
	//FING_PWR  
	#define   MG200_ON       GPIO_SetBits(GPIOB,GPIO_Pin_9)     //开
	#define   MG200_OFF      GPIO_ResetBits(GPIOB,GPIO_Pin_9)   //关
	//FING_DETECT
	#define   MG200_DETECT   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)    //用于判断FING_DETECT的电平状态
	
	//数据包某些固定地址
	#define   STARTCODE      0x6C      //数据包起始码
	#define   SENDADDR       0x63      //数据包发送地址
	#define   RECEIVEADDR    0x62      //数据包接收地址
	#define   RESERVE        0x00      //预留字节,为以后MG200产商继续开发用的，不用理，默认0就行
	
	//接收数据包错误提示
	#define  DATA_OK             0x0   //数据包没有问题
	#define  STARTCODE_ERROR     0x1   //数据包的起始码错误
	#define  SENDADDR_ERROR      0x2   //数据包的发送地址错误
	#define  RECEIVEADDR_ERROR   0x3   //数据包的接收地址错误
	#define  ACKCODE_ERROR       0x4   //数据包的应答错误
	#define  CHECKSUM_ERROR      0x5   //数据包的校验和错误
	
	#define  EXTRACT_OK          0x0   //指纹采样成功
	#define  EXTRACT_ERROR       0x1   //指纹采样失败
	
	#define  ENROLL_OK           0x0    //指纹登记成功
	#define  ENROLL_ERROR        0x1    //指纹登记失败
	
	#define  Match1Nl_ERROR      0x0    //指纹比对失败
	
	extern MG200DATA MG200data;
	
	void MG200_Init(void);
	void MG200_SendCmd(u8 cmd ,u8 parameter);
	u8 MG200_ReceiveAck(u8 cmd,u8 *result,u8 *parameter);
	u8 MG200_ExtractFingerprint(u8 parameter);
	u8 MG200_Enroll(void);
	u8 MG200_Match1Nl(void);
	void MG200_EraseAll(void);
	void MG200_EraseOne(u8 ID);
	u8 MG200_GetUserNum(void);
	
#endif






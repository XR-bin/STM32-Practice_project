#ifndef _BS8116A_H
#define _BS8116A_H

	#include "sys.h"

	//BS8116A器件地址
	#define   BS81XX_Addr_Write   0xa0       //BS8116A的器件地址    写操作
	#define   BS81XX_Addr_Read    0xa1       //BS8116A的器件地址    读操作
	
	//内部操作    读
	#define   BS81XX_KeyStatus0   0x08       //BS8116A的输出寄存器KeyStatus0地址
	#define   BS81XX_KeyStatus1   0x09       //BS8116A的输出寄存器KeyStatus1地址
	
	//内部操作    写
	//一般我们使用默认值，不去进行设设置
	//注意：主机对BS8116A写入设置时，必须从0xb0开始连续写入22个数据字节，最后一个字节是检验和。
	//前五个地址的数据保持默认(查看手册发默认值)
	#define   K1_TH               0xb5      //Key1触发阀门值设置
	#define   K2_TH               0xb6      //Key2触发阀门值设置
	#define   K3_TH               0xb7      //Key3触发阀门值设置
	#define   K4_TH               0xb8      //Key4触发阀门值设置
	#define   K5_TH               0xb9      //Key5触发阀门值设置
	#define   K6_TH               0xba      //Key6触发阀门值设置
	#define   K7_TH               0xbb      //Key7触发阀门值设置
	#define   K8_TH               0xbc      //Key8触发阀门值设置
	#define   K9_TH               0xbd      //Key9触发阀门值设置
	#define   K10_TH              0xbe      //Key10触发阀门值设置
	#define   K11_TH              0xbf      //Key11触发阀门值设置
	#define   K12_TH              0xf0      //Key12触发阀门值设置
	#define   K13_TH              0xf1      //Key13触发阀门值设置
	#define   K14_TH              0xf2      //Key14触发阀门值设置
	#define   K15_TH              0xf3      //Key15触发阀门值设置
	#define   K16_TH              0xf4      //Key16触发阀门值设置
	
	//IIC通信错误标识
	#define   BS81XX_ERR1        1      //器件地址数据传输失败
	#define   BS81XX_ERR2        2      //字节地址数据传输失败
	
	#define   TK_IRQ       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)    //用于判断TK_IRQ的电平状态
	
	void BS81xx_Init1(void);
	u8 BS81xx_ReadKey(u8 KeyStatus_arrd,u8 *data);
	u8 BS81xx_Key(void);
	
#endif














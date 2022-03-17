#include "sys.h"
#include "usart.h"
#include "MG200.h"
#include "stdio.h"
#include "string.h"

//用于接收MG200指纹模块传回来的数据的变量
MG200DATA MG200data;

/*****************************************************
*函数功能  ：对MG200指纹模块对应的GPIO口进行初始化设置
*函数名    ：MG200_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*           FING_PWR    -------   PB9    指纹模块的开关
*           FING_DETECT -------   PC13   查看否有手指按下
********************************************************/
void MG200_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;                           //GPIOx配置结构体
	
	//时钟使能   GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	       //使能PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	       //使能PB端口时钟
	
	//GPIO配置
	//PB9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;          				     //PB9 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		           //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		           //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                 //根据设定参数初始化PB9 
	//PC13
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;          				     //PC13 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; 		               //下拉输入
	GPIO_Init(GPIOC, &GPIO_InitStruct);					                 //根据设定参数初始化PB9 

	//PB9输出低电平    先关闭
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);                            //PB9 输出低电平
	//下拉电阻
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);                           //PC13 输出低电平
	
	//串口2初始化
	USART2_Init(115200);
}

/***********************************************************************
*函数功能  ：向MG200指纹模块发送指令
*函数名    ：MG200_SendCmd
*函数参数  ：u8 cmd  ， u8 parameter       //指令码   参数
*函数返回值：void
*描述      ：
*           数据包结构：起始码+发送地址+接收地址+指令码+参数+预留+校验和
*
*           STARTCODE      0x6C      //数据包起始码
*           SENDADDR       0x63      //数据包发送地址
*           RECEIVEADDR    0x62      //数据包接收地址
*           RESERVE        0x00      //预留字节,为以后MG200产商继续开发用的，不用理，默认0就行
*************************************************************************/
void MG200_SendCmd(u8 cmd ,u8 parameter)
{
	u8 checksum;    //校验和变量
	
	//检验和 = 除起始码外的其他数据和，如果和值超过1个字节的大小就只取前8位
	checksum = (SENDADDR + RECEIVEADDR + cmd + parameter + RESERVE) & 0xff;
	
	//接收接收标志置1
	MG200data.End = 1;
	
	USART2_Send_Byte(STARTCODE);    //发送起始码
	USART2_Send_Byte(SENDADDR);     //发送发送地址
	USART2_Send_Byte(RECEIVEADDR);  //发送接收地址
	USART2_Send_Byte(cmd);          //发送指令码
	USART2_Send_Byte(parameter);    //发送参数
	USART2_Send_Byte(RESERVE);      //发送预留位
	USART2_Send_Byte(checksum);     //发送校验和

}

/***********************************************************************
*函数功能  ：接收MG200发回来的数据包
*函数名    ：MG200_ReceiveAck
*函数参数  ：u8 cmd ， u8* result ， u8* parameter    //指令码   返回值   参数
*函数返回值：u8 
*描述      ：
*           DATA_OK             0x0   数据包没有问题
*						STARTCODE_ERROR     0x1   数据包的起始码错误
*						SENDADDR_ERROR      0x2   数据包的发送地址错误
*						RECEIVEADDR_ERROR   0x3   数据包的接收地址错误
*           ACKCODE_ERROR       0x4   数据包的应答错误
*						CHECKSUM_ERROR      0x5   数据包的校验和错误
*************************************************************************/
u8 MG200_ReceiveAck(u8 cmd,u8 *result,u8 *parameter)
{
	u8 checksum;    //校验和变量
	
	while(MG200data.End);   //等待接收完成
	
	//校验数据包是否正确
	if(MG200data.StartCode != STARTCODE) return STARTCODE_ERROR;      //数据包的起始码
	if(MG200data.SendAddr != 0x62)       return SENDADDR_ERROR;       //数据包的发送地址校验
	if(MG200data.ReceiveAddr != 0x63)    return RECEIVEADDR_ERROR;    //数据包的接收地址校验
	if(MG200data.AckCode != cmd)         return ACKCODE_ERROR;        //数据包应答码校验  (应答码==指令码)
	checksum = (MG200data.SendAddr+MG200data.ReceiveAddr+MG200data.AckCode+MG200data.Result+MG200data.Parameter+MG200data.Reserve) & 0xff;
	if(MG200data.Checksum != checksum)   return CHECKSUM_ERROR;       //数据包校验和校验
	
	//确认数据包没问题后，我们只需获取数据包里的返回值和参数就行了
	*result = MG200data.Result;
	*parameter = MG200data.Parameter;
	
	//结构体清零
	memset(&MG200data,0,sizeof(MG200data));
	
	return DATA_OK;
}

/*****************************************************************************
*函数功能  ：使用0x51指令码让MG200指纹模块来抓取指纹图像并同时提取特征量
*函数名    ：MG200_ExtractFingerprint
*函数参数  ：u8 cmd ， u8* result ， u8* parameter    //指令码   返回值   参数
*函数返回值：u8 
*描述      ：
*             EXTRACT_OK          0x0   //指纹采样成功
*             EXTRACT_ERROR       0x1   //指纹采样失败
*********************************************************************************/
u8 MG200_ExtractFingerprint(u8 parameter)
{
	u8 result,ackparam,ack;
	
	//发送0x51指令采取指纹
	MG200_SendCmd(0x51,parameter);
	//接收MG200的数据包
	ack = MG200_ReceiveAck(0x51,&result,&ackparam);
	if(ack != 0) return EXTRACT_ERROR;
	/*********************************************************
	*错误对应编号：
	*		00h : 抓取指纹图像及特征量提取成功
	*		B1h : 指纹过小(small finger)
	*		B2h : 无指纹 (no finger)
	*		B3h : 指纹位置过于靠左，需要将手指靠右边摁按（left finger）
	*		B4h : 指纹位置过于靠右，需要将手指靠左边摁按（right finger）
	*		B5h : 指纹位置过于靠上，需要将手指靠下边摁按（up finger）
	*		B6h : 指纹位置过于靠下，需要将手指靠上边摁按 (down finger）
	*		B7h : 湿手指 （指纹表面出汗或者水渍过多） (wet finger)
	*		B8h : 干燥手指(dry finger)
	*		C0h: 特征提取错误
	*		C2h: 特征提取错误 (图像质量差)
	***********************************************************/
	switch(result)
	{
		case 0x00 : printf("抓取指纹图像及特征量提取成功\r\n"); return EXTRACT_OK;
		case 0xB1 : printf("small finger               \r\n"); return EXTRACT_ERROR;
		case 0xB2 : printf("no finger                  \r\n"); return EXTRACT_ERROR;
		case 0xB3 : printf("left finger                \r\n"); return EXTRACT_ERROR;
		case 0xB4 : printf("right finger               \r\n"); return EXTRACT_ERROR;
		case 0xB5 : printf("up finger                  \r\n"); return EXTRACT_ERROR;
		case 0xB6 : printf("down finger                \r\n"); return EXTRACT_ERROR;
		case 0xB7 : printf("wet finger                 \r\n"); return EXTRACT_ERROR;
		case 0xB8 : printf("dry finger                 \r\n"); return EXTRACT_ERROR;
		case 0xC0 : printf("特征提取错误                \r\n"); return EXTRACT_ERROR;
		case 0xC2 : printf("特征提取错误 (图像质量差)   \r\n"); return EXTRACT_ERROR;
		default   : printf("抓取指纹图像及特征量提取错误\r\n");  return EXTRACT_ERROR; 
	}
}

/*****************************************************************************
*函数功能  ：使用0x7F指令码让MG200指纹模块对用户指纹进行登记
*函数名    ：MG200_Enroll
*函数参数  ：void
*函数返回值：u8 
*描述      ：
*          ENROLL_OK          0x0      指纹登记成功
*          ENROLL_ERROR       0x1      指纹登记失败
*********************************************************************************/
u8 MG200_Enroll(void)
{
	u8 result,ackparam,ack;
	
	//采集指纹信息，我们采用三次捕获指纹数据
	/**********************第一次指纹录入**********************/
	printf("第一次录入，请放下手指\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x00);
	while(ack != 0)
	{
		printf("重新录入....\r\n");
		//手指先重新抬起
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x00);
	}
	//等待抬起手指
	while(MG200_DETECT);
	/**********************第二次指纹录入**********************/
	printf("第二次录入，请放下手指\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x01);
	while(ack != 0)
	{
		printf("重新录入....\r\n");
		//手指先重新抬起
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x01);
	}
	//等待抬起手指
	while(MG200_DETECT);
	/**********************第二次指纹录入**********************/
	printf("第三次录入，请放下手指\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x02);
	while(ack != 0)
	{
		printf("重新录入....\r\n");
		//手指先重新抬起
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x02);
	}
	//等待抬起手指
	while(MG200_DETECT);
	
	//开对指纹信息进行登记
	MG200_SendCmd(0x7F,0x00);
	//接收返回的数据包
	ack = MG200_ReceiveAck(0x7F,&result,&ackparam);
	if(ack != 0) 
	{
		printf("数据包有问题...\r\n");
		return ENROLL_ERROR;
	}
	
	switch(result)
	{
		case 0x00:printf("注册成功                                       \r\n"); return ENROLL_OK;
		case 0x83:printf("ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误\r\n"); return ENROLL_ERROR;
		case 0x91:printf("注册失败(用户区域已满)                          \r\n"); return ENROLL_ERROR;
		case 0x93:printf("已经注册的 ID                                  \r\n"); return ENROLL_ERROR;
		case 0x94:printf("指纹提取次数 < 3                               \r\n"); return ENROLL_ERROR;
		default  : return ENROLL_ERROR; 
	}
}

/*****************************************************************************
*函数功能  ：使用0x71指令码让MG200指纹模块进行当前指纹与模块内存储的用户指纹进行比对
*函数名    ：MG200_Match1Nl
*函数参数  ：void
*函数返回值：u8 
*描述      ：
*               Match1Nl_ERROR       0x0       指纹比对失败
*********************************************************************************/
u8 MG200_Match1Nl(void)
{
	u8 result,ackparam,ack;
	
	printf("请放下手指\r\n");
	while(!MG200_DETECT);
	ack = MG200_ExtractFingerprint(0x00);
	while(ack != 0)
	{
		printf("重新按手指....\r\n");
		//手指先重新抬起
		while(MG200_DETECT);
		while(!MG200_DETECT);
		ack = MG200_ExtractFingerprint(0x00);
	}
	//等待手指抬起
	while(MG200_DETECT);
	
	//开始对指纹信息进行比对
	MG200_SendCmd(0x71,0x00);
	//接收返回的数据包
	ack = MG200_ReceiveAck(0x71,&result,&ackparam);
	if(ack != 0) 
	{
		printf("数据包有问题...\r\n");
		return Match1Nl_ERROR;
	}

	switch(result)
	{
		case 0x00: printf("比对成功  \r\n"); return ackparam;         //返回ID号
		case 0x92: printf("没有该指纹\r\n"); return Match1Nl_ERROR;   //比对失败break;
		default:   printf("失败      \r\n"); return Match1Nl_ERROR;   //比对失败
	}
}

/*****************************************************************************
*函数功能  ：使用0x54指令码删除MG200指纹模块里所有用户指纹信息
*函数名    ：MG200_EraseAll
*函数参数  ：void
*函数返回值：void  
*描述      ：
*********************************************************************************/
void MG200_EraseAll(void)
{
	u8 result,ackparam,ack;
	
	//开始删除所有信息
	MG200_SendCmd(0x54,0x00);
	//接收返回的数据包
	ack = MG200_ReceiveAck(0x54,&result,&ackparam);
	if(ack != 0) 
	{
		printf("数据包有问题...\r\n");
		return;
	}
	
	switch(result)
	{
		case 0x00:printf("所有信息删除成功\r\n");break;
		case 0x90:printf("删除失败（注册数量为 0）\r\n");break;
		default:printf("删除失败\r\n");break;
	}
	
}

/*****************************************************************************
*函数功能  ：使用0x73指令码删除MG200指纹模块里删除指定用户的指纹信息
*函数名    ：MG200_EraseOne
*函数参数  ：u8 ID
*函数返回值：void  
*描述      ：
*********************************************************************************/
void MG200_EraseOne(u8 ID)
{
	u8 result,ackparam,ack;
	
	MG200_SendCmd(0x73,ID);
	//接收返回的数据包
	ack = MG200_ReceiveAck(0x73,&result,&ackparam);
	if(ack != 0) 
	{
		printf("数据包有问题...\r\n");
		return;
	}
	switch(result)
	{
		case 0x00:printf("删除成功    \r\n");break;
		case 0x83:printf("参数错误    \r\n");break;
		case 0x90:printf("未注册的用户\r\n");break;
		case 0xFF:printf("未注册的用户\r\n");break;
    default:printf("删除失败      \r\n");break;
	}
}

/*****************************************************************************
*函数功能  ：使用0x55指令码删除MG200指纹模块里删除指定用户的指纹信息
*函数名    ：MG200_GetUserNum
*函数参数  ：u8 ID
*函数返回值：void  
*描述      ：
*********************************************************************************/
u8 MG200_GetUserNum(void)
{
	u8 result,ackparam,ack;
	
	MG200_SendCmd(0x55,0x00);
	//接收返回的数据包
	ack = MG200_ReceiveAck(0x55,&result,&ackparam);
	if(ack != 0) 
	{
		printf("数据包有问题...\r\n");
		return 0;
	}
	
	return ackparam;
}






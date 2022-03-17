#include "sys.h"     
#include "spi.h"
#include "w25q64.h"
#include "stdio.h"

/*********************************************
*W25Q64存储芯片
*容量大小             ：8Mbyte  或  64Mbit
*共有块区             ：128块
*每块区共有扇区       ：16扇区
*每扇区共有页         ：16页
*每页字节             ：256字节
*地址可写最大十六进制数：0x7fffff
*        7f:块区   f：扇区    f：页    ff：字节
**********************************************/

/*******************************************
*函数功能  ：W25Q64初始化
*函数名    ：W25Q64_Init
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*           SPI_CS     PA4
********************************************/
void W25Q64_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;  //GPIOx配置结构体
	
	/*SPI初始化*/
	SPI1_Init();
	
	/*片选引脚*/
  //时钟使能  GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;				     //PA4 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					       //根据设定参数初始化PA4
	
	//片选拉高
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}
	
/*******************************************
*函数功能  ：对W25Q64进行写使能
*函数名    ：W25Q64_Write_Enable
*函数参数  ：void
*函数返回值：void
*函数描述  ：写使能指令    0x06
*            此函数用于写、擦除
********************************************/
void W25Q64_Write_Enable(void)
{
  SPI_CS_L;
  SPI1_RS_Byte(0x06);
  SPI_CS_H;
}

/*******************************************
*函数功能  ：读W25Q64的状态寄存器的值
*函数名    ：W25Q64_Read_Status
*函数参数  ：void
*函数返回值：u8
*函数描述  ：读状态指令    0x05
*            此函数用于写、擦除
*            用返回值判断是否写入完成
********************************************/
u8 W25Q64_Read_Status(void)
{
  u8 status;
  
  SPI_CS_L;
  SPI1_RS_Byte(0x05);
  status = SPI1_RS_Byte(0xff);
  SPI_CS_H;
  
  return status;
}

/*****************************************************
*函数功能  ：主控芯片从W25Q64的某个地址读n个字节数据
*函数名    ：W25Q64_Read_Bytes
*函数参数  ：void
*函数返回值：u32 address  u32 num_byte  u8 *str
*函数描述  ：写指令    0x03
*******************************************************/
void W25Q64_Read_Bytes(u32 address,u32 num_byte,u8 *str)
{
  SPI_CS_L;
  SPI1_RS_Byte(0x03);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  
  while(num_byte)
  {
    *str = SPI1_RS_Byte(0xff);
    str++;
    num_byte--;
  }
  SPI_CS_H;
}

/***************************************************************
*函数功能  ：主控芯片从W25Q64的某个地址连写n个字节数据(不可跨页)
*函数名    ：W25Q64_Page_Write
*函数参数  ：void
*函数返回值：u32 address  u16 num_byte  u8 *str
*函数描述  ：页读取指令    0x02
****************************************************************/
void W25Q64_Page_Write(u32 address,u16 num_byte,u8 *str)
{
  //写使能
  W25Q64_Write_Enable();
  //写数据过程
  SPI_CS_L;
  SPI1_RS_Byte(0x02);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  
  while(num_byte)
  {
    SPI1_RS_Byte(*str);
    str++;
    num_byte--;
  }
  SPI_CS_H;
  //等待写完过程
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q64扇区擦除
*函数名    ：W25Q64_Sector_Erase
*函数参数  ：void
*函数返回值：u32 address
*函数描述  ：扇区擦除指令    0x20
****************************************************************/
void W25Q64_Sector_Erase(u32 address)
{
  //写使能
  W25Q64_Write_Enable();
  //擦除
  SPI_CS_L;
  SPI1_RS_Byte(0x20);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  SPI_CS_H;
  //等待擦除完成
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q64块区擦除
*函数名    ：W25Q64_Block_Erase
*函数参数  ：void
*函数返回值：u32 address
*函数描述  ：块擦除指令    0xd8
****************************************************************/
void W25Q64_Block_Erase(u32 address)
{
  //写使能
  W25Q64_Write_Enable();
  //擦除
  SPI_CS_L;
  SPI1_RS_Byte(0xd8);
  SPI1_RS_Byte(address>>16);
  SPI1_RS_Byte(address>>8);
  SPI1_RS_Byte(address);
  SPI_CS_H;
  //等待擦除完成
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q64芯片擦除
*函数名    ：W25Q64_Chip_Erase
*函数参数  ：void
*函数返回值：void
*函数描述  ：芯片擦除指令    0xc7
****************************************************************/
void W25Q64_Chip_Erase(void)
{
  //写使能
  W25Q64_Write_Enable();
  //擦除
  SPI_CS_L;
  SPI1_RS_Byte(0xc7);
  SPI_CS_H;
  //等待擦除完成
  while(W25Q64_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q64连续多个块擦除
*函数名    ：W25Q64_Blocks_Erase
*函数参数  ：void
*函数返回值：void
*函数描述  ：
****************************************************************/
void W25Q64_Blocks_Erase(u32 address,u8 num_blcok)
{
  while(num_blcok)
	{
		W25Q64_Block_Erase(address);
		address += 65536;
		num_blcok--;
	}
}

/******************************
函数功能： 读取W25Q64的ID
函数形参：无
函数返回值：无
*******************************/
u16 W25Q64_Read_id(void)
{
	u8 id_h = 0;
	u8 id_l = 0;
	u16 id= 0;
	SPI_CS_L;				//拉低片选
	SPI1_RS_Byte(0X90);			//发送读ID命令
	SPI1_RS_Byte(0X00);			
	SPI1_RS_Byte(0X00);			
	SPI1_RS_Byte(0X00);			//发送24位地址
	id_h = SPI1_RS_Byte(0x00);
	id_l = SPI1_RS_Byte(0x00);
	SPI_CS_H;
	id = (id_h << 8) | id_l;
	return id;
}


















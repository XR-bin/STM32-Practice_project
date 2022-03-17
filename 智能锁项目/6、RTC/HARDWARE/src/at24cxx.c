#include "sys.h" 
#include "at24cxx.h"
#include "delay.h"
#include "iic.h"
#include "stdio.h"

/*****************************************************
*函数功能  ：对at24cxx所需要的GPIO口进行初始化设置
*函数名    ：AT24C04_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*          时钟线  IIC_SCL  PB8   推挽输出
*          数据线  IIC_SDA  PB9   开漏输出
********************************************************/
void AT24CXX_Init(void)
{
  IIC_Init();
}

/*****************************************************************
*函数功能  ：写一个字节的数据到AT24CXX的某个内部地址里
*函数名    ：AT24CXX_Write_Byte
*函数参数  ：u16 address   u8 data
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*******************************************************************/
u8 AT24CXX_Write_Byte(u16 address,u8 data)
{
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXX > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //主机发送数据
  IIC_Send_Byte(data);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //主机发送停止信号
  IIC_Stop();
  //延时6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*****************************************************************
*函数功能  ：从AT24CXX的里读取1个字节的数据
*函数名    ：AT24CXX_Read_Byte
*函数参数  ：u8 address   u8 *data
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*******************************************************************/
u8 AT24CXX_Read_Byte(u16 address,u8 *data)
{
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXX > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //主机发送起始信号
  IIC_Start();
  //主机发送器件地址(读)
  IIC_Send_Byte(AT24CXX_READ);
  //主机接收应答
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //主机开始接收数据
  *data = IIC_Receive_Byte();
  //主机发送应答信号    //不应答信号
  IIC_Send_Ack(1);
  //主机发送停止信号
  IIC_Stop();
  
  return ack;
}

/*****************************************************************
*函数功能  ：写多个字节的数据到AT24CXX里（不能跨页）
*函数名    ：AT24CXX_Write_Bytes1
*函数参数  ：u16 address u8 byte_num u8 *Str
*函数返回值：u8 
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*******************************************************************/
u8 AT24CXX_Write_Bytes1(u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXX > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  for(i=0;i<byte_num;i++)
  {
    //主机发送数据
    IIC_Send_Byte(*Str);
    //主机接收应答
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      delay_ms(6);
      return AT24CXX_ERR3;
    }
    Str++;
  }
  //主机发送停止信号
  IIC_Stop();
  //延时6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*****************************************************************
*函数功能  ：写多个字节的数据到AT24CXX里（可跨页）
*函数名    ：AT24CXX_Write_Bytes
*函数参数  ：u16 address u8 byte_num u8 *Str
*函数返回值：u8 
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*******************************************************************/
u8 AT24CXX_Write_Bytes(u16 address,u8 byte_num,u8 *Str)
{
  u8 sck = 0;
  u8 remain;
  
  while(1)
  {
    //计算当前要存储的页还剩多少字节可以写
    remain =AT24CXX_PAGE - address%AT24CXX_PAGE;
    if(remain >= byte_num)
    {
      sck = AT24CXX_Write_Bytes1(address,byte_num,Str);
      break;
    }
    else
    {
      sck = AT24CXX_Write_Bytes1(address,remain,Str);
      byte_num -= remain;   //减去写入的数量
      address += remain;    //计算存储新地址
      Str += remain;        //计算数组的新地址
    } 
  }
  return sck;
}


/*****************************************************************
*函数功能  ：从AT24CXX的里读取多个字节的数据
*函数名    ：AT24CXX_Read_Bytes
*函数参数  ：u16 address   u8 byte_num    u8 *Str
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*******************************************************************/
u8 AT24CXX_Read_Bytes(u16 address,u8 byte_num,u8 *Str)
{
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXX > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //主机发送起始信号
  IIC_Start();
  //主机发送器件地址(读)
  IIC_Send_Byte(AT24CXX_READ);
  //主机接收应答
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  while(1)
  {
   //主机开始接收数据
   *Str = IIC_Receive_Byte();
   byte_num--;
   if(byte_num == 0)
   {
     IIC_Send_Ack(1);   //结束应答
     break;
   }
   IIC_Send_Ack(0);
   Str++;
  }
  //主机发送停止信号
  IIC_Stop();

  return ack;
}

/*******************************************************************************
*函数功能  ：写多个字节的数据到AT24CXX里(直接一个一个地址写，不用考虑跨不跨页)
*函数名    ：AT24CXX_Write_Addr
*函数参数  ：u16 address u8 byte_num u8 *Str
*函数返回值：u8 
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*********************************************************************************/
u8 AT24CXX_Write_Addr(u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXX_Write_Byte(address,*Str);
    address++;
    Str++;
  }
  return ack;
}

/*******************************************************************************
*函数功能  ：从AT24CXX的里读取多个字节的数据(直接一个一个地址写，不用考虑跨不跨页)
*函数名    ：AT24CXX_Read_Addr
*函数参数  ：u16 address   u8 byte_num    u8 *Str
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*********************************************************************************/
u8 AT24CXX_Read_Addr(u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXX_Read_Byte(address,Str);
    address++;
    Str++;
  }
  return ack;
}

/******************************************************************************************
*函数功能  ：(多个AT24CXX连接时)写一个字节的数据到某个AT24CXX的某个内部地址里
*函数名    ：AT24CXXaddr_Write_Byte
*函数参数  ：u16 AT24CXXmodel(器件型号)  u8 AT24CXXaddr(器件地址(写))   u16 address   u8 data   
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*********************************************************************************************/
u8 AT24CXXaddr_Write_Byte(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 data)
{
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //主机发送数据
  IIC_Send_Byte(data);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //主机发送停止信号
  IIC_Stop();
  //延时6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*****************************************************************************************
*函数功能  ：(多个AT24CXX连接时)从某个AT24CXX的某个存储地址里读取1个字节的数据
*函数名    ：AT24CXXaddr_Read_Byte
*函数参数  ：u16 AT24CXXmodel(器件型号)  u8 AT24CXXaddr(器件地址(写))   u16 address   u8 data   
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
******************************************************************************************/
u8 AT24CXXaddr_Read_Byte(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 *data)
{
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //主机发送起始信号
  IIC_Start();
  //主机发送器件地址(读)
  IIC_Send_Byte(AT24CXXaddr|0x1);
  //主机接收应答
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  //主机开始接收数据
  *data = IIC_Receive_Byte();
  //主机发送应答信号    //不应答信号
  IIC_Send_Ack(1);
  //主机发送停止信号
  IIC_Stop();
  
  return ack;
}

/*******************************************************************************************************
*函数功能  ：(多个AT24CXX)从某个AT24CXX的某个存储地址开始写多个字节的数据到（不能跨页）
*函数名    ：AT24CXXaddr_Write_Bytes1
*函数参数  ：u16 AT24CXXmodel(器件型号)  u8 AT24CXXaddr(器件地址(写))  u16 address   u8 byte_num  u8 *Str
*函数返回值：u8 
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
**********************************************************************************************************/
u8 AT24CXXaddr_Write_Bytes1(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  for(i=0;i<byte_num;i++)
  {
    //主机发送数据
    IIC_Send_Byte(*Str);
    //主机接收应答
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      delay_ms(6);
      return AT24CXX_ERR3;
    }
    Str++;
  }
  //主机发送停止信号
  IIC_Stop();
  //延时6ms
  delay_ms(6);
  
  return AT24CXX_NO_ERR;
}

/*******************************************************************************************************
*函数功能  ：(多个AT24CXX)从某个AT24CXX的某个存储地址开始写多个字节的数据到（可跨页）
*函数名    ：AT24CXXaddr_Write_Bytes
*函数参数  ：u16 AT24CXXmodel(器件型号)  u8 AT24CXXaddr(器件地址(写))  u16 address u8 byte_num u8 *Str
*函数返回值：u8 
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
**********************************************************************************************************/
u8 AT24CXXaddr_Write_Bytes(u16 AT24CXXmodel,u8 AT24CXXaddr,u8 AT24CXXpage,u16 address,u8 byte_num,u8 *Str)
{
  u8 sck = 0;
  u8 remain;
  
  while(1)
  {
    //计算当前要存储的页还剩多少字节可以写
    remain =AT24CXXpage - address%AT24CXXpage;
    if(remain >= byte_num)
    {
      sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,byte_num,Str);
      break;
    }
    else
    {
      sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,remain,Str);
      byte_num -= remain;   //减去写入的数量
      address += remain;    //计算存储新地址
      Str += remain;        //计算数组的新地址
    } 
  }
  return sck;
}

/**********************************************************************************************************
*函数功能  ：(多个AT24CXX)从某个AT24CXX的某个地址开始读取多个字节的数据
*函数名    ：AT24CXX_Read_Bytes
*函数参数  ：u16 AT24CXXmodel(器件型号)  u8 AT24CXXaddr(器件地址(写))   u16 address   u8 byte_num    u8 *Str
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
**********************************************************************************************************/
u8 AT24CXXaddr_Read_Bytes(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 ack;
  
  //主机发送起始信号
  IIC_Start();
  /*判断AT24CXX型号*/
  //如果是AT24C16以上的型号(不包括AT24C16)
  if(AT24CXXmodel > AT24C16)
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
    //发送高八位存储地址(字地址)
    IIC_Send_Byte(address>>8);
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR2;
    }
  }
  else 
  {
    //主机发送器件地址(写)
    IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));
    //接收从机应答信号
    ack = IIC_Receive_Ack();
    //判断应答
    if(ack != 0)
    {
      IIC_Stop();
      return AT24CXX_ERR1;
    }
  }
  //发送低八位存储地址(字地址)
  IIC_Send_Byte(address%256);
  //接收从机应答信号
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }  
  //主机发送起始信号
  IIC_Start();
  //主机发送器件地址(读)
  IIC_Send_Byte(AT24CXXaddr|1);
  //主机接收应答
  ack = IIC_Receive_Ack();
  //判断应答
  if(ack != 0)
  {
    IIC_Stop();
    return AT24CXX_ERR2;
  }
  while(1)
  {
   //主机开始接收数据
   *Str = IIC_Receive_Byte();
   byte_num--;
   if(byte_num == 0)
   {
     IIC_Send_Ack(1);   //结束应答
     break;
   }
   IIC_Send_Ack(0);
   Str++;
  }
  //主机发送停止信号
  IIC_Stop();

  return ack;
}

/*******************************************************************************
*函数功能  ：(多个AT24CXX)写多个字节的数据到AT24CXX里(直接一个一个地址写，不用考虑跨不跨页)
*函数名    ：AT24CXXaddr_Write_Addr
*函数参数  ：u16 address u8 byte_num u8 *Str
*函数返回值：u8 
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*********************************************************************************/
u8 AT24CXXaddr_Write_Addr(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXXaddr_Write_Byte(AT24CXXmodel,AT24CXXaddr,address,*Str);
    address++;
    Str++;
  }
  return ack;
}

/*******************************************************************************
*函数功能  ：从AT24CXX的里读取多个字节的数据(直接一个一个地址写，不用考虑跨不跨页)
*函数名    ：AT24CXX_Read_Addr
*函数参数  ：u16 address   u8 byte_num    u8 *Str
*函数返回值：u8
*描述      ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*********************************************************************************/
u8 AT24CXXaddr_Read_Addr(u16 AT24CXXmodel,u8 AT24CXXaddr,u16 address,u8 byte_num,u8 *Str)
{
  u8 i;
  u8 ack;
  for(i=0;i<byte_num;i++)
  {
    ack = AT24CXXaddr_Read_Byte(AT24CXXmodel,AT24CXXaddr,address,Str);
    address++;
    Str++;
  }
  return ack;
}





















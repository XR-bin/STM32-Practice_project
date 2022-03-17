#include "sys.h"	
#include "delay.h"	
#include "led.h"
#include "OLED.h"
#include "usart.h"
#include "bluetooth.h"
#include "at24cxx.h"
#include "stdio.h"
#include "rtc.h"
#include "BS8116A.h"
#include "MG200.h"
#include "spi.h"
#include "MFRC522.h"

/**********************************************
**RCC522工作流程
**RCC522初始化
**寻卡
**if(寻卡 == yes) 防冲撞
**选卡
**验证卡片
**读卡片/写卡片
**结束卡片操作
**********************************************/
u8 CardId[4];  				//卡号
u8 CardType[2];//卡类型
unsigned char RFID[16];			//存放RFID 
u8 writeDataBuf[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0,1,2,3,4,0xff,0xff,0xff}; //写入的数据

unsigned char total=0; 
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //卡密码-初始密码--白卡的出厂密码--
u8 MIMA_1[16]={88,88,88,88,88,88,0xff,0x07,0x80,0x29,88,88,88,88,88,88}; //扇区1的密码 格式(A密码 控制位  B密码 )
u8 MIMA_2[16]={88,88,88,88,88,88};//扇区1的A密码


void print_info(unsigned char *p,int cnt)
{
  int i;
  for(i=0;i<cnt;i++)
  {
		if(!i)
			printf("0x%02X ",p[i]);
		else
			printf("%02X ",p[i]);
  }
  printf("\r\n");
}

/**********************************************
**函数功能：寻卡---》防冲撞 ---》选卡

**参数说明: 
pTagType[OUT]：卡片类型代码(做输出参数)
**                0x4400 = Mifare_UltraLight
**                0x0400 = Mifare_One(S50)
**                0x0200 = Mifare_One(S70)
**                0x0800 = Mifare_Pro(X)
**                0x4403 = Mifare_DESFire
pSnr[OUT]:卡片序列号，4字节(做输出参数)

**返回值：成功返回MI_OK
**作者：weihao Mo
**********************************************/
u8 Identificate_Card(u8 *pTagType,u8 *pSnr)
{
  u8 status=MI_ERR;
	u8 STA=0;

   if(MFRC_PcdRequest(PICC_REQALL,pTagType)==MI_OK)//寻卡成功
   {
      STA |=0x01;
      if(MFRC_PcdAnticoll(pSnr)==MI_OK)/*防冲撞成功*/
      {
				STA |=0x02;
		
        if(MFRC_PcdSelect(pSnr)==MI_OK)//选卡成功
        {
					STA |=0x04;
        }
      }
   }
	 if(STA==0x07)
		 status=MI_OK;
	 
  return status;
}

/*******************************************************************************************************************
*功能：读数据到指定块
*参数：
*u8   auth_mode ：验证密码的类型。（分为PICC_AUTHENT1A和PICC_AUTHENT1B）
*u8   addr      ：数据存放的地址。每个扇区的0、1、2块是存放数据。3是存放密码。
*一般填：0、1、2 、4、5、6、8、9、10
*u8 *Src_Key    ：原密码 
*u8 *data       ：读出的数据
*u8 *pSnr       ：卡号
*
*数据一般格式：u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //写入的金额；
*********************************************************************************************************************/
u8 ReadCardData(u8 auth_mode,u8 addr,u8 *Src_Key,u8 *data,u8 *pSnr)
{
  int status;
	
  /*1. 寻卡--防冲撞--选卡*/
  status=Identificate_Card(CardType,pSnr);  
  /*2. 验证卡密码*/
  if(status==MI_OK)
  {
		printf("卡类型:");
		print_info(CardType,2);//打印类型
		printf("卡号:");
		print_info(pSnr,4);//打印卡号		
    status=MFRC_PcdAuthState(auth_mode,addr/4*4+3,KEY,pSnr);   //验证卡片密码 形参参数：验证方式，块地址，密码，卡许列号
    printf("验证卡密码成功\r\n");
  }
  /*3. 读出数据*/
  if(status==MI_OK)
  {
    status=MFRC_PcdRead(addr,data); //从第addr块读出数据值。
  }
  if(status==MI_OK)
  {
    printf("数据读出成功!\r\n");
  }
  return status;
}

/*********************************************************************************************************************
*功能：写数据到指定块
*参数：
*			u8   auth_mode ：验证密码的类型。（分为PICC_AUTHENT1A和PICC_AUTHENT1B）
*			u8   addr      ：数据存放的地址。每个扇区的0、1、2块是存放数据。3是存放密码。
*			一般填：0、1、2 、4、5、6、8、9、10----  (绝对地址块)
*			u8 *Src_Key    ：原密码 
*			u8 *data       ：新写入的数据
*			u8 *pSnr       ：卡号
*
*数据一般格式：u8 writeDataBuf[16]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}; //写入的金额；
*****************************************************************************************************************************/
u8 WriteCardData(u8 auth_mode,u8 addr,u8 *Src_Key,u8 *data,u8 *pSnr)
{
  int status=MI_ERR;

  /*1. 寻卡--防冲撞--选卡*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. 验证卡密码*/
  if(status==MI_OK)
  {
    status =MFRC_PcdAuthState(0x60,addr/4*4+3,KEY,pSnr);   //验证卡片密码 形参参数：验证方式，块地址，密码，卡许列号
  }
  if(status==MI_OK)
  {
    printf("密码验证成功!\r\n");
  }
  /*3. 写数据到卡*/
  if(status==MI_OK)
  {
    status = MFRC_PcdWrite(addr,data); //写数据到第addr块，data入的数据值。
  }
  if(status==MI_OK)
  {
    printf("数据写入OK!\r\n");
  }
  return status;
}

/**********************************************
*函数功能：修改卡密码
*
*参数说明: 
* 		u8   auth_mode ：验证密码的类型。（分为PICC_AUTHENT1A和PICC_AUTHENT1B）
*			u8   addr      ：密码存放的地址。（密码放在每个扇区的第3个块。第一个扇区比较特殊。）
*			一般填：3、7、11.....规律
*			u8 *Src_Key    ：原密码 
*			u8 *New_Key    ：新密码
*			u8 *pSnr       ：卡号
*
*			密码格式：
*			u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //验证A或者B密码的格式
*			u8 MIMA_1[16]={88,88,88,88,88,88,0xff,0x07,0x80,0x29,88,88,88,88,88,88}; //修改新密码的格式(A密码 控制位  B密码 )
*
*			返回值：成功返回MI_OK
**********************************************/
int Card_Passworld(u8 auth_mode,u8 addr,u8 *Src_Key,u8 *New_Key,u8 *pSnr)
{
  int status;

  /*1. 寻卡--防冲撞--选卡*/
  status=Identificate_Card(CardType,pSnr); 
  /*2. 验证卡密码*/
  if(status==MI_OK)
  {
    status = MFRC_PcdAuthState(auth_mode,addr,Src_Key,pSnr);   //验证卡片密码 形参参数：验证方式，块地址，密码，卡许列号
  }
  /*3. 写数据到卡*/
  if(status==MI_OK)
  {
    status = MFRC_PcdWrite(addr,New_Key); //写数据到第addr块，New_Key写入的数据值。
  }
  return status;
}

int main(void)
{		
	u8 readDataBuf[16];	
	
	SysTick_Init();	  	 //延时初始化
	USART1_Init(115200);
	LED_Init();
	SPI2_Init();
	MFRC_Init();
	
	while(1)
	{
		delay_ms(1000);
		//注意：第0块是没法修改的
		if(ReadCardData(PICC_AUTHENT1A,10,KEY,readDataBuf,CardId)==MI_OK)
		print_info(readDataBuf,16);
	}
}





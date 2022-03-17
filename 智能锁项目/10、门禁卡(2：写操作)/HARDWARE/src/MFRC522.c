#include "MFRC522.h"
#include "sys.h"
#include "delay.h"
#include "spi.h"
#include "string.h"

void delay_spi(void)
{
	u8 t=5;
	while(t--);
}

/*************************************************************************
*函数功能  ：初始化MFRC对应的GPIO口
*函数名    ：MFRC522_IO_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SPI2_MOSI ------- PB15  主输出数据线
*            SPI2_MISO ------- PB14  主输入数据线
*            SPI2_SCLK ------- PB13  时钟线
*            RC_RES    ------- PA7   复位 
*            RC_CS     ------- PA6  片选选线
***************************************************************************/
void MFRC522_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx配置结构体
  
  //GPIOx时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	     //使能PA端口时钟
	
	//PA端口
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;                 //FMRC522  PA6/7 端口配置
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 		             //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                   //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					                       //根据设定参数初始化PA6/7
	
	//片选线拉高
	GPIO_SetBits(GPIOA, GPIO_Pin_6); 
	GPIO_SetBits(GPIOA, GPIO_Pin_7); 
}

/**********************************************************************
*函数功能  ：对MFRC522的某个寄存器进行写操作
*函数名    ：MFRC_Write
*函数参数  ：u8 addr ,u8 data  
*函数返回值：u8
*描述      ：
*            数据命令选择线    MFRC_DC_L   MFRC_DC_H
*            时钟线            MFRC_CLK_L  MFRC_CLK_H
*            硬复位MFRC       MFRC_RES_L   MFRC_RES_H
*            主输出数据线      MFRC_MOSI_L  MFRC_MOSI_H
************************************************************************/
void MFRC_Write(u8 addr,u8 data)
{
	u8 ucAddr;

  //片选线拉低
	MFRC_CS_L;
	
	//地址格式：第7字节：读写判断字节  +  第6-1字节：地址  +  第8字节：0
	ucAddr = ((addr<<1)&0x7E);
	
	//发送要修改的寄存器地址
	SPI2_RS_Byte(ucAddr);
	//修改值
	SPI2_RS_Byte(data);
	
	//片选线拉高
	MFRC_CS_H;
}

/**********************************************************************
*函数功能  ：对MFRC522的某个寄存器进行写操作
*函数名    ：MFRC_Read
*函数参数  ：u8 addr ,u8 data  
*函数返回值：u8
*描述      ：
*            数据命令选择线    MFRC_DC_L   MFRC_DC_H
*            时钟线            MFRC_CLK_L  MFRC_CLK_H
*            硬复位MFRC       MFRC_RES_L   MFRC_RES_H
*            主输出数据线      MFRC_MOSI_L  MFRC_MOSI_H
************************************************************************/
u8 MFRC_Read(u8 addr)
{
	u8   ucAddr;
	u8   ucResult=0;
	
	//片选线拉低
	MFRC_CS_L;
	
	//地址格式：第7字节：读写判断字节  +  第6-1字节：地址  +  第8字节：0
	ucAddr = ((addr<<1)&0x7E)|0x80;

	SPI2_RS_Byte(ucAddr);
	ucResult=SPI2_RS_Byte(0);
	
	//片选线拉高
	MFRC_CS_H;
	
	return ucResult;
}

/**********************************************************************
*函数功能  ：对MFRC522进行复位
*函数名    ：MFRC_RES
*函数参数  ：void  
*函数返回值：void
*描述      ：
*             MFRC_RES_L      //开始复位
*             MFRC_RES_H      //结束复位
************************************************************************/
u8 MFRC_RES(void)
{
	//对MFRC522进行复位
	MFRC_RES_H ;
	delay_us(2);
	MFRC_RES_L;
	delay_us(2);
	MFRC_RES_H ;
	delay_us(2);
	
	//设置MFRC522内部的寄存器
	MFRC_Write(CommandReg,PCD_RESETPHASE);
	MFRC_Write(CommandReg,PCD_RESETPHASE);
	delay_us(2);
	
	MFRC_Write(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
	MFRC_Write(TReloadRegL,30);           
	MFRC_Write(TReloadRegH,0);
	MFRC_Write(TModeReg,0x8D);
	MFRC_Write(TPrescalerReg,0x3E);
	
	MFRC_Write(TxAutoReg,0x40);//必须要
	
	return MI_OK;
}

/**********************************************************************
*函数功能  ：清除MFRC522的某个寄存器的位
*函数名    ：MFRC_ClearBitMask
*函数参数  ：u8 reg,u8 mask    //要处理的寄存器地址     要清的位(二进制跳出要清的位置1) 
*函数返回值：void
*描述      ：
************************************************************************/
void MFRC_ClearBitMask(u8 reg,u8 mask) 
{
    char   tmp = 0x0;
    tmp = MFRC_Read(reg);
    MFRC_Write(reg, tmp & ~mask);  // clear bit mask
} 

/**********************************************************************
*函数功能  ：置1MFRC522的某个寄存器的位
*函数名    ：MFRC_SetBitMask
*函数参数  ：u8 reg,u8 mask    //要处理的寄存器地址     要清的位(二进制跳出要清的位置1) 
*函数返回值：void
*描述      ：
************************************************************************/
void MFRC_SetBitMask(u8 reg,u8 mask) 
{
    u8 tmp = 0x0;
    tmp = MFRC_Read(reg);
    MFRC_Write(reg, tmp|mask);  
}

/**********************************************************************
*函数功能  ：对MFRC522的天线进行关闭
*函数名    ：MFRC_PcdAntennaOff
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
void MFRC_PcdAntennaOff(void)
{
	MFRC_ClearBitMask(TxControlReg, 0x03);
}

/**********************************************************************
*函数功能  ：对MFRC522的天线进行开启
*函数名    ：MFRC_PcdAntennaON
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
void MFRC_PcdAntennaON(void)
{
	u8 i;
	
	i = MFRC_Read(TxControlReg);
	if (!(i & 0x03))
	{
			MFRC_SetBitMask(TxControlReg, 0x03);
	}
}

/**********************************************************************
*函数功能  ：对MFRC522的工作模式进行设置
*函数名    ：MFRC_M500PcdConfigISOType
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_M500PcdConfigISOType(u8 type)
{
   if (type == 'A')                     //ISO14443_A
   { 
			MFRC_ClearBitMask(Status2Reg,0x08);
			MFRC_Write(ModeReg,0x3D);//3F
			MFRC_Write(RxSelReg,0x86);//84
			MFRC_Write(RFCfgReg,0x7F);   //4F
			MFRC_Write(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
			MFRC_Write(TReloadRegH,0);
			MFRC_Write(TModeReg,0x8D);
			MFRC_Write(TPrescalerReg,0x3E);
			delay_us(2);
			MFRC_PcdAntennaON();
   }
   else 
		  return 1;
   
   return 0;
}


/**********************************************************************
*函数功能  ：对MFRC522的进行初始化
*函数名    ：MFRC_Init
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
void MFRC_Init(void)
{
	MFRC522_IO_Init();
	MFRC_RES();             //对MFRC522进行复位操作
	MFRC_PcdAntennaOff();   //关闭天线
	MFRC_PcdAntennaON();    //开启天线
	MFRC_M500PcdConfigISOType('A');   //设置工作模式
} 

/**********************************************************************
*函数功能  ：MFRC522与ISO14443卡通讯
*函数名    ：MFRC_PcdComMF522
*函数参数  ：
            Command[IN]     :RC522命令字
					  pIn [IN]        :通过RC522发送到卡片的数据
						InLenByte[IN]   :发送数据的字节长度
						pOut [OUT]      :接收到的卡片返回数据
					  *pOutLenBit[OUT]:返回数据的位长度 
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_PcdComMF522(u8 Command, u8 *pIn ,u8 InLenByte,u8 *pOut ,u8 *pOutLenBit)
{
	u8 status = MI_ERR;   //通信情况变量
	u8 irqEn   = 0x00;
	u8 waitFor = 0x00;
	u8 lastBits;
	u8 n;
	u16 i;
	
	//判断通信是为了验证秘钥还是收发数据
	switch (Command)
	{
		//验证秘钥
		case PCD_AUTHENT:   
										irqEn   = 0x12;
										waitFor = 0x10;
										break;
		//收发数据
		case PCD_TRANSCEIVE:
										irqEn   = 0x77;
										waitFor = 0x30;
										break;
		default:	break;
	}
	
	MFRC_Write(ComIEnReg,irqEn|0x80);
	MFRC_ClearBitMask(ComIrqReg,0x80);	    //清所有中断位
	MFRC_Write(CommandReg,PCD_IDLE);
	MFRC_SetBitMask(FIFOLevelReg,0x80);	 	  //清FIFO缓存
	
	//准备好要对SO14443卡通信的数据
	for (i=0; i<InLenByte; i++) MFRC_Write(FIFODataReg, pIn [i]); 
	
  MFRC_Write(CommandReg, Command);

	//判断是否是发送并接收数据指令
	if (Command == PCD_TRANSCEIVE) MFRC_SetBitMask(BitFramingReg,0x80); //开始传送
	
	i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
	
	do 
	{
			n = MFRC_Read(ComIrqReg);
			i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitFor));
	MFRC_ClearBitMask(BitFramingReg,0x80);
	
	if (i!=0)
	{    
		if(!(MFRC_Read(ErrorReg)&0x1B))
		{
				status = MI_OK;
				if (n & irqEn & 0x01)  status = MI_NOTAGERR;
				if (Command == PCD_TRANSCEIVE)
				{
						n = MFRC_Read(FIFOLevelReg);
						lastBits = MFRC_Read(ControlReg) & 0x07;
						if (lastBits)  *pOutLenBit = (n-1)*8 + lastBits;  
						else  *pOutLenBit = n*8;
						if (n == 0)  n = 1;
						if (n > MAXRLEN) n = MAXRLEN;
						//获取数据
						for (i=0; i<n; i++) pOut[i] = MFRC_Read(FIFODataReg);
				}
		}
		else
			 status = MI_ERR;
	}
	
	MFRC_SetBitMask(ControlReg,0x80);           // stop timer now
	MFRC_Write(CommandReg,PCD_IDLE); 
	return status;
}



/**********************************************************************
*函数功能  ：MFRC522进行寻卡操作
*函数名    ：MFRC_PcdRequest
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_PcdRequest(u8 req_code,u8 *pTagType)
{
	u8 status; 
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 
	
	MFRC_ClearBitMask(Status2Reg,0x08);
	MFRC_Write(BitFramingReg,0x07);
	MFRC_SetBitMask(TxControlReg,0x03);
	
	ucComMF522Buf[0] = req_code;
	
	status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	
	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
     status = MI_ERR;  
   
	return status;
}

/**********************************************************************
*函数功能  ：MFRC522进行防冲撞操作
*函数名    ：MFRC_PcdAnticoll
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_PcdAnticoll(u8 *pSnr)
{
	u8 status;
	u8 i,snr_check=0;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 
    

  MFRC_ClearBitMask(Status2Reg,0x08);
  MFRC_Write(BitFramingReg,0x00);
  MFRC_ClearBitMask(CollReg,0x80);
 
	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x20;

	status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

	if (status == MI_OK)
	{
		for (i=0; i<4; i++)
		{   
			 *(pSnr+i)  = ucComMF522Buf[i];
			 snr_check ^= ucComMF522Buf[i];
		}

		if (snr_check != ucComMF522Buf[i]) status = MI_ERR;
	}
    
  MFRC_SetBitMask(CollReg,0x80);
	
	return status;
}

/**********************************************************************
*函数功能  ：用MFRC522计算CRC16函数
*函数名    ：CalulateCRC
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
void CalulateCRC(u8 *pIn ,u8 len,u8 *pOut )
{
    u8 i,n;
	
    MFRC_ClearBitMask(DivIrqReg,0x04);
    MFRC_Write(CommandReg,PCD_IDLE);
    MFRC_SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++) MFRC_Write(FIFODataReg, *(pIn +i)); 
	
    MFRC_Write(CommandReg, PCD_CALCCRC);
	
    i = 0xFF;
	
    do 
    {
        n = MFRC_Read(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = MFRC_Read(CRCResultRegL);
    pOut [1] = MFRC_Read(CRCResultRegM);
}

/**********************************************************************
*函数功能  ：MFRC522进行选定卡操作
*函数名    ：MFRC_PcdSelect
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_PcdSelect(u8 *pSnr)
{
    u8 status;
    u8 i;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    MFRC_ClearBitMask(Status2Reg,0x08);

    status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18)) status = MI_OK; 
    else  status = MI_ERR;

    return status;
}

/**********************************************************************
*函数功能  ：MFRC522进行卡密验证
*函数名    ：MFRC_PcdAuthState
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr)
{
	u8 status;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = auth_mode;
	ucComMF522Buf[1] = addr;
	
	memcpy(&ucComMF522Buf[2], pKey, 6); 
	memcpy(&ucComMF522Buf[8], pSnr, 4); 
	
	status = MFRC_PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
	if ((status != MI_OK) || (!(MFRC_Read(Status2Reg) & 0x08))) status = MI_ERR;
	
	return status;
}

/**********************************************************************
*函数功能  ：MFRC522读取卡的一个块的数据
*函数名    ：MFRC_PcdRead
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_PcdRead(u8 addr,u8 *p)
{
    u8 status;
    u8 unLen;
    u8 i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
           *(p +i) = ucComMF522Buf[i]; 
    }
    else
         status = MI_ERR; 
    
    return status;
}

/**********************************************************************
*函数功能  ：MFRC522写数据到一个块里
*函数名    ：MFRC_PcdWrite
*函数参数  ：void  
*函数返回值：void
*描述      ：
************************************************************************/
u8 MFRC_PcdWrite(u8 addr,u8 *p)
{
	u8 status;
	u8 unLen;
	u8 i,ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_WRITE;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

	status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

	if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A)) status = MI_ERR;
			
	if (status == MI_OK)
	{
			for (i=0; i<16; i++)
			{    
				ucComMF522Buf[i] = *(p +i);   
			}
			CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

			status = MFRC_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
			
			if ((status != MI_OK)||(unLen != 4)||((ucComMF522Buf[0]&0x0F) != 0x0A)) status = MI_ERR;
	}

	return status;
}











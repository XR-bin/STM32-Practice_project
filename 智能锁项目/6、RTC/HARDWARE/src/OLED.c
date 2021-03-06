#include "sys.h"
#include "OLED.h"
#include "delay.h"
#include "matrix.h"
#include "stdio.h"

/*************************************************************************
*函数功能  ：初始化OLED对应的GPIO口
*函数名    ：OLED_IO_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SPI2_MOSI ------- PB15  主输出数据线
*            SPI2_SCLK ------- PB13  时钟线
*            OLED_DC   ------- PA11  数据命令线(0，读写命令； 1，读写数据)
*            OLED_CS   ------- PA15  片选选线
*            OLED_RES  ------- PA12  复位 
***************************************************************************/
void OLED_IO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;         //GPIOx配置结构体
  
  //GPIOx时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	     //使能PA端口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	     //使能PB端口时钟
	
  //GPIOx配置
	//PB端口
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_15;	//OLED  PB13/15 端口配置
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 		//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		      //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					          //根据设定参数初始化PB13/15
	
	//PA端口
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;   //OLED  PA11/12/15 端口配置
	GPIO_Init(GPIOA, &GPIO_InitStructure);					                       //根据设定参数初始化PA11/12/15
	
	//片选线拉高
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
}

/**********************************************************************
*函数功能  ：向SSD1306写入一个字节的数据或命令
*函数名    ：OLED_WR_Byte
*函数参数  ：u8 data    u8 DC
*函数返回值：void
*描述      ：
*            数据命令选择线    OLED_DC_L    OLED_DC_H
*            时钟线            OLED_CLK_L   OLED_CLK_H
*            硬复位OLED        OLED_RES_L   OLED_RES_H
*            主输出数据线      OLED_MOSI_L  OLED_MOSI_H
*
*            当参数DC为 1 的时候发送数据 ， 为 0 的时候发送命令
************************************************************************/
void OLED_WR_Byte(u8 data,u8 DC)
{
  u8 i;
  
  //数据线拉低
  OLED_MOSI_L;
  //片选线拉低
	OLED_DS_L;
	
  //判断接下来要发送的 data 是命令还是数据
  if(DC) OLED_DC_H;     //发送数据
  else OLED_DC_L;       //发送命令
  
  //发送1个字节命令/数据
  for(i=0;i<8;i++)
  {
    //拉低时钟线
    OLED_CLK_L;
    //判断命令/数据的第7位是 0 还是 1
    if(data & 0x80) OLED_MOSI_H;   //数据线拉高
    else OLED_MOSI_L;              //数据线拉低
    //拉高时钟线
    OLED_CLK_H;
    //命令/数据左移
    data <<= 1;
  }
	//片选线拉高
	OLED_DS_H;
}

/******************************************************************************************
*函数功能  ：对OLE屏清屏(清屏后屏幕是暗的)
*函数名    ：OLED_Clear
*函数参数  ：void
*函数返回值：void
*描述      ：        
*            命令              OLED_COM     0      
*            数据              OLED_DATA    1      
*
*                              指令集
*                    0x00~0x0f:  设置8位起始列地址的低四位。
*                    0x10~0x1f:  设置8位起始列地址的高四位。
*        0x81指令后接0x00~0xff:  设置对比度，数值越大越亮。
*     0x8D指令后接0X14或者0x10:  开启或者关闭电荷泵。
*                   0xAE或0xAF:  设置屏幕显示关（睡眠模式）或者显示开（正常模式）
*                    0xb0~0xb7:  设置页地址（共8页，每页8个像素点），本款OLED有128*64个像素点
*
*        注意：  
*              这里说的页指的是行，一页控制八行，一共有8页(64行)
*              这里的列有两个指令控制，分别控制低四位和高四位，一共有128列
*              发送数据时，一个字节数据控制8行1列的状态
**********************************************************************************************/
void OLED_Clear(void)
{
  u8 i,j;
  for(i=0;i<OLED_ROW;i++)
  {
    OLED_WR_Byte(0xb0+i,OLED_COM);   //选择0+i页(控制8*i ~ 8*(i+1)-1行)
		OLED_WR_Byte(0x10,OLED_COM);     //设置显示位置—列高地址  从第0列开始
    OLED_WR_Byte(0x00,OLED_COM);     //设置显示位置—列低地址
    for(j=0;j<OLED_LINE;j++) OLED_WR_Byte(0,OLED_DATA); //一个字节控制8行1列
  }
}

/******************************************************************************************
*函数功能  ：设置显示坐标
*函数名    ：OLED_XY
*函数参数  ：u8 x    u8 y
*函数返回值：void
*描述      ： 
*            命令              OLED_COM     0      
*            数据              OLED_DATA    1      
*
*                              指令集
*                    0x00~0x0f:  设置8位起始列地址的低四位。
*                    0x10~0x1f:  设置8位起始列地址的高四位。
*        0x81指令后接0x00~0xff:  设置对比度，数值越大越亮。
*     0x8D指令后接0X14或者0x10:  开启或者关闭电荷泵。
*                   0xAE或0xAF:  设置屏幕显示关（睡眠模式）或者显示开（正常模式）
*                    0xb0~0xb7:  设置页地址（共8页，每页8个像素点），本款OLED有128*64个像素点
*
*        注意：  
*              这里说的页指的是行，一页控制八行，一共有8页(64行)
*              这里的列有两个指令控制，分别控制低四位和高四位，一共有128列
*              发送数据时，一个字节数据控制8行1列的状态
**********************************************************************************************/
void OLED_XY(u8 x,u8 y)
{
  OLED_WR_Byte(0xb0+y,OLED_COM);           //设置哪一页(哪八行)
	OLED_WR_Byte(0x10|(x>>4),OLED_COM);      //设置哪一列的高8位
  OLED_WR_Byte(0x00|(x&0x0f),OLED_COM);    //设置哪一列的低8位
}

/******************************************************************************************
*函数功能  ：启动OLED屏
*函数名    ：OLED_Open
*函数参数  ：void 
*函数返回值：void
*描述      ： 
*            命令              OLED_COM     0      
*            数据              OLED_DATA    1      
*
*                              指令集
*                    0x00~0x0f:  设置8位起始列地址的低四位。
*                    0x10~0x1f:  设置8位起始列地址的高四位。
*        0x81指令后接0x00~0xff:  设置对比度，数值越大越亮。
*     0x8D指令后接0X14或者0x10:  开启或者关闭电荷泵。
*                   0xAE或0xAF:  设置屏幕显示关（睡眠模式）或者显示开（正常模式）
*                    0xb0~0xb7:  设置页地址（共8页，每页8个像素点），本款OLED有128*64个像素点
*
*        注意：  
*              这里说的页指的是行，一页控制八行，一共有8页(64行)
*              这里的列有两个指令控制，分别控制低四位和高四位，一共有128列
*              发送数据时，一个字节数据控制8行1列的状态
**********************************************************************************************/
void OLED_Open(void)
{
  //开启电荷泵
  OLED_WR_Byte(0x8D,OLED_COM);
  OLED_WR_Byte(0x14,OLED_COM);
  //让OLED进入正常工作模式
  OLED_WR_Byte(0xAF,OLED_COM);
}


/******************************************************************************************
*函数功能  ：关闭OLED屏
*函数名    ：OLED_Close
*函数参数  ：void 
*函数返回值：void
*描述      ： 
*            命令              OLED_COM     0      
*            数据              OLED_DATA    1      
*
*                              指令集
*                    0x00~0x0f:  设置8位起始列地址的低四位。
*                    0x10~0x1f:  设置8位起始列地址的高四位。
*        0x81指令后接0x00~0xff:  设置对比度，数值越大越亮。
*     0x8D指令后接0X14或者0x10:  开启或者关闭电荷泵。
*                   0xAE或0xAF:  设置屏幕显示关（睡眠模式）或者显示开（正常模式）
*                    0xb0~0xb7:  设置页地址（共8页，每页8个像素点），本款OLED有128*64个像素点
*
*        注意：  
*              这里说的页指的是行，一页控制八行，一共有8页(64行)
*              这里的列有两个指令控制，分别控制低四位和高四位，一共有128列
*              发送数据时，一个字节数据控制8行1列的状态
**********************************************************************************************/
void OLED_Close(void)
{
  //关闭电荷泵
  OLED_WR_Byte(0x8D,OLED_COM);
  OLED_WR_Byte(0x10,OLED_COM);
  //让OLED进入睡眠模式
  OLED_WR_Byte(0xAE,OLED_COM);
}

/*************************************************************************
*函数功能  ：OLED正式初始化
*函数名    ：OLED_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            数据命令选择线    OLED_DC_L    OLED_DC_H
*            时钟线            OLED_CLK_L   OLED_CLK_H
*            硬复位OLED        OLED_RES_L   OLED_RES_H
*            主输出数据线      OLED_MOSI_L  OLED_MOSI_H
*
*            命令              OLED_COM          0      
*            数据              OLED_DATA         1 
***************************************************************************/
void OLED_Init(void)
{
  //GPIOx初始化
  OLED_IO_Init();
  
  //硬复位OLED (这一步是必须的)
	OLED_RES_L;
  delay_ms(200);
  OLED_RES_H;
	delay_ms(200);
  
  /*厂家代码*/  
	OLED_WR_Byte(0xAE,OLED_COM); //关闭显示
  OLED_WR_Byte(0xD5,OLED_COM); //设置时钟分频因子,震荡频率
  OLED_WR_Byte(80,OLED_COM);   //[3:0],分频因子;[7:4],震荡频率
  OLED_WR_Byte(0xA8,OLED_COM); //设置驱动路数
  OLED_WR_Byte(0X3F,OLED_COM); //默认0X3F(1/64)
  OLED_WR_Byte(0xD3,OLED_COM); //设置显示偏移
  OLED_WR_Byte(0X00,OLED_COM); //默认为0

  OLED_WR_Byte(0x40,OLED_COM); //设置显示开始行 [5:0],行数.

  OLED_WR_Byte(0x8D,OLED_COM); //电荷泵设置
  OLED_WR_Byte(0x14,OLED_COM); //bit2，开启/关闭
  OLED_WR_Byte(0x20,OLED_COM); //设置内存地址模式
  OLED_WR_Byte(0x02,OLED_COM); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
  OLED_WR_Byte(0xA1,OLED_COM); //段重定义设置,bit0:0,0->0;1,0->127;
  OLED_WR_Byte(0xC8,OLED_COM); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
  OLED_WR_Byte(0xDA,OLED_COM); //设置COM硬件引脚配置
  OLED_WR_Byte(0x12,OLED_COM); //[5:4]配置

  OLED_WR_Byte(0x81,OLED_COM); //对比度设置
  OLED_WR_Byte(0xEF,OLED_COM); //1~255;默认0X7F (亮度设置,越大越亮)
  OLED_WR_Byte(0xD9,OLED_COM); //设置预充电周期
  OLED_WR_Byte(0xf1,OLED_COM); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLED_WR_Byte(0xDB,OLED_COM); //设置VCOMH 电压倍率
  OLED_WR_Byte(0x30,OLED_COM); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLED_WR_Byte(0xA4,OLED_COM); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
  OLED_WR_Byte(0xA6,OLED_COM); //设置显示方式;bit0:1,反相显示;0,正常显示
  OLED_WR_Byte(0xAF,OLED_COM); //开启显示
	
  /*清屏*/
  OLED_Clear();
  /*初始坐标*/
  OLED_XY(0,0);
}

/*************************************************************************
*函数功能  ：OLED显示一个size大小的字符
*函数名    ：OLED_ShowChar
*函数参数  ：u8 x  u8 y  u8 size  u8 ch
*函数返回值：void
*描述      ：
***************************************************************************/
void OLED_ShowChar(u16 x,u8 y,u8 size,u8 ch)
{
  u8 i,c;
  
  //计算字符偏移量
  c = ch - ' ';
  
  //判断是否超出列数
  if(x+size > (OLED_LINE-1))
  {
    y=y+(size/8)*((x+size)/(OLED_LINE-1));   //y值不要超过7
    if((x+size-(OLED_LINE-1))<size)x=0;
    else x=(x+size-(OLED_LINE-1))/size*size;
  }
  
  //选择字体大小
  if(size == 8)
  {
    /*注意：8的字符由1页组成*/
    //确定坐标
    OLED_XY(x,y);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<4;i++)
		OLED_WR_Byte(CH8[c*4+i],OLED_DATA);
  }
  else if(size == 16)
  {
    /*注意：16的字符由2页组成*/
    //确定坐标
    OLED_XY(x,y);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<8;i++)
		OLED_WR_Byte(CH16[c*16+i],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+1);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<8;i++)
    OLED_WR_Byte(CH16[c*16+i+8],OLED_DATA);
  }
  else if(size == 24)
  {
    /*注意：24的字符由3页组成*/
    //确定坐标
    OLED_XY(x,y);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<12;i++)
		OLED_WR_Byte(CH24[c*36+i],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+1);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<12;i++)
    OLED_WR_Byte(CH24[c*36+i+12],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+2);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<12;i++)
    OLED_WR_Byte(CH24[c*36+i+24],OLED_DATA);
  }
  else if(size == 32)
  {
    /*注意：24的字符由4页组成*/
    //确定坐标
    OLED_XY(x,y);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<16;i++)
		OLED_WR_Byte(CH32[c*64+i],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+1);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<16;i++)
    OLED_WR_Byte(CH32[c*64+i+16],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+2);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<16;i++)
    OLED_WR_Byte(CH32[c*64+i+32],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+3);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<16;i++)
    OLED_WR_Byte(CH32[c*64+i+48],OLED_DATA);
  }
}

/*************************************************************************
*函数功能  ：OLED显示一组size大小的字符串
*函数名    ：OLED_ShowString
*函数参数  ：u8 x  u8 y  u8 size  u8 *str
*函数返回值：void
*描述      ：
***************************************************************************/
void OLED_ShowString(u8 x,u8 y,u8 size,u8 *str)
{
  u8 i=0;
  
  while(*str != '\0')
  {
    OLED_ShowChar(x+i*(size/2),y,size,*str);
    str++;
    i++;
  }
}

/*************************************************************************
*函数功能  ：OLED显示一个size大小的汉字
*函数名    ：OLED_ShowHanzi
*函数参数  ：u8 x  u8 y  u8 size  u8 ch
*函数返回值：void
*描述      ：
***************************************************************************/
void OLED_ShowHanzi(u16 x,u8 y,u8 size,u8 *hz)
{
  u8 i,h=0;
  
  //计算汉字偏移量
  while(1)
  {
    if((*hz == HZ[h*2])&&(*(hz+1)==HZ[h*2+1]))
    {
      break;
    }
    h++;
  }
  //判断是否超出列数
  if(x+size > (OLED_LINE-1))
  {
    y=y+(size/8)*((x+size)/(OLED_LINE-1));   //y值不要超过7
    if((x+size-(OLED_LINE-1))<size)x=0;
    else x=(x+size-(OLED_LINE-1))/size*size;
  }
  
  if(size == 16)
  {
    /*注意：16的字符由2页组成*/
    //确定坐标
    OLED_XY(x,y);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<16;i++)
		OLED_WR_Byte(HZ16[h*32+i],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+1);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<16;i++)
    OLED_WR_Byte(HZ16[h*32+i+16],OLED_DATA);
  }
  if(size == 24)
  {
    /*注意：16的字符由2页组成*/
    //确定坐标
    OLED_XY(x,y);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<24;i++)
		OLED_WR_Byte(HZ24[h*72+i],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+1);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<24;i++)
    OLED_WR_Byte(HZ24[h*72+i+24],OLED_DATA);
    //确定坐标
    OLED_XY(x,y+2);
    //显示这一页的字符数据(1页=8行)
    for(i=0;i<24;i++)
    OLED_WR_Byte(HZ24[h*72+i+48],OLED_DATA);
  }
}

/*************************************************************************
*函数功能  ：OLED显示一组size大小的汉字
*函数名    ：OLED_HanziString
*函数参数  ：u8 x  u8 y  u8 size  u8 *str
*函数返回值：void
*描述      ：
***************************************************************************/
void OLED_HanziString(u8 x,u8 y,u8 size,u8 *str)
{
  u16 i=0;
  
  while(*str != '\0')
  {
    OLED_ShowHanzi(x+i*size,y,size,str);
    str+=2;
    i++;
  }
}

/*************************************************************************
*函数功能  ：OLED显示一组size大小的汉字/字符
*函数名    ：OLED_Show
*函数参数  ：u8 x  u8 y  u8 size  u8 *str
*函数返回值：void
*描述      ：
*             注意：size只能是16或24
***************************************************************************/
void OLED_Show(u8 x,u8 y,u8 size,u8 *str)
{
  u16 temp=0;
  
  while(*str != '\0')
  {
    if((32<=*str) && (*str<=127))
    {
      OLED_ShowChar(x+temp,y,size,*str);
      temp += (size/2);
      str++;
    }
    else
    {
      OLED_ShowHanzi(x+temp,y,size,str);
      temp += size;
      str+=2;
    }
  }
}

/*************************************************************************
*函数功能  ：OLED显示一张图片
*函数名    ：OLED_ShowPicture
*函数参数  ：u8 x  u8 y  u8 *tu
*函数返回值：void
*描述      ：
***************************************************************************/
void OLED_ShowPicture(u8 x,u8 y,u8 *tu)
{
  u8 i,j;
  for(i=0;i<8;i++)
  {
    OLED_XY(x,y+i);
    for(j=0;j<64;j++) 
    {
      OLED_WR_Byte(*tu,OLED_DATA); //一个字节控制8行1列
      tu++;
    }
  }
}










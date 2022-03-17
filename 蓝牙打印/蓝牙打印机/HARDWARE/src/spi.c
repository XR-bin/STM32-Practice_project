#include "sys.h"
#include "stdio.h"

/******************************************************************
*函数功能  ：对SPI1通信要用的GPIO口进行初始化和对SPI1的寄存器进行设置
*函数名    ：SPI1_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*           PA5   SPI1_SCK   时钟线  复用推挽
*           PA6   SPI1_MISO  主输入  复用推挽
*           PA7   SPI1_MOSI  主输出  复用推挽
*******************************************************************/
void SPI1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
	SPI_InitTypeDef   SPI_InitStruct;      //SPIx配置结构体
	
	//时钟使能   GPIOA   SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	/*GPIOx初始化设置*/
  //GPIOx端口配置
  //PA5、PA6、PA7
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 //PA5、PA6、PA7 端口配置
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	             //复用推挽输
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		               //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                              //根据设定参数初始化PA5、PA6、PA7
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);            //端口输出寄存器输出高(上拉电阻)
	
	/*SPI1寄存器初始化设置*/
	SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;   //SPI设置为双线双向全双工,且NSS上的电平为高电平(SPI_CR1寄存器的第10、15位)
	SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                   //设置为主SPI(SPI_CR1寄存器的第2、8位)
	SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                   //使用8位数据帧格式进行发送/接收(SPI_CR1寄存器的第11位)
	SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;                      //空闲状态时， SCK保持低电平(SPI_CR1寄存器的第1位)
	SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;                    //数据采样从第一个时钟边沿开始(SPI_CR1寄存器的第0位)
	SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                      //启用软件从设备管理，NSS引脚上的电平由SSI位的值决定(SPI_CR1寄存器的第9位)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;           //波特率预分频值为4  (SPI_CR1寄存器的第3-5位)
	SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                  //数据传输从MSB位开始(对齐标准:左对齐)(SPI_CR1寄存器的第7位)
	SPI_InitStruct.SPI_CRCPolynomial     = 7;                                 //CRC值计算的多项式:0x7是复位值(SPI_CRCPR寄存器)
	SPI_Init(SPI1, &SPI_InitStruct);                                          //初始化SPI1
	
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设(SPI_CR1寄存器的第6位)
}


/******************************************************************
*函数功能  ：SPI1接收/发送一个字节数据（8位）
*函数名    ：SPI1_RS_Byte
*函数参数  ：u8 data
*函数返回值：u8
*描述      ：
*           PA5   SPI1_SCK   时钟线
*           PA6   SPI1_MISO  主输入
*           PA7   SPI1_MOSI  主输出
*******************************************************************/
u8 SPI1_RS_Byte(u8 data)
{
  //判断发送缓存区是否为空(SPI->SR寄存器的第1位)
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
	//发送数据
	SPI_I2S_SendData(SPI1,data);
  //判断接收缓存区是否为空(SPI->SR寄存器的第0位)
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};
	//接收数据
  data = SPI_I2S_ReceiveData(SPI1);
  
  return data;
}








#include "sys.h"
#include "printer.h"
#include "motor.h"

u8 p_flag = 1;//打印刷新数据标志变量

/*****************************************************
*函数功能  ：对打印头对应的GPIO口进行初始化设置
*函数名    ：Printer_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;    //GPIOx配置结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

	//GPIOA
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;	//PA8/11/12 端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		              //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		              //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					                    //根据设定参数初始化PA8/11/12
	
	//GPIOB
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //PB10/11/12/13/14/15 端口配置
	GPIO_Init(GPIOB, &GPIO_InitStruct);					                    //根据设定参数初始化PB10/11/12/13/14/15
}


/*****************************************************
*函数功能  ：向打印头发送8位数据
*函数名    ：Printer_Write_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Write_Byte(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		CLK_L;
		if(data & 0x80) DI_H;
		else DI_L;
		CLK_H;
		data <<= 1;
	}
}

/*****************************************************
*函数功能  ：对打印头的左边加热
*函数名    ：Printer_Left
*函数参数  ：void
*函数返回值：void
*描述      ：
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Left(void)
{
	STB1_H ;
	STB2_H ;
	STB3_H ;
	STB4_L ;
	STB5_L ;
	STB6_L ;
}

/*****************************************************
*函数功能  ：对打印头的右边加热
*函数名    ：Printer_Right
*函数参数  ：void
*函数返回值：void
*描述      ：
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Right(void)
{
	STB1_L ;
	STB2_L ;
	STB3_L ;
	STB4_H ;
	STB5_H ;
	STB6_H ;
}

/*****************************************************
*函数功能  ：关闭打印头加热
*函数名    ：Printer_OFF
*函数参数  ：void
*函数返回值：void
*描述      ：
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_OFF(void)
{
	STB1_L ;
	STB2_L ;
	STB3_L ;
	STB4_L ;
	STB5_L ;
	STB6_L ;
}

/*****************************************************
*函数功能  ：打印条纹
*函数名    ：Printer_Stripe
*函数参数  ：void
*函数返回值：void
*描述      ：
*            STB6 ---- PB10          STB5 ---- PB11
*            STB4 ---- PB12          STB3 ---- PB13
*            STB2 ---- PB14          STB1 ---- PB15
*
*            LAT  ---- PA8           CLK  ---- PA11
*            DI   ---- PA12
********************************************************/
void Printer_Stripe(void)
{
	u8 i =0;
	for(i=0; i<48;i++)
	{
		Printer_Write_Byte(0xf0);
	}
}

/**************************************************************
*函数功能  ：打印一张图片
*函数名    ：Printer_Picture
*函数参数  ：u8 *tu
*函数返回值：void
*描述      ：
****************************************************************/
void Printer_Picture(u8 *tu)
{
	u8 i;
	static u16 line = 0;
	
	if(p_flag == 1)//判断允许更新数据标志位 //此位是在中断中置1，初始值设置为1，这样第一次数据就在启动定时器前准备好了
	{
		p_flag = 0;   //允许更新数据标志位 清0           //为了下一次在中断中置1
		for (i=0; i<48; i++)  //写图片的一行数据 // 48字节
		{
			Printer_Write_Byte(*(tu+i+line*48));//图片取模方向：从左到右，从上到下，一行48字节，所以每一行数组偏移48
		}
		line++;
	}
	
	if (line >= 473)  //图片高473 行号0~472    但是要把472行打印，所以到473时关闭
	{
		line = 0; //行号清零，下一次打印从0行开始
		MOTOR_OFF;
		TIM_Cmd(TIM3, DISABLE);
		Printer_OFF();
	}
}

/**************************************************************
*函数功能  ：打印一串24*24字符串
*函数名    ：Printer_ShowStr
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
*            
****************************************************************/














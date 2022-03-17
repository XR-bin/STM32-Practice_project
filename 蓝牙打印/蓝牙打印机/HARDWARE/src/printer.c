#include "sys.h"
#include "printer.h"
#include "motor.h"
#include "w25q64.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "matrix.h"
#include "time.h"


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
	CLK_L;
	for(i=0;i<8;i++)
	{
		CLK_L;
		if(data & 0x80) DI_H;
		else DI_L;
		CLK_H;
		data <<= 1;
		delay_us(1);
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

/*****************************************************
*函数功能  ：向打印头发送48个字节数据
*函数名    ：Printer_Write_48Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
********************************************************/
void Printer_Write_48Byte(u8 *buf)
{
	u8 i;
	
	for(i=0;i<48;i++)
	{
		Printer_Write_Byte(*(buf+i));
	}
}

/**************************************************************
*函数功能  ：打印一行字符16*16(练习思路)
*函数名    ：Printer_CH16
*函数参数  ：u8 *ch
*函数返回值：void
*描述      ：
****************************************************************/
void Printer_CH16(u8 *str)
{
	u8 i=0;
	u8 buf[48] = {0};
	u8 num = 0;
	static u8 hang=0;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			num = *(str+i) - ' ';  //这里的空格是指20号空格，而不是字符字库的第一个空格
			buf[i] = ch16[num*16+hang];
			i++;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//锁存输出数据	
		
		hang++;
		
		if(hang >= 16)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*函数功能  ：打印一行字符24*24(练习思路)
*函数名    ：Printer_CH24
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
****************************************************************/
void Printer_CH24(u8 *str)
{
	u8 i=0,j=0;
	u8 buf[48] = {0};
	u8 num = 0;
	static u8 hang=0;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			num = *(str+i) - 0;
			buf[j] = ch24[num*48+hang*2];
			j++;
			buf[j] = ch24[num*48+hang*2+1];
			j++;
			i++;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//锁存输出数据	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*函数功能  ：打印一行字符24*24(在w25q64里拿数据)
*函数名    ：Printer_Str24
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
****************************************************************/
void Printer_Str24(u8 *str)
{
	u8 i=0,j=0;
	u32 addr;
	u8 buf[48] = {0};
	u8 num = 0;
	static u8 hang=0;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			num = *(str+i) - 0;
			addr = 0x0b0000 + num*48 + hang*2;
			W25Q64_Read_Bytes(addr,2,&buf[j]);
			j += 2;
			i++;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//锁存输出数据	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*函数功能  ：打印一串汉字24*24(在w25q64里拿数据)
*函数名    ：Printer_HZ24
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
****************************************************************/
void Printer_HZ24(u8 *str)
{
	u8 i=0,j=0;
	u32 addr;
	u8 buf[48] = {0};
	u32 num = 0;
	static u8 hang=0;
	u32 H,L;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			//区码
			H = (*(str+i) - 0xa1) * 94;
			//位码
			L = *(str+i+1) - 0xa1;
			num = H + L;
			addr = 0x0b0000 + 0x00001806 + num*72 + hang*3;
			W25Q64_Read_Bytes(addr,3,&buf[j]);
			j += 3;
			i += 2;
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//锁存输出数据	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
		}
	}
}

/**************************************************************
*函数功能  ：打印一串汉字/字符24*24(在w25q64里拿数据)
*函数名    ：Printer_String24
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
****************************************************************/
u8 Printer_String24(u8 *str)
{
	u8 i=0,j=0;
	u32 addr;
	u8 buf[48] = {0};
	u32 num = 0;
	static u8 hang=0;
	u32 H,L;
	
	if(p_flag == 1)
	{
		p_flag = 0;
		while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
		{
			
			if((32<=*(str+i)) && (*(str+i)<=127))
			{
				num = *(str+i) - 0;
				addr = 0x0b0000 + num*48 + hang*2;
				W25Q64_Read_Bytes(addr,2,&buf[j]);
				j += 2;
				i++;
			}
			else
			{
				//区码
				H = (*(str+i) - 0xa1) * 94;
				//位码
				L = *(str+i+1) - 0xa1;
				num = H + L;
				addr = 0x0b0000 + 0x00001806 + num*72 + hang*3;
				W25Q64_Read_Bytes(addr,3,&buf[j]);
				j += 3;
				i += 2;
			}
		}
		
		Printer_Write_48Byte(buf);
		
		LAT_H;
		LAT_L;
		LAT_H;//锁存输出数据	
		
		hang++;
		
		if(hang >= 24)
		{
			hang = 0;
			MOTOR_OFF;
			TIM_Cmd(TIM3, DISABLE);
			Printer_OFF();
			return 0;
		}
	}
	return 1;
}

/**************************************************************
*函数功能  ：打印一串汉字/字符24*24(在w25q64里拿数据)(可自动换行)
*函数名    ：Printer1_String24
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
****************************************************************/
void Printer1_String24(u8 *str)
{
	u8 i=0,j=0,k=0,key=1,t=1;
	u8 buf[51] = {0};
	
	THIS:
	while((*(str+i) != '\r')&& (*(str+i+1) != '\n')) 
	{
		if((32<=*(str+i)) && (*(str+i)<=127))
		{
			if(j+2>48)
			{
				key=0;
				break;
			}
				
			buf[k] = *(str+i);
			j += 2;
			i++;
			k++;
		}
		else
		{
			if(j+2>48)
			{
				key=0;
				break;
			}
			
			buf[k] = *(str+i);
			buf[k+1] = *(str+i+1);
			j += 3;
			i += 2;
			k += 2;
		}
		key = 1;
	}
	
	buf[k] = '\r';
	buf[k+1] = '\n';
	
	TIM_Cmd(TIM3, ENABLE);
	while(t)
	{
		t = Printer_String24(buf);
	}
	if(key != 1)
	{
		t = 1;
		j = 0;
		k = 0;
		goto THIS;
	}
}







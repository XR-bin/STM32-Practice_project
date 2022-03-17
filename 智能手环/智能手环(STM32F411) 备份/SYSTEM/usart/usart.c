#include "stm32f4xx.h"
#include "stdio.h"

/******************************************************************
*函数功能  ：对串口1对应的GPIO口进行初始化和对串口1的寄存器进行设置
*函数名    ：USART1_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
*           PA9   TX    输出
*           PA10  RX    输入
*******************************************************************/
void USART1_Init(u32 bps)
{
	GPIO_InitTypeDef   GPIO_InitStruc;    //GPIOx配置结构体
	USART_InitTypeDef  USART_InitStruct;  //USART配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;   //NVIC(中断)配置结构体
	
  //时钟使能   GPIOA   USART1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	
	/*GPIO口配置*/
	//PA9、PA10
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //选择PA9、PA10
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStruc.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_InitStruc.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;     	//速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruc);                   //根据设定参数初始化PA9，PA10
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //PA9 复用为USART1的输出(TX)
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //PA10复用为USART1的输入(RX)
	
	/*串口初始化*/
	//USART1
	USART_InitStruct.USART_BaudRate = bps;                    //波特率设置(USART->BRR寄存器)
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  //字长为8位数据格式(USART->CR1寄存器的第12位)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;       //一个停止位(USART->CR2寄存器的第12、13位)
	USART_InitStruct.USART_Parity = USART_Parity_No;          //无奇偶校验位(USART->CR1寄存器的第10位)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制(USART->CR3寄存器的第9位)
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式(USART->CR1寄存器的第2、3位)
  USART_Init(USART1, &USART_InitStruct);                        //初始化串口1
	
	/*中断配置*/
	//选择中断方式并使能该方式中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     //读取数据寄存器不为空(接收完成)中断 (USART->CR1的第5位)  
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);     //检测到空闲线路(空闲)中断 (USART->CR1的第4位)
	
	//串口1中断配置-----NVIC 配置
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;      //串口1中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3;		  //子优先级3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	                      //根据指定的参数初始化VIC寄存器
	
	//串口使能
	USART_Cmd(USART1, ENABLE);  //使能串口1
}


/******************************************************************
*函数功能  ：串口1发送一个字节数据（8位）
*函数名    ：USART1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);   //等待发送结束(USART1->SR的第6位) 
	USART_SendData(USART1, data);                              //发送一个字节的数据(USART1->DR)
}

/******************************************************************
*函数功能  ：串口1接收一个字节数据（8位）
*函数名    ：USART1_Receive_Byte
*函数参数  ：void
*函数返回值：u8 
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
u8 USART1_Receive_Byte(void)
{
  u8 data;
  while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != SET);    //等待发送结束(USART1->SR的第5位)
  data = USART_ReceiveData(USART1);                              //读取一个字节的数据(USART1->DR)
  
  return data;
}

/******************************************************************
*函数功能  ：串口1发送一个字符串
*函数名    ：USART1_Send_Str
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART1_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*函数功能  ：串口1接收一个字符串
*函数名    ：USART1_Receive_Str
*函数参数  ：u8 *str
*函数返回值：void 
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART1_Receive_Byte();
    if(data == '#')   //以 # 为结束标志
    {
      break;
    }
    *str = data;
    str++;
  }
  *str = '\0'; 
}














































































/**********************************printf函数*****************************************/
#if 1
	#pragma import(__use_no_semihosting)             
	//标准库需要的支持函数                 
	struct __FILE 
	{ 
		int handle; 
		/* Whatever you require here. If the only file you are using is */ 
		/* standard output using printf() for debugging, no file handling */ 
		/* is required. */ 
	}; 
	/* FILE is typedef’ d in stdio.h. */ 
	FILE __stdout;       
	//定义_sys_exit()以避免使用半主机模式    
  _sys_exit(int x) 
	{ 
		x = x; 
	} 
	//重定义fputc函数 
	int fputc(int ch, FILE *f)
	{      
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		USART1->DR = (u8) ch;      
		return ch;
	}
	#endif 
/***********************************************************************************/




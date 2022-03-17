#include "sys.h"
#include "usart.h"
#include "stdio.h"

/*****************************************************
*函数功能  ：对USART1对应的GPIO口进行初始化设置
*函数名    ：USART1_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
*           PA9     TX     ---------输出
*           PA10    RX     ---------输入
********************************************************/
void USART1_Init(u32 baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
  USART_InitTypeDef USART_InitStruct;    //USARTx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;    //NVIC(中断)配置结构体
  
  //时钟使能   GPIOA   USART1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); 
  
  /*GPIOx初始化设置*/
  //GPIOx端口配置
  //PA9   USART1_TX ----- 复用串口输出
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;				     //PA9 端口配置
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	 //复用推挽输
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //根据设定参数初始化PA9
  //PA10  USART1_RX ----- 复用串口输入
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;				     //PA10 端口配置
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //浮空输入
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //根据设定参数初始化PA10
  
  /*USART 初始化设置*/
  //USART1配置
  USART_InitStruct.USART_BaudRate      = baud;                 //波特率设置(USART->BRR寄存器)
  USART_InitStruct.USART_WordLength    = USART_WordLength_8b;  //字长为8位数据格式:一个起始位， 8个数据位， n个停止位；(USART->CR1寄存器的第12位)
  USART_InitStruct.USART_StopBits      = USART_StopBits_1;     //一个停止位(USART->CR2寄存器的第12、13位)
  USART_InitStruct.USART_Parity        = USART_Parity_No;      //无奇偶校验(USART->CR1寄存器的第10位)
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制(USART->CR3寄存器的第9位)
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式(USART->CR1寄存器的第2、3位)
  USART_Init(USART1,&USART_InitStruct);                        //初始化USART1
	
	/*NVIC配置*/
	//选择中断方式并使能该方式中断
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    //读取数据寄存器不为空(接收完成)中断 (USART->CR1的第5位) 
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);     //检测到空闲线路(空闲)中断 (USART->CR1的第4位)

  //串口中断配置
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;           //选择串口1中断
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //子优先级3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQ通道使能
  NVIC_Init(&NVIC_InitStruct); //串口1中断初始化
  
  //使能串口1
  USART_Cmd(USART1, ENABLE);
}

/*****************************************************
*函数功能  ：串口1发送一个字节的数据（u8）
*函数名    ：USART1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
*           PA9     TX     ---------输出
*           PA10    RX     ---------输入
********************************************************/
void USART1_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);//等待发送结束(USART1->SR的第6位)
	USART_SendData(USART1, data);                           //发送一个字节的数据(USART1->DR)
}

/*****************************************************
*函数功能  ：串口1接收一个字节的数据（u8）
*函数名    ：USART1_Receive_Byte
*函数参数  ：void
*函数返回值：u8
*描述      ：
********************************************************/
u8 USART1_Receive_Byte(void)
{
  u8 data;
	
  while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!=SET){};//等待发送结束(USART1->SR的第5位)
  data = USART_ReceiveData(USART1);                         //读取一个字节的数据(USART1->DR)
  
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


/*****************************************************
*函数功能  ：对USART3对应的GPIO口进行初始化设置
*函数名    ：USART3_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
*           PB10     TX     ---------输出
*           PB11     RX     ---------输入
********************************************************/
void USART3_Init(u32 baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
  USART_InitTypeDef USART_InitStruct;    //USARTx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;    //NVIC(中断)配置结构体
  
  //时钟使能   GPIOB   USART3
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 
  
  /*GPIOx初始化设置*/
  //GPIOx端口配置
  //PB10   USART3_TX ----- 复用串口输出
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;				     //PB10 端口配置
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	 //复用推挽输
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                  //根据设定参数初始化PB10
  //PB11  USART3_RX ----- 复用串口输入
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;				     //PB11 端口配置
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //浮空输入
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStruct);                  //根据设定参数初始化PB11
  
  /*USART 初始化设置*/
  //USART3配置
  USART_InitStruct.USART_BaudRate      = baud;                 //波特率设置(USART->BRR寄存器)
  USART_InitStruct.USART_WordLength    = USART_WordLength_8b;  //字长为8位数据格式:一个起始位， 8个数据位， n个停止位；(USART->CR1寄存器的第12位)
  USART_InitStruct.USART_StopBits      = USART_StopBits_1;     //一个停止位(USART->CR2寄存器的第12、13位)
  USART_InitStruct.USART_Parity        = USART_Parity_No;      //无奇偶校验(USART->CR1寄存器的第10位)
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制(USART->CR3寄存器的第9位)
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式(USART->CR1寄存器的第2、3位)
  USART_Init(USART3,&USART_InitStruct);                        //初始化USART3
	
	/*NVIC配置*/
	//选择中断方式并使能该方式中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);     //读取数据寄存器不为空(接收完成)中断 (USART->CR1的第5位) 
  USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);     //检测到空闲线路(空闲)中断 (USART->CR1的第4位)

  //串口中断配置
  NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;           //选择串口3中断
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		       //子优先级3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQ通道使能
  NVIC_Init(&NVIC_InitStruct); //串口3中断初始化
  
  //使能串口3
  USART_Cmd(USART3, ENABLE);
}

/*****************************************************
*函数功能  ：串口3发送一个字节的数据（u8）
*函数名    ：USART3_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
*           PB10     TX     ---------输出
*           PB11     RX     ---------输入
********************************************************/
void USART3_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET);//等待发送结束(USART3->SR的第6位)
	USART_SendData(USART3, data);                           //发送一个字节的数据(USART3->DR)
}

/*****************************************************
*函数功能  ：串口3接收一个字节的数据（u8）
*函数名    ：USART3_Receive_Byte
*函数参数  ：void
*函数返回值：u8
*描述      ：
********************************************************/
u8 USART3_Receive_Byte(void)
{
  u8 data;
	
  while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=SET){};//等待发送结束(USART3->SR的第5位)
  data = USART_ReceiveData(USART3);                         //读取一个字节的数据(USART3->DR)
  
  return data;
}

/******************************************************************
*函数功能  ：串口3发送一个字符串
*函数名    ：USART3_Send_Str
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
            PB10   TX    输出
            PB11   RX    输入
*******************************************************************/
void USART3_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART3_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*函数功能  ：串口3接收一个字符串
*函数名    ：USART3_Receive_Str
*函数参数  ：u8 *str
*函数返回值：void 
*描述      ：
            PB10   TX    输出
            PB11   RX    输入
*******************************************************************/
void USART3_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART3_Receive_Byte();
    if(data == '#')   //以 # 为结束标志
    {
      break;
    }
    *str = data;
    str++;
  }
  *str = '\0'; 
}

/*****************************************************
*函数功能  ：对USART2对应的GPIO口进行初始化设置
*函数名    ：USART2_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
*           PA2     TX     ---------输出
*           PA3     RX     ---------输入
********************************************************/
void USART2_Init(u32 baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
  USART_InitTypeDef USART_InitStruct;    //USARTx配置结构体
	NVIC_InitTypeDef   NVIC_InitStruct;    //NVIC(中断)配置结构体
  
  //时钟使能   GPIOA   USART1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
  
  /*GPIOx初始化设置*/
  //GPIOx端口配置
  //PA2   USART2_TX ----- 复用串口输出
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;				     //PA2 端口配置
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	 //复用推挽输
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //根据设定参数初始化PA9
  //PA3  USART2_RX ----- 复用串口输入
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;				     //PA3 端口配置
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //浮空输入
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStruct);                  //根据设定参数初始化PA10
  
  /*USART 初始化设置*/
  //USART2配置
  USART_InitStruct.USART_BaudRate      = baud;                 //波特率设置(USART->BRR寄存器)
  USART_InitStruct.USART_WordLength    = USART_WordLength_8b;  //字长为8位数据格式:一个起始位， 8个数据位， n个停止位；(USART->CR1寄存器的第12位)
  USART_InitStruct.USART_StopBits      = USART_StopBits_1;     //一个停止位(USART->CR2寄存器的第12、13位)
  USART_InitStruct.USART_Parity        = USART_Parity_No;      //无奇偶校验(USART->CR1寄存器的第10位)
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制(USART->CR3寄存器的第9位)
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式(USART->CR1寄存器的第2、3位)
  USART_Init(USART2,&USART_InitStruct);                        //初始化USART1
	
	/*NVIC配置*/
	//选择中断方式并使能该方式中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);     //读取数据寄存器不为空(接收完成)中断 (USART->CR1的第5位) 
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);     //检测到空闲线路(空闲)中断 (USART->CR1的第4位)

  //串口中断配置
  NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;           //选择串口2中断
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级3
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;		       //子优先级3
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 		         //IRQ通道使能
  NVIC_Init(&NVIC_InitStruct); //串口2中断初始化
  
  //使能串口2
  USART_Cmd(USART2, ENABLE);
}

/*****************************************************
*函数功能  ：串口2发送一个字节的数据（u8）
*函数名    ：USART2_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
*           PA2     TX     ---------输出
*           PA3    RX     ---------输入
********************************************************/
void USART2_Send_Byte(u8 data)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);//等待发送结束(USART2->SR的第6位)
	USART_SendData(USART2, data);                           //发送一个字节的数据(USART2->DR)
}

/*****************************************************
*函数功能  ：串口2接收一个字节的数据（u8）
*函数名    ：USART2_Receive_Byte
*函数参数  ：void
*函数返回值：u8
*描述      ：
********************************************************/
u8 USART2_Receive_Byte(void)
{
  u8 data;
	
  while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!=SET){};//等待发送结束(USART2->SR的第5位)
  data = USART_ReceiveData(USART2);                         //读取一个字节的数据(USART2->DR)
  
  return data;
}

/******************************************************************
*函数功能  ：串口2发送一个字符串
*函数名    ：USART2_Send_Str
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
            PA2   TX    输出
            PA3  RX    输入
*******************************************************************/
void USART2_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART2_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*函数功能  ：串口2接收一个字符串
*函数名    ：USART2_Receive_Str
*函数参数  ：u8 *str
*函数返回值：void 
*描述      ：
            PA2   TX    输出
            PA3   RX    输入
*******************************************************************/
void USART2_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART2_Receive_Byte();
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









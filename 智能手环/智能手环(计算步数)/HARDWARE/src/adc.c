#include "stm32f4xx.h"   
#include "stdio.h"
#include "adc.h"

/************************************
*   F407的ADC时钟频率官方建议: 36Mhz
*   F103的ADC时钟频率官方建议: 14Mhz
*************************************/

//延时函数毫秒
static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;   //由系统时钟频率决定的
	while(i)
	{
		i--;
	}
}

/******************************************
*函数名    ：ADC1_Init_In3
*函数功能  ：ADC1的第三个通道初始化配置
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA3----------ADC1_IN3   模拟模式
*********************************************/
void ADC1_Init_In3(void)
{
	GPIO_InitTypeDef        GPIO_InitStruc;         //GPIOx结构体配置
	ADC_CommonInitTypeDef   ADC_CommonInitStruct;   //ADCx一般配置结构体
	ADC_InitTypeDef         ADC_InitStruct;         //ADCx初始化配置结构体
  
  //时钟使能  GPIOA  ADC1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
  /*GPIOx端口配置*/
  //PA5
  GPIO_InitStruc.GPIO_Pin   = GPIO_Pin_3;         //选择PA5
  GPIO_InitStruc.GPIO_Mode  = GPIO_Mode_AN;       //模拟输入
  GPIO_InitStruc.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //无上下拉
  GPIO_Init(GPIOA,&GPIO_InitStruc);               //根据设定参数初始化PA5
	
	/*ADCx寄存器配置*/
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                    //独立模式(ADC->CCR寄存器的第0-4位)
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟(ADC->CCR寄存器的第8-11位)
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //DMA失能(ADC->CCR寄存器的第14-15位)
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;                 //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz(ADC->CCR寄存器的第16-17位)
  ADC_CommonInit(&ADC_CommonInitStruct);                                   //ADCx一般配置初始化(ADC->CCR)
	
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                      //12位模式(ADC->CR1寄存器的第24、25位)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                               //非扫描模式(ADC->CR1寄存器的第8位)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                         //关闭连续转换(ADC->CR2寄存器的第1位)
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //禁止触发检测，使用软件触发(ADC->CR2寄存器的第20-21位/第28-29位)
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      //右对齐	(ADC->CR2寄存器的第11位)
  ADC_InitStruct.ADC_NbrOfConversion = 1;                                  //1个转换在规则序列中 也就是只转换规则序列1(ADC->SQR1寄存器的第20-23位)
  ADC_Init(ADC1, &ADC_InitStruct);                                         //ADCx配置初始化
	
	//ADC规则通道序列偏序及采样时间配置函数
  //ADC1的第一个转换时通道3，采样时间480个周期        (采样：ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1,ADC_SampleTime_480Cycles);  
	
	ADC_Cmd(ADC1, ENABLE);//使能ADCx，并开启AD转换器	
}

/******************************************
*函数名    ：ADC1_In3_Data
*函数功能  ：获取ADC1通道3的转换数据
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换可调电阻数据
*********************************************/
u16 ADC1_In3_Data(void)
{
  u16 data;
  
  //开启规则通道转换开关(ADC1->CR2寄存器的第30位)
	ADC_SoftwareStartConv(ADC1);    
  //等待转换完成
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){};//等待转换结束(ADC1->SR寄存器的第1位)
  //读取转换后的数据(ADC1->DR)
  data = ADC_GetConversionValue(ADC1);   
  
  return data;
}

/******************************************
*函数名    ：ADC_KEY_Data
*函数功能  ：获取五方向按键的值
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换可调电阻数据
*********************************************/
u16 ADC_KEY_Data(void)
{
	u16 data[5],temp, sum;
	u8 i,j;
	
	//获取5次
	for(i=0;i<5;i++)
	{
		data[i] = ADC1_In3_Data();
	}
	
	//排序
	for(i=1;i<5;i++)
	{
		for(j=0;j<5-i;j++)
		{
			if(data[j] > data[j+1])
			{
				temp = data[j];
				data[j] = data[j+1];
				data[j+1] = temp;
			}
		}
	}
	
	//去掉最大最小求平均
	for(i=1;i<4;i++)
	{
		sum = sum + data[i];
	}
	
	return (sum / 3);
}

/******************************************
*函数名    ：ADC_KEY_Scan
*函数功能  ：获取五方向按键的按键扫描
*函数参数  ：void
*函数返回值：u8
*函数描述  ：用于转换可调电阻数据
*********************************************/
u8 ADC_KEY_Scan(void)
{
	u16 adc_key = 0;
	static u8 temp = 0;
	static u8 key_flay = 1;
	
	adc_key = ADC_KEY_Data();
	adc_key = adc_key *0.40 / 100;
	
	if(key_flay&&adc_key)
	{
		delay_ms(10);
		if(adc_key)
		{
			key_flay = 0;
			if(adc_key==4) return (temp = 1);          //上
			else if(adc_key==5) return (temp = 2);     //下
			else if(adc_key==8) return (temp =3);      //右
			else if(adc_key==16) return (temp = 4);    //左
		}
	}
	
	if(adc_key == 0)
  {
    key_flay = 1;
		temp = 0;
  }
	
	return temp;
}


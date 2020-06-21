/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   按键测试（中断模式/EXTI模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h" 
#include "bsp_usart.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"

#include "bsp_adc.h"

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint32_t ADC_ConvertedValue[NOFCHANEL];



/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
	
int Work_Statue = 1;
int Page = 1;
int Work_Mode = 1;
int Target_Moisture = 10;

uint16_t temp2=0 ,temp1=0;
	
	float SoilHumidity;
	float LightValue;
	float tempL;

void GPIO_SInit(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

int main(void)
{
	  	

	
	EXTI_Key_Config(); 
	USART_Config();
	delay_init();
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	
	OLED_Clear();
	ADCx_Init();
	GPIO_SInit();
	
	// Page 1 模式选择
	
		OLED_ShowChinese(0,0,15,16);//
		OLED_ShowChinese(18,0,0,16);//手
		OLED_ShowChinese(36,0,1,16);//动
		OLED_ShowChinese(54,0,2,16);//模
		OLED_ShowChinese(72,0,3,16);//式
		OLED_ShowChinese(18,18,4,16);//自
		OLED_ShowChinese(36,18,1,16);//动
		OLED_ShowChinese(54,18,2,16);//电
		OLED_ShowChinese(72,18,3,16);//子
		OLED_Refresh();
		while(Page == 1);
		
			
			// Page 2 设置阈值
			Page = 2;
			OLED_ShowChinese(0,18,5,16);//土
			OLED_ShowChinese(18,18,6,16);//壤
			OLED_ShowChinese(36,18,7,16);//湿
			OLED_ShowChinese(54,18,8,16);//度
			OLED_ShowChinese(72,18,17,16);//阈
			OLED_ShowChinese(90,18,18,16);//值
			OLED_ShowChar(108,18,':',16);
			OLED_ShowNum(54,36,Target_Moisture,2,16);
			OLED_ShowChar(72,36,'%',16);
			OLED_Refresh();
			while(Page == 2);

			
				// Page 3 显示信息
		while(1)
		{
				
				
				// 取出ADC1数据寄存器的高16位，这个是ADC2的转换数据
			temp2 = (ADC_ConvertedValue[0]&0XFFFF0000) >> 16;
			// 取出ADC1数据寄存器的低16位，这个是ADC1的转换数据
			temp1 = (ADC_ConvertedValue[0]&0XFFFF);	

			SoilHumidity = (float)(4096-temp1)/4096*100;
			printf("SoilHumidity=%.2f%%\r\n",SoilHumidity);
			
			tempL = (float)temp2/4096*3.3;
			LightValue = 15 - (10/2.2) * tempL;
			printf("LightValue=%.2f Lux\r\n",LightValue);
			
			OLED_ShowChinese(0,0,5,16);//土
			OLED_ShowChinese(18,0,6,16);//壤
			OLED_ShowChinese(36,0,7,16);//湿
			OLED_ShowChinese(54,0,8,16);//度
			OLED_ShowChar(72,0,':',16);//显示ASCII字符
			OLED_ShowNum(90,0,SoilHumidity,2,16);
			OLED_ShowChar(108,0,'%',16);
			
			OLED_ShowChinese(0,18,9,16);//光
			OLED_ShowChinese(18,18,10,16);//
			OLED_ShowChinese(36,18,11,16);//电
			OLED_ShowChinese(54,18,8,16);//子
			OLED_ShowChar(72,18,':',16);//显示ASCII字符
			OLED_ShowNum(90,18,LightValue,2,16);
			//OLED_ShowString(93,18,"L",16);
			
			
			
			
			
			OLED_Refresh();
			if(SoilHumidity <= Target_Moisture)
			{
				if(Work_Statue == 1)
				{
					OLED_ShowChinese(18,36,12,16);//土
					OLED_ShowChinese(36,36,13,16);//壤
					OLED_ShowChinese(54,36,14,16);//湿
					OLED_ShowChinese(72,36,16,16);//!
					OLED_ShowChinese(90,36,16,16);//!
					OLED_ShowChinese(108,36,16,16);//!
					OLED_Refresh();
				}
				if(Work_Statue == 2)
				{
					GPIO_SetBits(GPIOA, GPIO_Pin_4);
				}
					
			}
			
			if(SoilHumidity > Target_Moisture)
			{
				if(Work_Statue == 1)
				{
					OLED_ShowChinese(18,36,19,16);//土
					OLED_ShowChinese(36,36,19,16);//壤
					OLED_ShowChinese(54,36,19,16);//湿
					OLED_ShowChinese(72,36,19,16);//!
					OLED_ShowChinese(90,36,19,16);//!
					OLED_ShowChinese(108,36,19,16);//!
					OLED_Refresh();
				}
				if(Work_Statue == 2)
				{
					GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				}
		}
		
		
		
		
		
		
		
		
		
		
	

	
}
		}
/*********************************************END OF FILE**********************/

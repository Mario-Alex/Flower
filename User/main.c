/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �������ԣ��ж�ģʽ/EXTIģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint32_t ADC_ConvertedValue[NOFCHANEL];



/**
  * @brief  ������
  * @param  ��
  * @retval ��
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
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

int main(void)
{
	  	

	
	EXTI_Key_Config(); 
	USART_Config();
	delay_init();
	OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	
	OLED_Clear();
	ADCx_Init();
	GPIO_SInit();
	
	// Page 1 ģʽѡ��
	
		OLED_ShowChinese(0,0,15,16);//
		OLED_ShowChinese(18,0,0,16);//��
		OLED_ShowChinese(36,0,1,16);//��
		OLED_ShowChinese(54,0,2,16);//ģ
		OLED_ShowChinese(72,0,3,16);//ʽ
		OLED_ShowChinese(18,18,4,16);//��
		OLED_ShowChinese(36,18,1,16);//��
		OLED_ShowChinese(54,18,2,16);//��
		OLED_ShowChinese(72,18,3,16);//��
		OLED_Refresh();
		while(Page == 1);
		
			
			// Page 2 ������ֵ
			Page = 2;
			OLED_ShowChinese(0,18,5,16);//��
			OLED_ShowChinese(18,18,6,16);//��
			OLED_ShowChinese(36,18,7,16);//ʪ
			OLED_ShowChinese(54,18,8,16);//��
			OLED_ShowChinese(72,18,17,16);//��
			OLED_ShowChinese(90,18,18,16);//ֵ
			OLED_ShowChar(108,18,':',16);
			OLED_ShowNum(54,36,Target_Moisture,2,16);
			OLED_ShowChar(72,36,'%',16);
			OLED_Refresh();
			while(Page == 2);

			
				// Page 3 ��ʾ��Ϣ
		while(1)
		{
				
				
				// ȡ��ADC1���ݼĴ����ĸ�16λ�������ADC2��ת������
			temp2 = (ADC_ConvertedValue[0]&0XFFFF0000) >> 16;
			// ȡ��ADC1���ݼĴ����ĵ�16λ�������ADC1��ת������
			temp1 = (ADC_ConvertedValue[0]&0XFFFF);	

			SoilHumidity = (float)(4096-temp1)/4096*100;
			printf("SoilHumidity=%.2f%%\r\n",SoilHumidity);
			
			tempL = (float)temp2/4096*3.3;
			LightValue = 15 - (10/2.2) * tempL;
			printf("LightValue=%.2f Lux\r\n",LightValue);
			
			OLED_ShowChinese(0,0,5,16);//��
			OLED_ShowChinese(18,0,6,16);//��
			OLED_ShowChinese(36,0,7,16);//ʪ
			OLED_ShowChinese(54,0,8,16);//��
			OLED_ShowChar(72,0,':',16);//��ʾASCII�ַ�
			OLED_ShowNum(90,0,SoilHumidity,2,16);
			OLED_ShowChar(108,0,'%',16);
			
			OLED_ShowChinese(0,18,9,16);//��
			OLED_ShowChinese(18,18,10,16);//
			OLED_ShowChinese(36,18,11,16);//��
			OLED_ShowChinese(54,18,8,16);//��
			OLED_ShowChar(72,18,':',16);//��ʾASCII�ַ�
			OLED_ShowNum(90,18,LightValue,2,16);
			//OLED_ShowString(93,18,"L",16);
			
			
			
			
			
			OLED_Refresh();
			if(SoilHumidity <= Target_Moisture)
			{
				if(Work_Statue == 1)
				{
					OLED_ShowChinese(18,36,12,16);//��
					OLED_ShowChinese(36,36,13,16);//��
					OLED_ShowChinese(54,36,14,16);//ʪ
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
					OLED_ShowChinese(18,36,19,16);//��
					OLED_ShowChinese(36,36,19,16);//��
					OLED_ShowChinese(54,36,19,16);//ʪ
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

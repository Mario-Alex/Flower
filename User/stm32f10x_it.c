/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_usart.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "sys.h"



extern int Work_Statue;
extern int Page;
extern int Work_Mode;
extern int Target_Moisture;



void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		if(Page == 1)
		{
			if(Work_Mode == 1)
			{
				Work_Mode = 2;
				OLED_Clear();
				OLED_ShowChinese(0,18,15,16);//
				OLED_ShowChinese(18,0,0,16);//手
				OLED_ShowChinese(36,0,1,16);//动
				OLED_ShowChinese(54,0,2,16);//模
				OLED_ShowChinese(72,0,3,16);//式
				OLED_ShowChinese(18,18,4,16);//自
				OLED_ShowChinese(36,18,1,16);//动
				OLED_ShowChinese(54,18,2,16);//电
				OLED_ShowChinese(72,18,3,16);//子
				OLED_Refresh();
			}
		}
		if(Page == 2)
		{
			Target_Moisture--;
			OLED_ShowNum(54,36,Target_Moisture,2,16);
			OLED_Refresh();
		}
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}

void KEY2_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		if(Page == 1)
		{
			if(Work_Mode == 2)
			{
				Work_Mode = 1;
				OLED_Clear();
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
			}
		}
		
		if(Page == 2)
		{
			Target_Moisture++;
			OLED_ShowNum(54,36,Target_Moisture,2,16);
			OLED_Refresh();
		}
		
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}


void KEY4_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY4_INT_EXTI_LINE) != RESET) 
	{
		if(Page == 1)
		{
			OLED_Clear();
			Page = 4;			//仅为跳出循环
			if(Work_Mode == 1)
				Work_Statue = 1;
			else
				Work_Statue = 2;
		}
		
		if(Page == 2)
		{
			OLED_Clear();
			Page = 4;
			Work_Mode = 1;	
		}
		EXTI_ClearITPendingBit(KEY4_INT_EXTI_LINE);     
	}  
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

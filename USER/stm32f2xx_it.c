/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
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
	SysTickCount++;
	if(SysTickCount >= 100)
	{
		SysTickCount = 0;
	}
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/



/*******************************************************************************
*
*函数原型：
*入口参数：无
*出口参数：无
*函数功能：串口1中断服务，
*
*******************************************************************************/
void USART1_IRQHandler(void)
{
	uint16_t crc_temp = 0;
	uint16_t buf_len1 = 0;
	
	//串口接收中断
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) 
	{     
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART1_ReceiveBuff[USART1_ReceiveDataCount++] = USART1->DR & (uint16_t)0x01FF;
	}	
	
	//串口空闲检测中断
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET) 
	{     
		USART_IdleClear = USART1 ->SR;								//序列清空闲标志位
		USART_IdleClear = USART1->DR;								//序列清空闲标志位
																	//读出值没有用
		
		
		if(!strncmp((const char *)USART1_ReceiveBuff,String_ResetDeviceCommand,strlen(String_ResetDeviceCommand)))
		{
			SoftReset();
			USART1_ReceiveDataCount = 0;
			return;
		}
		
		if((USART1_ReceiveBuff[0] == 0xFF) && (USART1_ReceiveBuff[1] == 0xFF))
		{
			buf_len1 = USART1_ReceiveBuff[6] * 256 + USART1_ReceiveBuff[7];
			if(USART1_ReceiveDataCount == buf_len1 + 2 + 6 + 2)
			{
				crc_temp = (USART1_ReceiveBuff[USART1_ReceiveDataCount - 2] << 8) | USART1_ReceiveBuff[USART1_ReceiveDataCount - 1];
				CRC_Value = USER_CRC16_X25(USART1_ReceiveBuff + 2,USART1_ReceiveDataCount - 2 - 2);
				if(CRC_Value == crc_temp)
				{
					if(USART1_ReceiveBuff[8] == PC_ENTER_BOOT_RQ)				//进入Boot指令
					{
						EnterBootFlag = 1;										//进入Boot标志
					}
					else
					{
						ReceiveOverFlag = 1;						
					}
				}
				else
				{
					//校验失败
					CrcErrFlag = 1;
				}
			}
		}
			
		USART1_ReceiveDataCount = 0;
		
	}	
}







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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

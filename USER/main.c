/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
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

#include "stm32f2xx.h"
#include "CRC_X25.h"

/*******************************************************************************
*
*����ԭ�ͣ�USER_Variable_Init();
*��ڲ�������
*���ڲ�������
*�������ܣ�������ʼ��
*
*******************************************************************************/
void USER_Variable_Init(void)
{
	
}

/*******************************************************************************
*
*����ԭ�ͣ�USER_GPIO_Init();
*��ڲ�������
*���ڲ�������
*�������ܣ�������ʼ��
*
*******************************************************************************/
void USER_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_1);
	GPIO_SetBits(GPIOE,GPIO_Pin_15);
}

void USER_Beep(void)
{
	uint16_t i;
	for(i = 0; i < 300; i++)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_1);
		GPIO_SetBits(GPIOE,GPIO_Pin_15);
		USER_Delay1ms(1);
		GPIO_ResetBits(GPIOD,GPIO_Pin_1);
		GPIO_ResetBits(GPIOE,GPIO_Pin_15);
		USER_Delay1ms(1);
	}	
}

int main(void)
{
	USER_Variable_Init();
//	USER_SysTick_Init();
//	USER_SysTick_Start();
	USER_GPIO_Init();
	USER_USART1_Config(115200);
//	USER_USART6_Config(921600);
	USER_NVIC_Config();		
	
	USER_Beep();
	
	USER_Buffer_To_App();
	
	if(EnterBootFlag == 1)
	{
		EnterBootFlag = 0;
		PC_EnterBootFunction();
	}
	else
	{
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

		USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
		USART_Cmd(USART1, DISABLE);
		
		//��ת��Ӧ�ó���
		USER_IAPLoadAPP();				//����Ӧ�ó���ǰ���رմ����жϣ��رմ���
		
		//���û��Ӧ�ó��򣬻�Ӧ�ó�����������´򿪴����жϣ��򿪴���
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		USART_Cmd(USART1, ENABLE);
	}
	
	for( ; ;)
	{	
		if(ReceiveOverFlag == 1)
		{
			ReceiveOverFlag = 0;
			PC_DataProcess();
		}
		
		if(CrcErrFlag == 1)
		{
			CrcErrFlag = 0;
			PC_ReTransFrameFunction();
		}
	}	
}



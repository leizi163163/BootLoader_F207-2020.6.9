
#include "stm32f2xx.h"

uint16_t USART_IdleClear;

uint8_t USART1_ReceiveBuff[DATA_FRAME_SIZE];
uint16_t USART1_ReceiveDataCount = 0;

uint8_t ReceiveOverFlag = 0;
uint8_t EnterBootFlag = 0;
uint8_t CrcErrFlag = 0;

char const String_ResetDeviceCommand[] = "Reset Device Command!";


/*************************************************************************************************/

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)
	{
		//ѭ������,ֱ��������� 
	}		
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/************************************************************************************************/

/*******************************************************************************
*
*����ԭ�ͣ�USER_USART1_Config(uint32_t baud_rate);
*��ڲ�����baud_rate��������
*���ڲ�������
*�������ܣ�����1��ʼ��
*
*******************************************************************************/

void USER_USART1_Config(uint32_t baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
//	GPIO_PinAFConfig(, , GPIO_AF_MCO);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
//	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
//	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	USART_GetFlagStatus(USART1, USART_FLAG_TC);						//�����һ�ֽڶ�ʧ����

}

/*******************************************************************************
*
*����ԭ�ͣ�USER_USART2_Config(uint32_t baud_rate);
*��ڲ�����baud_rate��������
*���ڲ�������
*�������ܣ�����2��ʼ��
*
*******************************************************************************/
void USER_USART2_Config(uint32_t baud_rate)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
	
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
////	
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
////	GPIO_Init(GPIOA,&GPIO_InitStructure);
////	
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
////	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	USART_InitStructure.USART_BaudRate = baud_rate;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_Init(USART2,&USART_InitStructure);
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_Cmd(USART2, ENABLE);
//	USART_GetFlagStatus(USART2, USART_FLAG_TC);					//�����һ�ֽڶ�ʧ����
	
}


/*******************************************************************************
*
*����ԭ�ͣ�USER_USART6_Config(uint32_t baud_rate);
*��ڲ�����baud_rate��������
*���ڲ�������
*�������ܣ�����1��ʼ��
*
*******************************************************************************/

void USER_USART6_Config(uint32_t baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
//	GPIO_PinAFConfig(, , GPIO_AF_MCO);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART6,&USART_InitStructure);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART6, ENABLE);
	
//	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);
//	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
	
	USART_GetFlagStatus(USART6, USART_FLAG_TC);						//�����һ�ֽڶ�ʧ����

}

/*******************************************************************************
*
*����ԭ�ͣ�USART_Send_String(USART_TypeDef* USARTx,uint8_t *usart_string,uint8_t usart_string_count);
*��ڲ�����USARTx,���ڱ�ţ�USART1,	USART2,	USART3, UART4 or UART5	;
*		   usart_string,Ҫ���͵����ݵ��׵�ַ
*		   usart_string_count��Ҫ���͵����ݸ���
*���ڲ�������
*�������ܣ��������ݷ���
*
*******************************************************************************/
void USART_Send_String(USART_TypeDef* USARTx,uint8_t *usart_string,uint8_t usart_string_count)
{
	uint8_t i;
	for(i = 0; i < usart_string_count; i++)
	{
//		USART_SendData(USARTx,*usart_string++);
		USARTx->DR = (*usart_string & (uint16_t)0x01FF);
		usart_string++;		
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TC);
	}
		
}



/*************************************************************************
static void NVIC_Config(void)

�������û�д��ÿ��������ߣ�����һ�������ĺ������ҿ���ͬ����
����ǰ�߼�static������˵���˺���ֻ���ڱ�Դ�ļ���ʹ�ã�
��������Щ���Ժ����ü��ɡ�
*************************************************************************/

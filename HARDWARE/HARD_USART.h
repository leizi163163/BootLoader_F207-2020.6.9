#ifndef	__HARD_USART_H
#define	__HARD_USART_H

#include "stm32f2xx.h"

#define PACKAGE_LEN    4096
#define	DATA_FRAME_SIZE		(PACKAGE_LEN + 16)

extern uint16_t USART_IdleClear;

extern uint8_t USART1_ReceiveBuff[DATA_FRAME_SIZE];
extern uint16_t USART1_ReceiveDataCount;

extern uint8_t ReceiveOverFlag;
extern uint8_t EnterBootFlag;
extern uint8_t CrcErrFlag;


extern char const String_ResetDeviceCommand[];

void USER_USART1_Config(uint32_t baud_rate);
void USER_USART2_Config(uint32_t baud_rate);
void USER_USART6_Config(uint32_t baud_rate);
void USART_Send_String(USART_TypeDef* USARTx,uint8_t *usart_string,uint8_t usart_string_count);

#endif


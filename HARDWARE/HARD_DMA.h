#ifndef	__HARD_DMA_H
#define	__HARD_DMA_H

#include "stm32f10x.h"


extern uint16_t DMA_CurrentCndtrValue;

void DMA_Config_ReceiveMode(void);
void DMA_Config_SendMode(void);


#endif

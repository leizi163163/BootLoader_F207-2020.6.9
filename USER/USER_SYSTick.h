#ifndef __USER_SYSTICK_H
#define	__USER_SYSTICK_H

#include "stm32f2xx.h"

extern uint16_t SysTickCount;


void USER_SysTick_Init(void);
void USER_SysTick_Start(void);
void USER_SysTick_Stop(void);

void USER_Delay1ms(uint16_t ms_count);

#endif

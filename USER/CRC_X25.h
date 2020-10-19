#ifndef	__CRC_X25_H
#define	__CRC_X25_H

#include "stm32f2xx.h"

extern uint16_t CRC_Value;


uint16_t USER_CRC16_X25(uint8_t *data, uint16_t len);

#endif


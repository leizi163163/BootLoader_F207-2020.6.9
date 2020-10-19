#ifndef __HARD_FLASH_H
#define __HARD_FLASH_H

#include "stm32f2xx.h"

typedef  void (*pFunction)(void);


#define ApplicationAddress    		0x08004000

//ȫƬ����ʱ����������ˣ�ֻʹ����ǰ512K	0x08007FFF
#define FLASH_USER_APP_START_ADDR				ADDR_FLASH_SECTOR_1			//APP��ʼFLASH��--����
#define FLASH_USER_APP_END_ADDR					ADDR_FLASH_SECTOR_7			//APP����FLASH��--������
#define FLASH_USER_BUFFER_START_ADDR			ADDR_FLASH_SECTOR_7			//����ʱ�Ļ��濪ʼFLASH��--����
#define FLASH_USER_BUFFER_END_ADDR				ADDR_FLASH_SECTOR_8			//����ʱ�Ļ������FLASH��--������
																			
#define BIN_COMPLETE_FLAG_ADDR    	FLASH_USER_BUFFER_START_ADDR
#define BIN_FILE_LEN_ADDR    		(FLASH_USER_BUFFER_START_ADDR + 4)

#define BIN_COMPLETE_FLAG    		0x55AAAA55


/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */


uint8_t FLASH_ReadByte(uint32_t address);
uint16_t FLASH_ReadHalfWord(uint32_t address);
uint32_t FLASH_ReadWord(uint32_t address);

void STMFLASH_Erase(uint32_t start_address,uint32_t end_address);
void STMFLASH_Write(uint32_t address,uint8_t *pBuffer,uint16_t pBuffer_Num);		//��ָ����ַ��ʼд��ָ�����ȵ�����	

uint32_t GetSector(uint32_t Address);

void USER_IAPLoadAPP(void);

#endif


















#include "stm32f2xx.h"

uint8_t StartSector;
uint8_t EndSector;

 
pFunction Jump_To_Application;
uint32_t JumpAddress;


uint8_t FLASH_ReadByte(uint32_t address)
{
	return (uint8_t)(*(__IO uint32_t *)address);
}

uint16_t FLASH_ReadHalfWord(uint32_t address)
{
	return (uint16_t)(*(__IO uint32_t *)address);
}

uint32_t FLASH_ReadWord(uint32_t address)
{
	return (uint32_t)(*(__IO uint32_t *)address);
}

void STMFLASH_Erase(uint32_t start_address,uint32_t end_address)
{
	uint8_t i;
	FLASH_Unlock(); //解锁FLASH后才能向FLASH中写数据。
	//下面这个清flash的状态标志很重要，如果没有清可能会导致擦除flash失败或者不能擦除，我就遇到了这个问题，希望后面的兄弟一定要记得。
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
			  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	/* Get the number of the start and end sectors */
	StartSector = GetSector(start_address);  //获取FLASH的Sector编号
	EndSector = GetSector(end_address);
	
	//擦除FLASH
	for (i = StartSector; i < EndSector; i += 8)  //每次FLASH编号增加8，可参考上边FLASH Sector的定义。
	{
	/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
	be done by word */ 
		if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
		{ 
			while (1)
			{
				
			}
		}
	}
	
	/*擦除完毕*/
	
	FLASH_Lock();  //读FLASH不需要FLASH处于解锁状态。
}
void STMFLASH_Write(uint32_t address,uint8_t *pBuffer,uint16_t pBuffer_Num)	
{
	
	
	FLASH_Unlock(); //解锁FLASH后才能向FLASH中写数据。
	
	/*开始写入*/
	for( ;pBuffer_Num > 0;pBuffer_Num--)
	{
		FLASH_ProgramByte(address,*pBuffer);
		pBuffer++;
		address++;
	}
	
	FLASH_Lock();  //读FLASH不需要FLASH处于解锁状态。
}

uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;  
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;  
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;  
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;  
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;  
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_Sector_11;  
	}

	return sector;
}



void USER_IAPLoadAPP(void)
{
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)  
	{   
		JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);  
		Jump_To_Application = (pFunction) JumpAddress;    
		__set_MSP(*(__IO uint32_t*) ApplicationAddress);  
		Jump_To_Application();  
	}  
	else  
	{  
//		USART_Send_String(USART2,"NO Program\r\n",12);  
	} 
}







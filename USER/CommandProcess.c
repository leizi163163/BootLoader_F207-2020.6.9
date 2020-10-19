#include "stm32f2xx.h"


RSU_DataFrameDef			RSU_DataFrameStructure = {0xFF,0xFF,0x00};
uint16_t RSU_DataFrameCount = 0;


RSU_EnterBootDef			RSU_EnterBootStructure = {0xAA,0x00};
RSU_EraseFlashDef			RSU_EraseFlashStructure = {0xAB,0x00};
RSU_BinDataDef				RSU_BinDataStructure = {0xAC,0x00};
RSU_UpdateSuccDef			RSU_UpdateSuccStructure = {0xAD,0x00};
RSU_ReTransFrameDef			RSU_ReTransFrameStructure = {0xAE,0x01,0x00};

uint8_t BinCompleteTab[4] = {0x55,0xAA,0xAA,0x55};



void RSU_USART_SendFunction(uint8_t *buf,uint8_t buf_len)
{
//	从VER到DATA		
	CRC_Value = USER_CRC16_X25(buf,buf_len);
	buf[buf_len] = (uint8_t)(CRC_Value >> 8);
	buf[buf_len + 1] = (uint8_t)(CRC_Value);
		
	USART_Send_String(USART1,buf - 2,buf_len + 2 + 2);
}

void PC_EnterBootFunction(void)
{
	RSU_EnterBootStructure.Status = 0x00;
	RSU_DataFrameCount = sizeof(RSU_EnterBootStructure);
	memcpy(RSU_DataFrameStructure.DATA,&RSU_EnterBootStructure,RSU_DataFrameCount);
	RSU_DataFrameStructure.SEQ = (USART1_ReceiveBuff[3] << 4) | (USART1_ReceiveBuff[3] >> 4);
	
	RSU_DataFrameStructure.LEN[2] = (uint8_t)(RSU_DataFrameCount >> 8);
	RSU_DataFrameStructure.LEN[3] = (uint8_t)RSU_DataFrameCount;
	
	RSU_USART_SendFunction(&RSU_DataFrameStructure.VER,RSU_DataFrameCount + DATA_FRAME_LEN);
}

void PC_EraseFlashFunction(void)
{
	STMFLASH_Erase(FLASH_USER_BUFFER_START_ADDR,FLASH_USER_BUFFER_END_ADDR);
	
	if(FLASH_ReadWord(FLASH_USER_BUFFER_START_ADDR) == 0xFFFFFFFF)
	{
		RSU_EraseFlashStructure.Status = 0x00;
	}
	else
	{
		RSU_EraseFlashStructure.Status = 0x01;
	}
	
	RSU_DataFrameCount = sizeof(RSU_EraseFlashStructure);	
	memcpy(RSU_DataFrameStructure.DATA,&RSU_EraseFlashStructure,RSU_DataFrameCount);
	RSU_DataFrameStructure.SEQ = (USART1_ReceiveBuff[3] << 4) | (USART1_ReceiveBuff[3] >> 4);
	
	RSU_DataFrameStructure.LEN[2] = (uint8_t)(RSU_DataFrameCount >> 8);
	RSU_DataFrameStructure.LEN[3] = (uint8_t)RSU_DataFrameCount;
	
	RSU_USART_SendFunction(&RSU_DataFrameStructure.VER,RSU_DataFrameCount + DATA_FRAME_LEN);
}

void PC_BinDataFunction(uint16_t frame_len)
{
	uint32_t iap_address;
	uint32_t bin_len;
	uint8_t bin_len_buf[4];
	
	//前４字节保存升级文件完整标志,后4字节保存升级文件字节数
	iap_address = FLASH_USER_BUFFER_START_ADDR + 8 + (USART1_ReceiveBuff[10] - 1) * PACKAGE_LEN;		
	STMFLASH_Write(iap_address,USART1_ReceiveBuff + 11,frame_len);
	
	if(USART1_ReceiveBuff[10] == USART1_ReceiveBuff[9])
	{	
		bin_len = (USART1_ReceiveBuff[10] - 1) * PACKAGE_LEN + frame_len;
		bin_len_buf[0] = (uint8_t)(bin_len >> 24);
		bin_len_buf[1] = (uint8_t)(bin_len >> 16);
		bin_len_buf[2] = (uint8_t)(bin_len >> 8);
		bin_len_buf[3] = (uint8_t)(bin_len);
		STMFLASH_Write(BIN_COMPLETE_FLAG_ADDR,BinCompleteTab,4);
		STMFLASH_Write(BIN_FILE_LEN_ADDR,bin_len_buf,4);
		
//		FLASH_ProgramWord(BIN_COMPLETE_FLAG_ADDR,BIN_COMPLETE_FLAG);
//		FLASH_ProgramWord(BIN_FILE_LEN_ADDR,bin_len);
		
		RSU_BinDataStructure.FrameNum = USART1_ReceiveBuff[10];
		RSU_BinDataStructure.Status = 0x01;						//缓存区写成功
		RSU_DataFrameCount = sizeof(RSU_BinDataStructure);
		memcpy(RSU_DataFrameStructure.DATA,&RSU_BinDataStructure,RSU_DataFrameCount);
		
	}
	else
	{
		RSU_BinDataStructure.FrameNum = USART1_ReceiveBuff[10];
		RSU_BinDataStructure.Status = 0x00;
		RSU_DataFrameCount = sizeof(RSU_BinDataStructure);
		memcpy(RSU_DataFrameStructure.DATA,&RSU_BinDataStructure,RSU_DataFrameCount);
	}
		
	RSU_DataFrameStructure.SEQ = (USART1_ReceiveBuff[3] << 4) | (USART1_ReceiveBuff[3] >> 4);
	
	RSU_DataFrameStructure.LEN[2] = (uint8_t)(RSU_DataFrameCount >> 8);
	RSU_DataFrameStructure.LEN[3] = (uint8_t)RSU_DataFrameCount;
	
	RSU_USART_SendFunction(&RSU_DataFrameStructure.VER,RSU_DataFrameCount + DATA_FRAME_LEN);
	
	if(RSU_BinDataStructure.Status == 0x01)						//缓存区写成功
	{
		SoftReset();					//升级缓存文件完成，重启
	}
}

void PC_ReTransFrameFunction(void)
{
	RSU_ReTransFrameStructure.Status = 0x01;
	RSU_DataFrameCount = sizeof(RSU_ReTransFrameStructure);	
	memcpy(RSU_DataFrameStructure.DATA,&RSU_ReTransFrameStructure,RSU_DataFrameCount);
	RSU_DataFrameStructure.SEQ = (USART1_ReceiveBuff[3] << 4) | (USART1_ReceiveBuff[3] >> 4);
	
	RSU_DataFrameStructure.LEN[2] = (uint8_t)(RSU_DataFrameCount >> 8);
	RSU_DataFrameStructure.LEN[3] = (uint8_t)RSU_DataFrameCount;
	
	RSU_USART_SendFunction(&RSU_DataFrameStructure.VER,RSU_DataFrameCount + DATA_FRAME_LEN);
}

void PC_DataProcess(void)
{
	uint16_t data_len;
	uint8_t cmder;
	cmder = USART1_ReceiveBuff[8];
	data_len = USART1_ReceiveBuff[6] * 256 + USART1_ReceiveBuff[7];
	
	switch(cmder)
	{
		case PC_ERASE_FLASH_RQ:
			PC_EraseFlashFunction();			
			break;			
		case PC_BIN_DATA_RQ:
			PC_BinDataFunction(data_len - 3);		
			break;	
		
		default:
			break;
	}	
}

void USER_Buffer_To_App(void)
{
	uint32_t bin_file_len;
	uint16_t i;
	uint8_t j;
	uint8_t j_max;
	uint32_t temp;
	
	if(FLASH_ReadWord(BIN_COMPLETE_FLAG_ADDR) == BIN_COMPLETE_FLAG)
	{
		STMFLASH_Erase(FLASH_USER_APP_START_ADDR,FLASH_USER_APP_END_ADDR);
		if(FLASH_ReadWord(FLASH_USER_APP_START_ADDR) != 0xFFFFFFFF)
		{
			STMFLASH_Erase(FLASH_USER_APP_START_ADDR,FLASH_USER_APP_END_ADDR);
		}
		
		temp = FLASH_ReadWord(BIN_FILE_LEN_ADDR);
		bin_file_len = (uint16_t)((temp >> 8) | (temp >> 24));
		if((bin_file_len % PACKAGE_LEN) == 0)
		{
			j_max = bin_file_len / PACKAGE_LEN;
		}
		else
		{
			j_max = (bin_file_len + PACKAGE_LEN) / PACKAGE_LEN;
		}
		
		for(j = 0; j < j_max; j++)
		{	
			for(i = 0; i < PACKAGE_LEN; i++)
			{
				USART1_ReceiveBuff[i] = FLASH_ReadByte(FLASH_USER_BUFFER_START_ADDR + 8 + j * PACKAGE_LEN + i);
			}
			STMFLASH_Write(FLASH_USER_APP_START_ADDR + j * PACKAGE_LEN,USART1_ReceiveBuff,PACKAGE_LEN);
		}
		
		STMFLASH_Erase(FLASH_USER_BUFFER_START_ADDR,FLASH_USER_BUFFER_END_ADDR);
		if(FLASH_ReadWord(FLASH_USER_BUFFER_START_ADDR) != 0xFFFFFFFF)
		{
			STMFLASH_Erase(FLASH_USER_BUFFER_START_ADDR,FLASH_USER_BUFFER_END_ADDR);
		}
		
		RSU_UpdateSuccStructure.Status = 0x00;					//APP区写成功
		RSU_DataFrameCount = sizeof(RSU_UpdateSuccStructure);
		memcpy(RSU_DataFrameStructure.DATA,&RSU_UpdateSuccStructure,RSU_DataFrameCount);
		
		RSU_DataFrameStructure.SEQ = (USART1_ReceiveBuff[3] << 4) | (USART1_ReceiveBuff[3] >> 4);
	
		RSU_DataFrameStructure.LEN[2] = (uint8_t)(RSU_DataFrameCount >> 8);
		RSU_DataFrameStructure.LEN[3] = (uint8_t)RSU_DataFrameCount;
		
		RSU_USART_SendFunction(&RSU_DataFrameStructure.VER,RSU_DataFrameCount + DATA_FRAME_LEN);
		if(RSU_UpdateSuccStructure.Status == 0x00)
		{
			SoftReset();					//升级APP完成，重启
		}
		
	}
}



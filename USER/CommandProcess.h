#ifndef	__CommandProcess_H
#define	__CommandProcess_H

#include "stm32f2xx.h"


#define	PC_ENTER_BOOT_RQ				0xDA				//DA--����Bootָ��
#define	PC_ERASE_FLASH_RQ				0xDB				//DB--����FLASHָ��
#define	PC_BIN_DATA_RQ					0xDC				//DC--�������ݴ���ָ��

#define	RSU_ENTER_BOOT_RS				0xAA				//AA--����Boot��Ϣ֡
#define	RSU_ERASE_FLASH_RS				0xAB				//AB--����FLASH��Ϣ֡
#define	RSU_BIN_DATA_RS					0xAC				//AC--�������ݴ�����Ϣ֡
#define	RSU_UPDATE_SUCC_RS				0xAD				//AD--�����ɹ���Ϣ֡
#define	RSU_RE_TRANS_FRAME_RS			0xAE				//AE--�ش���ǰ��Ϣ֡

/* 
�ֶ�	����
STX 	֡��ʼ��־, 2 �ֽ�,ȡֵΪ FFFFH
VER 	Э��汾��, ��ǰ�汾Ϊ 00H
SEQ 	֡���к�, 1 ���ֽ�
		1,RSU ��֡���кŵĵͰ��ֽ�Ϊ 1��9, �߰��ֽ�Ϊ 0
		2,֡���к�ÿ�μ�һ,���ڱ�ʶÿһ�ε�ͨ��
		3,RSU ���͵�֡���Ϊ 0XH,���� X Ϊ 1,2,3,4,5,6,7,8,9
		4,���������͵�֡���Ϊ X0H,���� X Ϊ 1,2,3,4,5,6,7,8,9
LEN		DATA ��ĳ���, 4 �ֽڣ�VER= 0x00, �� 2 �ֽڱ���,�����ֽ�Ϊ DATA �򳤶ȣ�
DATA 	֡��������
CRC 	������־�õ�ʹ�ô�CRCУ��:	CRC-16/X25	x16+x12+x5+1
		�� VER �� DATA �����ֽڵ� CRC16 У��ֵ, 2 �ֽ�,��ʼֵΪ FFFFH 
*/

#define	DATA_FRAME_LEN					6

typedef struct
{
	uint8_t STX[2];							//֡ͷ	FFFF
	uint8_t VER;							//Э��汾��
	uint8_t SEQ;							//֡���к�
	uint8_t LEN[4];							//DATA�򳤶�,��2�ֽڱ���,���ֽ���ǰ,���ֽ��ں�
	uint8_t DATA[PACKAGE_LEN + 8];			//DATA��
}PC_DataFrameDef;

typedef struct
{
	uint8_t STX[2];							//֡ͷ	FFFF
	uint8_t VER;							//Э��汾��
	uint8_t SEQ;							//֡���к�
	uint8_t LEN[4];							//DATA�򳤶�,��2�ֽڱ���,���ֽ���ǰ,���ֽ��ں�
	uint8_t DATA[16];						//DATA��
}RSU_DataFrameDef;

typedef struct
{
	uint8_t CmdType;			//DA--����Bootָ��
	uint8_t Reserved[4];
}PC_EnterBootDef;

typedef struct
{
	uint8_t CmdType;			//DB--����FLASHָ��
	uint8_t Reserved[4];
}PC_EraseFlashDef;

typedef struct
{
	uint8_t CmdType;			//DC--�������ݴ���ָ��
	uint8_t FrameSum;			//֡����
	uint8_t FrameNum;			//��ǰ����֡֡��
	uint8_t Data[PACKAGE_LEN];
	uint8_t Reserved[4];
}PC_BinDataDef;

typedef struct
{
	uint8_t FrameType;			//AA--����Boot��Ϣ֡
	uint8_t Status;				//00H,����    01H,����
	uint8_t Reserved[4];
}RSU_EnterBootDef;

typedef struct
{
	uint8_t FrameType;			//AB--����FLASH��Ϣ֡
	uint8_t Status;				//00H,����    01H,����
	uint8_t Reserved[4];
}RSU_EraseFlashDef;

typedef struct
{
	uint8_t FrameType;			//AC--�������ݴ�����Ϣ֡
	uint8_t Status;				//00H,����    01H,����
	uint8_t FrameNum;			//��ǰ����֡֡��
	uint8_t Reserved[4];
}RSU_BinDataDef;

typedef struct
{
	uint8_t FrameType;			//AD--�����ɹ���Ϣ֡
	uint8_t Status;				//01H,������д�ɹ�    02H,APP��д�ɹ�
	uint8_t Reserved[4];
}RSU_UpdateSuccDef;

typedef struct
{
	uint8_t FrameType;			//AE--�ش���ǰ��Ϣ֡
	uint8_t Status;				//�̶�Ϊ01
	uint8_t Reserved[4];
}RSU_ReTransFrameDef;


extern RSU_DataFrameDef			RSU_DataFrameStructure;
extern uint16_t RSU_DataFrameCount;


void PC_EnterBootFunction(void);
void PC_ReTransFrameFunction(void);

void RSU_USART_SendFunction(uint8_t *buf,uint8_t buf_len);
void PC_DataProcess(void);

void USER_Buffer_To_App(void);


#endif



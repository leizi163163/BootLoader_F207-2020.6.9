#ifndef	__CommandProcess_H
#define	__CommandProcess_H

#include "stm32f2xx.h"


#define	PC_ENTER_BOOT_RQ				0xDA				//DA--进入Boot指令
#define	PC_ERASE_FLASH_RQ				0xDB				//DB--擦除FLASH指令
#define	PC_BIN_DATA_RQ					0xDC				//DC--升级数据传输指令

#define	RSU_ENTER_BOOT_RS				0xAA				//AA--进入Boot信息帧
#define	RSU_ERASE_FLASH_RS				0xAB				//AB--擦除FLASH信息帧
#define	RSU_BIN_DATA_RS					0xAC				//AC--升级数据传输信息帧
#define	RSU_UPDATE_SUCC_RS				0xAD				//AD--升级成功信息帧
#define	RSU_RE_TRANS_FRAME_RS			0xAE				//AE--重传当前信息帧

/* 
字段	描述
STX 	帧开始标志, 2 字节,取值为 FFFFH
VER 	协议版本号, 当前版本为 00H
SEQ 	帧序列号, 1 个字节
		1,RSU 的帧序列号的低半字节为 1～9, 高半字节为 0
		2,帧序列号每次加一,用于标识每一次的通信
		3,RSU 发送的帧序号为 0XH,其中 X 为 1,2,3,4,5,6,7,8,9
		4,车道机发送的帧序号为 X0H,其中 X 为 1,2,3,4,5,6,7,8,9
LEN		DATA 域的长度, 4 字节（VER= 0x00, 高 2 字节保留,低两字节为 DATA 域长度）
DATA 	帧数据内容
CRC 	根据日志得到使用此CRC校验:	CRC-16/X25	x16+x12+x5+1
		从 VER 到 DATA 所有字节的 CRC16 校验值, 2 字节,初始值为 FFFFH 
*/

#define	DATA_FRAME_LEN					6

typedef struct
{
	uint8_t STX[2];							//帧头	FFFF
	uint8_t VER;							//协议版本号
	uint8_t SEQ;							//帧序列号
	uint8_t LEN[4];							//DATA域长度,高2字节保留,高字节在前,低字节在后
	uint8_t DATA[PACKAGE_LEN + 8];			//DATA域
}PC_DataFrameDef;

typedef struct
{
	uint8_t STX[2];							//帧头	FFFF
	uint8_t VER;							//协议版本号
	uint8_t SEQ;							//帧序列号
	uint8_t LEN[4];							//DATA域长度,高2字节保留,高字节在前,低字节在后
	uint8_t DATA[16];						//DATA域
}RSU_DataFrameDef;

typedef struct
{
	uint8_t CmdType;			//DA--进入Boot指令
	uint8_t Reserved[4];
}PC_EnterBootDef;

typedef struct
{
	uint8_t CmdType;			//DB--擦除FLASH指令
	uint8_t Reserved[4];
}PC_EraseFlashDef;

typedef struct
{
	uint8_t CmdType;			//DC--升级数据传输指令
	uint8_t FrameSum;			//帧总数
	uint8_t FrameNum;			//当前数据帧帧号
	uint8_t Data[PACKAGE_LEN];
	uint8_t Reserved[4];
}PC_BinDataDef;

typedef struct
{
	uint8_t FrameType;			//AA--进入Boot信息帧
	uint8_t Status;				//00H,正常    01H,错误
	uint8_t Reserved[4];
}RSU_EnterBootDef;

typedef struct
{
	uint8_t FrameType;			//AB--擦除FLASH信息帧
	uint8_t Status;				//00H,正常    01H,错误
	uint8_t Reserved[4];
}RSU_EraseFlashDef;

typedef struct
{
	uint8_t FrameType;			//AC--升级数据传输信息帧
	uint8_t Status;				//00H,正常    01H,错误
	uint8_t FrameNum;			//当前数据帧帧号
	uint8_t Reserved[4];
}RSU_BinDataDef;

typedef struct
{
	uint8_t FrameType;			//AD--升级成功信息帧
	uint8_t Status;				//01H,缓存区写成功    02H,APP区写成功
	uint8_t Reserved[4];
}RSU_UpdateSuccDef;

typedef struct
{
	uint8_t FrameType;			//AE--重传当前信息帧
	uint8_t Status;				//固定为01
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



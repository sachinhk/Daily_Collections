
#ifndef EepromFileSystem_h
#define EepromFileSystem_h


#include "my_stdint.h"




#define EepromFile_Error_FilesMax20	1
#define EepromFile_CreateOK			2
#define EepromFile_HaveThisFile		3
#define EepromFile_HaveNotThisFile		4
#define EepromFile_OpenOk				5
#define EepromFile_SEEK_CUR			6
#define EepromFile_SEEK_END			7
#define EepromFile_SEEK_SET			8
#define EepromFile_Error_SeekFlag		9
#define EepromFile_SeekOK				10
#define EepromFile_Error_SeekOutofArea	11
#define EepromFile_Error_NoContext		12
#define EepromFile_ReadOk				13
#define EepromFile_WriteOk				14
#define EepromFile_DeleteOk			15
#define EepromFile_Error_AreaNotEnough	16
#define EepromFile_OperateNormal		17
#define EepromFile_FileNameToLong		18




struct EepromFile
{
	unsigned char Name[12];
	unsigned short Size;
	unsigned short StartPage;

	unsigned short CurPos;	//这个不需要存在eeprom中
};

typedef struct EepromFile *pEFILE;
typedef struct EepromFile EFILE;



u8 EepromFile_readPage(u8 *buff,u16 page);
u8 EepromFile_WritePage(uc8 *buff,u16 page);



void EepromFile_FileSystemFormat(void);
u8 EepromFile_Create(uc8 Name[12],pEFILE efp);
u8 EepromFile_Open(uc8 Name[12],pEFILE efp);
u8 EepromFile_Seek(pEFILE efp,s16 size,u8 flag);
u8 EepromFile_read(u8 *buffer, u16 size,  pEFILE efp);
u8 EepromFile_Write(uc8 *buffer, u16 size,  pEFILE efp);




u8 EepromFile_CheckFileExist(uc8 *name);
u8 EepromFile_WriteData(const unsigned char *buff,unsigned int wLen,const unsigned char *filename);
u8 EepromFile_ReadData(unsigned char *buff,unsigned int rLen,const unsigned char *filename);
u8 EepromFile_Delete(uc8 *Name);
void EepromFile_GetFileName(u8 *Name,u8 number);


#endif






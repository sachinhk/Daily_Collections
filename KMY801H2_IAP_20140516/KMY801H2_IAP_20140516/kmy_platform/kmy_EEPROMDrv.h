#ifndef __KMY_EEPROMDRV_H
#define __KMY_EEPROMDRV_H



void kmy_EepromInit(void);


/*param:	uint16_t addr ,write address 
param:	uint8_t *data, the datas write to eeprom
param:	uint8_t n, length of the datas
reval:	char 0 ,write success ;1 write fail
*/
char kmy_EepromWrite(unsigned short addr,const unsigned char *data,int n);


char kmy_EepromWriteByte(unsigned short addr,const unsigned char data);


char kmy_EepromWritePage(unsigned short addr,const unsigned char *data,unsigned char n);


char kmy_EepromReadByte(unsigned short addr,unsigned char *readvalue);


char kmy_EepromReadSequent(unsigned short addr,unsigned char *readvalue,unsigned short nByte);	  


unsigned char kmy_EepromCheckWhetherBlankChip(void);
//函数功能：检查eeprom芯片是否为新的空芯片
//函数参数：无			  
//返回值：  
//        芯片是新的空芯片返回1
//		  芯片不是新的空芯片返回0
//注意事项：无

#endif

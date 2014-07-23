#ifndef __KMY_EEPROMDRV_H
#define __KMY_EEPROMDRV_H





/*
����function��EEPROM drive function initialization 
����parameter��no
����return value��no
*/
void kmy_EepromInit(void);


/*
����function��read the first byte of EEPROM
����parameter��addr address��
         ������*readvalue save the read data
����return value��0 read success��1 read fail
*/
char kmy_EepromReadByte(unsigned short addr, unsigned char *readvalue);


/*
����function��writer 1 byte to EEPROM
����parameter��addr address��
             ��data 1 byte of data to write
����return value��0 write success��1 write fail
*/
char kmy_EepromWriteByte(unsigned short addr, unsigned char data);


/*
����function��write n byte to EEPROM
����parameter��addr address ��
             ��*data pointer point at the data to wrtie��
             ��n length of the data to write
����return value��0 write success��1 write fail
*/
char kmy_EepromWrite(unsigned short addr, const unsigned char *data, unsigned short n);


/*
����function��read n byte of EEPROM 
����parameter��addr address��
             ��*readvalue save read data��
             �� nByte length of the read data by byte
����return value��0 read success��1 read fail
*/
char kmy_EepromReadSequent(unsigned short addr,unsigned char *readvalue,unsigned short nByte);


/*
����function��write n byte data to EEPROM 
����parameter��addr address��
         ������*data pointer point at the data to write��
         ������n data length by byte
����return value��0 write success��1 write fail
	note��n<64,each write the length is less than 64 byte
*/
char kmy_EepromWritePage(unsigned short addr,const unsigned char *data,unsigned char n);


/*
����function��Check whether the eeprom for the new chip
����parameter��No
����return value��0 not new��1 new
*/
unsigned char kmy_EepromCheckWhetherBlankChip(void);

#endif

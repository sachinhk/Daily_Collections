#ifndef __KMY_EEPROMDRV_H
#define __KMY_EEPROMDRV_H





/*
¡¡¡¡function£ºEEPROM drive function initialization 
¡¡¡¡parameter£ºno
¡¡¡¡return value£ºno
*/
void kmy_EepromInit(void);


/*
¡¡¡¡function£ºread the first byte of EEPROM
¡¡¡¡parameter£ºaddr address£»
         ¡¡¡¡¡¡*readvalue save the read data
¡¡¡¡return value£º0 read success£¬1 read fail
*/
char kmy_EepromReadByte(unsigned short addr, unsigned char *readvalue);


/*
¡¡¡¡function£ºwriter 1 byte to EEPROM
¡¡¡¡parameter£ºaddr address£»
             ¡¡data 1 byte of data to write
¡¡¡¡return value£º0 write success£¬1 write fail
*/
char kmy_EepromWriteByte(unsigned short addr, unsigned char data);


/*
¡¡¡¡function£ºwrite n byte to EEPROM
¡¡¡¡parameter£ºaddr address £»
             ¡¡*data pointer point at the data to wrtie£»
             ¡¡n length of the data to write
¡¡¡¡return value£º0 write success£¬1 write fail
*/
char kmy_EepromWrite(unsigned short addr, const unsigned char *data, unsigned short n);


/*
¡¡¡¡function£ºread n byte of EEPROM 
¡¡¡¡parameter£ºaddr address£»
             ¡¡*readvalue save read data£»
             ¡¡ nByte length of the read data by byte
¡¡¡¡return value£º0 read success£¬1 read fail
*/
char kmy_EepromReadSequent(unsigned short addr,unsigned char *readvalue,unsigned short nByte);


/*
¡¡¡¡function£ºwrite n byte data to EEPROM 
¡¡¡¡parameter£ºaddr address£»
         ¡¡¡¡¡¡*data pointer point at the data to write£»
         ¡¡¡¡¡¡n data length by byte
¡¡¡¡return value£º0 write success£¬1 write fail
	note£ºn<64,each write the length is less than 64 byte
*/
char kmy_EepromWritePage(unsigned short addr,const unsigned char *data,unsigned char n);


/*
¡¡¡¡function£ºCheck whether the eeprom for the new chip
¡¡¡¡parameter£ºNo
¡¡¡¡return value£º0 not new£¬1 new
*/
unsigned char kmy_EepromCheckWhetherBlankChip(void);

#endif

#ifndef __KMY_TAMPERDRV_H
#define __KMY_TAMPERDRV_H




#define TAMPER_TRIGGERED	0x01
#define TAMPER_NO_TRIGGER	0x02




/*
    Function��initialization for disassemble terminal to check the function
����Function parameter��No
����Return value��No
*/
void kmy_BackUpInit(void);


/*
    Function��check whether to disassemble terminal
����Function parameter��No
����Return value��disassemble terminal return TAMPER_TRIGGERED
����            not disassemble terminal return TAMPER_NO_TRIGGER	
*/
unsigned char kmy_CheckTamper(void);


/*
    Function��clear tamper flag
����Function parameter��No
����Return value��No
*/
void kmy_ClearTamper(void);




#endif

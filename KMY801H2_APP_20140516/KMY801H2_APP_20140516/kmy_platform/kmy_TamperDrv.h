#ifndef __KMY_TAMPERDRV_H
#define __KMY_TAMPERDRV_H




#define TAMPER_TRIGGERED	0x01
#define TAMPER_NO_TRIGGER	0x02




/*
    Function£ºinitialization for disassemble terminal to check the function
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_BackUpInit(void);


/*
    Function£ºcheck whether to disassemble terminal
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºdisassemble terminal return TAMPER_TRIGGERED
¡¡¡¡            not disassemble terminal return TAMPER_NO_TRIGGER	
*/
unsigned char kmy_CheckTamper(void);


/*
    Function£ºclear tamper flag
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_ClearTamper(void);




#endif

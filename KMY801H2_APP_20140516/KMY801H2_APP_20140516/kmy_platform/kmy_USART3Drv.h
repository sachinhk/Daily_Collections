#ifndef __KMY_USART3DRV_H
#define __KMY_USART3DRV_H




/*
¡¡¡¡function£ºserial port 3initialization
¡¡¡¡parameter£ºbrd serial port communication baud rate
¡¡¡¡return value£ºno
*/
void kmy_USART3Init(unsigned int brd);


/*
    function£ºsend character string
    parameter£º*Buff: the buffer of the sent character string.
    return value£ºno
*/
void kmy_USART3SendString(const unsigned char *Buff);


/*
    function£ºsend data
    parameter£º*Buff: the buffer of the sent data
    		  SendSize: data length
    return value£ºno
*/
void kmy_USART3SendData(const unsigned char *Buff, unsigned int SendSize);


/*
    function£ºclear all buffer in USART3£¬flag bytes set to 0
    parameter£ºno
    return value£ºno
*/
void kmy_USART3ReceiveReset(void);


/*
    function£ºreceive data from serial port
    parameter£º*Buff: location to save data
    		  RecSize: size of received data
    		  timeout_MilliSeconds: millisecond number for the timeout
    return value£ºreturn the byte number for the received data
*/
unsigned int kmy_USART3ReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);


/*
    function£ºCheck whether containing the string 'ExpectString'.
    parameter£º*ExpectString: To check the string.
    return value£º 1  if exist
                   0  if no
*/
char kmy_USART3CheckBuff(char *ExpectString);


/*
    function£ºUse the function printf() display USART2 buffer data.
    parameter£ºno.
    return value£ºno.
*/
void kmy_USART3ShowBuff(void);


/*
    function£ºreceive all the data from serial port
    parameter£º*Buff: the location to save the received data
    		  BuffSize: size of the received data
    return value£ºreturn the byte number for the received data
*/
unsigned int kmy_USART3ReadToEnd(char *Buff, unsigned int BuffSize);


/*
    function£º clear the buffer of USART2, bit zone to 0
    parameter£ºno
    return value£ºno
*/
void kmy_USART3ReceiveReset(void);


/*
    function£ºreceive the data from serial port until the end string.
    parameter£º*Buff: the location to save the received data
    		  BuffSize: size of the received data
    		  *EndString: the end string
    return value£ºreturn the byte number for the received data
                  If there is no end of string return a negative number.              
*/
int kmy_USART3ReadTo(char *Buff, short int BuffSize, char *EndString);

#endif

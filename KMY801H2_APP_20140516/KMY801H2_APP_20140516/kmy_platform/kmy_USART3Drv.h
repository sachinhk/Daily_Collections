#ifndef __KMY_USART3DRV_H
#define __KMY_USART3DRV_H




/*
����function��serial port 3initialization
����parameter��brd serial port communication baud rate
����return value��no
*/
void kmy_USART3Init(unsigned int brd);


/*
    function��send character string
    parameter��*Buff: the buffer of the sent character string.
    return value��no
*/
void kmy_USART3SendString(const unsigned char *Buff);


/*
    function��send data
    parameter��*Buff: the buffer of the sent data
    		  SendSize: data length
    return value��no
*/
void kmy_USART3SendData(const unsigned char *Buff, unsigned int SendSize);


/*
    function��clear all buffer in USART3��flag bytes set to 0
    parameter��no
    return value��no
*/
void kmy_USART3ReceiveReset(void);


/*
    function��receive data from serial port
    parameter��*Buff: location to save data
    		  RecSize: size of received data
    		  timeout_MilliSeconds: millisecond number for the timeout
    return value��return the byte number for the received data
*/
unsigned int kmy_USART3ReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);


/*
    function��Check whether containing the string 'ExpectString'.
    parameter��*ExpectString: To check the string.
    return value�� 1  if exist
                   0  if no
*/
char kmy_USART3CheckBuff(char *ExpectString);


/*
    function��Use the function printf() display USART2 buffer data.
    parameter��no.
    return value��no.
*/
void kmy_USART3ShowBuff(void);


/*
    function��receive all the data from serial port
    parameter��*Buff: the location to save the received data
    		  BuffSize: size of the received data
    return value��return the byte number for the received data
*/
unsigned int kmy_USART3ReadToEnd(char *Buff, unsigned int BuffSize);


/*
    function�� clear the buffer of USART2, bit zone to 0
    parameter��no
    return value��no
*/
void kmy_USART3ReceiveReset(void);


/*
    function��receive the data from serial port until the end string.
    parameter��*Buff: the location to save the received data
    		  BuffSize: size of the received data
    		  *EndString: the end string
    return value��return the byte number for the received data
                  If there is no end of string return a negative number.              
*/
int kmy_USART3ReadTo(char *Buff, short int BuffSize, char *EndString);

#endif

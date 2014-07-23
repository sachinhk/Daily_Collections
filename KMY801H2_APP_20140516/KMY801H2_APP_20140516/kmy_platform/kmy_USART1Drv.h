#ifndef __KMY_USART1DRV_H
#define __KMY_USART1DRV_H



/*
¡¡¡¡function£ºserial 1 initialization
¡¡¡¡parameter£ºbrd serail port communication baud rate
¡¡¡¡return value£ºno
*/
void kmy_USART1Init(unsigned int brd);


/*
    function £ºsend character string
    parameter£º*Buff: buffer for the sent data
    return value£ºno
*/
void kmy_USART1SendString(const unsigned char *Buff);


/*
    function£ºsend data
    parameter£º*Buff: buffer for the sent data
    		  SendSize: data length
    Return value£ºno
*/
void kmy_USART1SendData(const unsigned char *Buff, unsigned int SendSize);


/*
    function£ºclear the buffer in USART1£¬bit zone to 0
    parameter£ºno
    return value£ºno
*/
void kmy_USART1ReceiveReset(void);


/*
    function£ºreceive data from serial port
    parameter£º*Buff: location to save received data
    		  RecSize: data in byte received
    		  timeout_MilliSeconds: timeout in millisecond
    return value£ºreturn received data byte number
*/
unsigned int kmy_USART1ReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);


#endif


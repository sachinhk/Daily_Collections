#ifndef __KMY_USART1DRV_H
#define __KMY_USART1DRV_H



/*
����function��serial 1 initialization
����parameter��brd serail port communication baud rate
����return value��no
*/
void kmy_USART1Init(unsigned int brd);


/*
    function ��send character string
    parameter��*Buff: buffer for the sent data
    return value��no
*/
void kmy_USART1SendString(const unsigned char *Buff);


/*
    function��send data
    parameter��*Buff: buffer for the sent data
    		  SendSize: data length
    Return value��no
*/
void kmy_USART1SendData(const unsigned char *Buff, unsigned int SendSize);


/*
    function��clear the buffer in USART1��bit zone to 0
    parameter��no
    return value��no
*/
void kmy_USART1ReceiveReset(void);


/*
    function��receive data from serial port
    parameter��*Buff: location to save received data
    		  RecSize: data in byte received
    		  timeout_MilliSeconds: timeout in millisecond
    return value��return received data byte number
*/
unsigned int kmy_USART1ReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);


#endif


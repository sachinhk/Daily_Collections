#ifndef __KMY_WIFI_USARTDRV_H
#define __KMY_WIFI_USARTDRV_H


				   


/*
    Function£ºUSART initialization
    Function parameter£ºBaudRate: The communication baud rate
    Return value£ºNo
*/
void kmy_WiFi_USARTInit(unsigned long BaudRate);


/*
    Function£ºWiFi switch
    Function parameter£ºOnOff£»1-->On£¬0-->Off
    Return value£ºNo
*/
void kmy_WiFiOnOff (unsigned char OnOff);


/*
    Function£º Send character string
    Function parameter£ºBuff: buffer for the sent data
    Return value£ºNo
*/
void kmy_WiFi_USARTSendString(const unsigned char *Buff);


/*
    Function£º  Send data
    Function parameter£ºBuff: buffer for the sent data
	                  SendSize: data length
¡¡¡¡Return value£ºNo
*/
void kmy_WiFi_USARTSendData(const unsigned char *Buff, unsigned int SendSize);


/*
    Function£º Clear the buffer of USART
    Function parameter£ºNo
    Return value£ºNo
*/
void kmy_WiFi_USARTReceiveReset(void);


/*
    Function£º Receive data from the USART
    Function parameter£ºBuff: Receive buffer
	          RecSize: Receive buffer Size
	          timeout_MilliSeconds: Timeout in milliseconds
    Return value£ºreturn received data byte number
*/
unsigned int kmy_WiFi_USARTReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);


/*
    Function£º Check whether the USART buffer containing the string ¡°ExpectString¡±
¡¡¡¡Function parameter£ºExpectString£ºTo check the string
¡¡¡¡Return value£ºContain the string    1
¡¡¡¡             Not contain a string  0
*/
char kmy_WiFi_USARTCheckBuff(char *ExpectString);


/*
    Function£º Display the USART buffer data through the USART1
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_WiFi_USARTShowBuff(void);


/*
    Function£º Read the data of USART
¡¡¡¡Function parameter£ºBuff£ºReceive buffer
¡¡¡¡          BuffSize£ºReceive buffer size
¡¡¡¡Return value£ºThe actual received data length
*/
unsigned int kmy_WiFi_USARTReadToEnd(char *Buff, unsigned int BuffSize);


/*
    Function£º Read the data of USART until the ending string ¡°EndString¡±
¡¡¡¡Function parameter£ºBuff£ºReceive buffer
¡¡¡¡          BuffSize£ºReceive buffer size
¡¡¡¡          EndString£ºThe ending string 
¡¡¡¡Return value£ºThe actual received data length
*/
int kmy_WiFi_USARTReadTo(char *Buff, short int BuffSize, char *EndString);

#endif

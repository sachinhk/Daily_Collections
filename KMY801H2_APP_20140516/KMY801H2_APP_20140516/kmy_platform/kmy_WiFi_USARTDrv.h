#ifndef __KMY_WIFI_USARTDRV_H
#define __KMY_WIFI_USARTDRV_H


				   


/*
    Function��USART initialization
    Function parameter��BaudRate: The communication baud rate
    Return value��No
*/
void kmy_WiFi_USARTInit(unsigned long BaudRate);


/*
    Function��WiFi switch
    Function parameter��OnOff��1-->On��0-->Off
    Return value��No
*/
void kmy_WiFiOnOff (unsigned char OnOff);


/*
    Function�� Send character string
    Function parameter��Buff: buffer for the sent data
    Return value��No
*/
void kmy_WiFi_USARTSendString(const unsigned char *Buff);


/*
    Function��  Send data
    Function parameter��Buff: buffer for the sent data
	                  SendSize: data length
����Return value��No
*/
void kmy_WiFi_USARTSendData(const unsigned char *Buff, unsigned int SendSize);


/*
    Function�� Clear the buffer of USART
    Function parameter��No
    Return value��No
*/
void kmy_WiFi_USARTReceiveReset(void);


/*
    Function�� Receive data from the USART
    Function parameter��Buff: Receive buffer
	          RecSize: Receive buffer Size
	          timeout_MilliSeconds: Timeout in milliseconds
    Return value��return received data byte number
*/
unsigned int kmy_WiFi_USARTReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);


/*
    Function�� Check whether the USART buffer containing the string ��ExpectString��
����Function parameter��ExpectString��To check the string
����Return value��Contain the string    1
����             Not contain a string  0
*/
char kmy_WiFi_USARTCheckBuff(char *ExpectString);


/*
    Function�� Display the USART buffer data through the USART1
����Function parameter��No
����Return value��No
*/
void kmy_WiFi_USARTShowBuff(void);


/*
    Function�� Read the data of USART
����Function parameter��Buff��Receive buffer
����          BuffSize��Receive buffer size
����Return value��The actual received data length
*/
unsigned int kmy_WiFi_USARTReadToEnd(char *Buff, unsigned int BuffSize);


/*
    Function�� Read the data of USART until the ending string ��EndString��
����Function parameter��Buff��Receive buffer
����          BuffSize��Receive buffer size
����          EndString��The ending string 
����Return value��The actual received data length
*/
int kmy_WiFi_USARTReadTo(char *Buff, short int BuffSize, char *EndString);

#endif

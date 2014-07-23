#ifndef _KMY_WIFI_SPI_H_
#define _KMY_WIFI_SPI_H_


/**
* @brief  Initializes the spi1 dev.
* @param  None
* @retval None
*/
void kmy_SPIInit(void);

/**
* @brief  Empty the the receive buffer.
* @param  None
* @retval None
*/
void kmy_SPIReceiveReset(void);

/**
* @brief  Read the receive buffer data length.
* @param  None
* @retval The value of length.
*/
unsigned int kmy_ReadBufferLen(void);

/**
* @brief  Sends a byte through the SPI interface and return the byte received
*         from the SPI bus.
* @param  Buff: byte to send.
* @retval The value of the received byte.
*/
unsigned char kmy_SPISendOneByte(unsigned char Buff);

/**
* @brief  Reads a block of data from the receive buffer.
* @param  Buff: pointer to the buffer that receives the data.
* @param  Len: number of bytes to read.
* @retval The actual number of bytes to read.
*/
unsigned int kmy_SPIReadData(unsigned char *Buff,unsigned int Len);

/**
* @brief  Writes block of data.
* @param  Buff: pointer to the buffer  containing the data to be written.
* @param  Len: number of bytes to write to.
* @retval The actual number of bytes to write to.
*/
unsigned int kmy_SPIWriteData(unsigned char *Buff,unsigned int Len);

/*
¡¡¡¡Function£ºRead the data until the end of string(EndString).
¡¡¡¡Function parameter£ºBuff: pointer to the buffer that receives the data.
¡¡¡¡                    BuffSize: Receive buffer size
¡¡¡¡                    EndString: The end of string
¡¡¡¡Return value£ºThe actual number of bytes to read.
                  If there is no end of string return a negative number. 
*/
unsigned int kmy_SPIReadTo(unsigned char *Buff, unsigned int BuffSize,unsigned char *EndString);



#endif

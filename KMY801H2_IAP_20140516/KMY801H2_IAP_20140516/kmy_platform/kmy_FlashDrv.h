#ifndef __KMY_FLASHDRV_H
#define __KMY_FLASHDRV_H


#include "stm32f10x.h"


/**
* @brief  Initializes the flash ic.
* @param  None
* @retval None
*/
void kmy_FlashInit(void);


/**
* @brief  Sends a byte through the SPI interface and return the byte received
*         from the SPI bus.
* @param  byte: byte to send.
* @retval The value of the received byte.
*/
uint8_t kmy_FlashSendByte(uint8_t byte);



/**
* @brief  Erases the specified FLASH sector.
* @param  SectorAddr: address of the sector to erase.
* @retval None
*/
void kmy_FlashEraseSector(uint32_t SectorAddr);

/**
* @brief  Erases the entire FLASH.
* @param  None
* @retval None
*/
void kmy_FlashEraseBulk(void);



/**
* @brief  Enables the write access to the FLASH.
* @param  None
* @retval None
*/
void kmy_FlashWriteEnable(void);

/**
* @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
*         status register and loop until write opertaion has completed.
* @param  None
* @retval None
*/
void kmy_FlashWaitForWriteEnd(void);


/**
* @brief  Writes block of data to the FLASH. In this function, the number of
*         WRITE cycles are reduced, using Page WRITE sequence.
* @param  pBuffer: pointer to the buffer  containing the data to be written
*         to the FLASH.
* @param  WriteAddr: FLASH's internal address to write to.
* @param  NumByteToWrite: number of bytes to write to the FLASH.
* @retval None
*/
void kmy_FlashWriteBuffer(const unsigned char* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

/**
* @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
*         (Page WRITE sequence).
* @note   The number of byte can't exceed the FLASH page size.
* @param  pBuffer: pointer to the buffer  containing the data to be written
*         to the FLASH.
* @param  WriteAddr: FLASH's internal address to write to.
* @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
*         or less than "kmy_FlashPAGESIZE" value.
* @retval None
*/
void kmy_FlashWritePage(const unsigned char* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);




/**
* @brief  Reads FLASH identification.
* @param  None
* @retval FLASH identification
*/
//uint32_t kmy_FlashReadID(void);


/**
* @brief  Reads a block of data from the FLASH.
* @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
* @param  ReadAddr: FLASH's internal address to read from.
* @param  NumByteToRead: number of bytes to read from the FLASH.
* @retval None
*/
void kmy_FlashReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);



#endif

/**
******************************************************************************
* @file IAP/inc/common.h 
* @author   MCD Application Team
* @version  V3.0.0
* @date     04/27/2009
* @brief    This file provides all the headers of the common functions.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _COMMON_H
#define _COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "ymodem.h"
#include "platform_config.h" 


/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE       128

#define ApplicationAddress    0x800C000

#define ALineContainDotLine	13		//a line contian How many dot line
#define ALinePixNum			131		//a row total pixels
#define AAscciiPixNum		6		//a Asccii  cod contain pixels
#define MENU_COL 22
#define Menu_LIMIT 10


typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef struct
{
	char CurMenuIndex;
	char OldHighLightRow;
	char CurHighLightRow;
	char MenuScrollCount;
	char MenuLayerCnt;
	char FlashFlag;
} MENUVAR;


extern unsigned char key_flag;




/* Exported macro ------------------------------------------------------------*/
/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

#define SerialPutString(x) Serial_PutString((uint8_t*)(x))

/* Exported functions ------------------------------------------------------- */
void Int2Str(uint8_t* str,int32_t intnum);
uint32_t Str2Int(uint8_t *inputstr,int32_t *intnum);
uint32_t GetIntegerInput(int32_t * num);
uint32_t SerialKeyPressed(uint8_t *key);
//uint32_t SerialKeyPressed2(uint8_t *key);

uint8_t GetKey(void);
void SerialPutChar(uint8_t c);
//void SerialPutChar2(uint8_t c);

void Serial_PutString(uint8_t *s);
void GetInputString(uint8_t * buffP);
uint32_t FLASH_PagesMask(__IO uint32_t Size);
void FLASH_DisableWriteProtectionPages(void);
void Main_Menu(void);
void SerialDownload(void);

void LcdWriteRowX (unsigned char row, unsigned char x, const unsigned char *buff);
void DrawTitle_to_row(unsigned char row,const unsigned char *str);
void LcdClearRow (char startrow, char endrow);
char GetString_abcABC123PYWuBi (unsigned char srfFlag, unsigned char DefaultSrf, unsigned char *receive, unsigned char recSize, unsigned char HaveoldFlag);


#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2009 STMicroelectronics *****END OF FILE******/

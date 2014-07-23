/**
******************************************************************************
* @file IAP/src/download.c
* @author   MCD Application Team
* @version  V3.0.0
* @date     04/27/2009
* @brief    This file provides the software which allows to download an image
*           to internal Flash.
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

/** @addtogroup IAP
* @{
*/
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "kmy_LCDDrv.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t file_name[FILE_NAME_LENGTH];
uint8_t tab_1024[1024] =
{
	0
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Download a file via serial port
* @param  None
* @retval : None
*/
void SerialDownload(void)
{
	uint8_t Number[10] = "          ";
	int32_t Size = 0;

	SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	Size = Ymodem_Receive(&tab_1024[0]);
	if (Size > 0)
	{
		SerialPutString("\n\n\r Upgrade Successfully!\n\r--------------------------------\r\n Name: ");
		SerialPutString(file_name);
		Int2Str(Number, Size);
		SerialPutString("\n\r Size: ");
		SerialPutString(Number);
		SerialPutString(" Bytes\r\n");
		//SerialPutString("--------------------------------\r\n");
		SerialPutString("POS will restart!\r\n");

		kmy_LcdClear();
		DrawTitle_to_row(2,"Upgrade done");
		DrawTitle_to_row(3,"POS will restart");
		NVIC_SystemReset();
	}
	else if (Size == -1)
	{
		SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
	}
	else if (Size == -2)
	{
		SerialPutString("\n\n\rVerification failed!\n\r");
	}
	else if (Size == -3)
	{
		SerialPutString("\r\n\nAborted by user.\n\r");
	}
	else
	{
		SerialPutString("\n\rFailed to receive the file!\n\r");
	}
}




/*******************(C)COPYRIGHT 2009 STMicroelectronics *****END OF FILE******/

/**
  ******************************************************************************
  * @file    Smartcard/inc/smartcard.h
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file contains all the functions prototypes for the Smartcard
  *          firmware library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * PSAM_DIRECT, PSAM_INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 20009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KMY_PSAMDRV_H
#define __KMY_PSAMDRV_H

/* Exported constants --------------------------------------------------------*/
#define PSAM_T0_PROTOCOL        0x00  /* T0 protocol */
#define PSAM_DIRECT             0x3B  /* Direct bit convention */
#define PSAM_INDIRECT           0x3F  /* Indirect bit convention */
#define PSAM_SETUP_LENGTH       20
#define PSAM_HIST_LENGTH        20
#define PSAM_LC_MAX             20

#define PSAM_RECEIVE_TIMEOUT 0x2000  /* Direction to reader */

/* SC Tree Structure -----------------------------------------------------------
                              MasterFile
                           ________|___________
                          |        |           |
                        System   UserData     Note
------------------------------------------------------------------------------*/

/* SC ADPU Command: Operation Code -------------------------------------------*/
#define PSAM_CLA_GSM11       0xA0

/*------------------------ Data Area Management Commands ---------------------*/
#define PSAM_SELECT_FILE     0xA4
#define PSAM_GET_RESPONCE    0xC0
#define PSAM_STATUS          0xF2
#define PSAM_UPDATE_BINARY   0xD6
#define PSAM_READ_BINARY     0xB0
#define PSAM_WRITE_BINARY    0xD0
#define PSAM_UPDATE_RECORD   0xDC
#define PSAM_READ_RECORD     0xB2

/*-------------------------- Administrative Commands -------------------------*/
#define PSAM_CREATE_FILE     0xE0

/*-------------------------- Safety Management Commands ----------------------*/
#define PSAM_VERIFY          0x20
#define PSAM_CHANGE          0x24
#define PSAM_DISABLE         0x26
#define PSAM_ENABLE          0x28
#define PSAM_UNBLOCK         0x2C
#define PSAM_EXTERNAL_AUTH   0x82
#define PSAM_GET_CHALLENGE   0x84

/*-------------------------- Answer to reset Commands ------------------------*/
#define PSAM_GET_A2R         0x00

/* SC STATUS: Status Code ----------------------------------------------------*/
#define PSAM_EF_SELECTED     0x9F
#define PSAM_DF_SELECTED     0x9F
#define PSAM_OP_TERMINATED   0x9000

/* Smartcard Voltage */
#define PSAM_VOLTAGE_5V      0
#define PSAM_VOLTAGE_3V      1

/* PSAM slot */
#define PSAM_PSAM1			1
#define PSAM_PSAM2			2
#define IC_CARD			 	3


#define IC_DISABLE()

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	PSAM_POWER_ON = 0x00,
	PSAM_RESET_LOW = 0x01,
	PSAM_RESET_HIGH = 0x02,
	PSAM_ACTIVE = 0x03,
	PSAM_ACTIVE_ON_T0 = 0x04,
	PSAM_POWER_OFF = 0x05
} PSAM_State;

/* ATR structure - Answer To Reset -------------------------------------------*/
typedef struct
{
	uint8_t TS;               /* Bit Convention */
	uint8_t T0;               /* High nibble = Number of setup byte; low nibble = Number of historical byte */
	uint8_t T[PSAM_SETUP_LENGTH];  /* Setup array */
	uint8_t H[PSAM_HIST_LENGTH];   /* Historical array */
	uint8_t Tlength;          /* Setup array dimension */
	uint8_t Hlength;          /* Historical array dimension */
} PSAM_ATR;

/* ADPU-Header command structure ---------------------------------------------*/
typedef struct
{
	uint8_t CLA;  /* Command class */
	uint8_t INS;  /* Operation code */
	uint8_t P1;   /* Selection Mode */
	uint8_t P2;   /* Selection Option */
} PSAM_Header;

/* ADPU-Body command structure -----------------------------------------------*/
typedef struct
{
	uint8_t LC;           /* Data field length */
	uint8_t Data[PSAM_LC_MAX];  /* Command parameters */
	uint8_t LE;           /* Expected length of data to be returned */
} PSAM_Body;

/* ADPU Command structure ----------------------------------------------------*/
typedef struct
{
	PSAM_Header Header;
	PSAM_Body Body;
} PSAM_ADPU_Commands;

/* SC response structure -----------------------------------------------------*/
typedef struct
{
	uint8_t Data[PSAM_LC_MAX];  /* Data returned from the card */
	uint8_t SW1;          /* Command Processing status */
	uint8_t SW2;          /* Command Processing qualification */
} PSAM_ADPU_Responce;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* APPLICATION LAYER ---------------------------------------------------------*/

/**
  * @brief  Initializes all peripheral used for Smartcard interface.
  * @param  None
  * @retval None
  */
void kmy_psamInit(void);

/**
  * @brief  Enables or disables the power to the Smartcard.
  * @param  NewState: new state of the Smartcard power supply.
  *   This parameter can be: 1 or 0.
  * @retval None
  */
void kmy_psamPowerCmd (uint8_t NewState);

/**
  * @brief  Configures the card power voltage.
  * @param  PSAM_Voltage: specifies the card power voltage.
  *   This parameter can be one of the following values:
  *     @arg PSAM_VOLTAGE_5V: 5V cards.
  *     @arg PSAM_VOLTAGE_3V: 3V cards.
  * @retval None
  */
void kmy_psamVoltageConfig (uint8_t PSAM_Voltage);

/**
  * @brief  get ATR from smart card
  * @param  atrBuff: store atr, atrBuff[0] store the len of atr
  *   
  * @retval 0 success
  *			1 failed
  */
uint8_t kmy_psamGetATR (uint8_t *atrBuff);

/*
¡¡¡¡Function£ºSend APDU command
¡¡¡¡Function parameter£º*buffer        APDU command
¡¡¡¡                  Length        APDU Command length
¡¡¡¡                  Apdu0_rbuffer  Response data
¡¡¡¡Return value£ºReturns the length of the response data
*/
uint16_t kmy_psamAPDU (uint8_t *buffer, uint16_t length, uint8_t *Apdu0_rbuffer);

/*
    Function£ºSelect psam card location
    Function parameter£ºPSAM_PSAM1		PSAM card 1
                        PSAM_PSAM2		PSAM card 2
                        IC_CARD			IC card
¡¡¡¡Return value£ºNo
*/
void kmy_psamSelect(uint8_t slot);

/*
    Function£ºConfiguration function before select psam card location
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_CardNoslect(void);

/*
    Function£ºOpen the PSAM card module power supply
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_psamPowerUp (void);

/*
    Function£ºClose the PSAM card module power supply
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_psamPowerDown (void);


#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

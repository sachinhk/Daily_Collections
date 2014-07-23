#include "kmy_LowPowerDrv.h"
#include "common.h"
#include "kmy_Time.h"
#include "kmy_KeyDrv.h"
#include "misc.h"
#include "Gprs.h"
#include "kmy_USART1Drv.h"
#include "kmy_LCDDrv.h"
#include "kmy_FlashDrv.h"
#include "kmy_GprsDrv.h"
#include "kmy_WiFi_SPI.h"
#include "kmy_WiFi_USARTDrv.h"




unsigned char gFlashDataLED=0;
uint8_t SW_Status=0;
uint8_t FirstPowerOn = 1;


extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;
extern char NetworkSwitch[2];

unsigned char key_flag=0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
static void IAP_Init(void);

/* Private functions ---------------------------------------------------------*/


//The Lib call this function one milisecond inerval
void kmy_OneMiliSecondCallThread (void)
{
}


unsigned char Upgrade_GetKeyValue(unsigned char key)
{
	unsigned char i;
	unsigned char keyval;

	for(i=0;i<10;i++)
	{
		keyval=kmy_GetKeyValue();
		if(keyval==key)
		{
			key_flag=1;
			return 1;
		}
	}

	return KEY_NONE;
}

void PowerOn(void)
{
	kmy_VCC3V5V_Init();


//------------------上电检测开关--------------------
		
		SW_Status = kmy_PowerCheck();
		if(SW_Status)
		{		
			msleep(1000);	 //延时1秒后开机			
			SW_Status = kmy_PowerCheck();
			if(SW_Status)
			{
				kmy_PowerOn();
				msleep(100);
				kmy_VCC3On();
				kmy_VCC5On();
				msleep(100);				
			}			
		}

	kmy_PowerOn();
	msleep(100);
	kmy_VCC3On();
	kmy_VCC5On();
	msleep(100);
}

/**
* @brief  Main program.
* @param  None
* @retval : None
*/
int main(void)
{
	/* Flash unlock */
	FLASH_Unlock();
	IAP_Init();
	kmy_RCCInit();
	kmy_TimeInit();
	kmy_KeyInit();
	PowerOn();
    GetEepromVariable();
	Upgrade_GetKeyValue(KEY_Enter);
	/* Test if Key push-button on STM3210x-EVAL Board is pressed */
	if(key_flag==1||Upgrade_Flag[0]=='1')
	{
		kmy_LcdInit();
		kmy_LcdClear();
		kmy_LcdBlackLightOn();
DOWNLOAD:DrawTitle_to_row(1,"Enter Upgrade Program");
		DrawTitle_to_row(2,"Version:2014-04-01");
		DrawTitle_to_row(3,"Please wait");
		kmy_FlashInit();
		WriteEepromVariable();
		if(NetworkSwitch[0]==WiFi)
		{
		    kmy_WiFi_USARTInit(9600);
	        kmy_SPIInit();
			kmy_WiFiOnOff (1);
			sleep(10);
			kmy_WiFi_USARTReceiveReset();
			printf("WiFi Mode\r\n");
		}
		else
		{
			kmy_GprsInit();
			printf("GPRS Mode\r\n");
		}
		sleep(1);
		/* Execute the IAP driver in order to re-program the Flash */
		Main_Menu ();
	}
	/* Keep the user application running */
	else
	{
		/* Test if user code is programmed starting from address "ApplicationAddress" */
/*
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//USART1中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;				
	    NVIC_Init(&NVIC_InitStructure);
*/		printf("Jump To Application\r\n");
		if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
		{   			
		    /* Jump to user application */
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_PSP(*(__IO uint32_t*) ApplicationAddress);
			__set_CONTROL(0);
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			Jump_To_Application();
		}

	}

	while (1)
	{
		kmy_LcdInit();
		kmy_LcdClear();
		kmy_LcdBlackLightOn();
		DrawTitle_to_row(1,"No application");
		DrawTitle_to_row(2,"Please download");
		sleep(5);
		goto DOWNLOAD;
	}
}

/**
* @brief  Initialize the IAP: Configure RCC, USART and GPIOs.
* @param  None
* @retval : None
*/
	void IAP_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
	
		/* Clock configuration -----------------------------------------------------*/
		RCC_Configuration();
	
		/* Configure the GPIO ports( USART1 Transmit and Receive Lines) */
		/* Configure the USART1_Tx as Alternate function Push-Pull */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* Configure the USART1_Rx as input floating */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
		/* Configure the USART1 */
		USART_Init(USART1, &USART_InitStructure);
	
		/* Enable the USART1 */
		USART_Cmd(USART1, ENABLE);
	}

/**
* @brief  Configures the different system clocks.
* @param  None
* @retval : None
*/
void RCC_Configuration(void)
{
	/* Setup STM32 system (clock, PLL and Flash configuration) */
	SystemInit();

	/* Enable GPIOA, GPIOC and USART1 clock  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
	| RCC_APB2Periph_USART1, ENABLE);
}

#ifdef USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
* @}
*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

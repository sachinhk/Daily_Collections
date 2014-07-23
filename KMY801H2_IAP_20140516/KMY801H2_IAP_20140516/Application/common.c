#include "common.h"
#include "misc.h"
#include "gprs.h"
#include "kmy_KeyDrv.h"
#include "kmy_USART3Drv.h"
#include "kmy_Time.h"
#include "stm32f10x_spi.h"
#include "kmy_FlashDrv.h"
#include "kmy_LCDDrv.h"
#include "Input.h"
#include "kmy_LowPowerDrv.h"

#include "kmy_WiFi_USARTDrv.h"
#include "kmy_WiFi_SPI.h"
#include "kmy_GprsDrv.h"





pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask = 0;
extern uint32_t FlashProtection;
extern uint32_t FlashDestination;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	USART_SendData(USART1, (unsigned char) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}




/* Private functions ---------------------------------------------------------*/

/**
* @brief  Convert an Integer to a string
* @param str: The string
* @param intnum: The intger to be converted
* @retval : None
*/
void Int2Str(uint8_t* str, int32_t intnum)
{
	uint32_t i, Div = 1000000000, j = 0, Status = 0;

	for (i = 0; i < 10; i++)
	{
		str[j++] = (intnum / Div) + 48;

		intnum = intnum % Div;
		Div /= 10;
		if ((str[j-1] == '0') & (Status == 0))
		{
			j = 0;
		}
		else
		{
			Status++;
		}
	}
}

/**
* @brief  Convert a string to an integer
* @param inputstr: The string to be converted
* @param intnum: The intger value
* @retval : 1: Correct
*   0: Error
*/
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
	uint32_t i = 0, res = 0;
	uint32_t val = 0;

	if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
	{
		if (inputstr[2] == '\0')
		{
			return 0;
		}
		for (i = 2; i < 11; i++)
		{
			if (inputstr[i] == '\0')
			{
				*intnum = val;
				/* return 1; */
				res = 1;
				break;
			}
			if (ISVALIDHEX(inputstr[i]))
			{
				val = (val << 4) + CONVERTHEX(inputstr[i]);
			}
			else
			{
				/* return 0, Invalid input */
				res = 0;
				break;
			}
		}
		/* over 8 digit hex --invalid */
		if (i >= 11)
		{
			res = 0;
		}
	}
	else 	/* max 10-digit decimal input */
	{
		for (i = 0; i < 11; i++)
		{
			if (inputstr[i] == '\0')
			{
				*intnum = val;
				/* return 1 */
				res = 1;
				break;
			}
			else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
			{
				val = val << 10;
				*intnum = val;
				res = 1;
				break;
			}
			else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
			{
				val = val << 20;
				*intnum = val;
				res = 1;
				break;
			}
			else if (ISVALIDDEC(inputstr[i]))
			{
				val = val * 10 + CONVERTDEC(inputstr[i]);
			}
			else
			{
				/* return 0, Invalid input */
				res = 0;
				break;
			}
		}
		/* Over 10 digit decimal --invalid */
		if (i >= 11)
		{
			res = 0;
		}
	}

	return res;
}

/**
* @brief  Get an integer from the HyperTerminal
* @param num: The inetger
* @retval : 1: Correct
*           0: Error
*/
uint32_t GetIntegerInput(int32_t * num)
{
	uint8_t inputstr[16];

	while (1)
	{
		GetInputString(inputstr);
		if (inputstr[0] == '\0') continue;
		if ((inputstr[0] == 'a' || inputstr[0] == 'A') && inputstr[1] == '\0')
		{
			SerialPutString("User Cancelled \r\n");
			return 0;
		}

		if (Str2Int(inputstr, num) == 0)
		{
			SerialPutString("Error, Input again: \r\n");
		}
		else
		{
			return 1;
		}
	}
}

/**
* @brief  Test to see if a key has been pressed on the HyperTerminal
* @param key: The key pressed
* @retval : 1: Correct
*   0: Error
*/
uint32_t SerialKeyPressed(uint8_t *key)
{

	if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
	{
		*key = (uint8_t)USART1->DR;
		return 1;
	}
	else
	{
		return 0;
	}
}
/*
uint32_t SerialKeyPressed2(uint8_t *key)
{

	if ( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)
	{
		*key = (uint8_t)USART3->DR;
		return 1;
	}
	else
	{
		return 0;
	}
}
*/

/**
* @brief  Get a key from the HyperTerminal
* @param  None
* @retval : The Key Pressed
*/
uint8_t GetKey(void)
{
	uint8_t key = 0;

	/* Waiting for user input */
	while (1)
	{
		if (SerialKeyPressed((uint8_t*)&key)) break;
	}
	return key;

}

/**
* @brief  Print a character on the HyperTerminal
* @param c: The character to be printed
* @retval : None
*/
void SerialPutChar(uint8_t c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/*
void SerialPutChar2(uint8_t c)
{
	USART_SendData(USART3, c);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}
*/

/**
* @brief  Print a string on the HyperTerminal
* @param s: The string to be printed
* @retval : None
*/
void Serial_PutString(uint8_t *s)
{
	while (*s != '\0')
	{
		SerialPutChar(*s);
		s ++;
	}
}

/**
* @brief  Get Input string from the HyperTerminal
* @param buffP: The input string
* @retval : None
*/
void GetInputString (uint8_t * buffP)
{
	uint32_t bytes_read = 0;
	uint8_t c = 0;
	do
	{
		c = GetKey();
		if (c == '\r')
		break;
		if (c == '\b') 		/* Backspace */
		{
			if (bytes_read > 0)
			{
				SerialPutString("\b \b");
				bytes_read --;
			}
			continue;
		}
		if (bytes_read >= CMD_STRING_SIZE )
		{
			SerialPutString("Command string size overflow\r\n");
			bytes_read = 0;
			continue;
		}
		if (c >= 0x20 && c <= 0x7E)
		{
			buffP[bytes_read++] = c;
			SerialPutChar(c);
		}
	}
	while (1);
	SerialPutString(("\n\r"));
	buffP[bytes_read] = '\0';
}

/**
* @brief  Calculate the number of pages
* @param Size: The image size
* @retval : The number of pages
*/
uint32_t FLASH_PagesMask(__IO uint32_t Size)
{
	uint32_t pagenumber = 0x0;
	uint32_t size = Size;

	if ((size % PAGE_SIZE) != 0)
	{
		pagenumber = (size / PAGE_SIZE) + 1;
	}
	else
	{
		pagenumber = size / PAGE_SIZE;
	}
	return pagenumber;
}

/**
* @brief  Disable the write protection of desired pages
* @param  None
* @retval : None
*/
void FLASH_DisableWriteProtectionPages(void)
{
	uint32_t useroptionbyte = 0, WRPR = 0;
	uint16_t var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
	FLASH_Status status = FLASH_BUSY;

	WRPR = FLASH_GetWriteProtectionOptionByte();

	/* Test if user memory is write protected */
	if ((WRPR & UserMemoryMask) != UserMemoryMask)
	{
		useroptionbyte = FLASH_GetUserOptionByte();

		UserMemoryMask |= WRPR;

		status = FLASH_EraseOptionBytes();

		if (UserMemoryMask != 0xFFFFFFFF)
		{
			status = FLASH_EnableWriteProtection((uint32_t)~UserMemoryMask);
		}

		/* Test if user Option Bytes are programmed */
		if ((useroptionbyte & 0x07) != 0x07)
		{   			/* Restore user Option Bytes */
			if ((useroptionbyte & 0x01) == 0x0)
			{
				var1 = OB_IWDG_HW;
			}
			if ((useroptionbyte & 0x02) == 0x0)
			{
				var2 = OB_STOP_RST;
			}
			if ((useroptionbyte & 0x04) == 0x0)
			{
				var3 = OB_STDBY_RST;
			}

			FLASH_UserOptionByteConfig(var1, var2, var3);
		}

		if (status == FLASH_COMPLETE)
		{
			SerialPutString("Write Protection disabled...\r\n");

			SerialPutString("...and a System Reset will be generated to re-load the new option bytes\r\n");

			/* Enable WWDG clock */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

			/* Generate a system Reset to re-load the new option bytes: enable WWDG and set
			counter value to 0x4F, as T6 bit is cleared this will generate a WWDG reset */
			WWDG_Enable(0x4F);
		}
		else
		{
			SerialPutString("Error: Flash write unprotection failed...\r\n");
		}
	}
	else
	{
		SerialPutString("Flash memory not write protected\r\n");
	}
}

void DispHighLightRow (unsigned int row)
{
	kmy_LcdDrawRect (0, ALineContainDotLine * row, 131, ALineContainDotLine * row + 11, mode_Reverse);
}


char GetSelectItemOfMenus (char (*Menus)[MENU_COL], MENUVAR *MenuVar,char MaxItems,char *SelectItemIndex) //0 <= MenuVar->CurMenuIndex[0] <MaxItems
{
    #define _4RowOneScr 5
    #define KEY_FLASHINTERFACE 0x55
	char tmp[4]={0};
	char tmpbuff[20]={0};
	char TempKey = KEY_NONE;

	MenuVar->FlashFlag = 0x01;
	if (MenuVar->CurMenuIndex >= _4RowOneScr)
	{
		MenuVar->MenuScrollCount = (MenuVar->CurMenuIndex + 1) - _4RowOneScr;
		MenuVar->CurHighLightRow = _4RowOneScr - 1;
		MenuVar->OldHighLightRow = _4RowOneScr - 1;
	}
	else
	{
		MenuVar->CurHighLightRow = MenuVar->CurMenuIndex;
		MenuVar->OldHighLightRow = MenuVar->CurMenuIndex;
	}

	PostKey (KEY_FLASHINTERFACE);

	while (1)
	{
		TempKey = kmy_GetKeyValue();

		if (TempKey == KEY_NONE) continue;

		switch (TempKey)
		{
		    case KEY1:
			    MenuVar->MenuLayerCnt=1;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY2:
				MenuVar->MenuLayerCnt=2;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY3:
				MenuVar->MenuLayerCnt=3;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY4:
				MenuVar->MenuLayerCnt=4;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY5:
				MenuVar->MenuLayerCnt=5;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY6:
				MenuVar->MenuLayerCnt=6;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY7:
				MenuVar->MenuLayerCnt=7;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY8:
				MenuVar->MenuLayerCnt=8;
				PostKey (KEY_FLASHINTERFACE);
			    break;
			case KEY9:
				MenuVar->MenuLayerCnt=9;
				PostKey (KEY_FLASHINTERFACE);
			    break;
					
			case KEY_UP:
				
				if ( (MenuVar->CurHighLightRow == 0) && (MenuVar->MenuScrollCount - 1 >= 0) ) //
				{
					MenuVar->MenuScrollCount --;
					MenuVar->FlashFlag |= 0x01;//0x01---需要刷新整个界面
				}

				if (MenuVar->CurMenuIndex - 1 >= 0)
				{
					if (MenuVar->CurHighLightRow - 1 >= 0) MenuVar->CurHighLightRow --;

					MenuVar->CurMenuIndex --;
					MenuVar->FlashFlag |= 0x02;//0x02---需要刷新高亮行
				}
				else
				{
				    MenuVar->CurMenuIndex=MaxItems-1;
					if (MenuVar->CurMenuIndex >= _4RowOneScr)
					{
						MenuVar->MenuScrollCount =(MenuVar->CurMenuIndex + 1) - _4RowOneScr;					    
		                MenuVar->OldHighLightRow = MenuVar->CurHighLightRow=_4RowOneScr - 1;
						MenuVar->FlashFlag |= 0x01;//0x01---需要刷新整个界面
					}
					else
					{
					    MenuVar->MenuScrollCount=0;
						MenuVar->CurHighLightRow = MenuVar->CurMenuIndex;
		                MenuVar->OldHighLightRow = MenuVar->CurMenuIndex;	
						MenuVar->FlashFlag |= 0x01;//0x01---需要刷新整个界面
					}					
				}
				PostKey (KEY_FLASHINTERFACE);
				break;

			case KEY_DOWN:
				
				if ( (MenuVar->CurHighLightRow >= (_4RowOneScr - 1) ) && (MenuVar->MenuScrollCount + 1 <= (MaxItems - _4RowOneScr) ) )
				{
					MenuVar->MenuScrollCount ++;
					MenuVar->FlashFlag |= 0x01;//0x01---需要刷新整个界面
				}

				if (MenuVar->CurMenuIndex + 1 < MaxItems)
				{
					if (MenuVar->CurHighLightRow + 1 < _4RowOneScr) MenuVar->CurHighLightRow ++;

					MenuVar->CurMenuIndex ++;
					MenuVar->FlashFlag |= 0x02;//0x02---需要刷新高亮行
				}
				else
				{
					MenuVar->MenuScrollCount=0;
					MenuVar->FlashFlag |= 0x01;//0x01---需要刷新整个界面
					MenuVar->CurMenuIndex=0;
		            MenuVar->OldHighLightRow = MenuVar->CurHighLightRow=0;
				}
				PostKey (KEY_FLASHINTERFACE);
				break;

			case KEY_Enter://enter
				
				*SelectItemIndex = MenuVar->CurMenuIndex;
				return KEY_Enter;

			case KEY_ReturnInitInterface://exit
			
				return KEY_ReturnInitInterface;

			case KEY_ReturnPrevious://return
				
				return KEY_ReturnPrevious;

			case KEY_FLASHINTERFACE:
				if(MenuVar->MenuLayerCnt!=0)
				{
				    if(MenuVar->MenuLayerCnt>MaxItems)break;
					MenuVar->CurMenuIndex=MenuVar->MenuLayerCnt-1;
					MenuVar->MenuLayerCnt=0;
					if (MenuVar->CurMenuIndex >= _4RowOneScr)
                	{
                		MenuVar->MenuScrollCount = (MenuVar->CurMenuIndex + 1) - _4RowOneScr;
                		MenuVar->CurHighLightRow = _4RowOneScr - 1;
                		MenuVar->OldHighLightRow = _4RowOneScr - 1;
                	}
                	else
                	{
                	    MenuVar->MenuScrollCount=0;
                		MenuVar->CurHighLightRow = MenuVar->CurMenuIndex;
                		MenuVar->OldHighLightRow = MenuVar->CurMenuIndex;
                	}
					MenuVar->FlashFlag |= 0x01;//0x01---需要刷新整个界面
					
				    *SelectItemIndex = MenuVar->CurMenuIndex;
					return KEY_Enter;
				}
				
				if ( (MenuVar->FlashFlag & 0x01) == 0x01) //0x01---需要刷新整个界面
				{
					kmy_LcdClear();

					if (MaxItems >= 1)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar->MenuScrollCount+1);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[0 + MenuVar->MenuScrollCount]);
						text_out_to_row(0,(unsigned char *)tmpbuff);
					//	text_out_to_row (0, (const unsigned char *)Menus[0 + MenuVar->MenuScrollCount]);
					}
//					printf("MaxItems=%d,MenuScrollCount=%d,%s\r\n",MaxItems,MenuVar->MenuScrollCount,*Menus[0]);

					if (MaxItems >= 2)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar->MenuScrollCount+2);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[1 + MenuVar->MenuScrollCount]);
						text_out_to_row(1,(unsigned char *)tmpbuff);
						//text_out_to_row (1, (const unsigned char *)Menus[1 + MenuVar->MenuScrollCount]);
					}
//					printf("MaxItems=%d,MenuScrollCount=%d,%s\r\n",MaxItems,MenuVar->MenuScrollCount,*Menus[1]);
					if (MaxItems >= 3)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar->MenuScrollCount+3);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[2 + MenuVar->MenuScrollCount]);
						text_out_to_row(2,(unsigned char *)tmpbuff);
						//text_out_to_row (2, (const unsigned char *)Menus[2 + MenuVar->MenuScrollCount]);
					}

					if (MaxItems >= 4)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar->MenuScrollCount+4);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[3 + MenuVar->MenuScrollCount]);
						text_out_to_row(3,(unsigned char *)tmpbuff);
						//text_out_to_row (3, (const unsigned char *)Menus[3 + MenuVar->MenuScrollCount]);
					}

					if (MaxItems >= 5)
					{
						memset(tmp,0,sizeof(tmp));						
						memset(tmpbuff,0,sizeof(tmpbuff));
						sprintf(tmp,"%d.",MenuVar->MenuScrollCount+5);
						strcpy(tmpbuff,tmp);
						strcat(tmpbuff,Menus[4 + MenuVar->MenuScrollCount]);
						text_out_to_row(4,(unsigned char *)tmpbuff);
						//text_out_to_row (4, (const unsigned char *)Menus[4 + MenuVar->MenuScrollCount]);
					}

					DispHighLightRow (MenuVar->CurHighLightRow + 0); //高亮
					MenuVar->FlashFlag &= ~0x01;
				}

				if ( (MenuVar->OldHighLightRow != MenuVar->CurHighLightRow) && ( (MenuVar->FlashFlag & 0x02) == 0x02) ) //0x02---需要刷新高亮行
				{
					DispHighLightRow (MenuVar->OldHighLightRow + 0); //取消高亮
					DispHighLightRow (MenuVar->CurHighLightRow + 0); //高亮
					MenuVar->OldHighLightRow = MenuVar->CurHighLightRow;
					MenuVar->FlashFlag &= ~0x02;
				}

				break;

			default:
				
				break;
		}

	}
}

extern char NetworkSwitch[2];

char NetworkSet_WiFi (void)
{
    kmy_LcdClear();
	DrawTitle_to_row(2,"Please Wait...");	
	memset (NetworkSwitch, 0, sizeof (NetworkSwitch) );
	NetworkSwitch[0] = WiFi;
	kmy_GprsOnOff(0);
	kmy_WiFi_USARTInit(9600);
    kmy_SPIInit();
    kmy_WiFiOnOff (1);
	sleep(10);
	kmy_WiFi_USARTReceiveReset();
    printf("WiFi Mode\r\n");
	kmy_LcdClear();
	DrawTitle_to_row(2,"Change OK");
	sleep(1);
	return 0;
}

char NetworkSet_GPRS (void)
{
	kmy_LcdClear();
	DrawTitle_to_row(2,"Please Wait...");	
	memset (NetworkSwitch, 0, sizeof (NetworkSwitch) );
	NetworkSwitch[0] = GPRS;
	kmy_WiFiOnOff (0);
	kmy_GprsInit();
    printf("GPRS Mode\r\n");
	kmy_LcdClear();
	DrawTitle_to_row(2,"Change OK");
	sleep(1);
	return 0;
}


/**
* @brief  Display the Main Menu on to HyperTerminal
* @param  None
* @retval : None
*/
void Main_Menu(void)
{
    MENUVAR MenuVar1 = {0, 0, 0, 0, 0, 0x01};
	MENUVAR MenuVar2 = {0, 0, 0, 0, 0, 0x01};
	MENUVAR MenuVar3 = {0, 0, 0, 0, 0, 0x01};
	char Menulist[Menu_LIMIT][MENU_COL]={0};
	char retval=0;
	char index;	
	
	/* Get the number of block (4 or 2 pages) from where the user program will be loaded */
	BlockNbr = (FlashDestination - 0x08000000) >> 12;

	/* Compute the mask to test if the Flash memory, where the user program will be
	loaded, is write protected */
	#ifdef USE_STM3210B_EVAL
	UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
	#else 	/* USE_STM3210E_EVAL */
	if (BlockNbr < 62)
	{
		UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
	}
	else
	{
		UserMemoryMask = ((uint32_t)0x80000000);
	}
	#endif 	/* USE_STM3210B_EVAL */


	/* Test if any page of Flash memory where program user will be loaded is write protected */
	if ((FLASH_GetWriteProtectionOptionByte() & UserMemoryMask) != UserMemoryMask)
	{
		FlashProtection = TRUE;
		/* Disable the write protection of desired pages */
		FLASH_DisableWriteProtectionPages();
	}
	else
	{
		FlashProtection = FALSE;
	}
	while(1)
	{
		if((Upgrade_Flag[0]=='1')&&(key_flag==0))
		{
			while(1)
			{
				retval = GPRSmenu();
				if(retval==KEY_ReturnPrevious)
				{
					if(Upgrade_Flag[0]=='1')
					{
						Upgrade_Flag[0]='0';
						WriteEepromVariable();
					}
					break;
				}
			}
		}		
    	strcpy(Menulist[0],"Serial1 Upgrade");
    	strcpy(Menulist[1],"Upgrade Set");
    	strcpy(Menulist[2],"Start Upgrade");
    	strcpy(Menulist[3],"Quit Upgrade");
    	retval=GetSelectItemOfMenus(Menulist,&MenuVar1,4,&index);


		if(retval == KEY_Enter)
		{
			if(index==0)
			{
				kmy_LcdClear();
				kmy_LcdTextOut(10,0,"Serial1 Upgrade");
				SerialDownload();
			}
			else if(index==1)
			{	
			    while(1)
			    {
			        strcpy(Menulist[0],"Network set");
    				strcpy(Menulist[1],"UpgradeURL Set");
                    strcpy(Menulist[2],"UpgradeIPandPort");
                    strcpy(Menulist[3],"APN Set");
                    strcpy(Menulist[4],"ApnUsername Set");
                    strcpy(Menulist[5],"ApnPassword Set");
					
                 	retval=GetSelectItemOfMenus(Menulist,&MenuVar2,6,&index);
					if(retval==KEY_ReturnPrevious)break;
    				if(retval==KEY_Enter)
    				{
    					switch(index)
    					{
    					    case 0:
								while(1)
								{
									strcpy(Menulist[0],"GPRS");
                                   	strcpy(Menulist[1],"WiFi");
									
                                    retval=GetSelectItemOfMenus(Menulist,&MenuVar3,2,&index);
									if(retval==KEY_ReturnPrevious)break;
									if(retval==KEY_Enter)
                                    { 
                                     	switch(index)
                                     	{
                                     		case 0:
                                     			NetworkSet_GPRS();
                                     			break;

											case 1:
                                     			NetworkSet_WiFi();
                                     			break;

											default: 
												break;		
                                     	}
                                    }
									if(retval==KEY_ReturnInitInterface)break;
								}
    							break;
    						case 1:
    							retval=InputUpgradeURL();
    							break;
    						case 2:
    							retval=InputUpgradeIPandPort();
    							break;	
    						case 3:
    							retval=InputUpgradeAPN();
    							break;
    						case 4:
    							retval=InputUpgradeApnUsername();
    							break;
    						case 5:
    							retval=InputUpgradeApnPassword();
    							break;
    						default:
    			                break;
    					}						
    				}
					if(retval==KEY_ReturnInitInterface)break;
			    }
			}
			else if(index==2)
			{
			    while(1)
				{
					retval = GPRSmenu();
					if(retval==KEY_ReturnPrevious)
					{
						if(Upgrade_Flag[0]=='1')
						{
							Upgrade_Flag[0]='0';
							WriteEepromVariable();
						}
						break;
					}
				}
			}
			else 
			{
				if(Upgrade_Flag[0]=='1')
				{
    				Upgrade_Flag[0]='0';
    				WriteEepromVariable();				
				}
				kmy_LcdBlackLightOn();
				kmy_LcdClear();
				DrawTitle_to_row(2,"Shutting down...");  
				msleep(2000);	
				kmy_VCC3Off();
				kmy_VCC5Off();
				kmy_PowerOff(); 
				kmy_LcdClear();
				while(1);
			}
		}
	}
}







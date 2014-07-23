#include "Input.h"
#include "kmy_KeyDrv.h"
#include "kmy_USART3Drv.h"
#include "kmy_Time.h"
#include "stm32f10x_spi.h"
#include "kmy_FlashDrv.h"
#include "kmy_LCDDrv.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MaxRowOneScr 		5		//the total rows of one screen
#define EditAreaTotalRow	2		//edit area total rows
#define EditAreaStartLine	1		//edit area start row, 1  represents the second row
//#define ALineNum			18		//edit area a row of the numbers of asccii byte.
#define ALineNum			20		//edit area a row of the numbers of asccii byte.
#define MaxByteOneLine 		23		//a row total bytes
#define ALinePixNum			131		//a row total pixels
#define AAscciiPixNum		6		//a Asccii  cod contain pixels
#define AGB2312PixNum		12		//a chinese character contain pixels
#define ALineContainDotLine	13		//a line contian How many dot line
static short cursorX;
static short cursorY;

void LcdWriteRowX (unsigned char row, unsigned char x, const unsigned char *buff)
{
	kmy_LcdTextOut (x, ALineContainDotLine * row, buff);
	//kmy_LcdTextOut(ALinePixNum-AAscciiPixNum,ALineContainDotLine*row," ");
}


void DrawTitle_to_row(unsigned char row,const unsigned char *str)//绘制标题
{
	unsigned short Len;

	Len=(strlen((const char *)str))*6;
	if(Len>132)Len=132;
	LcdWriteRowX(row,(132-Len)/2,str);
}


void Reverse_area(int xpos,int ypos,int width,int height)
{
	kmy_LcdDrawRect(xpos,ypos,xpos+width-1,ypos+height-1,mode_Reverse);
}


//clear_area_to_row
void LcdClearRow (char startrow, char endrow)
{
	kmy_LcdDrawRect (0, ALineContainDotLine * startrow, 132, ALineContainDotLine * (endrow + 1), mode_White);
}


void clear_area_to_row (unsigned int startrow, unsigned int endrow)
{
	kmy_LcdDrawRect (0, ALineContainDotLine * startrow, 132, ALineContainDotLine * (endrow + 1), mode_White);
}
void text_out_to_row_x (unsigned char row, unsigned char x, const unsigned char *buff)
{
	kmy_LcdTextOut (x, ALineContainDotLine * row, buff);
	//text_out(ALinePixNum-AAscciiPixNum,ALineContainDotLine*row," ");
}
void text_out_to_row (unsigned char row, const unsigned char *buff)
{
	kmy_LcdTextOut (0, ALineContainDotLine * row, buff);
	kmy_LcdTextOut (ALinePixNum - AAscciiPixNum, ALineContainDotLine * row, " ");
}

void reverse_area_to_row (unsigned int xpos, unsigned int rows, unsigned int width, unsigned int height)
{
	kmy_LcdDrawRect (xpos, ALineContainDotLine * rows, xpos + width - 1, ALineContainDotLine * rows + height, mode_Reverse);
}
static unsigned char *GetSrfString (unsigned char currentSrf)
{
	if (currentSrf == SRF_123) return "123 ";
	else if (currentSrf == SRF_abc) return "abc ";
	else if (currentSrf == SRF_ABC) return "ABC ";
	else return "abc ";
}

static void DisplaySrfAndInput (unsigned char currentSrf)
{
	clear_area_to_row (MaxRowOneScr - 1, MaxRowOneScr - 1);
	text_out_to_row_x (MaxRowOneScr - 1, 0, GetSrfString (currentSrf) );
	reverse_area_to_row (0, MaxRowOneScr - 1, 25, 16);
	text_out_to_row_x (MaxRowOneScr - 1, 60, "input...");
}
void flashcursor (signed int x, signed int y)
{
	cursorX = x;
	cursorY = y;
}

static void flashguangbiao_to_row (unsigned char x, unsigned char row)
{
	flashcursor (x, 13 * row);
}
unsigned int my_GetCurrentMs (void)
{
	return kmy_TimeGetMilliseconds();
}
void reverse_area (int xpos, int ypos, int width, int height)
{
	kmy_LcdDrawRect (xpos, ypos, xpos + width - 1, ypos + height - 1, mode_Reverse);
}

static void flightcursor (void)
{
	static unsigned int PreviousMs;
	unsigned int CurrentMs;

	CurrentMs = my_GetCurrentMs();

	if (CurrentMs > PreviousMs && CurrentMs - PreviousMs >= 500)
	{
		PreviousMs = CurrentMs;
		reverse_area (cursorX, cursorY, 1, 12);
	}
}
void PostKey (unsigned char key)
{
	kmy_PostKey (key);
}
void KeyEventPost (void)
{
	kmy_LcdBlackLightOn();
	kmy_AutoOffLcdLight (120);
	kmy_AutoPostKey (120);
}
static void CalculateCursorOffset (char srf)
{
	signed int x = cursorX, y = cursorY;

	x += 6;

	if (x >= 6 * ALineNum)
	{
		x = 0;
		y *= 13;

		if (y >= (EditAreaTotalRow + 1) * 13) y = EditAreaTotalRow * 13;
	}

#ifdef DEBUG_PRINTF
	printf ("cursorX=%d,cursorY=%d\n", x, y);
#endif

	flashcursor (x, y);
}
static void srf_strcat_insert (unsigned char *pMove, const unsigned char *PyCode)
{
	unsigned char insertlen;
	unsigned int pMovelen;

	const unsigned char *psrc;
	unsigned char *pdes;

	insertlen = strlen ( (const char*) PyCode);
	pMovelen = strlen ( (const char*) pMove);

	if (pMovelen)
	{
		psrc = pMove + pMovelen - 1;
		pdes = pMove + pMovelen + insertlen - 1;

		for (; pMovelen != 0; pMovelen--)
		{
			*pdes = *psrc;
			psrc--;
			pdes--;
		}
	}

	memcpy (pMove, PyCode, insertlen);
}
static void my_AccordPointInsertEdit (const unsigned char *TheFirstAddrOfBuff, const unsigned char *pMove, char StartLine, char EndLine, unsigned char *cursorx, unsigned char *cursory)
{
	unsigned char totalline;	//编辑区有几行
	int bytenum;	//从buff  头到光标处有多少个字符
	unsigned char byteline;	//从buff  头到光标处需要多少个行显示
	//int bytetotal;	//buff  总共有多少个字符
	unsigned char i, j;
	const unsigned char *display;
	unsigned char disp[23];

	*cursorx = 0;
	*cursory = 0;

	totalline = EndLine - StartLine + 1;
	bytenum = pMove - TheFirstAddrOfBuff;
	//bytetotal=strlen((char const*)TheFirstAddrOfBuff);

	byteline = bytenum / ALineNum;

	if (bytenum % ALineNum) byteline++;

	display = TheFirstAddrOfBuff;	//找出需要显示的地址

	if (byteline > totalline)
	{
		for (i = 0; i < byteline - totalline; i++)
		{
			for (j = 0; j < ALineNum; j++, display++)
			{
				if (*display > 0x80)
				{
					display++;
					j++;
				}
			}
		}
	}

#ifdef DEBUG_PRINTF
	printf ("display=%s\n", display);
#endif

	if (*display != 0)  		//如果有字符
	{
		for (i = 0; i < totalline; i++)  			//显示pMove  之前之后能在" 显示范围之内"  的字符
		{
			for (j = 0; j < ALineNum && *display != 0;)
			{
				if (*display > 0x80)
				{
					disp[j] = *display;
					j++;
					display++;

					if (display == pMove)
					{
						*cursory = i;
						*cursorx = j;
					}
				}

				disp[j] = *display;
				j++;
				display++;

				if (display == pMove)
				{
					*cursory = i;
					*cursorx = j;
				}
			}

			for (; j < ALineNum + 2; j++) disp[j] = ' ';			//此处ALineNum+1  为了去除删除字符是，最后可能显示半个汉字的问题

			disp[j] = 0;
#ifdef DEBUG_PRINTF
			printf ("disline=%s\n", disp);
#endif
			text_out_to_row (StartLine + i,  disp);
		}
	}
	else 		//如果没字符，则有可能是删除完了，则屏幕会剩一个字符
	{
		text_out_to_row (StartLine,"   ");
	}

#ifdef DEBUG_PRINTF
	printf ("bytenum=%d\n", bytenum);
#endif

	*cursorx *= 6;
	*cursory += StartLine;
	*cursory *= 13;
	flashcursor (*cursorx, *cursory);
}

//static unsigned char abcABCflag = 0;
static unsigned char ABCReadCode (const unsigned char *BuffTheFirstAddr, unsigned char *pMove)
{
	unsigned char prekey = 0, curkey = 0, keyflag;
	unsigned char curpos = 0;
	unsigned char x, y;
	unsigned char temp[2] = {0, 0};
	unsigned int PreviousMs, CurrentMs;
	unsigned char keyval;
	unsigned char flag = 1;

	const char *ABCcode[10] =
	{
		" 0",
		",.:%1@/?;\'\"|\\{[}]=+-_)(*&^$#!~`<>",
		"ABC2",
		"DEF3",
		"GHI4",
		"JKL5",
		"MNO6",
		"PQRS7",
		"TUV8",
		"WXYZ9"
	};

	CalculateCursorOffset (SRF_ABC);	//不管怎样，进入到这里必定会返回一个asccii  码
	x = pMove - BuffTheFirstAddr;
#ifdef DEBUG_PRINTF
	printf ("len=%d\n", x);
#endif
	PreviousMs = CurrentMs = my_GetCurrentMs();

	while (1)
	{
		CurrentMs = my_GetCurrentMs();

		if (CurrentMs - PreviousMs >= 500)
		{
			goto tuichu;
		}

		keyval = kmy_GetKeyValue();

		if (keyval == 0) continue;

		PreviousMs = CurrentMs;

		if (flag == 0) KeyEventPost();
		else flag = 0;

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				prekey = curkey;
				curkey = '0';
				break;

			case KEY1:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY2:
				keyflag = 1;
				prekey = curkey;
				curkey = '2';
				break;

			case KEY3:
				keyflag = 1;
				prekey = curkey;
				curkey = '3';
				break;

			case KEY4:
				keyflag = 1;
				prekey = curkey;
				curkey = '4';
				break;

			case KEY5:
				keyflag = 1;
				prekey = curkey;
				curkey = '5';
				break;

			case KEY6:
				keyflag = 1;
				prekey = curkey;
				curkey = '6';
				break;

			case KEY7:
				keyflag = 1;
				prekey = curkey;
				curkey = '7';
				break;

			case KEY8:
				keyflag = 1;
				prekey = curkey;
				curkey = '8';
				break;

			case KEY9:
				keyflag = 1;
				prekey = curkey;
				curkey = '9';
				break;

			case KEYXING:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY_Enter:
				goto tuichu;

			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1)
		{
			if (prekey != 0 && prekey != curkey)
			{
				PostKey (keyval);
				//abcABCflag = 1;
				return KEY_NONE;
			}
			else
			{
				temp[0] = * (ABCcode[curkey - '0'] + curpos);
				curpos++;				//"abc2" 之类的轮流的位置

				if (prekey == 0)
				{
					srf_strcat_insert ( (unsigned char*) pMove - 1, (unsigned char*) temp);
				}
				else
				{
					if (curpos > strlen (ABCcode[curkey - '0']) - 1) curpos = 0;

					memcpy (pMove - 1, temp, 1);
				}

				my_AccordPointInsertEdit (BuffTheFirstAddr, pMove, 1, 2, &x, &y);
				x -= 6;
#ifdef DEBUG_PRINTF
				printf ("x=%d  y=%d\n", x, y);
#endif

				if (x >= ALineNum * 6)
				{
					x = 0;

					if (y < (EditAreaTotalRow + EditAreaStartLine - 1) * 13) y += 13;
				}

				reverse_area (x, y, 7, 12);				//反显光标处字符
			}
		}

#ifdef DEBUG_PRINTF
		printf ("curkey=%c\n", curkey);
#endif

	}

tuichu:
	reverse_area (x, y, 7, 11);

	return KEY_NONE;
}

static unsigned char abcReadCode (const unsigned char *BuffTheFirstAddr, unsigned char *pMove)
{
	unsigned char prekey = 0, curkey = 0, keyflag;
	unsigned char curpos = 0;
	unsigned char x, y;
	unsigned char temp[2] = {0, 0};
	unsigned int PreviousMs, CurrentMs;
	unsigned char keyval;
	unsigned char flag = 1;

	const char *ABCcode[10] =
	{
		" 0",
		",.:%1@/?;\'\"|\\{[}]=+-_)(*&^$#!~`<>",
		"abc2",
		"def3",
		"ghi4",
		"jkl5",
		"mno6",
		"pqrs7",
		"tuv8",
		"wxyz9"
	};

	CalculateCursorOffset (SRF_abc);	//不管怎样，进入到这里必定会返回一个asccii  码
	x = pMove - BuffTheFirstAddr;

	PreviousMs = CurrentMs = my_GetCurrentMs();

	while (1)
	{
		CurrentMs = my_GetCurrentMs();

		if (CurrentMs - PreviousMs >= 500)
		{
			goto tuichu;
		}

		keyval = kmy_GetKeyValue();

		if (keyval == 0) continue;

		PreviousMs = CurrentMs;

		if (flag == 0) KeyEventPost();
		else flag = 0;

		keyflag = 0;

		switch (keyval)
		{
			case KEY0:
				keyflag = 1;
				prekey = curkey;
				curkey = '0';
				break;

			case KEY1:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY2:
				keyflag = 1;
				prekey = curkey;
				curkey = '2';
				break;

			case KEY3:
				keyflag = 1;
				prekey = curkey;
				curkey = '3';
				break;

			case KEY4:
				keyflag = 1;
				prekey = curkey;
				curkey = '4';
				break;

			case KEY5:
				keyflag = 1;
				prekey = curkey;
				curkey = '5';
				break;

			case KEY6:
				keyflag = 1;
				prekey = curkey;
				curkey = '6';
				break;

			case KEY7:
				keyflag = 1;
				prekey = curkey;
				curkey = '7';
				break;

			case KEY8:
				keyflag = 1;
				prekey = curkey;
				curkey = '8';
				break;

			case KEY9:
				keyflag = 1;
				prekey = curkey;
				curkey = '9';
				break;

			case KEYXING:
				keyflag = 1;
				prekey = curkey;
				curkey = '1';
				break;

			case KEY_Enter:
				goto tuichu;

			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1)
		{
			if (prekey != 0 && prekey != curkey)
			{
				PostKey (keyval);
				//abcABCflag = 1;
				return KEY_NONE;
			}
			else
			{
				temp[0] = * (ABCcode[curkey - '0'] + curpos);
				curpos++;				//"abc2" 之类的轮流的位置

				if (prekey == 0)
				{
					srf_strcat_insert (pMove - 1, (unsigned char*) temp);
				}
				else
				{
					if (curpos > strlen (ABCcode[curkey - '0']) - 1) curpos = 0;

					memcpy (pMove - 1, temp, 1);
				}

				my_AccordPointInsertEdit (BuffTheFirstAddr, pMove, 1, 2, &x, &y);
				x -= 6;
#ifdef DEBUG_PRINTF
				printf ("x=%d  y=%d\n", x, y);
#endif

				if (x >= ALineNum * 6)
				{
					x = 0;

					if (y < (EditAreaTotalRow + EditAreaStartLine - 1) * 13) y += 13;
				}

				reverse_area (x, y, 7, 12);				//反显光标处字符
			}
		}

#ifdef DEBUG_PRINTF
		printf ("curkey=%c\n", curkey);
#endif

	}

tuichu:
	reverse_area (x, y, 7, 11);

	return KEY_NONE;
}
static unsigned char _123ReadCode (unsigned char PyCode[3], unsigned char keyval)
{
	memset (PyCode, 0, 3);

	switch (keyval)
	{
		case KEY0:
			PyCode[0] = '0';
			break;

		case KEY1:
			PyCode[0] = '1';
			break;

		case KEY2:
			PyCode[0] = '2';
			break;

		case KEY3:
			PyCode[0] = '3';
			break;

		case KEY4:
			PyCode[0] = '4';
			break;

		case KEY5:
			PyCode[0] = '5';
			break;

		case KEY6:
			PyCode[0] = '6';
			break;

		case KEY7:
			PyCode[0] = '7';
			break;

		case KEY8:
			PyCode[0] = '8';
			break;

		case KEY9:
			PyCode[0] = '9';
			break;

		case KEYXING:
			PyCode[0] = '.';
			break;
	}

	if ( (PyCode[0] >= '0' && PyCode[0] <= '9') || PyCode[0] == '.')
	{
		CalculateCursorOffset (SRF_123);
		return KEY_Enter;
	}
	else return KEY_ReturnInitInterface;
}
/*
例如:
char ch[30]={"abcdefghijklmnopqrstuvwxyz"};
my_strcpy_DeleteTailByte(ch+5,ch+8);
则ch[30]={"abcdeijklmnopqrstuvwxyz\0\0\0"};
*/
void my_strcpy_DeleteTailByte (unsigned char *src, const unsigned char *des)
{
	unsigned char len;

	len = des - src;

	while (*des != 0)
	{
		*src = *des;
		src++;
		des++;
	}

	for (; len != 0; len--)
	{
		*src = 0;
		src++;
	}
}

static unsigned char SwitchSrf (unsigned char VolidSrf, unsigned char currentSrf)
{
	if (currentSrf == SRF_123)
	{
		if (VolidSrf & SRF_abc) return SRF_abc;
		else if (VolidSrf & SRF_ABC) return SRF_ABC;
		else return SRF_123;
	}
	else if (currentSrf == SRF_abc)
	{
		if (VolidSrf & SRF_ABC) return SRF_ABC;
		else if (VolidSrf & SRF_123) return SRF_123;
		else return SRF_abc;
	}
	else if (currentSrf == SRF_ABC)
	{

		if (VolidSrf & SRF_123) return SRF_123;
		else if (VolidSrf & SRF_abc) return SRF_abc;
		else return SRF_ABC;
	}
	else
	{
		return SRF_abc;
	}
}

char GetString_abcABC123PYWuBi (unsigned char srfFlag, unsigned char DefaultSrf, unsigned char *receive, unsigned char recSize, unsigned char HaveoldFlag)
{
	int x;
	unsigned char keyflag;
	unsigned char cursorx, cursory;
	unsigned char inputCode[3];
	unsigned char i = 0;
	unsigned char *pMove;
	unsigned char ChineseThinkFlag = 0;
	unsigned char keyval;
	unsigned char currentSrf;

	currentSrf = DefaultSrf;
	DisplaySrfAndInput (currentSrf);

	if (HaveoldFlag)
	{
		x = strlen (  (char*)receive);
		//text_out_to_row(1,receive);
		memset (receive + x, 0, recSize - x);		//洗净传进来的receive  缓冲区无用的尾部数据。
	}
	else
	{
		x = 0;
		memset (receive, 0, recSize);
	}

	flashguangbiao_to_row (0, EditAreaStartLine);
	pMove = receive + x;
	PostKey (KEY_pseudo);

	while (1)
	{
		flightcursor();
		keyval = kmy_GetKeyValue();

		if (keyval == KEY_NONE) continue;

		if (keyval != KEY_pseudo)
		{
			//KeyEventPost();
			

			kmy_LcdBlackLightOn();
			kmy_AutoOffLcdLight (120);
			kmy_AutoPostKey (120);
		}

		keyflag = KEY_NONE;

		switch (keyval)
		{
			case KEY0:
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
			case KEYXING:
				if (ChineseThinkFlag == 0)
				{
	loop:

					if (strlen ( (char*) receive) < recSize - 1)  		//此处recSize  为字符长度限制
					{
						if (currentSrf == SRF_abc)  				//abc
						{
							pMove++;
							PostKey (keyval);
							keyval = abcReadCode (receive, pMove);

							if (keyval != KEY_NONE && keyval != KEYJING)
							{
								goto loop;
							}
						}
						else if (currentSrf == SRF_ABC)  				//ABC
						{
							pMove++;
							PostKey (keyval);
							keyval = ABCReadCode (receive, pMove);

							if (keyval != KEY_NONE && keyval != KEYJING)
							{
								goto loop;
							}
						}
						else if (currentSrf == SRF_123)  				//123
						{
							keyval = _123ReadCode (inputCode, keyval);
						}

						if (keyval == KEY_Enter)
						{
							srf_strcat_insert (pMove, inputCode);

							if (*pMove > 0x80) pMove += 2;
							else pMove++;

							keyflag = 2;
						}
						else if (keyval == KEY_DEL)
						{
							keyflag = 2;
						}
						else if (keyval == KEYJING)
						{
							keyflag = 1;
						}
					}
				}

				break;

			case KEYJING:	//#切换输入法
				keyflag = 1;
				break;

			case KEY_Enter:	//确认返回
				//if(strlen((sc8*)receive)!=0){	//如果输入字符为空，不让按确认键返回
				return KEY_Enter;
				//}
				//break;

			case KEY_UP:			//光标上移
				if (ChineseThinkFlag == 0)
				{
					if (pMove - receive > ALineNum)
					{
						for (i = 0; i < ALineNum; i++)
						{
							if (* (pMove - 1) > 0x80)  							//GB2312  是可以的，GB18030  这种判断是不行的。
							{
								pMove--;
								i++;
								//if((pMove-receivetemp)%ALineNum==0)pMove++;
							}

							pMove--;
						}
					}
				}

				break;

			case KEY_DOWN:			//光标下移
				if (ChineseThinkFlag == 0)
				{
					for (i = 0; i < ALineNum && *pMove != 0; i++)
					{
						if (*pMove > 0x80)
						{
							pMove++;
							i++;
							//if((pMove-receivetemp)%ALineNum==0)pMove++;
						}

						pMove++;
					}
				}

				break;

			case KEY_LEFT:			//光标左移
				if (pMove > receive)
				{
					if (* (pMove - 1) > 0x80) pMove--;				//GB2312  是可以的，GB18030  这种判断是不行的。

					pMove--;
				}

				break;

			case KEY_RIGHT:			//光标右移
				if (*pMove)
				{
					if (*pMove > 0x80) pMove++;

					pMove++;
				}

				break;

				//case KEY_F4:
				//ChineseThinkFlag=1;
				//break;

			case KEY_DEL:
				x = strlen ( (char*) receive);

				if (x != 0)
				{
					if (pMove > receive)
					{
						if (*pMove != 0)  						//如果在字符串之中删除
						{
							if (* (pMove - 1) > 0x80)
							{
								pMove -= 2;
								my_strcpy_DeleteTailByte (pMove, pMove + 2);
							}
							else
							{
								pMove -= 1;
								my_strcpy_DeleteTailByte (pMove, pMove + 1);
							}
						}
						else
						{
							if (pMove > receive && * (pMove - 1) > 0x80)  							//delete a Chinese character
							{
								pMove--;
								*pMove = 0;
							}

							pMove--;
							*pMove = 0;
						}
					}
				}
				else
				{
					return KEY_LEFT;
				}

				break;

			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
				return keyval;
		}

		if (keyflag == 1 || (keyflag == 2 && currentSrf != 4) )
		{
			if (keyflag == 1)
			{
				currentSrf = SwitchSrf (srfFlag, currentSrf);
			}

			DisplaySrfAndInput (currentSrf);
		}

		my_AccordPointInsertEdit (receive, pMove, 1, 2, &cursorx, &cursory);
	}
}








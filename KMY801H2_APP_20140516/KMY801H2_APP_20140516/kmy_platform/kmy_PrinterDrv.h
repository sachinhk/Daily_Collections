#include "stm32f10x.h"

#ifndef __KMY_PRINTERDRV_H
#define __KMY_PRINTERDRV_H

#define Printer_OK							1
#define Printer_NoPaper						2

extern const  char WidthZoom1[4];//字体宽 X 1
extern const  char WidthZoom2[4];//字体宽 X 2
extern const  char WidthZoom3[4];//字体宽 X 3
extern const  char WidthZoom4[4];//字体宽 X 4
extern const  char WidthZoom5[4];//字体宽 X 5

extern const  char HighZoom1[4]; //字体高 X 1
extern const  char HighZoom2[4]; //字体高 X 2
extern const  char HighZoom3[4]; //字体高 X 3
extern const  char HighZoom4[4]; //字体高 X 4
extern const  char HighZoom5[4]; //字体高 X 5

extern const  char Zoom1[4];     //字体 X 1
extern const  char Zoom2[4];     //字体 X 2

extern const  char Font1[4];     //字体 16 X 16
extern const  char Font2[4];     //字体 24 X 24

extern const char underlineOFF[4];     //下划线关
extern const char underlineON[4];     //下划线开


/*
    Function: printer driver initialization
    Function parameter:
                       mode: 0 Initialize all
                       mode: 1 Initialize fonts only
    Return value: no
*/
void kmy_PrinterInit(u8 mode);


/*
    Function: printer instruction set
    Function parameter:command:instruction character string
                       len:instruction length
    Return value: set success return(macro definition) Printer_OK
                  Paper out return(macro definition)Printer_NoPaper
*/
char kmy_PrinterSetcommand(const char * command,unsigned char len);


/*
    Function: printer printing character string
    Function parameter:ch: character string
    Return value: print completed return(macro definition)Printer_OK
                  Paper out return(macro definition)Printer_NoPaper
*/
char kmy_PrinterString(const unsigned char * ch);


/*
　　Function:print bitmap
　　Function parameter:*p: first byte of bitmap data buffer 
			  width: bitmap width
			  height: bitmap height
			  LeftMarginNum:  print bitmap from left,from number x
　　Return value: print completed return(macro definition)Printer_OK
			Paper out return(macro definition)Printer_NoPaper
　　Notice: bitmap format  lognitudinal mod , byte reversed
　　　      LeftMarginNum+width value ,no more than 384；
　　　      Maximum width of bitmap ,384
*/
char kmy_PrinterBitmap(const unsigned char *p,unsigned int width, unsigned int height,unsigned int LeftMarginNum);


#endif


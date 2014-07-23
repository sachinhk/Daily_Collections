#ifndef Lcd_H
#define Lcd_H



#include "kmy_LCDDrv.h"



#define MaxRowOneScr 		5		//the total rows of one screen
#define EditAreaTotalRow	2		//edit area total rows
#define EditAreaStartLine	1		//edit area start row, 1  represents the second row
#define ALineNum			18		//edit area a row of the numbers of asccii byte.
#define MaxByteOneLine 		23		//a row total bytes
#define ALinePixNum			131		//a row total pixels
#define AAscciiPixNum		6		//a Asccii  cod contain pixels
#define AGB2312PixNum		12		//a chinese character contain pixels
#define ALineContainDotLine	13		//a line contian How many dot line



#define	OTHER_LANGUAGE	'1'
#define	LOCAL_LANGUAGE	'0'


extern unsigned char Language[2];					//”Ô—‘


#define text_out	kmy_LcdTextOut
#define clear_lcd	kmy_LcdClear



void text_out_length(char x,char y,const unsigned char *buff,unsigned char length);
void text_out_to_row(unsigned char row,const unsigned char *buff);
void text_out_to_row_Language2(unsigned char row,const unsigned char *StrLocal,const unsigned char *StrOther);
void text_out_to_row_x(unsigned char row,unsigned char x,const unsigned char *buff);
void text_out_to_row_x_Language2(unsigned char row,unsigned char x,const unsigned char *StrLocal,const unsigned char *StrOther);
void my_strcpy_fillspace(unsigned char *dest,const unsigned char *src);
void my_strcpy_fillspace_bytes(unsigned char *dest,const unsigned char *src,const unsigned char TotalBytes);
void text_out_to_row_fillSpace(unsigned char row,const unsigned char *buff);
void text_out_to_row_x_fillSpace(unsigned char row,unsigned char x,const unsigned char *buff);
void DrawTitle_to_row(unsigned char row,const unsigned char *str);
void DrawTitle_to_row_Language2(unsigned char row,const unsigned char *StrLocal,const unsigned char *StrOther);
void clear_area_to_row(unsigned int startrow,unsigned int endrow);
void reverse_area_to_row(unsigned int xpos,unsigned int rows,unsigned int width,unsigned int height);
void draw_line_to_row(unsigned int row);
void steaddraw_pic_to_row_x(const unsigned char *buff,unsigned int x,unsigned char row,unsigned int width,unsigned int height);
void DispHighLightRow_to_x(unsigned char x,unsigned int row);
void DispHighLightRow_to_x_width(int row,char x,char width);
void DispHighLightRow(unsigned int row);
void draw_pic(const unsigned char *pstr,int xpos,int ypos,int width,int height);



void draw_line(int x1,int y1,int x2,int y2,unsigned char mode);
void draw_rect(int xstart,int ystart,int xlen,int ylen,unsigned char mode);
void fill_rect(int xstart,int ystart,int xlen,int ylen,unsigned char mode);//mode 0 used space fill, mode 1 used solid fill, mode2 used take reverse
void reverse_area(int xpos,int ypos,int width,int height);
void clear_area(int xpos,int ypos,int width,int height);
void set_contrast(unsigned char level);


#endif



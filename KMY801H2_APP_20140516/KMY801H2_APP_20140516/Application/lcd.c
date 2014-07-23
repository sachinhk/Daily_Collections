#include "lcd.h"

#include <stdio.h>
#include <string.h>


unsigned char Language[2];					//语言




void text_out_to_row(unsigned char row,const unsigned char *buff)
{
	text_out(0,ALineContainDotLine*row,buff);
	text_out(ALinePixNum-AAscciiPixNum,ALineContainDotLine*row," ");
}
void text_out_to_row_Language2(unsigned char row,const unsigned char *StrLocal,const unsigned char *StrOther)
{
	if(Language[0]==OTHER_LANGUAGE)
	{
		if(StrOther!=NULL)text_out_to_row(row,StrOther);
	}
	else
	{
		if(StrLocal!=NULL)text_out_to_row(row,StrLocal);
	}
}


void text_out_length(char x,char y,const unsigned char *buff,unsigned char length)
{
	unsigned char temp[22];

	if(length>21)length=21;
	memcpy(temp,buff,length);
	temp[length]=0;
	text_out(x,y,temp);
}

void text_out_to_row_x(unsigned char row,unsigned char x,const unsigned char *buff)
{
	text_out(x,ALineContainDotLine*row,buff);
}
void text_out_to_row_x_Language2(unsigned char row,unsigned char x,const unsigned char *StrLocal,const unsigned char *StrOther)
{
	if(Language[0]==OTHER_LANGUAGE){
		if(StrOther!=NULL)text_out_to_row_x(row,x,StrOther);
	}else{
		if(StrLocal!=NULL)text_out_to_row_x(row,x,StrLocal);
	}
}


void my_strcpy_fillspace(unsigned char *dest,const unsigned char *src)
{
	signed char i=0;

	while(i<MaxByteOneLine-1&&*src!='\0'){*dest=*src;dest++;src++;i++;}
	while(i<MaxByteOneLine-1){*dest=' ';dest++;i++;}
	*dest=0;
}

void my_strcpy_fillspace_bytes(unsigned char *dest,const unsigned char *src,const unsigned char TotalBytes)
{
	signed char i=0;

	while(*src!='\0'){*dest=*src;dest++;src++;i++;if(i>=TotalBytes)break;}
	while(i<TotalBytes){*dest=' ';dest++;i++;}
	*dest=0;
}


void text_out_to_row_fillSpace(unsigned char row,const unsigned char *buff)
{
	unsigned char temp[MaxByteOneLine+2];
	my_strcpy_fillspace(temp,buff);
	text_out(0,ALineContainDotLine*row,temp);
	text_out(ALinePixNum-AAscciiPixNum,ALineContainDotLine*row," ");
}

void text_out_to_row_x_fillSpace(unsigned char row,unsigned char x,const unsigned char *buff)
{
	unsigned char temp[MaxByteOneLine+2];
	
	my_strcpy_fillspace(temp,buff);
	text_out(x,ALineContainDotLine*row,temp);
	text_out(ALinePixNum-AAscciiPixNum,ALineContainDotLine*row," ");
}



void DrawTitle_to_row(unsigned char row,const unsigned char *str)//绘制标题
{
	unsigned short Len;

	Len=(strlen((const char *)str))*6;
	if(Len>132)Len=132;
	text_out_to_row_x(row,(132-Len)/2,str);
}
void DrawTitle_to_row_Language2(unsigned char row,const unsigned char *StrLocal,const unsigned char *StrOther)//绘制标题
{
	if(Language[0]==OTHER_LANGUAGE){
		if(StrOther!=NULL)DrawTitle_to_row(row,StrOther);
	}else{
		if(StrLocal!=NULL)DrawTitle_to_row(row,StrLocal);
	}
}

void clear_area_to_row(unsigned int startrow,unsigned int endrow)
{
	kmy_LcdDrawRect(0,ALineContainDotLine*startrow,132,ALineContainDotLine*(endrow+1),mode_White);
}

void reverse_area_to_row(unsigned int xpos,unsigned int rows,unsigned int width,unsigned int height)
{
	kmy_LcdDrawRect(xpos,ALineContainDotLine*rows,xpos+width-1,ALineContainDotLine*rows+height,mode_Reverse);
}

void draw_line_to_row(unsigned int row)
{
}

void steaddraw_pic_to_row_x(const unsigned char *buff,unsigned int x,unsigned char row,unsigned int width,unsigned int height)
{
	kmy_LcdDrawBitMap(buff,x,ALineContainDotLine*row,width,height);
}

void draw_pic(const unsigned char *pstr,int xpos,int ypos,int width,int height)
{
	kmy_LcdDrawBitMap(pstr,(unsigned char)xpos,(unsigned char)ypos,(unsigned char)width,(unsigned char)height);
}


void draw_line(int x1,int y1,int x2,int y2,unsigned char mode)
{
	kmy_LcdDrawLine(x1,y1,x2,y2);
}

void draw_rect(int xstart,int ystart,int xlen,int ylen,unsigned char mode)
{
	kmy_LcdDrawRect(xstart,ystart,xstart+xlen-1,ystart+ylen-1,mode);
}

void fill_rect(int xstart,int ystart,int xlen,int ylen,unsigned char mode)
{
	kmy_LcdDrawRect(xstart,ystart,xstart+xlen-1,ystart+ylen-1,mode);
}

void reverse_area(int xpos,int ypos,int width,int height)
{
	kmy_LcdDrawRect(xpos,ypos,xpos+width-1,ypos+height-1,mode_Reverse);
}

void clear_area(int xpos,int ypos,int width,int height)
{
	kmy_LcdDrawRect(xpos,ypos,xpos+width-1,ypos+height-1,mode_White);
}


void set_contrast(unsigned char level)
{
	unsigned int temp;
	temp=(level*88)/100;
	kmy_LcdSetContrast(temp);
}


void DispHighLightRow_to_x(unsigned char x,unsigned int row)
{
	kmy_LcdDrawRect(x,ALineContainDotLine*row,128,ALineContainDotLine*row+11,mode_Reverse);
}

void DispHighLightRow_to_x_width(int row,char x,char width)
{
	reverse_area(x,ALineContainDotLine*row,width,12);
}


void DispHighLightRow(unsigned int row)
{
	kmy_LcdDrawRect(0,ALineContainDotLine*row,131,ALineContainDotLine*row+11,mode_Reverse);
}


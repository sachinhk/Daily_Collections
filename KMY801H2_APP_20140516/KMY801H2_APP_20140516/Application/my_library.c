
#include "my_library.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "MenuOperate.h"
#include "lcd.h"
#include "key.h"
#include "SelfTest.h"

#include "kmy_BeepDrv.h"
#include "kmy_Time.h"
#include "kmy_USART1Drv.h"
#include "kmy_LCDDrv.h"

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
//	USART_SendData(USART1, (uint16_t) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}




//以十六进制的方式打印Abuf
//参数Title  为首先打印的标题
//参数len  为打印abuf  的长度
void my_HexPrintf_buff(const char *Title,char *Abuf,int len)
{
	int i;

	printf("%s",Title);
	for(i=0;i<len;i++){printf("\\x%02x ",Abuf[i]);}
	printf("\n");
}

void my_charhexprintf_buff(const char *Title,const unsigned char *buff,unsigned int len)
{
	printf("%s",Title);

	for(;len!=0;len--){
		if(*buff<=0x20||*buff>0x7e){
			printf("\\x%02x",*buff);
		}else{
			printf("%c",*buff);
		}
		buff++;
	}
	printf("\r\n");
}


//l:要打印的数据
//bitlen: 要打印的位数
// 返回打印出1  的个数
//例如: my_PrintBinary(0xff,3),则输出:111， 返回3
unsigned char my_PrintfBinary(unsigned long l,unsigned char bitlen)
{
	unsigned long mask;
	unsigned char cnt1=0;

	mask=1<<(bitlen-1);

	for(;bitlen!=0;bitlen--){
		if((l&mask)!=0){putchar('1');cnt1++;}
		else putchar('0');
		l<<=1;
	}

	return cnt1;
}

//buff: 字符串buff
//l:要打印的数据
//bitlen: 要打印的位数
// 返回打印出1  的个数
//例如: my_PrintBinary(0xff,3),则输出:111， 返回3
unsigned char my_SprintfBinary(unsigned char *buff,unsigned long l,unsigned char bitlen)
{
	unsigned long mask;
	unsigned char cnt1=0;

	mask=1<<(bitlen-1);
	*buff=0;

	for(;bitlen!=0;bitlen--){
		if((l&mask)!=0){*buff='1';cnt1++;}
		else *buff='0';
		buff++;
		l<<=1;
	}

	*buff=0;

	return cnt1;
}


static unsigned char *my_movePointToNextLine(unsigned char *p)
{
	short CharCnt;

	CharCnt=0;
	while(*p!=0){
		if(*p>=0x80){			//汉子
			if(CharCnt<=20-2){	//一行可以显示10  个汉子，如果到了20，则此汉字放到下一行
				p++;CharCnt++;
				p++;CharCnt++;
				if(CharCnt>=20-1){
					return p;
				}
			}else{
				return p;
			}
		}
		else if(*p=='\n'){
			return p+1;
		}
		else{			//ascii码
			p++;CharCnt++;
			if(CharCnt>=20){
				return p;
			}
		}
	}

	return p;
}


static unsigned char *my_movePointToPrevLine(unsigned char *p,unsigned char *pHead)
{
	unsigned char *ptemp,*preturn;

	if(p==pHead)return p;

	ptemp=pHead;
	preturn=pHead;
	while(1){
		ptemp=my_movePointToNextLine(ptemp);
		if(ptemp==p)return preturn;
		else {preturn=ptemp;}
	}
}


//在StartLine  与EndLine  之间显示buff  指向的内容，
//会自动换行如果超过EndLine-StartLine+1  行，
//则只显示EndLine-StartLine+1  行的内容，剩下的内容
//会忽略掉。
void _my_DisplayBuff(const unsigned char *buff,unsigned char StartLine,unsigned char EndLine)
{
	short LineCnt;
	short CharCnt;
	unsigned char temp[ALineNum+3];

	CharCnt=0;
	LineCnt=StartLine;
	while(*buff!=0){
		if(*buff>=0x80){			//汉子
			if(CharCnt<=20-2){	//一行可以显示10  个汉子，如果到了20，则此汉字放到下一行
				temp[CharCnt]=*buff;buff++;CharCnt++;
				temp[CharCnt]=*buff;buff++;CharCnt++;
				if(CharCnt>=20-1){
					temp[CharCnt]=0;
					text_out_to_row_fillSpace(LineCnt,temp);
					LineCnt++;
					if(LineCnt>=EndLine)return;
					CharCnt=0;
				}
			}else{
				temp[CharCnt]=0;
				text_out_to_row_fillSpace(LineCnt,temp);
				LineCnt++;
				if(LineCnt>=EndLine)return;
				CharCnt=0;
				temp[CharCnt]=*buff;buff++;CharCnt++;
				temp[CharCnt]=*buff;buff++;CharCnt++;
			}
		}
		else if(*buff=='\n'){
			buff++;
			temp[CharCnt]=0;
			text_out_to_row_fillSpace(LineCnt,temp);
			LineCnt++;
			if(LineCnt>=EndLine)return;
			CharCnt=0;
		}
		else{			//ascii码
			temp[CharCnt]=*buff;buff++;CharCnt++;
			if(CharCnt>=20){
				temp[CharCnt]=0;
				text_out_to_row_fillSpace(LineCnt,temp);
				LineCnt++;
				if(LineCnt>=EndLine)return;
				CharCnt=0;
			}
		}
	}

	temp[CharCnt]=0;
	text_out_to_row_fillSpace(LineCnt,temp);
	LineCnt++;

	while(LineCnt<EndLine){
		text_out_to_row_fillSpace(LineCnt,"    ");
		LineCnt++;
	}
}

unsigned char my_DisplayBuff(unsigned char *buff,char StartLine,char EndLine)
{
	unsigned char keyval;
	unsigned char *p;

	clear_area_to_row(StartLine,EndLine);
	p=buff;
	_my_DisplayBuff(p,StartLine,EndLine);
	while(1){
		keyval=GetKeyValue();
		if(keyval == KEY_NONE)continue ;
		KeyEventPost();
		switch(keyval){
			case KEY_UP:
			if(p!=buff){p=my_movePointToPrevLine(p,buff);}
			_my_DisplayBuff(p,StartLine,EndLine);
			break;

			case KEY_DOWN:
			p=my_movePointToNextLine(p);
			_my_DisplayBuff(p,StartLine,EndLine);
			break;

			case KEY_RIGHT:
			for(keyval=StartLine;keyval<EndLine;keyval++){
				p=my_movePointToNextLine(p);
			}
			_my_DisplayBuff(p,StartLine,EndLine);
			break;

			case KEY_LEFT:
			for(keyval=StartLine;keyval<EndLine;keyval++){
				if(p!=buff){p=my_movePointToPrevLine(p,buff);}
				else break;
			}
			_my_DisplayBuff(p,StartLine,EndLine);
			break;

			case KEY_Enter:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
			return keyval;
		}
	}
}


void my_strcat_insert(unsigned char *InsertPoint,const unsigned char *InsertStr)
{
	unsigned int insertlen;
	unsigned int pMovelen;

	const unsigned char *psrc;
	unsigned char *pdes;

	insertlen=strlen((const char*)InsertStr);

	pMovelen=strlen((const char*)InsertPoint);
	if(pMovelen){
		psrc=InsertPoint+pMovelen-1;
		pdes=InsertPoint+pMovelen+insertlen-1;
		*(pdes+1)=0;
		for(;pMovelen!=0;pMovelen--){
			*pdes=*psrc;
			psrc--;
			pdes--;
		}
	}else{
		memcpy(InsertPoint,InsertStr,insertlen);
		*(InsertPoint+insertlen)=0;
	}
}


unsigned int my_GetCurrentMs(void)
{
	return kmy_TimeGetMilliseconds();
}

#if 0


//Traversal path, indentation quantity control 
void my_List( char *path, int indent )
{
	struct dirent	*ent=NULL;
	struct stat		statbuf;
	DIR				*pDir;
	char			dir[512];

	if((pDir=opendir(path))==NULL){
		#ifdef DEBUG_PRINTF
		printf("Cannot open directory:%s\n",path);
		#endif
		return;
	}else{
		#ifdef DEBUG_PRINTF
		printf("Can open directory:%s\n",path);
		#endif
	}

	while((ent=readdir(pDir))!=NULL){
		snprintf(dir,512,"%s/%s",path,ent->d_name);		//Get reading files the absolute path name
		lstat(dir,&statbuf);		//Get file information
		if(S_ISDIR(statbuf.st_mode)){			//is direct or is a file
			/*if(strcmp( ".",ent->d_name) == 0 || strcmp( "..",ent->d_name) == 0){//Eliminate the current directory and superior directory
			continue;
			}
			printf( "direct:%s\n",ent->d_name );//If is subdirectories, recursively function itself, realize subdirectory files traverse
			my_List( dir, indent+4 );//Recursion, traverse subdirectory files*/
		}else{
			printf("file:%s\n",ent->d_name);
		}
	}

	closedir(pDir);
}






static void ReadOneLineToBuff(char *displaybuff,const char **pMove)
{
	unsigned char i=0;
	
	while(**pMove!=0&&i<MaxByteOneLine-5){
		if(**pMove=='\r'||**pMove=='\n'){(*pMove)++;break;}
		if(**pMove>0x80){
			displaybuff[i]=**pMove;
			i++;
			(*pMove)++;
		}

		displaybuff[i]=**pMove;
		i++;
		(*pMove)++;
	}

	displaybuff[i]=0;
}

static void pointDownALine(const char **pUp)
{
	char i;
	
	for(i=0;**pUp!=0&&i<MaxByteOneLine-5;i++,(*pUp)++){
		if(**pUp=='\r'||**pUp=='\n'){(*pUp)++;break;}
		if(**pUp>0x80){
			i++;
			(*pUp)++;
		}
	}
}


static void pointUpALine(const char **pUp,const char *buff,signed char fromline)
{
	const char *p;
	char i;

	p=buff;

	while(fromline!=0){
		for(i=0;i<MaxByteOneLine-5;i++,p++){
			if(*p=='\r'||*p=='\n'){p++;break;}
			if(*p>0x80){
				i++;
				p++;
			}
		}

		fromline--;
	}

	(*pUp)=p;
}

//以记事本的方式显示缓冲区buff
//遇到\r  或\n  就换行，遇到tab  键不做处理
//如果一屏显示不完，遇到上下按键则向下或向上
//滚动一行，遇到左右键，则向上或向下滚动一屏
void my_notepadBuff(const char *buff,char StartLine,char EndLine)
{
	char displaybuff[MaxByteOneLine+2];
	const char *pDown,*pUp;
	char flashflag;
	signed char fromline=-1;
	char i;
	unsigned char keyval;

	clear_area_to_row(StartLine,EndLine);
	pUp=buff;
	flashflag=1;

	while(1){
		if(flashflag){
			flashflag=0;
			pDown=pUp;
			for(i=StartLine;i<EndLine&&*pDown!=0;i++){
				ReadOneLineToBuff(displaybuff,&pDown);
				text_out_to_row_fillSpace(i,displaybuff);
			}
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue ;
		KeyEventPost();

		switch(keyval){
			case KEY_LEFT:				break;
			case KEY_RIGHT: 			break;
			case KEY_UP:	if(fromline>=0){pointUpALine(&pUp,buff,fromline);fromline--;flashflag=1;} break;
			case KEY_DOWN:	if(*pDown!=0){pointDownALine(&pUp);fromline++;flashflag=1;}break;

			case KEY_Enter:
			case KEY_ReturnInitInterface:
			return;
		}

	}
}



void my_EnglishMonthToChinesMonth(char *Engmcm)
{
	signed char i;
	const char *EMCM[12][2]={
		{"Jan","01"},
		{"Feb","02"},
		{"Mar","03"},
		{"Apr","04"},
		{"May","05"},
		{"Jun","06"},
		{"Jul","07"},
		{"Aug","08"},
		{"Sep","09"},
		{"Oct","10"},
		{"Nov","11"},
		{"Dec","12"},
	};


	for(i=0;i<12;i++){
		if(strcmp(Engmcm,EMCM[i][0])==0){strcpy(Engmcm,EMCM[i][1]);break;}
	}
}


//把系统的当前年月日以"2011-06-14" 的格式输入到buff
void my_GetSystemCurrentYMD(unsigned char *buff)
{
	struct tm *tm_ptr;
	time_t the_time;

	time(&the_time);
	tm_ptr = gmtime(&the_time);
	sprintf(buff,"%4d-%02d-%02d",1900+tm_ptr->tm_year, tm_ptr->tm_mon+1, tm_ptr->tm_mday);
}

//把系统的当前时分秒以"08:45:22" 的格式输入到buff
void my_GetSystemCurrentHMS(unsigned char *buff)
{
	struct tm *tm_ptr;
	time_t the_time;

	time(&the_time);
	tm_ptr = gmtime(&the_time);
	sprintf(buff,"%02d:%02d:%02d",tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
}








//dest  转换后的字符buff
//value  要转的浮点值
//digit   需要小数点后的位数
//注意1. 最多转换100位(整数小数部分一起)
//注意2. dest   SIZE 最好大于103.
//注意3. 如果实际计算的小数位< digit, 会在末尾加'0' 字符 
void my_fcvt(unsigned char *dest,double value,unsigned char digit)
{
	char *p;
	int point,pn;

	p=fcvt(value,digit,&point,&pn);

	if(pn==1){*dest='-';dest++;}
	if(point==0){*dest='0';dest++;}

	for(pn=0;pn<point&&pn<100;pn++){*dest=*p;dest++;p++;}

	*dest='.';dest++;

	for(;*p!=0&&pn<100;pn++){*dest=*p;dest++;p++;}

	*dest=0;
}


//参数YMDHMS  为年月日时分秒的ascii  码形式，比如20110808120012
//比如:2011-08-08 12:00:22
//返回从1970年1月1日0时0分0秒到YMDHMS 所经过的秒数。
unsigned int my_YearMonthDayToSeconds(const unsigned char *YMDHMS)
{
	struct tm mytime;
	unsigned char ymdhms[25];
	unsigned char temp[5];
	unsigned char *p,*pprev;

	memcpy(ymdhms,YMDHMS,20);
	ymdhms[19]=0;
	p=ymdhms;
	my_MovePointToNum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向2011-08-08 12:00:22
	my_MovePointToNonnum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向-08-08 12:00:22
	
	my_MovePointToNum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向08-08 12:00:22
	pprev=p;
	
	my_MovePointToNonnum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向-08 12:00:22
	if(p-pprev<2){my_string_insert(pprev,"0");p++;}

	my_MovePointToNum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向08 12:00:22
	pprev=p;
	my_MovePointToNonnum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向 12:00:22
	if(p-pprev<2){my_string_insert(pprev,"0");p++;}

	my_MovePointToNum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向12:00:22
	pprev=p;
	my_MovePointToNonnum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向:00:22
	if(p-pprev<2){my_string_insert(pprev,"0");p++;}

	my_MovePointToNum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向00:22
	pprev=p;
	my_MovePointToNonnum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向:22
	if(p-pprev<2){my_string_insert(pprev,"0");p++;}

	my_MovePointToNum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向22
	pprev=p;
	my_MovePointToNonnum(&p,&ymdhms[sizeof(ymdhms)-1]);	//p指向
	if(p-pprev<2){my_string_insert(pprev,"0");p++;}

	my_strdel_allNonnum(ymdhms);
	p=ymdhms;

	#ifdef DEBUG_PRINTF
	printf("ymdhms=%s\n",ymdhms);
	#endif

	memcpy(temp,p,4);p+=4;temp[4]=0;
	mytime.tm_year=atoi(temp)-1900;
	memcpy(temp,p,2);p+=2;temp[2]=0;
	mytime.tm_mon=atoi(temp);
	memcpy(temp,p,2);p+=2;temp[2]=0;
	mytime.tm_mday=atoi(temp);
	memcpy(temp,p,2);p+=2;temp[2]=0;
	mytime.tm_hour=atoi(temp);
	memcpy(temp,p,2);p+=2;temp[2]=0;
	mytime.tm_min=atoi(temp);
	memcpy(temp,p,2);p+=2;temp[2]=0;
	mytime.tm_sec=atoi(temp);
	
	return mktime(&mytime);
}


//参数buff :为要写入文件的缓冲区，
//参数wLen:为要写入文件长度
//参数filename: 文件名
//返回值:
//成功写入返回0
//打开文件失败返回1
//写入数据错误返回2
//关闭文件失败返回3
unsigned char my_WriteStrToFile(const unsigned char *buff,unsigned int wLen,const unsigned char *filename)
{
	FILE *fp;
	//unsigned int len;
	unsigned int retval;

	fp=fopen(filename,"wt");	//"wt"	只写打开或建立一个文本文件，只允许写数据 
	if(fp==NULL){
		return 1;
	}

	retval=fwrite(buff,wLen,1,fp);
	if(retval==-1){fclose(fp);return 2;}

	retval=fclose(fp);
	if(retval!=0)return 3;

	return 0;
}

//参数buff :为要读出文件的缓冲区
//参数rLen:为要读出文件长度
//参数filename: 文件名
//返回值:
//成功读出返回0, 读出的数据存于buff，读出的有效长度为rLen.
//打开文件失败返回1
//读出数据错误返回2
//关闭文件失败返回3
unsigned char my_ReadStrFromFile(unsigned char *buff,unsigned int rLen,const unsigned char *filename)
{
	FILE *fp;
	unsigned int retval;

	fp=fopen(filename,"rt");	//"rt"	 只读打开一个文本文件，只允许读数据 
	if(fp==NULL){
		return 1;
	}

	retval=fread(buff,rLen,1,fp);
	if(retval==-1){fclose(fp);return 2;}

	retval=fclose(fp);
	if(retval!=0)return 3;

	return 0;
}

//参数fileLength:文件长度
//参数filename: 文件名
//返回值:
//成功得到文件长度返回0，文件长度存于fileLength 中
//打开文件失败返回1
//fseek错误返回2
//关闭文件失败返回3
unsigned char my_GetFileLength(unsigned int *fileLength,const unsigned char *filename)
{
	FILE *fp;
	unsigned int retval;

	fp=fopen(filename,"rt");		//"rt"	 只读打开一个文本文件，只允许读数据 
	if(fp==NULL){
		return 1;
	}

	retval=fseek(fp,0,SEEK_END);	//move to file end
	if(retval==-1){
		fclose(fp);
		return 2;
	}
	*fileLength = ftell(fp);		//file length

	retval=fclose(fp);
	if(retval!=0)return 3;

	return 0;
}


//参数filename: 文件名
//返回值:
//文件存在返回0
//打开文件失败返回1
//关闭文件失败返回3
unsigned char my_CheckFileWhetherExist(const unsigned char *filename)
{
	FILE *fp;
	unsigned int retval;

	fp=fopen(filename,"rt");		//"rt"	 只读打开一个文本文件，只允许读数据 
	if(fp==NULL){
		return 1;
	}

	retval=fclose(fp);
	if(retval!=0)return 3;

	return 0;
}


//参数filename: 文件名
//返回值:
//创建文件成功返回0
//打开文件失败返回1
//关闭文件失败返回3
unsigned char my_CreateFile(const unsigned char *filename)
{
	FILE *fp;
	unsigned int retval;

	fp=fopen(filename,"wt");		//"wt"	只写打开或建立一个文本文件，只允许写数据 
	if(fp==NULL){
		return 1;
	}

	retval=fclose(fp);
	if(retval!=0)return 3;

	return 0;
}



#endif



#ifndef My_library_H
#define My_library_H



#define TimeGetMilliseconds	kmy_TimeGetMilliseconds




void my_HexPrintf_buff(const char *Title,char *Abuf,int len);
void _my_DisplayBuff(const unsigned char *buff,unsigned char StartLine,unsigned char EndLine);
unsigned char my_DisplayBuff(unsigned char *buff,char StartLine,char EndLine);
void my_notepadBuff(const char *buff,char StartLine,char EndLine);
void my_EnglishMonthToChinesMonth(char *Engmcm);
//把系统的当前年月日以"2011-06-14" 的格式输入到buff
void my_GetSystemCurrentYMD(unsigned char *buff);
//把系统的当前时分秒以"08:45:22" 的格式输入到buff
void my_GetSystemCurrentHMS(unsigned char *buff);
void my_charhexprintf_buff(const char *Title,const unsigned char *buff,unsigned int len);
void my_strcat_insert(unsigned char *pMove,const unsigned char *PyCode);
unsigned int my_GetCurrentMs(void);
void my_fcvt(unsigned char *dest,double value,unsigned char digit);
unsigned int my_YearMonthDayToSeconds(const unsigned char *YMDHMS);
unsigned char my_WriteStrToFile(const unsigned char *buff,unsigned int wLen,const unsigned char *filename);
unsigned char my_ReadStrFromFile(unsigned char *buff,unsigned int rLen,const unsigned char *filename);
unsigned char my_GetFileLength(unsigned int *fileLength,const unsigned char *filename);
unsigned char my_CheckFileWhetherExist(const unsigned char *filename);
unsigned char my_CreateFile(const unsigned char *filename);
unsigned char my_PrintfBinary(unsigned long l,unsigned char bitlen);
unsigned char my_SprintfBinary(unsigned char *buff,unsigned long l,unsigned char bitlen);


#endif


#ifndef Mypure_library_H
#define Mypure_library_H



/*#ifdef DEBUG_PRINTF
#define	my_printf(msg...)   printf(msg)
#else
#define	my_printf(msg...)
#endif
*/


extern const unsigned char HexMapChar[16];
extern const unsigned char CharMapHex[256];


void my_OctorToAscii(unsigned char *buff,long num);
long my_AsciiToOctor(unsigned char *buff);
unsigned char * my_strstr_BuffEnd(const unsigned char *s1,const unsigned char *s1BuffEnd,const unsigned char *s2);
char * my_strstr_size(const char *s1,int size,const char *s2);
char my_FFstrstr(const char *s1,const char *s2);
char my_KonggeStrlen(const char *str);
void my_strcpy_DeleteTailByte(unsigned char *src,const unsigned char *des);
void My_strcpy_PointInc(char **ps,const char *temp);
void My_strcpy_Int_PointInc(char **ps, char temp);
void my_itoa(char*string,unsigned int i);
void my_itoa_digit(char*string,int operand,char digit);
void my_itoa_digit_hex(char*string,int operand,char digit);
void my_HexPrintf_buff(const char *Title,char *Abuf,int len);
void my_strcpy_EncounterHexEnd(unsigned char *DesBuff,unsigned char *SourceBuff,unsigned int len,unsigned char Hex);
void my_strcpy_EncounterLess0x1fEnd(unsigned char *DesBuff,unsigned char *SourceBuff,unsigned int len);
void my_MovePointUpto0x20(unsigned char **p,unsigned char *AddrOfBuffEnd);
void my_MovePointDownto0x20(unsigned char **p,unsigned char *AddrOfBuffEnd);
void my_MovePointToNonnum(unsigned char **p,unsigned char *AddrOfBuffEnd);
//把指针移到从p  开始第一个数字ascii  码(即非'0' ---'9' )的字符处。
//不判断'\0'  字符
void my_MovePointToNum(unsigned char **p,unsigned char *AddrOfBuffEnd);
void my_MovePointToHex(unsigned char **p,unsigned char *AddrOfBuffEnd,unsigned char hex);
void my_OneHexToTwoAsccii(char **ps,char hex);
void my_TwoAscciiToOneHex(unsigned char **pt,unsigned char *hex);
void my_buff_TwoAscciiToOneHex(unsigned char *buff,int len);
void my_buff_OneHexToTwoAsccii(char *tobuff,const char *frombuff,int len);
unsigned char my_strcmp(const unsigned char *s1,const unsigned char *s2);
unsigned char my_strcmp_noCaseSensitve(const unsigned char *s1,const unsigned char *s2);
void my_strcpy0123456789(char *tobuff,int len,const char *frombuff);
void my_round(float *pri,char weisu);
unsigned char my_AscciiToBCD(unsigned char *AscciiBuf,unsigned char *BcdBuf);
void my_memcpy_space(unsigned char *p,unsigned char len);
void my_strdel(unsigned char *buff,unsigned char c);
void my_strdel_allNonnum(unsigned char *buff);
unsigned char my_CheckBuffIsAllLetter(unsigned char *buff,unsigned char c,unsigned char CheckLen);
unsigned char my_CheckBuffIsContainLetter(unsigned char *buff,unsigned char c,unsigned char CheckLen);
void my_string_insert(unsigned char *Postion,const unsigned char *insertStr);
void my_string_delete(unsigned char *to,const unsigned char *from);
void my_string_replace(unsigned char *Buff,unsigned char c1,unsigned char c2);
unsigned int my_FloatConvertToInt(float f);
unsigned char my_CheckSum(unsigned char * buff,unsigned int len);
void my_strdel_lessHex(unsigned char *buff,unsigned char hex);
unsigned int my_axtoi(unsigned char* p);



#endif


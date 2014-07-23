

#include "mypure_library.h"



const unsigned char HexMapChar[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const unsigned char CharMapHex[256]={
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x00----0x0f 空字符
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x10----0x1f 空字符
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x20----0x2f
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x00,0x00,0x00,0x00,0x00,	//0x30----0x3f
	0x00,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x40----0x4f
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x50----0x5f
	0x00,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x60----0x6f
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x70----0x7f
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x80----0x8f
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x90----0x9f
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0xa0----0xaf
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0xb0----0xbf
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0xc0----0xcf
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0xd0----0xdf
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0xe0----0xef
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 	//0xf0----0xff
};


void my_OctorToAscii(unsigned char *buff,long num)
{
	unsigned char i,weisu;
	int div;

	if(num>=100000){weisu=6;div=100000;}
	else if(num>=10000){weisu=5;div=10000;}
	else if(num>=1000){weisu=4;div=1000;}
	else if(num>=100){weisu=3;div=100;}
	else if(num>=10){weisu=2;div=10;}
	else if(num<10){weisu=1;div=1;}
	
	for(i=0;i<weisu;i++){
		*buff=HexMapChar[num/div];
		num%=div;
		div/=10;
		buff++;
	}
	*buff='\0';
}


long my_AsciiToOctor(unsigned char *buff)
{
	long temp=0;
	while(*buff>='0'&&*buff<='9'){
		temp*=10;
		temp+=*buff-'0';
		buff++;
	}

	return temp;
}


float my_AsciiToFloat(char *buff)
{
	float temp=0.0;
	int fi=10;
	while(*buff>='0'&&*buff<='9'){
		temp*=10;
		temp+=*buff-'0';
		buff++;
	}

	if(*buff=='.'){
		buff++;
		while(*buff>='0'&&*buff<='9'){
			temp+=(*buff-'0')/fi;
			buff++;
			fi*=10;
		}
	}
	
	return temp;
}



//比较s1  开始向后size  个字节中有没有s2  字符串。
//此函数考虑s1  中有\0  的情况。
char * my_strstr_size(const char *s1,int size,const char *s2)
{
	if (*s1 == 0){
		if (*s2)return (char *)0;
		return (char *) s1;
	}

	while (size>0){
		int i;

		i = 0;
		while (1){
			if (s2[i] == 0){
				return (char *) s1;
			}
			if (s2[i] != s1[i]){
				break;
			}
			i++;
		}

		s1++;size--;
	}

	return (char *)0;
}


//例如:my_FFstrstr("123456","123456")、my_FFstrstr("1234567","123456")返回1   
//my_FFstrstr("123456","1234567")、my_FFstrstr("23456","12345")都返回0
char my_FFstrstr(const char *s1,const char *s2)
{
	while(*s1!=0){
		if(*s1!=*s2)break;
		s1++;
		s2++;
		if(*s2==0)return 1;
	}

	return 0;
}


//例如:my_KonggeStrlen("123 45 6 asd ")返回4   
//my_KonggeStrlen("123 45 6 asd")返回3
char my_KonggeStrlen(const char *str)
{
	char len=0;

	while(*str!=0){
		if(*str==' '){len++;}
		str++;
	}

	return len;
}

/*
例如:
char ch[30]={"abcdefghijklmnopqrstuvwxyz"};
my_strcpy_DeleteTailByte(ch+5,ch+8);
则ch[30]={"abcdeijklmnopqrstuvwxyz\0\0\0"};
*/
void my_strcpy_DeleteTailByte(unsigned char *src,const unsigned char *des)
{
	unsigned char len;

	len=des-src;
	while(*des!=0){
		*src=*des;
		src++;
		des++;
	}
	for(;len!=0;len--){*src=0;src++;}
}



void My_strcpy_PointInc(char **ps,const char *temp)
{
	while(*temp!=0){
		**ps=*temp;
		(*ps)++;
		temp++;
	}

	**ps=0;
}

void My_strcpy_Int_PointInc(char **ps, char temp)
{
	**ps=temp;
	(*ps)++;
}




void my_itoa(char*string,unsigned int i)
{
	unsigned int power,j;

	j=i;
	for(power=1;j>=10;j/=10)power*=10;
	for(;power>0;power/=10){
		*string++='0'+i/power;
		i%=power;
	}
	*string='\0';
}



void my_itoa_digit(char*string,int operand,char digit)
{
	int power,i,j;
	char *t;

	i=operand;
	t=string;
	for(j=power=1;i>=10;i/=10){power*=10;j++;}
	if(j>digit){
		for(;j>digit;j--){operand%=power;power/=10;}
	}else{
		for(;j<digit;j++){*t++='0';}
	}

	for(;power>0;power/=10){
		*t++='0'+operand/power;
		operand%=power;
	}
	*t='\0';
}


void my_itoa_digit_hex(char*string,int operand,char digit)
{
	unsigned char temp;

	if(digit>8)digit=8;

	for(;digit!=0;digit--){
		temp=(operand>>4*(digit-1))&0x0f;
		*string=HexMapChar[temp];
		string++;
	}

	*string=0;
}



void my_strcpy_EncounterHexEnd(unsigned char *DesBuff,unsigned char *SourceBuff,unsigned int len,unsigned char Hex)
{
	for(;len!=0;len--){
		if(*SourceBuff!=Hex){
			*DesBuff=*SourceBuff;
			DesBuff++;
			SourceBuff++;
		}else{
			break;
		}
	}

	*DesBuff=0;
}


void my_strcpy_EncounterLess0x1fEnd(unsigned char *DesBuff,unsigned char *SourceBuff,unsigned int len)
{
	for(;len!=0;len--){
		if(*SourceBuff>=0x20){
			*DesBuff=*SourceBuff;
			DesBuff++;
			SourceBuff++;
		}else{
			break;
		}
	}

	*DesBuff=0;
}


//把指针移到从p  开始第一个大于空格的字符处。
//不判断'\0'  字符
void my_MovePointUpto0x20(unsigned char **p,unsigned char *AddrOfBuffEnd)
{
	while((**p<0x20)&&(*p<=AddrOfBuffEnd))(*p)++;
}


//把指针移到从p  开始第一个小于空格的字符处。
//不判断'\0'  字符
void my_MovePointDownto0x20(unsigned char **p,unsigned char *AddrOfBuffEnd)
{
	while((**p>=0x20)&&(*p<=AddrOfBuffEnd))(*p)++;
}

//把指针移到从p  开始第一个数字ascii  码(即非'0' ---'9' )的字符处。
//不判断'\0'  字符
void my_MovePointToNonnum(unsigned char **p,unsigned char *AddrOfBuffEnd)
{
	while((**p>='0'&&**p<='9')&&(*p<=AddrOfBuffEnd))(*p)++;
}

//把指针移到从p  开始第一个非数字ascii  码(即非'0' ---'9' )的字符处。
//不判断'\0'  字符
void my_MovePointToNum(unsigned char **p,unsigned char *AddrOfBuffEnd)
{
	while((**p<'0'||**p>'9')&&(*p<=AddrOfBuffEnd))(*p)++;
}

//把指针移到从p  开始第一个非数字ascii  码(即非'0' ---'9' )的字符处。
//不判断'\0'  字符
void my_MovePointToHex(unsigned char **p,unsigned char *AddrOfBuffEnd,unsigned char hex)
{
	while((**p!=hex)&&(*p<=AddrOfBuffEnd))(*p)++;
}


//把一个16  进制数转成两位的ASCII  码
//如:  把0x34  转成"34"
void my_OneHexToTwoAsccii(char **ps,char hex)
{
	unsigned char temp;

	temp=hex>>4;
	temp=HexMapChar[temp];
	**ps=temp;
	(*ps)++;

	temp=hex&0x0f;
	temp=HexMapChar[temp];
	**ps=temp;
	(*ps)++;

	**ps=0;
}


//把两位的ASCII  码转成一个16  进制数
//如:  把"34"  转成0x34  
void my_TwoAscciiToOneHex(unsigned char **pt,unsigned char *hex)
{
	char temp=0;

	temp=CharMapHex[**pt];
	(*pt)++;
	temp<<=4;

	temp+=CharMapHex[**pt];
	(*pt)++;

	*hex=temp;
}


//参数buff  是:  要转换的开始地址，
//参数len   是:  要转换的长度。
void my_buff_TwoAscciiToOneHex(unsigned char *buff,int len)
{
	unsigned char *pbuff;
	unsigned char *ptemp;
	int temp;

	pbuff=buff;
	ptemp=buff;
	len/=2;
	temp=len;
	for(;len>0;len--){
		my_TwoAscciiToOneHex(&pbuff,ptemp);
		ptemp++;
	}
	if(temp>10)temp=10;

	for(;temp!=0;temp--){*ptemp=0;ptemp++;}
}


//参数tobuff  是    :转换后数据的存放开始地址，
//参数frombuff  是:要转换的数据开始地址，
//参数len   是       :要转换的长度。
void my_buff_OneHexToTwoAsccii(char *tobuff,const char *frombuff,int len)
{
	const char *pfrom;
	char *pto;

	pto=tobuff;
	pfrom=frombuff;
	for(;len>0;len--){
		my_OneHexToTwoAsccii(&pto,*pfrom);
		pfrom++;
	}
}

unsigned char my_strcmp(const unsigned char *s1,const unsigned char *s2)
{
	if(s1==(void *)0||s2==(void *)0)return 0xff;

	for(;*s1++==*s2++;){  
		if(*s1==0)break;
	}

	return(*s1-*s2);
}
unsigned char my_strcmp_noCaseSensitve(const unsigned char *s1,const unsigned char *s2)
{
	if(s1==(void *)0||s2==(void *)0)return 0xff;

	while(*s1!=0){
		if(*s1<*s2){			//a==0x61,A=0x41
			if(*s1+('a'-'A')!=*s2)break;
		}
		else if(*s1>*s2){
			if(*s1-('a'-'A')!=*s2)break;
		}
		s1++;s2++;
	}

	return (*s1-*s2);
}


void my_strcpy0123456789(char *tobuff,int len,const char *frombuff)
{
	while(*frombuff>='0'&&*frombuff<='9'&&len>1){
		*tobuff=*frombuff;
		tobuff++;
		frombuff++;
	}

	*tobuff=0;
}



//四舍五入函数。
//参数pri  为要转换的浮点数指针。
//参数weisu  为小数点后的位数。
void my_round(float *pri,char weisu)
{
	float temp;
	char minwei;
	long tt;

	temp=*pri;
	for(minwei=0;minwei<weisu+1;minwei++){temp*=10;}

	tt=(long)(temp);
	minwei=tt%10;
	if(minwei>=5){
		if(*pri>0)tt+=10;
		else if(*pri<0)tt-=10;
	}
	
	temp=(float)(tt/10);
	for(minwei=0;minwei<weisu;minwei++){temp/=10;}

	*pri=temp;
}


//把ascii  码转为BCD  码，ascii  码缓冲区中有单数和双数之分，
//如果是单数便会把第一个ascii  码转为一个字节，例如:
//"1234567", 转到bcd  缓冲区便成了，0x01,0x23,0x45,0x67。如果是
//"12345678", 转到bcd  缓冲区便成了，0x12,0x34,0x56,0x78。
//这两返回值都是4 。注意不对缓冲区做越界检查
unsigned char my_AscciiToBCD(unsigned char *AscciiBuf,unsigned char *BcdBuf)
{
	unsigned char *ptemp;
	unsigned char len;

	ptemp=AscciiBuf;
	while(*ptemp>='0'&&*ptemp<='9')ptemp++;
	len=(ptemp-AscciiBuf)/2;
	if((ptemp-AscciiBuf)%2==1){
		len++;
		*BcdBuf=CharMapHex[*AscciiBuf];
		BcdBuf++;AscciiBuf++;
	}

	while(*AscciiBuf>='0'&&*AscciiBuf<='9'){
		*BcdBuf=(CharMapHex[*AscciiBuf]<<4)+CharMapHex[*(AscciiBuf+1)];
		BcdBuf+=1;AscciiBuf+=2;
	}

	return len;
}


void my_memcpy_space(unsigned char *p,unsigned char len)
{
	for(;len!=0;len--){
		*p=' ';
		p++;
	}
}


//delet all c charactor of buff
void my_strdel(unsigned char *buff,unsigned char c)
{
	unsigned char *p;

	p=buff;
	while(*p!=0){
		if(*p==c)p++;
		else{
			*buff=*p;
			buff++;p++;
		}
	}
	*buff=0;
}


//删除所有非数字(即非0-9)的字符
void my_strdel_allNonnum(unsigned char *buff)
{
	unsigned char *p;

	p=buff;
	while(*p!=0){
		if(*p<'0'||*p>'9')p++;
		else{
			*buff=*p;
			buff++;p++;
		}
	}
	*buff=0;
}


//如果buff  中全是c  则返回1，
//否则返回0。
//CheckLen 为检查buff  的长度
//超过部分不检查
unsigned char my_CheckBuffIsAllLetter(unsigned char *buff,unsigned char c,unsigned char CheckLen)
{
	if(buff==(void *)0)return 0;

	for(;CheckLen!=0&&*buff!=0;CheckLen--){
		if(*buff!=c)return 0;
		buff++;
	}

	return 1;
}


//如果buff  中有c  则返回1，
//否则返回0。
//CheckLen 为检查buff  的长度
//超过部分不检查
unsigned char my_CheckBuffIsContainLetter(unsigned char *buff,unsigned char c,unsigned char CheckLen)
{
	for(;CheckLen!=0;CheckLen--){
		if(*buff==c)return 1;
		buff++;
	}

	return 0;
}

void my_string_delete_redundantbyte(unsigned char *buff)
{
	while(*buff!='\0'){
		if(*buff=='.'){
			buff+=3;
			*buff=0;
			break;
		}
		else buff++;
	}
}

//参数Postion，要插入的位置
//参数insertStr，要插入的字符串
//注意必须保证Postion  有足够的空间容纳insertStr
void my_string_insert(unsigned char *Postion,const unsigned char *insertStr)
{
	unsigned char *pMove,*pCopy;
	unsigned int len;
	const unsigned char *ptemp;

	if(Postion==(void *)0||insertStr==(void *)0)return;

	for(ptemp=insertStr,len=0;*ptemp!='\0';ptemp++)len++;

	pMove=Postion;
	while(*pMove!='\0')pMove++;
	pCopy=pMove+len;

	while(pMove!=Postion){
		*pCopy=*pMove;
		pMove--;
		pCopy--;
	}
	*pCopy=*pMove;

	pMove=Postion;
	while(*insertStr!=0){
		*pMove=*insertStr;
		pMove++;
		insertStr++;
	}
}

#if 0
//参数Postion，要插入的位置
//参数insertStr，要插入的hex串
//注意必须保证Postion  有足够的空间容纳insertHex
void my_hex_insert(unsigned char *Postion,const unsigned char *insertHex,unsigned char insertlen)
{
	unsigned char *pMove,*pCopy;
	unsigned int len;
	//const unsigned char *ptemp;

	if(Postion==(void *)0||insertHex==(void *)0)return;

	//for(ptemp=insertHex,len=0;*ptemp!='\0';ptemp++)len++;
	len=insertlen;

	pMove=Postion;
	while(*pMove!='\0')pMove++;
	pCopy=pMove+len;

	while(pMove!=Postion){
		*pCopy=*pMove;
		pMove--;
		pCopy--;
	}
	*pCopy=*pMove;

	pMove=Postion;
	while(insertlen!=0){
		*pMove=*insertHex;
		pMove++;
		insertHex++;
		insertlen--;
	}
}
#endif


//删除to  到from-1 一段空间
void my_string_delete(unsigned char *to,const unsigned char *from)
{
	while(*from!=0){
		*to=*from;
		to++;
		from++;
	}

	while(to!=from){
		*to=0;
		to++;
	}
}

//把Buff  中的所有c1  替换为c2
void my_string_replace(unsigned char *Buff,unsigned char c1,unsigned char c2)
{
	if(Buff==(void *)0)return;

	while(*Buff!=0){
		if(*Buff==c1)*Buff=c2;
		Buff++;
	}
}


//float 型转unsigned int 类型
//只要有小数位，便+ 1  取整返回，比如1.00001 返回2.
unsigned int my_FloatConvertToInt(float f)
{
	unsigned int retval;

	retval=(unsigned int)(f);

	if((float)(retval)==f)return retval;
	else return retval+1;
}

unsigned char my_CheckSum(unsigned char * buff,unsigned int len)
{
	unsigned char checksum=0;

	while(len!=0){
		checksum+=*buff;
		buff++;
		len--;
	}

	return checksum;
}

//delet all c charactor of buff
void my_strdel_lessHex(unsigned char *buff,unsigned char hex)
{
	unsigned char *p;

	p=buff;
	while(*p!=0){
		if(*p<hex)p++;
		else{
			*buff=*p;
			buff++;p++;
		}
	}
	*buff=0;
}

unsigned int my_axtoi(unsigned char* p)
{
	unsigned int len=0;

	while(1){
		if(*p>='0'&&*p<='9'){len<<=4;len+=*p-'0';}
		else if(*p>='a'&&*p<='f'){len<<=4;len+=*p-'a'+10;}
		else if(*p>='A'&&*p<='F'){len<<=4;len+=*p-'A'+10;}
		else break;
		
		p++;
	}

	return len;
}







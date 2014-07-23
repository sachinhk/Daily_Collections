
#include "sim900B.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "mypure_library.h"
#include "gprs.h"
#include "global_extern_variable.h"


#define MyChinaSelfTest


#define PDUsendMode7bit		1
#define PDUsendMode8bit		2
#define PDUsendModeUCS2		3



//把8613693092030  转成68 31 96 03 29 30 F0  之类
static void HaomaToMassageCode(char *temp,const char *pt)
{
	char t;

	while(1){
		if(*pt>='0'&&*pt<='9'){*(temp+1)=*pt;temp++;pt++;t=1;}
		else{break;}
		if(*pt>='0'&&*pt<='9'){*(temp-1)=*pt;temp++;pt++;t=0;}
		else{break;}
	}

	if(t){*(temp-1)='F';temp++;}
	*temp=0;
}


//把68 31 96 03 29 30 F0  转成8613693092030  之类
static void MassageCodeToHaoma(unsigned char **pt,char *PhoneNumber,char len)//68 31 96 03 29 30 F0
{
	for(;len>0;len--){
		if(**pt!='F')*(PhoneNumber+1)=**pt;
		else{*(PhoneNumber+1)=0;}
		PhoneNumber++;
		(*pt)++;

		*(PhoneNumber-1)=**pt;
		PhoneNumber++;
		(*pt)++;
	}

	*PhoneNumber=0;
}




//把一个16  进制数转成两位的ASCII  码
//如:  把0x34  转成"34"
static void OneHexToTwoAsccii(char **ps,char hex)
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
static void TwoAscciiToOneHex(unsigned char **pt,char *len)
{
	char temp=0;

	temp=CharMapHex[**pt];
	(*pt)++;
	temp<<=4;

	temp+=CharMapHex[**pt];
	(*pt)++;

	*len=temp;
}


//检查buff  中是不是全是ASCII  码
static char CheckBuffWhetherAllisAsccii(const char *buff)
{
	while(*buff!=0){
		if(*buff>=0x80)return PDUsendModeUCS2;
		buff++;
	}

	return PDUsendMode7bit;
}


//7bit  PDU  模式发送短信编码
static void PDU7bitEncode(char *buff)
{
	char *pbuff=buff;
	char temp,i=0;
	char tp[512];	//用户信息(TP-UD)段最大容量是140 字节，所以在UCS2 编码方式下，可发送短消息的最大字符数是70 个
	char *ptp;

	ptp=tp;
	while(*pbuff!=0){
		if(*(pbuff+1)!=0){
			temp=*(pbuff+1)<<(7-i);
		}else{
			temp=0;
		}

		temp=*pbuff>>i|temp;
		*ptp=HexMapChar[temp>>4];ptp++;
		*ptp=HexMapChar[temp&0x0f];ptp++;
		
		i++;
		if(i>=7){i=0;pbuff++;}
		pbuff++;
	}
	*ptp=0;

	strcpy(buff,tp);
}


/*
这个函数要实现把一个byte 的ASCII  码转换成两个UCS2，
再把两个byte的ucs2转换成4个ASCII码表示的16进制数
例如把'a'  转换成0x0061,  再把0x0061转换成"0061"
汉字一样例如把"你好(0xc4e3,0xbac3)"--->0x4f60,0x597d--->"4f60597d"
*/
static void AsciiGB2312ToUCS2(char *tempbuff,char *Msg)//内码到UCS2  转换
{
	#ifdef HaveIconvLib
	iconv_t cd;
	char outbuf[3];
	char inbuf[3]={0x00,0x00,0x00};
	size_t outlen;
	char *inptr;
	size_t inlen;
	char *outptr;
	size_t n;

	#ifdef DEBUG_PRINTF
	char *pp=tempbuff;
	#endif
	cd=iconv_open("UCS-2BE","GB2312");
	if (cd == (iconv_t) -1){
		#ifdef DEBUG_PRINTF
		printf("iconv_open failed\n");
		#endif
		perror("error:");
		return;
	}

	while(*Msg!=0){
		if(*Msg<0x80){	//如果是ASCCII 码
			OneHexToTwoAsccii(&tempbuff,0x00);
			OneHexToTwoAsccii(&tempbuff,*Msg);
		}else{			//如果是汉字
			inbuf[0]=*Msg;Msg++;
			inbuf[1]=*Msg;
			outlen=3;outptr=outbuf;
			inlen =3;inptr=inbuf;
			n = iconv(cd, &inptr, &inlen, &outptr, &outlen);
			OneHexToTwoAsccii(&tempbuff,outbuf[0]);
			OneHexToTwoAsccii(&tempbuff,outbuf[1]);
			if (n == (size_t) -1)
			#ifdef DEBUG_PRINTF
			printf ("iconv() returned error: %m\n");
			else
			printf ("iconv() returned %Zd, expected 7\n", n);
			#else
			;
			#endif
		}
		Msg++;
	}

	*tempbuff=0;
	iconv_close(cd);

	#ifdef DEBUG_PRINTF
	printf("AsciiGB2312ToUCS2=%s\n",pp);
	#endif
	#endif
}


//短信PDU  模式ucs2  解码
static void GetContext_UCS2(unsigned char **pt,char *context,char len)
{
	#ifdef HaveIconvLib
	iconv_t cd;
	char outbuf[3];
	char inbuf[3]={0x00,0x00,0x00};
	size_t outlen;
	char *inptr;
	size_t inlen;
	char *outptr;
	size_t n;

	#ifdef DEBUG_PRINTF
	char *pp=context;
	#endif
	cd=iconv_open("GB2312","UCS-2BE");
	if (cd == (iconv_t) -1){
		#ifdef DEBUG_PRINTF
		printf("iconv_open failed\n");
		#endif
		perror("error:");
		return;
	}

	for(;len>0;len--){		//ucs2 PDU  模式的len  是十六进制数的个数，但每个十六进制用两个ASCCII  码表示
		TwoAscciiToOneHex(pt,&inbuf[0]);
		TwoAscciiToOneHex(pt,&inbuf[1]);
		if(inbuf[0]==0){			//ucs2   Ascii  码
			*context=inbuf[1];context++;
		}else{			//ucs2  汉字
			outlen=3;outptr=outbuf;
			inlen =3;inptr=inbuf;
			n = iconv(cd, &inptr, &inlen, &outptr, &outlen);
			*context=outbuf[0];context++;
			*context=outbuf[1];context++;
			if (n == (size_t) -1)
			#ifdef DEBUG_PRINTF
			printf ("iconv() returned error: %m\n");
			else
			printf ("iconv() returned %Zd, expected 7\n", n);
			#else
			;
			#endif
		}
	}

	*context=0;
	iconv_close(cd);

	#ifdef DEBUG_PRINTF
	printf("GetContext_UCS2=%s\n",pp);
	#endif
	#endif
}


//短信PDU  模式7bit  解码
static void GetContext_7bit(unsigned char **pt,char *context,char len)
{
	unsigned char *tp,*tt;
	char temp=0,i=0,j;

	tt=tp=*pt;
	while(**pt>' '&&**pt<0x80){
		TwoAscciiToOneHex(pt,(char *)tp);
		tp++;
	}

	for(j=0;j<len;j++){		//7bit  PDU  模式的len  就是ASCCII  码的个数
		*context=(*tt<<i|temp)&0x7f;
		temp=*tt>>(7-i);
		tt++;
		context++;
		i++;
		if(i>=7){
			*context=temp&0x7f;
			context++;
			i=0;
			temp=0;
			j++;
		}
	}

	*context=0;
}


//短信PDU  模式8bit  解码
static void GetContext_8bit(unsigned char **pt,char *context,char len)//短信PDU  模式8bit  解码
{
	
}


//发送短信
char SendMessage(char *Msg,const char *Phone)
{
	char sendbuff[1024];
	char tempbuff[1024];
	char temp[200];
	char *pt,*ps;
	char len;
	char PDUsendMode;


	PDUsendMode=CheckBuffWhetherAllisAsccii(Msg);

	memset(sendbuff,0,sizeof(sendbuff));
	ps=sendbuff;

	memset(tempbuff,0,sizeof(tempbuff));	//设置PDU mode 
	send_at_cmd("AT+CMGF=0\r");
	if(ReadDatasFromSIM300_OKstr((unsigned char *)tempbuff,sizeof(tempbuff),3,"OK\r")!=0){goto tuichu;}

	memset(tempbuff,0,sizeof(tempbuff));	//取短信中心号码
	send_at_cmd("AT+CSCA?\r");
	if(ReadDatasFromSIM300_OKstr((unsigned char *)tempbuff,sizeof(tempbuff),3,"OK\r")!=0){goto tuichu;}
	pt=strstr(tempbuff,"+CSCA: \"+");
	if(pt!=NULL)pt+=sizeof("+CSCA: \"+")-1;
	else goto tuichu;
	HaomaToMassageCode(temp,pt);
	OneHexToTwoAsccii(&ps,strlen(temp)/2+1);		//A：短信息中心地址长度，2位十六进制数(1字节)。
	OneHexToTwoAsccii(&ps,0x91);					//B：短信息中心号码类型，2位十六进制数。
	My_strcpy_PointInc(&ps,temp);					//C：短信息中心号码，B+C的长度将由A中的数据决定。
	len=strlen(sendbuff);

	OneHexToTwoAsccii(&ps,0x11);					//D：文件头字节，2位十六进制数。
	OneHexToTwoAsccii(&ps,0x00);					//E：信息类型，2位十六进制数。
	OneHexToTwoAsccii(&ps,strlen(Phone)+2);			//F：被叫号码长度，2位十六进制数。
	OneHexToTwoAsccii(&ps,0x91);					//G：被叫号码类型，2位十六进制数，取值同B。

	#ifdef MyChinaSelfTest
	OneHexToTwoAsccii(&ps,0x68);
	#else
	OneHexToTwoAsccii(&ps,0x34);	//+4369910797801奥地利国际区号
	#endif

	HaomaToMassageCode(temp,Phone);
	My_strcpy_PointInc(&ps,temp);					//H：被叫号码，长度由F中的数据决定。
	OneHexToTwoAsccii(&ps,0x00);					//I：协议标识，2位十六进制数。

	if(PDUsendMode==PDUsendMode7bit){				//J：数据编码方案，2位十六进制数。08 用户信息编码方式(TP-DCS) UCS2编码
		OneHexToTwoAsccii(&ps,0x00);
	}else if(PDUsendMode==PDUsendMode8bit){
		OneHexToTwoAsccii(&ps,0x04);
	}else if(PDUsendMode==PDUsendModeUCS2){
		OneHexToTwoAsccii(&ps,0x08);
	}

	OneHexToTwoAsccii(&ps,0x00);					//K：有效期，2位十六进制数。

	if(PDUsendMode==PDUsendMode7bit){
		strcpy(tempbuff,Msg);
		OneHexToTwoAsccii(&ps,strlen(tempbuff));	//L：用户数据长度，2位十六进制数。
		PDU7bitEncode(tempbuff);					//M：用户数据，其长度由L中的数据决定。J中设定采用UCS2编码，这里是中英文的Unicode字符。
		My_strcpy_PointInc(&ps,tempbuff);
	}else if(PDUsendMode==PDUsendMode8bit){

	}else if(PDUsendMode==PDUsendModeUCS2){
		AsciiGB2312ToUCS2(tempbuff,Msg);
		OneHexToTwoAsccii(&ps,strlen(tempbuff)/2);	//L：用户数据长度，2位十六进制数。
		My_strcpy_PointInc(&ps,tempbuff);			//M：用户数据，其长度由L中的数据决定。J中设定采用UCS2编码，这里是中英文的Unicode字符。
	}

	My_strcpy_PointInc(&ps,"\x1a");					//加上0x1A

	ps=temp;
	len=(strlen(sendbuff)-len-1)/2;
	My_strcpy_PointInc(&ps,"AT+CMGS=");	//以PDU  模式开始发送短信
	sprintf(ps,"%d",len);
	strcat(temp,"\r");
	send_at_cmd((const unsigned char*)temp);
	memset(tempbuff,0,sizeof(tempbuff));
	ReadDatasFromSIM300_OKstr((unsigned char *)tempbuff,sizeof(tempbuff)-1024,2,">");
	if(strstr(tempbuff,">")){
		send_at_cmd((const unsigned char *)sendbuff);
		if(ReadDatasFromSIM300_OKstr((unsigned char *)tempbuff,sizeof(tempbuff)-1024,8,"OK\r")!=0){goto tuichu;}
	}else goto tuichu;
	return Sim900B_ProccessOK;


	tuichu:
	return Sim900B_ProccessERROR;
}


//成功发送返回0，错误返回1
unsigned char SendMessage_TextMode(const unsigned char *Context,const unsigned char *PhoneNumber)
//unsigned char SendMessage_TextMode(const unsigned char *Context,const unsigned char *PhoneNumber)
{
	unsigned char retval;
	unsigned char tempbuff[30];

	retval=SendAtCommand_UntillSuccess("AT+CMGF=1\r");
	if(retval==SendAtCommand_ERROR)return 1;

	strcpy((char *)tempbuff,"AT+CMGS=\"");	//AT+CMGS="15112475662"\r
	strcat((char *)tempbuff,(const char *)PhoneNumber);
	strcat((char *)tempbuff,"\"\r");
	retval=send_at_cmd_and_receive(tempbuff,sizeof(tempbuff)-1,2000,">");
	printf("SendAtCmdre=%s\r\n",tempbuff);
	if(retval!=TCP_OK)return 1;

	send_at_cmd(Context);
	send_at_cmd("\x1a");

	retval=ReadDatasFromSIM300_OKstr(tempbuff,sizeof(tempbuff)-1,10,"OK");
	printf("SendAtCmdre=%s\r\n",tempbuff);

	return retval;
}


//只要解出号码就返回
char ReceiveMessageDecode_onlyPhoneNumber(const char *codebuff,char *PhoneNumber,const char *type)
{
	unsigned char *pt;
	char len;
	//char temp[100];
	//char EncodeMode;

	#ifdef DEBUG_PRINTF
	char *pph;
	pph=PhoneNumber;
	#endif

	pt=(unsigned char *)strstr(codebuff,type);	//让pt  指向089168  的0位置。
	if(pt==NULL)goto tuichu;
	pt=(unsigned char *)strstr((char const*)pt,"\n");
	if(pt==NULL)goto tuichu;
	while(*pt<'0'||*pt>0x80)pt++;

	TwoAscciiToOneHex(&pt,&len);				//08 地址信息的长度 个八位字节(包括91)
	pt+=len*2;									//91 SMSC地址格式(TON/NPI) 用国际格式号码(在前面加‘+’)
	//68 31 08 20 05 05 F0 SMSC地址 8613800250500，补‘F’凑成偶数个
	pt+=2;										//84 基本参数(TP-MTI/MMS/RP) 接收，无更多消息，有回复地址
	TwoAscciiToOneHex(&pt,&len);				//0D 回复地址数字个数 共13个十进制数(不包括91和‘F’)
	pt+=2;										//91 回复地址格式(TON/NPI) 用国际格式号码(在前面加‘+’)
	if(len%2)len++;
	len/=2;
	MassageCodeToHaoma(&pt,PhoneNumber,len);	//68 31 96 03 29 30 F0 回复地址(TP-RA) 8613693092030，补‘F’凑成偶数个

	return Sim900B_ProccessOK;

	tuichu:
	#ifdef DEBUG_PRINTF
	printf("Message decode faulire!\n");
	#endif
	return Sim900B_ProccessERROR;
}


//接收短信数据流解码
char ReceiveMessageDecode(const char *codebuff,char *PhoneNumber,char *time,char *context,const char *type)
{
	unsigned char *pt;
	char len;
	char temp[100];
	char EncodeMode;
	char SMS_DELIVER;

	#ifdef DEBUG_PRINTF
	char *pph,*pti,*pco;
	pph=PhoneNumber;
	pti=time;
	pco=context;
	#endif

	pt=(unsigned char *)strstr(codebuff,type);	//让pt  指向089168  的0位置。
	if(pt==NULL)goto tuichu;
	pt=(unsigned char *)strstr((char const*)pt,"\n");
	if(pt==NULL)goto tuichu;
	while(*pt<'0'||*pt>0x80)pt++;

	TwoAscciiToOneHex(&pt,&len);				//08 地址信息的长度 个八位字节(包括91)
	pt+=len*2;									//91 SMSC地址格式(TON/NPI) 用国际格式号码(在前面加‘+’)
	//68 31 08 20 05 05 F0 SMSC地址 8613800250500，补‘F’凑成偶数个
	TwoAscciiToOneHex(&pt,&SMS_DELIVER);		//84 基本参数(TP-MTI/MMS/RP) 接收，无更多消息，有回复地址
	TwoAscciiToOneHex(&pt,&len);				//0D 回复地址数字个数 共13个十进制数(不包括91和‘F’)
	pt+=2;										//91 回复地址格式(TON/NPI) 用国际格式号码(在前面加‘+’)
	if(len%2)len++;
	len/=2;
	MassageCodeToHaoma(&pt,PhoneNumber,len);	//68 31 96 03 29 30 F0 回复地址(TP-RA) 8613693092030，补‘F’凑成偶数个
	pt+=2;										//00 协议标识(TP-PID) 是普通GSM类型，点到点方式
	TwoAscciiToOneHex(&pt,&EncodeMode);			//08 用户信息编码方式(TP-DCS) UCS2编码
	MassageCodeToHaoma(&pt,temp,7);				//30 30 21 80 63 54 80 时间戳(TP-SCTS) 2003-3-12 08:36:45  +8时区

	strcpy(time,"20");time+=2;
	memcpy(time,temp,2);time+=2;
	memcpy(time,"-",1);time+=1;
	memcpy(time,temp+2,2);time+=2;
	memcpy(time,"-",1);time+=1;
	memcpy(time,temp+4,2);time+=2;
	memcpy(time," ",1);time+=1;
	memcpy(time,temp+6,2);time+=2;
	memcpy(time,":",1);time+=1;
	memcpy(time,temp+8,2);time+=2;
	memcpy(time,":",1);time+=1;
	memcpy(time,temp+10,2);time+=2;
	*time=0;

	TwoAscciiToOneHex(&pt,&len);				//06 用户信息长度(TP-UDL) 实际长度6个字节
	if(SMS_DELIVER&0x40){
		TwoAscciiToOneHex(&pt,&temp[0]);		//用户短信头长度
		pt+=temp[0]*2;len-=temp[0];
	}

	if(EncodeMode==0x08){		//UCS2  编码
		GetContext_UCS2(&pt,context,len);		//4F 60 59 7D 00 21 用户信息(TP-UD) “你好!”
	}else if(EncodeMode==0x04){		//8-bit编码
		GetContext_8bit(&pt,context,len);
	}else if(EncodeMode==0x00){		//7-bit编码
		GetContext_7bit(&pt,context,len);
	}else goto tuichu;

	#ifdef DEBUG_PRINTF
	printf("pph=%s\r\n",pph);
	printf("pti=%s\r\n",pti);
	printf("pco=%s\r\n",pco);
	#endif

	return Sim900B_ProccessOK;

	tuichu:
	#ifdef DEBUG_PRINTF
	printf("Message decode faulire!\n");
	#endif
	return Sim900B_ProccessERROR;
}





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



//��8613693092030  ת��68 31 96 03 29 30 F0  ֮��
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


//��68 31 96 03 29 30 F0  ת��8613693092030  ֮��
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




//��һ��16  ������ת����λ��ASCII  ��
//��:  ��0x34  ת��"34"
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


//����λ��ASCII  ��ת��һ��16  ������
//��:  ��"34"  ת��0x34  
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


//���buff  ���ǲ���ȫ��ASCII  ��
static char CheckBuffWhetherAllisAsccii(const char *buff)
{
	while(*buff!=0){
		if(*buff>=0x80)return PDUsendModeUCS2;
		buff++;
	}

	return PDUsendMode7bit;
}


//7bit  PDU  ģʽ���Ͷ��ű���
static void PDU7bitEncode(char *buff)
{
	char *pbuff=buff;
	char temp,i=0;
	char tp[512];	//�û���Ϣ(TP-UD)�����������140 �ֽڣ�������UCS2 ���뷽ʽ�£��ɷ��Ͷ���Ϣ������ַ�����70 ��
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
�������Ҫʵ�ְ�һ��byte ��ASCII  ��ת��������UCS2��
�ٰ�����byte��ucs2ת����4��ASCII���ʾ��16������
�����'a'  ת����0x0061,  �ٰ�0x0061ת����"0061"
����һ�������"���(0xc4e3,0xbac3)"--->0x4f60,0x597d--->"4f60597d"
*/
static void AsciiGB2312ToUCS2(char *tempbuff,char *Msg)//���뵽UCS2  ת��
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
		if(*Msg<0x80){	//�����ASCCII ��
			OneHexToTwoAsccii(&tempbuff,0x00);
			OneHexToTwoAsccii(&tempbuff,*Msg);
		}else{			//����Ǻ���
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


//����PDU  ģʽucs2  ����
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

	for(;len>0;len--){		//ucs2 PDU  ģʽ��len  ��ʮ���������ĸ�������ÿ��ʮ������������ASCCII  ���ʾ
		TwoAscciiToOneHex(pt,&inbuf[0]);
		TwoAscciiToOneHex(pt,&inbuf[1]);
		if(inbuf[0]==0){			//ucs2   Ascii  ��
			*context=inbuf[1];context++;
		}else{			//ucs2  ����
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


//����PDU  ģʽ7bit  ����
static void GetContext_7bit(unsigned char **pt,char *context,char len)
{
	unsigned char *tp,*tt;
	char temp=0,i=0,j;

	tt=tp=*pt;
	while(**pt>' '&&**pt<0x80){
		TwoAscciiToOneHex(pt,(char *)tp);
		tp++;
	}

	for(j=0;j<len;j++){		//7bit  PDU  ģʽ��len  ����ASCCII  ��ĸ���
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


//����PDU  ģʽ8bit  ����
static void GetContext_8bit(unsigned char **pt,char *context,char len)//����PDU  ģʽ8bit  ����
{
	
}


//���Ͷ���
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

	memset(tempbuff,0,sizeof(tempbuff));	//����PDU mode 
	send_at_cmd("AT+CMGF=0\r");
	if(ReadDatasFromSIM300_OKstr((unsigned char *)tempbuff,sizeof(tempbuff),3,"OK\r")!=0){goto tuichu;}

	memset(tempbuff,0,sizeof(tempbuff));	//ȡ�������ĺ���
	send_at_cmd("AT+CSCA?\r");
	if(ReadDatasFromSIM300_OKstr((unsigned char *)tempbuff,sizeof(tempbuff),3,"OK\r")!=0){goto tuichu;}
	pt=strstr(tempbuff,"+CSCA: \"+");
	if(pt!=NULL)pt+=sizeof("+CSCA: \"+")-1;
	else goto tuichu;
	HaomaToMassageCode(temp,pt);
	OneHexToTwoAsccii(&ps,strlen(temp)/2+1);		//A������Ϣ���ĵ�ַ���ȣ�2λʮ��������(1�ֽ�)��
	OneHexToTwoAsccii(&ps,0x91);					//B������Ϣ���ĺ������ͣ�2λʮ����������
	My_strcpy_PointInc(&ps,temp);					//C������Ϣ���ĺ��룬B+C�ĳ��Ƚ���A�е����ݾ�����
	len=strlen(sendbuff);

	OneHexToTwoAsccii(&ps,0x11);					//D���ļ�ͷ�ֽڣ�2λʮ����������
	OneHexToTwoAsccii(&ps,0x00);					//E����Ϣ���ͣ�2λʮ����������
	OneHexToTwoAsccii(&ps,strlen(Phone)+2);			//F�����к��볤�ȣ�2λʮ����������
	OneHexToTwoAsccii(&ps,0x91);					//G�����к������ͣ�2λʮ����������ȡֵͬB��

	#ifdef MyChinaSelfTest
	OneHexToTwoAsccii(&ps,0x68);
	#else
	OneHexToTwoAsccii(&ps,0x34);	//+4369910797801�µ�����������
	#endif

	HaomaToMassageCode(temp,Phone);
	My_strcpy_PointInc(&ps,temp);					//H�����к��룬������F�е����ݾ�����
	OneHexToTwoAsccii(&ps,0x00);					//I��Э���ʶ��2λʮ����������

	if(PDUsendMode==PDUsendMode7bit){				//J�����ݱ��뷽����2λʮ����������08 �û���Ϣ���뷽ʽ(TP-DCS) UCS2����
		OneHexToTwoAsccii(&ps,0x00);
	}else if(PDUsendMode==PDUsendMode8bit){
		OneHexToTwoAsccii(&ps,0x04);
	}else if(PDUsendMode==PDUsendModeUCS2){
		OneHexToTwoAsccii(&ps,0x08);
	}

	OneHexToTwoAsccii(&ps,0x00);					//K����Ч�ڣ�2λʮ����������

	if(PDUsendMode==PDUsendMode7bit){
		strcpy(tempbuff,Msg);
		OneHexToTwoAsccii(&ps,strlen(tempbuff));	//L���û����ݳ��ȣ�2λʮ����������
		PDU7bitEncode(tempbuff);					//M���û����ݣ��䳤����L�е����ݾ�����J���趨����UCS2���룬��������Ӣ�ĵ�Unicode�ַ���
		My_strcpy_PointInc(&ps,tempbuff);
	}else if(PDUsendMode==PDUsendMode8bit){

	}else if(PDUsendMode==PDUsendModeUCS2){
		AsciiGB2312ToUCS2(tempbuff,Msg);
		OneHexToTwoAsccii(&ps,strlen(tempbuff)/2);	//L���û����ݳ��ȣ�2λʮ����������
		My_strcpy_PointInc(&ps,tempbuff);			//M���û����ݣ��䳤����L�е����ݾ�����J���趨����UCS2���룬��������Ӣ�ĵ�Unicode�ַ���
	}

	My_strcpy_PointInc(&ps,"\x1a");					//����0x1A

	ps=temp;
	len=(strlen(sendbuff)-len-1)/2;
	My_strcpy_PointInc(&ps,"AT+CMGS=");	//��PDU  ģʽ��ʼ���Ͷ���
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


//�ɹ����ͷ���0�����󷵻�1
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


//ֻҪ�������ͷ���
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

	pt=(unsigned char *)strstr(codebuff,type);	//��pt  ָ��089168  ��0λ�á�
	if(pt==NULL)goto tuichu;
	pt=(unsigned char *)strstr((char const*)pt,"\n");
	if(pt==NULL)goto tuichu;
	while(*pt<'0'||*pt>0x80)pt++;

	TwoAscciiToOneHex(&pt,&len);				//08 ��ַ��Ϣ�ĳ��� ����λ�ֽ�(����91)
	pt+=len*2;									//91 SMSC��ַ��ʽ(TON/NPI) �ù��ʸ�ʽ����(��ǰ��ӡ�+��)
	//68 31 08 20 05 05 F0 SMSC��ַ 8613800250500������F���ճ�ż����
	pt+=2;										//84 ��������(TP-MTI/MMS/RP) ���գ��޸�����Ϣ���лظ���ַ
	TwoAscciiToOneHex(&pt,&len);				//0D �ظ���ַ���ָ��� ��13��ʮ������(������91�͡�F��)
	pt+=2;										//91 �ظ���ַ��ʽ(TON/NPI) �ù��ʸ�ʽ����(��ǰ��ӡ�+��)
	if(len%2)len++;
	len/=2;
	MassageCodeToHaoma(&pt,PhoneNumber,len);	//68 31 96 03 29 30 F0 �ظ���ַ(TP-RA) 8613693092030������F���ճ�ż����

	return Sim900B_ProccessOK;

	tuichu:
	#ifdef DEBUG_PRINTF
	printf("Message decode faulire!\n");
	#endif
	return Sim900B_ProccessERROR;
}


//���ն�������������
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

	pt=(unsigned char *)strstr(codebuff,type);	//��pt  ָ��089168  ��0λ�á�
	if(pt==NULL)goto tuichu;
	pt=(unsigned char *)strstr((char const*)pt,"\n");
	if(pt==NULL)goto tuichu;
	while(*pt<'0'||*pt>0x80)pt++;

	TwoAscciiToOneHex(&pt,&len);				//08 ��ַ��Ϣ�ĳ��� ����λ�ֽ�(����91)
	pt+=len*2;									//91 SMSC��ַ��ʽ(TON/NPI) �ù��ʸ�ʽ����(��ǰ��ӡ�+��)
	//68 31 08 20 05 05 F0 SMSC��ַ 8613800250500������F���ճ�ż����
	TwoAscciiToOneHex(&pt,&SMS_DELIVER);		//84 ��������(TP-MTI/MMS/RP) ���գ��޸�����Ϣ���лظ���ַ
	TwoAscciiToOneHex(&pt,&len);				//0D �ظ���ַ���ָ��� ��13��ʮ������(������91�͡�F��)
	pt+=2;										//91 �ظ���ַ��ʽ(TON/NPI) �ù��ʸ�ʽ����(��ǰ��ӡ�+��)
	if(len%2)len++;
	len/=2;
	MassageCodeToHaoma(&pt,PhoneNumber,len);	//68 31 96 03 29 30 F0 �ظ���ַ(TP-RA) 8613693092030������F���ճ�ż����
	pt+=2;										//00 Э���ʶ(TP-PID) ����ͨGSM���ͣ��㵽�㷽ʽ
	TwoAscciiToOneHex(&pt,&EncodeMode);			//08 �û���Ϣ���뷽ʽ(TP-DCS) UCS2����
	MassageCodeToHaoma(&pt,temp,7);				//30 30 21 80 63 54 80 ʱ���(TP-SCTS) 2003-3-12 08:36:45  +8ʱ��

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

	TwoAscciiToOneHex(&pt,&len);				//06 �û���Ϣ����(TP-UDL) ʵ�ʳ���6���ֽ�
	if(SMS_DELIVER&0x40){
		TwoAscciiToOneHex(&pt,&temp[0]);		//�û�����ͷ����
		pt+=temp[0]*2;len-=temp[0];
	}

	if(EncodeMode==0x08){		//UCS2  ����
		GetContext_UCS2(&pt,context,len);		//4F 60 59 7D 00 21 �û���Ϣ(TP-UD) �����!��
	}else if(EncodeMode==0x04){		//8-bit����
		GetContext_8bit(&pt,context,len);
	}else if(EncodeMode==0x00){		//7-bit����
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






#include "gprs.h"

#include "stm32f10x_gpio.h"
//#include "key.h"
#include "kmy_Time.h"
#include "kmy_USART3Drv.h"
#include <string.h>
//#include "ShuRuFa.h"
#include <stdlib.h>
#include <stdio.h>
//#include "mypure_library.h"
#include "kmy_LCDDrv.h"
#include "kmy_KeyDrv.h"
#include "kmy_EEPROMDrv.h"
#include "Common.h"
#include "Input.h"
#include "kmy_WiFi_SPI.h"
#include "kmy_WiFi_USARTDrv.h"




#define		PinGprsKey		GPIO_Pin_10
#define		PinGprsPWR		GPIO_Pin_11

#define IniStartAddress 	0
#define IniMaxSize 			300
#define pos_ini_max 14

extern uint32_t FlashDestination;
extern uint32_t BlockNbr;
extern uint32_t UserMemoryMask;

__IO uint32_t FlashProtection = 0;

uint8_t IniTempBuf[IniMaxSize] = {0};



static char APN[50 + 1]={0};
	   char NetworkSwitch[2]={0};
       char Upgrade_Flag[2]={0};
static char ApnUsername[21 + 1]={0};
static char ApnPassword[21 + 1]={0};
static char IPandPort[28]={0};
//static char VersionURL[150]={0};
static char UpgradeURL[101]={0};



//char gPublicBuff[200];		//此处申请一个公用大buff
unsigned char pBuff[2048] = {0};
#define AT_CheckCPIN						"AT+CPIN?\r"
#define AT_CPIN_efood						"AT+CPIN=\"7533\"\r"

//define AT commands
#define AT_ATE0								"ATE0\r"
#define AT_SearchingForNetworks				"AT+COPS?\r"
#define AT_SignalStrength					"AT+CSQ\r"
#define AT_ADC								"AT+QADC?\r"
#define AT_GetIMEI							"AT+GSN\r"
#define AT_TcpSetApn						"AT+QIREGAPP=\""
#define AT_TcpHeadOff						"AT+QIHEAD=0\r"
#define AT_TcpConnect						"AT+QIOPEN=\"TCP\",\""
#define AT_TcpSend							"AT+QISEND="
#define AT_TcpClose1						"AT+QICLOSE\r"
#define AT_TcpClose2						"AT+QIDEACT\r"
#define AT_DialInit							"AT+COLP=1\r"
#define AT_Dial								"ATD"
#define AT_DialOff							"ATH\r"

	
//global variable in this file
uint8_t pBuf1[128] = {0};
uint8_t pBuf2[128] = {0};
uint8_t APNBuff[50] = {0};

/*
0:	block			for check Battary NetWorks SignalStrength
1:	non-block		for auto check order
*/
#define FLAG_Block		0
#define FLAG_NonBlock	1
__IO uint8_t gNonBlockFlag = FLAG_Block;

/*
0:	allow other process to send at
1:	now is auto check order state
*/
__IO uint8_t gAutoCheckOrderFlag = 0;

/*
0:	gprs init didn't finish
1:	init finish, you can check order
*/
__IO uint8_t gGprsInitFinish = 0;



void send_at_cmd(uc8 *cmd)
{
	kmy_USART3SendString(cmd);
}

void Gprs_SendDataLen(uc8 *buff,u32 len)
{
	kmy_USART3SendData(buff,len);
}

u32 ReadDatasFromtty(u8 *rcv_buf,u32 size,int sec,int usec)
{
	u32 reclen;

	reclen=kmy_USART3ReceiveData(rcv_buf,size,sec*1000+usec/1000);
	*(rcv_buf+reclen)=0;

	return reclen;
}


unsigned char ReadDatasFromSIM300_OKstr(unsigned char *rcv_buf,unsigned long size,unsigned long Msec,unsigned char *OKstr)
{
	u8 keyval;
	u8 *p;
	u32 pos;

	//	kmy_USART3ReceiveReset();

	//sec*=1000;
	pos = 0;
	if(size)size--;	//先减一个字节，怕接受溢出
	else return 5;

	p=rcv_buf;
	while(1) {
		pos=ReadDatasFromtty(p,size,0,1*1000);
		p+=pos;
		size-=pos;
		*p=0;
		Msec-=1;

		if(strstr((char const*)rcv_buf, (char const*)OKstr))return 0;
		else if(strstr((char const*)rcv_buf,"ERROR"))return 1;

		if(Msec<=0)return 3;

		keyval=kmy_GetKeyValue();
		if(keyval==KEY_ReturnInitInterface)return 2;
	}
}




unsigned char send_at_cmd_and_receive(unsigned char * CmdBuff,int CmdBuffSize,int Ms,const unsigned char *OkStr)
{
	int i,ret,size,pos;
	unsigned char retval=TCP_SendAtTimeout;

	#ifdef DEBUG_PRINTF
	printf("SEND_AT:%s\n",CmdBuff);	//设置加CIP  头,执行了这个指令sim900B  把接收的数据以打包的方式从串口输出。+IPD,1460:数据
	#endif

	kmy_USART3ReceiveReset();

	//	SetAutoExitTime(0);
	Ms/=20;
	send_at_cmd(CmdBuff);
	*CmdBuff=0;
	for(pos=i=0,size=CmdBuffSize; i<Ms; i++) {
		ret= ReadDatasFromtty(CmdBuff+pos,size,0,20000);
		pos+=ret;
		size-=ret;

		if(strstr((char const*)CmdBuff,(char const*)OkStr))break;
		retval=kmy_GetKeyValue();
		if(retval!=KEY_NONE) {
			if(retval==KEY_ReturnInitInterface||retval==KEY_ReturnPrevious) {
				retval=TCP_UserSelfReturn;
				break;
			}
		}
	}

	if(i<Ms)retval=TCP_OK;

	return retval;
}


unsigned char send_at_cmd_and_return(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr)
{
	unsigned char tempbuff[1024];
	int i,ret,size,pos;
	unsigned char retval;

	retval=TCP_SendAtTimeout;

	//	printf("SEND_AT:%s\n",cmd);	//设置加CIP  头,执行了这个指令sim900B  把接收的数据以打包的方式从串口输出。+IPD,1460:数据

	//	kmy_USART3ReceiveReset();

	//	SetAutoExitTime(0);
	ms/=20;
	send_at_cmd(cmd);
	
	for(pos=i=0,size=sizeof(tempbuff); i<ms; i++) {
		ret= ReadDatasFromtty(tempbuff+pos,size,0,20000);
		pos+=ret;
		size-=ret;

		if(strstr((char const*)tempbuff,(char const*)okstr)) {
			retval=TCP_OK;
			break;
		}
		if(errstr!=NULL) {
			if(strstr((char const*)tempbuff,(char const*)errstr)) {
				retval=TCP_ERROR;
				break;
			}
		}
	}

	//	printf("SEND_AT_Re:%s\n",tempbuff);
	return retval;
}

/*
unsigned char send_at_cmd_and_return_NotResposeKey(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr)
{
	unsigned char tempbuff[1024];
	int i,ret,size,pos;
	unsigned char retval;

	retval=TCP_SendAtTimeout;

	printf("SEND_AT:%s\n",cmd);	//设置加CIP  头,执行了这个指令sim900B  把接收的数据以打包的方式从串口输出。+IPD,1460:数据

	//	SetAutoExitTime(0);
	ms/=20;
	send_at_cmd(cmd);
	for(pos=i=0,size=sizeof(tempbuff); i<ms; i++) {
		ret= ReadDatasFromtty(tempbuff+pos,size,0,20000);
		pos+=ret;
		size-=ret;

		if(strstr((char const*)tempbuff,(char const*)okstr)) {
			retval=TCP_OK;
			break;
		}
		if(strstr((char const*)tempbuff,(char const*)errstr)) {
			retval=TCP_ERROR;
			break;
		}
	}

	printf("SEND_AT_Re:%s\n",tempbuff);

	return retval;
}
*/

static unsigned char send_at_cmd_and_return_3errs(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr,const unsigned char *errstr2,const unsigned char *errstr3)
{
	unsigned char tempbuff[200];
	int i,ret,size,pos;
	unsigned char retval;
	unsigned char keyval;

	kmy_USART3ReceiveReset();

	retval=TCP_SendAtTimeout;
	printf("SEND_AT:%s\n",cmd);	//设置加CIP  头,执行了这个指令sim900B  把接收的数据以打包的方式从串口输出。+IPD,1460:数据

	//	SetAutoExitTime(0);
	ms/=20;
	send_at_cmd(cmd);
	for(pos=i=0,size=sizeof(tempbuff); i<ms; i++) {
		ret= ReadDatasFromtty(tempbuff+pos,size,0,20000);
		pos+=ret;
		size-=ret;

		if(strstr((char const*)tempbuff,(char const*)okstr)) {
			retval=TCP_OK;
			break;
		}
		if(strstr((char const*)tempbuff,(char const*)errstr)) {
			retval=TCP_ERROR;
			break;
		}
		if(strstr((char const*)tempbuff,(char const*)errstr2)) {
			retval=TCP_ERROR2;
			break;
		}
		if(strstr((char const*)tempbuff,(char const*)errstr3)) {
			retval=TCP_ERROR3;
			break;
		}

		keyval=kmy_GetKeyValue();
		if(keyval==KEY_ReturnInitInterface||keyval==KEY_ReturnPrevious) {
			retval=TCP_UserSelfReturn;
			break;
		}
	}

	printf("SEND_AT_Re:%s\n",tempbuff);
	return retval;
}



//参数ipaddr 是要连接的ip  地址，
//其格式是如这样"116.255.144.110,9100"
//连接成功返回宏定义TCP_OK
//连接失败返回其它宏定义
static unsigned char _tcp_Connect(unsigned char *ipaddr)
{
	unsigned char tempbuff[150];
	unsigned char serverip[25];
	unsigned char serverport[6];
	unsigned char i;


	send_at_cmd_and_return("AT+QIHEAD=1\r",2000,"OK","ERROR");

	memset(serverip,0,sizeof(serverip));	//得到ip  地址和端口
	memset(serverport,0,sizeof(serverport));
	for(i=0; i<25; i++) {
		if(*ipaddr!=',') {
			serverip[i]=*ipaddr;
			ipaddr++;
		}
	}
	ipaddr++;
	for(i=0; i<5; i++) {
		if(*ipaddr>='0'&&*ipaddr<='9') {
			serverport[i]=*ipaddr;
			ipaddr++;
		}
	}
	memset(tempbuff,0,sizeof(tempbuff));
	strcat((char*)tempbuff , "AT+QIOPEN=\"TCP\",\"" );
	strcat((char *)tempbuff , (char const*)serverip );
	strcat((char *)tempbuff , "\",\"" );
	strcat((char *)tempbuff , (char const*)serverport );
	strcat((char *)tempbuff , "\"\r" );

	i=send_at_cmd_and_return_3errs(tempbuff,15000,"CONNECT OK","ALREADY CONNECT","PDP DEACT","ERROR");

	if(i==TCP_ERROR)i=TCP_OK;
	return i;
}

unsigned char tcp_Connect(unsigned char *ipaddr)
{
	unsigned char i;
	unsigned char ret=TCP_ERROR;

	//	gFlushDataFlag=1;

	printf("\r\n\r\nTCP connect\r\n");

	for(i=0; i<3; i++) {
		ret=_tcp_Connect(ipaddr);

		if(ret==TCP_OK||ret==TCP_UserSelfReturn) {
			goto fucexit;
		}
		else if(ret==TCP_ERROR3) {
			send_at_cmd_and_return("AT+QICLOSE\r",3000,"OK","ERROR");
		}
		else {
			sleep(3);
		}
	}
	ret=TCP_ConnectFailure;

	fucexit:
	//	gFlushDataFlag=0;
	return ret;
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

void my_itoa(char*string,unsigned int i)
{
	unsigned int power,j;

	j=i;
	for(power=1; j>=10; j/=10)power*=10;
	for(; power>0; power/=10) {
		*string++='0'+i/power;
		i%=power;
	}
	*string='\0';
}
//参数buff  是要发送的内容缓冲区，
//参数len   是要发送缓冲区的长度，
//此发送不检测buff  的\0  结束，
//不管怎样都发送len  个字节
unsigned char tcp_send(const unsigned char *buff,int len)
{
	unsigned char i;
	char temp[10];
	char tempbuff[50];

	printf("\r\nTCP send\r\n");
	my_charhexprintf_buff("buff=",buff,len);

	strcpy((char *)tempbuff,"AT+QISEND=");
	my_itoa(temp,len);
	strcat((char *)tempbuff,(char const*)temp);
	strcat((char *)tempbuff,"\r");

	i=send_at_cmd_and_return((unsigned char const *)tempbuff,2000,">","ERROR");
	if(i!=TCP_OK) {
		i=TCP_SendFailure;
		goto fucexit;
	}

	Gprs_SendDataLen(buff, len);
	memset(tempbuff,0,sizeof(tempbuff));
	i=ReadDatasFromSIM300_OKstr((unsigned char*)tempbuff,sizeof(tempbuff),500,"SEND OK\x0D\x0A");

	i=TCP_SendOK;

	fucexit:
	return i;
}

/*
unsigned int my_axtoi(unsigned char* p)
{
	unsigned int len=0;

	while(1) {
		if(*p>='0'&&*p<='9') {
			len<<=4;
			len+=*p-'0';
		}
		else if(*p>='a'&&*p<='f') {
			len<<=4;
			len+=*p-'a'+10;
		}
		else if(*p>='A'&&*p<='F') {
			len<<=4;
			len+=*p-'A'+10;
		}
		else break;

		p++;
	}

	return len;
}
*/

void gui_DisplayProgressBar(uint8_t Value)//0~100
{
	char buff[6] = {0};
	unsigned char Temp = Value;

	#define Width 100
	#define Height 13
 	#define xstart (132-Width)/2
	#define ystart 24

	Temp = (Temp*((Width-4)/10))/10;

	//printf("%s : Value is %d\r\n", __func__, Value);
	//printf("%s : Temp is %d\r\n", __func__, Temp);

	kmy_LcdDrawRect (xstart, ystart, xstart+Width, ystart+Height,mode_White);

	kmy_LcdDrawRect(xstart, ystart, xstart+Width, ystart+Height, mode_OnlyEdge);

	kmy_LcdDrawRect(xstart+2, ystart+2, xstart+2+Temp, ystart+Height-2, mode_Black);

	LcdClearRow (3, 4);
	sprintf ((char *)buff, "%d", Value);
	strcat ((char *)buff, "%");
	DrawTitle_to_row (3, (unsigned char*)buff);
}

/*

void Ini_WriteToEerpom (void)
{
	uint16_t i=0, j=0, k=0, m=0;
	uint8_t ret = 0;
	
	memset(IniTempBuf, 0, sizeof(IniTempBuf));
	
	for(i=0; i<pos_ini_max; i++)
	{
		m = pos_ini[i].VarLen;
		//printf("%s : i = %d, pos_ini[i].VarLen = %d\r\n", __func__, i, m);

		for(j=0; j<m; j++)
		{
			IniTempBuf[k++] = pos_ini[i].Var[j];
		}

		//PrintBuf2(__func__, "pos_ini[i]=", i, pos_ini[i].Var, pos_ini[i].VarLen);
	}

	//PrintBuf(__func__, IniTempBuf, 200);

	ret = kmy_EepromWrite(IniStartAddress, IniTempBuf, IniMaxSize);
	if(ret != 0)
		printf("%s : kmy_EepromWriteBytes return is %d\r\n", __func__, ret);
}
*/

void draw_pic (const unsigned char *pstr, int xpos, int ypos, int width, int height)
{
	kmy_LcdDrawBitMap (pstr, (unsigned char) xpos, (unsigned char) ypos, (unsigned char) width, (unsigned char) height);
}

static void DisplaySignalQuality (int temp)
{
	const unsigned char pstr[6][24] =
	{
		{0x06, 0x0A, 0x3E, 0x0A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x06, 0x0A, 0x3E, 0x8A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x06, 0x0A, 0x3E, 0x8A, 0x06, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x06, 0x0A, 0x3E, 0x8A, 0x06, 0xE0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00},
		{0x06, 0x0A, 0x3E, 0x8A, 0x06, 0xE0, 0x00, 0xF0, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00},
		{0x06, 0x0A, 0x3E, 0x8A, 0x06, 0xE0, 0x00, 0xF0, 0x00, 0xFC, 0x00, 0xFF, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03}
	};

	if (temp > 32)
	{
		temp = 0;
	}
	else
	{
		temp /= 5;

		if (temp > 5) temp = 5;
	}

	draw_pic (pstr[temp], 0, 0, 12, 12);
}


void UpgradeSignalQuality (void)
{
	unsigned char Buf[100];
	char *p;
	unsigned char retval;
	static int SignalQuality;
			
	strcpy ( (char*) Buf, "AT+CSQ\r");	//check signal quality
	retval = send_at_cmd_and_receive (Buf, sizeof (Buf) - 3, 100, "OK");

	if (retval == TCP_OK)
	{
		//printf("Signal ok\r\n");
		p = strstr ( (char *) Buf, "+CSQ: ");

		if (p != NULL)
		{
			SignalQuality = atoi (p + sizeof ("+CSQ: ") - 1);
			DisplaySignalQuality (SignalQuality);
		}
	}
	else
	{
		printf("Signal not ok\r\n");
	}
}

void clear_area (int xpos, int ypos, int width, int height)
{
	kmy_LcdDrawRect (xpos, ypos, xpos + width - 1, ypos + height - 1, mode_White);
}
void my_strcpy_EncounterHexEnd (unsigned char *DesBuff, unsigned char *SourceBuff, unsigned int len, unsigned char Hex)
{
	for (; len != 0; len--)
	{
		if (*SourceBuff != Hex)
		{
			*DesBuff = *SourceBuff;
			DesBuff++;
			SourceBuff++;
		}
		else
		{
			break;
		}
	}

	*DesBuff = 0;
}

void UpgradeCSPN (void)
{
	unsigned char Buf[100];
	unsigned char retval;
	char *p;
//	const unsigned char *p2;
	static unsigned char CSPNbuf[20]={0};
	//static unsigned char Buf

	strcpy ( (char*) Buf, "AT+COPS?\r");
	retval = send_at_cmd_and_receive (Buf, sizeof (Buf) - 3, 100, "OK");

	if (retval == TCP_OK)
	{
		//+COPS: 0,0,"CHINA MOBILE"
		p = strstr ( (const char *) Buf, "COPS");

		if (p != NULL)
		{
			p = strstr ( (const char *) p, "\"");

			if (p != NULL)
			{
				p++;
				my_strcpy_EncounterHexEnd (Buf, (unsigned char*) p, 16, '\"');
			}
			else
			{
				strcpy ( (char *) Buf, "Searching net");
			}
		}
		else
		{
			strcpy ( (char *) Buf, "Searching net");
		}
		if(strcmp((char*)CSPNbuf,(char*)Buf)!=0)
		{
			clear_area (20, 0, 84, 12);
			strcpy((char*)CSPNbuf,(char*)Buf);
		}
	
		DrawTitle_to_row(0, Buf);
	}
	else
	{
		//printf("CSPN no ok\r\n");
	}	
	
}

char Register_apn (char* Apn, char* ApnUsername, char* ApnPassword)
{
	char Regist[200];
	char retval;
	char cnt = 0;
	
again:
	memset ( (char*) Regist, 0, sizeof (Regist) );
	strcat ( (char*) Regist, "AT+QIREGAPP=\"");
	strcat ( (char*) Regist, (char*) Apn);
	strcat ( (char*) Regist, "\",\"");
	strcat ( (char*) Regist, (char*) ApnUsername);
	strcat ( (char*) Regist, "\",\"");
	strcat ( (char*) Regist, (char*) ApnPassword);
	strcat ( (char*) Regist, "\"\r");
	retval = send_at_cmd_and_return ( (const unsigned char*) Regist, 3000, "OK", "ERROR");

	if (retval == TCP_OK) return retval;
	if (cnt++ > 7) return retval;
	retval = send_at_cmd_and_return ("AT+QIDEACT\r", 2000, "OK", "ERROR");
	
	goto again;
}

/*
struct storeFile XStor[STORESIZE+1] =  	//the parameters are store in the eeprom address 0x540, each segment seperate by \t.
{
	{beepKG, "1"},
	{LightPercent, "44"},
	{Language, locallanguage},
	{ServerIpPortBuf, "113.020.014.114,80"},	// 121.37.59.238,2000
	(Upgrade_Flag,""),
	{Domain, ""},
	{VersionURL, ""},
	{UpgradeURL, ""},
	{ApnApn, "cmnet"},
	{ApnUsername, ""},
	{ApnPassword, ""},
...
...
}
*/


void GetEepromVariable(void)
{
	char buff[1024]={0};
	//int i=0;
	char *p=NULL;
	char *pt=NULL;
	
	kmy_EepromInit();
	if (kmy_EepromCheckWhetherBlankChip() == 1)return;
	memset(buff,0,sizeof(buff));
	memset(IPandPort,0,sizeof(IPandPort));	
	memset(NetworkSwitch,0,sizeof(NetworkSwitch));
	memset(Upgrade_Flag,0,sizeof(Upgrade_Flag));
	memset(UpgradeURL,0,sizeof(UpgradeURL));	
	memset(APN,0,sizeof(APN));
	memset(ApnUsername,0,sizeof(ApnUsername));
	memset(ApnPassword,0,sizeof(ApnPassword));

	printf("----------------\r\n");
	kmy_EepromReadSequent(0x540,(unsigned char*)buff,sizeof(buff));

	p=buff;
	p=strstr(p,"\t");
	if(p==NULL)return;
	p++;
	p=strstr(p,"\t");
	if(p==NULL)return;
	p++;
	p=strstr(p,"\t");
	if(p==NULL)return;
	p++;
	pt=p;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	strncpy(IPandPort,p,pt-p);

	pt++;
	p=pt;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	strncpy(NetworkSwitch,p,pt-p);

	pt++;
	p=pt;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	strncpy(Upgrade_Flag,p,pt-p);
	
	pt++;
	p=pt;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	pt++;
	p=pt;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	strncpy(UpgradeURL,p,pt-p);

	pt++;
	p=pt;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	strncpy(APN,p,pt-p);

	pt++;
	p=pt;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	strncpy(ApnUsername,p,pt-p);

	pt++;
	p=pt;
	pt=strstr(pt,"\t");
	if(pt==NULL)return;
	strncpy(ApnPassword,p,pt-p);
}

char WriteEepromVariable(void)
{
	char buff[1024]={0};
	char *pt=NULL;
	char i;
	
	kmy_EepromInit();
	kmy_EepromReadSequent(0x540,(unsigned char*)buff,sizeof(buff));
	pt=buff;
	for(i=0;i<4;i++)
	{
		pt=strstr(pt,"\t");
		if(pt==NULL)return 1;
	    pt++;
	}
	*pt=NetworkSwitch[0];
	pt=strstr(pt,"\t");
	if(pt==NULL)return 1;
    pt++;
	*pt='0';  //clear Upgrade_Flag
    return kmy_EepromWrite(0x540,(unsigned char*)buff,sizeof(buff));
}

char InputUpgradeURL(void)
{
	char ret=0;
	kmy_LcdClear();
	DrawTitle_to_row(0, "Input Upgrade URL");
	ret=GetString_abcABC123PYWuBi( SRF_123 | SRF_ABC | SRF_abc, SRF_abc, (unsigned char*)UpgradeURL, sizeof (UpgradeURL) - 1, 1);
	return ret;
}

char InputUpgradeIPandPort(void)
{
	char ret=0;
	kmy_LcdClear();
	DrawTitle_to_row(0, "Input IPandPort");
	ret=GetString_abcABC123PYWuBi( SRF_123 | SRF_ABC | SRF_abc, SRF_abc, (unsigned char*)IPandPort, sizeof (IPandPort) - 1, 1);
	return ret;
}


char InputUpgradeAPN(void)
{
	char ret=0;
	kmy_LcdClear();
	DrawTitle_to_row(0, "Input Upgrade APN");
	ret=GetString_abcABC123PYWuBi( SRF_123 | SRF_ABC | SRF_abc, SRF_abc, (unsigned char*)APN, sizeof (APN) - 1, 1);
	return ret;
}

char InputUpgradeApnUsername(void)
{
	char ret=0;
	kmy_LcdClear();
	DrawTitle_to_row(0, "Input ApnUsername");
	ret=GetString_abcABC123PYWuBi( SRF_123 | SRF_ABC | SRF_abc, SRF_abc, (unsigned char*)ApnUsername, sizeof (ApnUsername) - 1, 1);
	return ret;
}

char InputUpgradeApnPassword(void)
{
	char ret=0;
	kmy_LcdClear();
	DrawTitle_to_row(0, "Input ApnPassword");
	ret=GetString_abcABC123PYWuBi( SRF_123 | SRF_ABC | SRF_abc, SRF_abc, (unsigned char*)ApnPassword, sizeof (ApnPassword) - 1, 1);
	return ret;
}


char GPRSmenu(void)
{
	static char InitFlag = 1;
	char retval;
	
	if(NetworkSwitch[0]==WiFi)
	{
		while(1)if(WiFi_Upgrade()==1)break;			
	}
    else
    {
    	if (InitFlag)
    	{	
    		retval=Register_apn(APN,ApnUsername,ApnPassword);
    		if (retval != TCP_OK)
    		{
    			printf("Register APN fail\r\n");
    		}
    		kmy_LcdClear();
    
    		gui_DisplayProgressBar(0);
    		
    		text_out_to_row(4, "Waitting network...");
    
    		InitFlag = 0;
    			
    	}
    	
    	GPRS_Upgrade();
    }
	retval = kmy_GetKeyValue();
	if(retval==KEY_ReturnPrevious||retval==KEY_ReturnInitInterface)
	{
	    InitFlag = 1;
		return KEY_ReturnPrevious;
	}

	return KEY_NONE;
}
void gprs_SendATCmd (unsigned char *SendString)
{
	int i = 0xFFF;
	
	kmy_USART3SendString (SendString);

	printf ("\r\n%s : %s\r\n", __func__, SendString);
	while(i--);
}


char gprs_SendATAndReceive (unsigned char *RecvBuf,
								int RecvSize,
								unsigned char *SendString,
								unsigned char *OkString,
								unsigned char *ErrorString,
								int ms)
{
	unsigned char ReturnVar = GPRS_SendATRecvTimeout;
	int TimeoutCnt = 0;

	//reset receive buff
	kmy_USART3ReceiveReset();

	//send at command
	if (SendString != NULL) gprs_SendATCmd (SendString);

	while (1)
	{
		if ( (OkString != NULL) && (kmy_USART3CheckBuff((char*)OkString) == 1) )
		{
			ReturnVar = GPRS_SendATOK;
			break;
		}

		if ( (ErrorString != NULL) && (kmy_USART3CheckBuff((char*)ErrorString)  == 1) )
		{
			ReturnVar = GPRS_SendATERROR;
			break;
		}

		if (TimeoutCnt > ms)
		{
			ReturnVar = GPRS_SendATRecvTimeout;
			break;
		}

		TimeoutCnt += 5;
		
		msleep(5);
	}

	if (TimeoutCnt < ms)
	{
		////printf ("%s : %s-----END\r\n", __func__, RecvBuf);
	}
	else
	{
		////printf ("%s : Recv timeout-----END\r\n", __func__);
	}
	
	kmy_USART3ReadToEnd((char*)RecvBuf, RecvSize);
	printf("\r\n%s : %s\r\n", __func__, RecvBuf);
	
	return ReturnVar;
}


char SeperateIpAndPort(char *IPandPort, char *IP, char *Port)
{
	int i=0;
	char *p;
	char tmp[7]={0};
	
	if(strlen(IPandPort)<9)return 1;
	p=IPandPort;

	if(*p>='0'&&*p<='9')// ip mode
	{
		i=atoi(p);
		if(i>255)return 1;
		sprintf(tmp,"%d",i);
	//	printf("tmp=%s\r\n",tmp);
		strcpy(IP,tmp);
		strcat(IP,".");

		p=strstr(p,".");
		p++;
		i=atoi(p);
		if(i>255)return 1;
		sprintf(tmp,"%d",i);
	//	printf("tmp=%s\r\n",tmp);
		strcat(IP,tmp);
		strcat(IP,".");

		p=strstr(p,".");
		p++;
		i=atoi(p);
		if(i>255)return 1;
		sprintf(tmp,"%d",i);
	//	printf("tmp=%s\r\n",tmp);
		strcat(IP,tmp);
		strcat(IP,".");

		p=strstr(p,".");
		p++;
		i=atoi(p);
		if(i>255)return 1;
		sprintf(tmp,"%d",i);
	//	printf("tmp=%s\r\n",tmp);
		strcat(IP,tmp);
		p=strstr(p,",");

	}
	else // domain mode as www.baidu.com,8080
	{
		p=strstr(p,",");
		strncpy(IP,IPandPort,p-IPandPort);
	}
	

	p++;

	IPandPort=p;
	for (i = 0; i < 5; i++)
	{
		if (*IPandPort >= '0' && *IPandPort <= '9')
		{
			Port[i] = *IPandPort;
			IPandPort++;
		}
	}
	return 0;//IP correct
}


void gprs_GetTcpConnectATString (unsigned char*dest, char*IPandPort)
{	
	char IP[100]={0};
	char ServerPort[8]={0};
	SeperateIpAndPort(IPandPort,IP,ServerPort);
	printf("IP=%s;ServerPort=%s\r\n",IP,ServerPort);
	
	strcpy ( (char *) dest , AT_TcpConnect );
	
	strcat ( (char *) dest ,  IP );

	strcat ( (char *) dest , "\",\"" );
	strcat ( (char *) dest , (char const*) ServerPort );
	strcat ( (char *) dest , "\"\r" );
}



void GetHTTPGETString(char* destbuff, char* pageurl,char*IPandPort)
{
    char IP[25]={0};
	char ServerPort[8]={0};
	memset(IP,0,sizeof(IP));	
	memset(ServerPort,0,sizeof(ServerPort));
	SeperateIpAndPort((char*)IPandPort,IP,ServerPort);
	
	strcpy(destbuff, "GET ");
	strcat(destbuff, pageurl);
	strcat(destbuff, " HTTP/1.1");
	strcat(destbuff, "\r\n");
	
	//Host: www.e-food.gr
	strcat(destbuff, "Host: ");
	strcat(destbuff, IP);
	strcat(destbuff, "\r\n");
	
	//Pragma: no-cache
	strcat(destbuff, "Pragma: no-cache");
	strcat(destbuff, "\r\n");
	
	//Accept: */*
	strcat(destbuff, "Accept: */*");
	strcat(destbuff, "\r\n");
	
	//Proxy-Connection: Keep-Alive
	strcat(destbuff, "Proxy-Connection: Keep-Alive");
	strcat(destbuff, "\r\n");
	
	strcat(destbuff, "\r\n");
}

char WiFi_Upgrade(void)
{
    #define AUTOCHECK_Connstatus       0
    #define AUTOCHECK_Wait_Connstatus  1
    #define AUTOCHECK_Erase_Flash      2
    #define AUTOCHECK_Send_Data        3
    #define AUTOCHECK_Receive_Length   4
    #define AUTOCHECK_Receive_Data     5
	#define AUTOCHECK_Error_Exit       6
	
    #define AUTOCHECK_TIMEOUT 60000  //60S

	static uint8_t AutoCheckState = 0;
	static uint32_t ContentLength = 0;
	static uint32_t OldContentLength = 0;

	uint32_t EraseCounter = 0x0;
	uint32_t NbrOfPage = 0;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint32_t DATAS_LENGTH = PAGE_SIZE;
	const uint16_t PageSize = PAGE_SIZE;
	uint32_t RamSource, j = 0;	
	static unsigned long timeout_MilliSeconds=0;
	static char count_error=0;
	static char cnt=0;
	static char cnt2=0;	
	char IP[100]={0};
	char ServerPort[8]={0};
	

	switch(AutoCheckState)
	{
		case AUTOCHECK_Connstatus:
	
			kmy_LcdClear();		
			LcdWriteRowX(0,24,"WiFi Upgrade");
			SeperateIpAndPort(IPandPort,IP,ServerPort);
			memset(pBuff, 0, sizeof(pBuff));
			kmy_WiFi_USARTReceiveReset();
			sprintf((char *)pBuff,"setdsthn %s\r",IP);
			printf("SendBuff:\r\n%s\r\n",pBuff);
            kmy_WiFi_USARTSendString(pBuff);
			
		    memset(pBuff, 0, sizeof(pBuff));
			sprintf((char *)pBuff,"setdstport %s\r",ServerPort);
            kmy_WiFi_USARTSendString(pBuff);
			printf("SendBuff:\r\n%s\r\n",pBuff);
			
			kmy_WiFi_USARTReceiveReset();
			kmy_WiFi_USARTSendString("manconn\r");
			kmy_WiFi_USARTSendString("connstatus 1\r");
	        kmy_WiFi_USARTSendString("connstatus\r");

			AutoCheckState++;
			gAutoCheckOrderFlag = 1;	        
		    timeout_MilliSeconds=kmy_TimeGetMilliseconds();
			printf("AUTOCHECK_Connstatus\r\n");
			break;	
		
		case AUTOCHECK_Wait_Connstatus:

			if(kmy_WiFi_USARTCheckBuff("CONNECTED"))
        	{
        	    AutoCheckState++;
        	}
			
			cnt++;
			msleep(10);
			if(cnt%100==0)
			{
				cnt2++;
				LcdClearRow(4, 5);
				if(cnt2%4==0)LcdWriteRowX(4,0, "Connecting.");
				else if(cnt2%4==1)LcdWriteRowX(4,0, "Connecting..");
				else if(cnt2%4==2)LcdWriteRowX(4,0, "Connecting...");
				else if(cnt2%4==3)LcdWriteRowX(4,0, "Connecting....");
				kmy_WiFi_USARTReceiveReset();					
				kmy_WiFi_USARTSendString("connstatus 1\r");
				kmy_WiFi_USARTSendString("connstatus\r");
			}

			if(kmy_TimeGetMilliseconds()>=timeout_MilliSeconds+AUTOCHECK_TIMEOUT)
			{
    			kmy_WiFi_USARTShowBuff();
    			printf("Connect fail\r\n");
    			AutoCheckState = AUTOCHECK_Error_Exit;
			}			
			break;
			
		case AUTOCHECK_Erase_Flash:

			AutoCheckState++;//To next state
			printf("Erase flash\r\n");
//--erase flash-----------------------------------------------------------------------------------------------------------------------------
			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Erase flash");//erase flash
			//printf("erase flash\r\n");
			
			/* Erase the needed pages where the user application will be loaded */
			/* Define the number of page to be erased */
			NbrOfPage = FLASH_PagesMask(0x74000);//512K-48K = 464K
			//printf("NbrOfPage is %d\r\n", NbrOfPage);

			/* Erase the FLASH pages */
			for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
			{
				FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
			}
			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Erase finish");//erase flash
//--erase flash end-----------------------------------------------------------------------------------------------------------------------------
		    break;

		case AUTOCHECK_Send_Data://send datas

			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Sending");
			printf("Sending\r\n");
			memset(pBuff, 0, sizeof(pBuff));
			GetHTTPGETString((char *)pBuff, UpgradeURL, IPandPort);
			printf("pBuff=%s\r\n",pBuff);
			kmy_SPIWriteData(pBuff,strlen((char *)pBuff));
			AutoCheckState++;//To next state
			timeout_MilliSeconds=kmy_TimeGetMilliseconds();
			break;
			
		case AUTOCHECK_Receive_Length:

			if(kmy_ReadBufferLen()>100) 
			{
				memset(pBuff, 0, sizeof(pBuff));
				kmy_SPIReadTo(pBuff,sizeof(pBuff),"Content-Length:");
				memset(pBuff, 0, sizeof(pBuff));
				kmy_SPIReadTo(pBuff,sizeof(pBuff),"\r\n\r\n");
				ContentLength = atoi((const char *)(pBuff+1));//+1 ingore a space
				OldContentLength = ContentLength;
				printf("\r\n\r\nContentLength is %d\r\n\r\n",ContentLength);
				memset(pBuff, 0, sizeof(pBuff));
				sprintf((char *)pBuff, "Length is %d", ContentLength);
				LcdClearRow(4, 5);
				LcdWriteRowX(4, 0, pBuff);//display how many datas will be received				
				if(ContentLength == 0)
				{
					LcdClearRow(4, 5);
					LcdWriteRowX(4, 0, "Length error!");
					AutoCheckState = AUTOCHECK_Error_Exit;
				}				
				timeout_MilliSeconds=kmy_TimeGetMilliseconds();
				AutoCheckState++;//To next state				
			}
			/* Timeout Handling */
			if(kmy_TimeGetMilliseconds()>=timeout_MilliSeconds+AUTOCHECK_TIMEOUT)
			{
				printf("AUTOCHECK_RECV: RECEIVE \"Content-Length:\"TIMEOUT\r\n");
				AutoCheckState = AUTOCHECK_Error_Exit;
			}
			break;

		case AUTOCHECK_Receive_Data:
			
			while(AutoCheckState == AUTOCHECK_Receive_Data)
			{
			    while(kmy_ReadBufferLen()<DATAS_LENGTH);
			    j=kmy_SPIReadData(pBuff,DATAS_LENGTH);	
				printf("j=<%4x>\r\n",j);
				
				RamSource = (uint32_t)pBuff;
				for (j = 0;(j < DATAS_LENGTH) && (FlashDestination <  (ApplicationAddress + OldContentLength));j += 4)
				{
					/* Program the data received into STM32F10x Flash */
					FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);

					if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)
					{
						/* End session */
						LcdClearRow(4,5);
						LcdWriteRowX(4, 0, "End session");
						NVIC_SystemReset();
					}
					FlashDestination += 4;
					RamSource += 4;
				}
				ContentLength -= DATAS_LENGTH;					
				if (ContentLength >= PageSize)DATAS_LENGTH = PageSize;
				else DATAS_LENGTH = ContentLength;

				memset(pBuff, 0, sizeof(pBuff));

				gui_DisplayProgressBar(100-((ContentLength*100)/OldContentLength));

				if (ContentLength == 0)
				{
					gui_DisplayProgressBar(100);

					LcdClearRow(4, 5);
					LcdWriteRowX(3, 0, "Update successfully!");
					LcdWriteRowX(4, 0, "Reboot...");
					msleep(1000);
					NVIC_SystemReset();
					AutoCheckState++;
				}
			}
			break;

		case AUTOCHECK_Error_Exit:
			
			printf("AUTOCHECK_EXIT_ERROR\r\n");
			AutoCheckState = AUTOCHECK_Connstatus;

			count_error++;
			if(count_error>2)return 1;
			
			gui_DisplayProgressBar(0);
			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Update failed,Retry");
			gAutoCheckOrderFlag = 0;
		break;

		default:
			/* Reset Variable*/
			AutoCheckState = AUTOCHECK_Connstatus;
			break;
	} 	
	return 0;
}


void GPRS_Upgrade(void)
{
#define AUTOCHECK_WAIT 0
#define AUTOCHECK_CLOSE 1
#define AUTOCHECK_CONN 2
#define AUTOCHECK_CHECK_CONN_OK 3
#define AUTOCHECK_SEND 4
#define AUTOCHECK_CHECK_SEND_CHAR 5
#define AUTOCHECK_SENDDATAS 6
#define AUTOCHECK_CHECK_SEND_OK 7
#define AUTOCHECK_RECV1 8
#define AUTOCHECK_RECV2 9
#define AUTOCHECK_RECV3 10
#define AUTOCHECK_EXIT 11
#define AUTOCHECK_EXIT_ERROR 12

//60s
#define AUTOCHECK_TIMEOUT 60000
//300s
//#define AUTOCHECK_TIMEOUT_RECV 300000

	static uint8_t AutoCheckState = 0;
	static uint32_t ContentLength = 0;
	static uint32_t OldContentLength = 0;
//	static uint32_t DebugPrintFlag = 0;

	uint32_t EraseCounter = 0x0;
	uint32_t NbrOfPage = 0;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	uint32_t DATAS_LENGTH = PAGE_SIZE;
	const uint16_t PageSize = PAGE_SIZE;
	uint32_t RamSource, j = 0;
	uint16_t Pos = 0;
	char tmp[1]={0};	
	static unsigned long timeout_MilliSeconds=0;
	//int BytesToBeRead = 0;

	//if( (gGprsInitFinish != 2))
	//{
	//	return ;
	//}

	switch(AutoCheckState)
	{
		case AUTOCHECK_WAIT:
		//	if(gGprsInitFinish == 2)//AutoCheck must be behind the GprsInit()
		//	{
				timeout_MilliSeconds=kmy_TimeGetMilliseconds();
				LcdClearRow(4, 5);
				LcdWriteRowX(4,0,"Prepare connection");
				printf("AUTOCHECK_WAIT:Prepare connection\r\n");
				
				AutoCheckState++;//To next state
				
				gAutoCheckOrderFlag = 1;
		//	}
			break;	
		
		case AUTOCHECK_CLOSE:
			//  "no ECHO"
			memset(pBuff, 0, sizeof(pBuff));
			gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
								 "ATE0\r", 
								 "OK",
								 "ERROR",
								 2000);
			memset(pBuff, 0, sizeof(pBuff));
			gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
								 "AT+QIHEAD=0\r", 
								 "OK",
								 "ERROR",
								 2000);
			// "Close gprs connect1"
/*			memset(pBuff, 0, sizeof(pBuff));
			gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
								 "AT+QICLOSE\r", 
								 "CLOSE OK",
								 "ERROR",
								 2000);
*/			
			//  "Close gprs connect2"
			memset(pBuff, 0, sizeof(pBuff));
			gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
								 "AT+QIDEACT\r", 
								 "DEACT OK",
								 "ERROR",
								 10000);
			
			//  "Transparent mode" "透传模式"
			memset(pBuff, 0, sizeof(pBuff));
			gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
								 "AT+QIMODE=1\r", 
								 "OK",
								 "ERROR",
								 10000);
			memset(pBuff, 0, sizeof(pBuff));
			if(IPandPort[0]>='0'&&IPandPort[0]<='9')
			{
				gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
												 "AT+QIDNSIP=0\r", 
												 "OK",
												 "ERROR",
												 10000);

			}
			else
			{
				gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
												 "AT+QIDNSIP=1\r", 
												 "OK",
												 "ERROR",
												 10000);
			}
			
			AutoCheckState++;//To next state
			break;

		case AUTOCHECK_CONN:

			LcdClearRow(4, 5);
			LcdWriteRowX(4,0, "Connecting");
			msleep(800);

			printf("AUTOCHECK_CONN:Connecting\r\n");

			//"58.60.195.53,81"
			memset (pBuff, 0, sizeof (pBuff) );
			gprs_GetTcpConnectATString (pBuff, IPandPort);
			kmy_USART3ReceiveReset();
			gprs_SendATCmd(pBuff);
			timeout_MilliSeconds=kmy_TimeGetMilliseconds();
			AutoCheckState++;//To next state
			
			break;

		case AUTOCHECK_CHECK_CONN_OK://AT+QIMODE=1  will get "CONNECT" only
			if(kmy_USART3CheckBuff("CONNECT") == 1)
			{
				printf("CONNECT OK\r\n");
				
				LcdClearRow(4, 5);
				LcdWriteRowX(4, 0, "Connect OK");

				AutoCheckState++;//To next state
			}
			else if(kmy_USART3CheckBuff("CONNECT FAIL") == 1)
			{
				printf("Connect Status: CONNECT FAIL\r\n");
				AutoCheckState = AUTOCHECK_EXIT_ERROR;
			}
			else if(kmy_USART3CheckBuff("ALREADY CONNECT") == 1)
			{
				printf("Connect Status: ALREADY CONNECT\r\n");

				AutoCheckState = AUTOCHECK_EXIT_ERROR;
			}
			else if(kmy_USART3CheckBuff("PDP DEACT") == 1)
			{
				printf("Connect Status: PDP DEACT\r\n");
				AutoCheckState = AUTOCHECK_EXIT_ERROR;
			}
			else if(kmy_USART3CheckBuff("ERROR") == 1)
			{
				printf("Connect Status: ERROR\r\n");
				AutoCheckState = AUTOCHECK_EXIT_ERROR;
			}
			// Timeout Handling 
		//	if(g1msTimer5 > AUTOCHECK_TIMEOUT)
		   if(kmy_TimeGetMilliseconds()>=timeout_MilliSeconds+AUTOCHECK_TIMEOUT)
			{
				printf("Connect Status: TIMEOUT\r\n");
				AutoCheckState = AUTOCHECK_EXIT_ERROR;
			}
			break;

		case AUTOCHECK_SEND://no need
			
			AutoCheckState++;//To next state
			
			break;
			
		case AUTOCHECK_CHECK_SEND_CHAR://no need
			
			AutoCheckState++;//To next state
		   
			printf("Erase flash\r\n");
//--erase flash-----------------------------------------------------------------------------------------------------------------------------
			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Erase flash");//erase flash
			//printf("erase flash\r\n");
			
			// Erase the needed pages where the user application will be loaded 
			// Define the number of page to be erased 
			NbrOfPage = FLASH_PagesMask(0x74000);//512K-48K = 464K
			//printf("NbrOfPage is %d\r\n", NbrOfPage);

			// Erase the FLASH pages 
			for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
			{
				FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
			}
			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Erase finish");//erase flash
//--erase flash end-----------------------------------------------------------------------------------------------------------------------------
		break;
			
		case AUTOCHECK_SENDDATAS://send datas

			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Sending");
			msleep(2000);
			
			printf("Sending\r\n");

			//printf("InputDownloadUrl is %s\r\n", InputDownloadUrl);
			memset(pBuff, 0, sizeof(pBuff));
			//GetHTTPGETString((char *)pBuff, InputDownloadUrl);
			GetHTTPGETString((char *)pBuff, UpgradeURL, IPandPort);
			//printf("\r\n----------------------------------------\r\n");
			
			kmy_USART3ReceiveReset();
			
			gprs_SendATCmd(pBuff);

			AutoCheckState++;//To next state
			timeout_MilliSeconds=kmy_TimeGetMilliseconds();
			break;
			
		case AUTOCHECK_CHECK_SEND_OK://no need

			AutoCheckState++;//To next state
				
			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Send OK");

			printf("Send OK\r\n");

			ContentLength = 0;
			OldContentLength = 0;
			DATAS_LENGTH = 0;			
			
		//	FlashDestination = ApplicationAddress;
			BlockNbr = 0;
			UserMemoryMask = 0;
			FlashProtection = 0;	
			timeout_MilliSeconds=kmy_TimeGetMilliseconds();
			break;
			
		case AUTOCHECK_RECV1:
			if(kmy_USART3CheckBuff("200 OK") == 1)//"HTTP/1.1 " 
			{
				LcdClearRow(4, 5);
				LcdWriteRowX(4, 0, "Get 200 OK");
				
				printf("AUTOCHECK_RECV1:HTTP/1.1 200 OK, goto NEXT\r\n");

				AutoCheckState++;//To next state
				timeout_MilliSeconds=kmy_TimeGetMilliseconds();
			}
			else if(kmy_USART3CheckBuff("ERROR") == 1)
			{
				AutoCheckState = AUTOCHECK_EXIT;
				printf("AUTOCHECK_RECV1: ERROR\r\n");
			}
			
			// Timeout Handling 
			//if (g1msTimer5 > AUTOCHECK_TIMEOUT)
			
			if(kmy_TimeGetMilliseconds()>=timeout_MilliSeconds+AUTOCHECK_TIMEOUT)
			{
				
				printf("AUTOCHECK_RECV1: RECEIVE \"HTTP/1.1 200 OK\" TIMEOUT\r\n");
				kmy_USART3ShowBuff();
				AutoCheckState = AUTOCHECK_EXIT;
			}
			break;
			
		case AUTOCHECK_RECV2:
			//check Content-Length:
			if(kmy_USART3CheckBuff("Content-Length:") == 1) 
			{
				msleep(50);

				memset(pBuff, 0, sizeof(pBuff));
				
				
				
				 kmy_USART3ReadTo((char*)pBuff, sizeof(pBuff), "Content-Length:");
				//printf("%s : |0|BytesToBeRead is %d\r\n%s\r\n\r\n\r\n", __func__, BytesToBeRead, pBuff);
				
				memset(pBuff, 0, sizeof(pBuff));
				kmy_USART3ReadTo((char*)pBuff, sizeof(pBuff), "\r\n\r\n");
				//printf("%s : |1|BytesToBeRead is %d\r\n%s\r\n\r\n\r\n", __func__, BytesToBeRead, pBuff);
				
				ContentLength = atoi((const char *)(pBuff+1));//+1 ingore a space
				OldContentLength = ContentLength;
				//printf("\r\n\r\n%s : |2|ContentLength is %d\r\n\r\n", __func__, ContentLength);
				
				memset(pBuff, 0, sizeof(pBuff));
				sprintf((char *)pBuff, "Length is %d", ContentLength);
				
				LcdClearRow(4, 5);
				LcdWriteRowX(4, 0, pBuff);//display how many datas will be received

				printf("AUTOCHECK_RECV2 is %s\r\n", pBuff);
				
				if(ContentLength == 0)
				{
					LcdClearRow(4, 5);
					LcdWriteRowX(4, 0, "Length error!");
					AutoCheckState = AUTOCHECK_EXIT_ERROR;
				}				

				AutoCheckState++;//To next state
				
				//InterruptONOFF(0);
				
				timeout_MilliSeconds=kmy_TimeGetMilliseconds();
				goto Recv3Loop;//for quickly start receive
			}
			// Timeout Handling 
			if(kmy_TimeGetMilliseconds()>=timeout_MilliSeconds+AUTOCHECK_TIMEOUT)
			{
				printf("AUTOCHECK_RECV2: RECEIVE \"Content-Length:\"TIMEOUT\r\n");
				AutoCheckState = AUTOCHECK_EXIT;
			}
			break;

		case AUTOCHECK_RECV3:
Recv3Loop:
			while(AutoCheckState == AUTOCHECK_RECV3)
			{
				if(kmy_USART3ReadToEnd(tmp,1)==1)
				{
					pBuff[Pos]=tmp[0];
					Pos++;
				}
//				if(kmy_USART3RecvOne(pBuff+Pos) == 1)
//				{
//					Pos++;
//				}
				
				// Timeout Handling 
				if(Pos < DATAS_LENGTH)continue;
			//	timeout_MilliSeconds=kmy_TimeGetMilliseconds();
				//DebugPrintFlag = 0;
				
				//printf("Total2 is 0x%08x\r\n", Total2);
				//printf("Pos is %d\r\n", Pos);
				//printf("ContentLength is %d\r\n", ContentLength);
				//printf("FlashDestination is 0x%08x\r\n\r\n", FlashDestination);

				//--write flash-----------------------------------------------------------------------------------------------------------------------------
				
				#if 1
				RamSource = (uint32_t)pBuff;

				for (j = 0;(j < DATAS_LENGTH) && (FlashDestination <  (ApplicationAddress + OldContentLength));j += 4)
				{
					// Program the data received into STM32F10x Flash 
					FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);

					if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)
					{
						// End session 
					//	kmy_BeepOnce(BeepNormal);

						LcdClearRow(4,5);
						LcdWriteRowX(4, 0, "End session");
						//printf("End session\r\n");

					//	gUpgradeFlag.Var[0] = 0;
					//	Ini_WriteToEerpom();

						NVIC_SystemReset();
						msleep(10000);
					}
					FlashDestination += 4;
					RamSource += 4;
				}
				//--write flash end-----------------------------------------------------------------------------------------------------------------------------
				#endif
				ContentLength -= DATAS_LENGTH;					
				
				if (ContentLength >= PageSize)DATAS_LENGTH = PageSize;
				else DATAS_LENGTH = ContentLength;

				memset(pBuff, 0, sizeof(pBuff));
				Pos = 0;

				gui_DisplayProgressBar(100-((ContentLength*100)/OldContentLength));

				//printf("%s : |4|ContentLength is %d\r\n", __func__, ContentLength);
				if (ContentLength == 0)
				{
					//printf("Pos is %d\r\n", Pos);
					//printf("ContentLength is %d\r\n", ContentLength);
					//printf("FlashDestination is 0x%08x\r\n\r\n", FlashDestination);

					//printf("\r\n\r\nUpgrade successfully\r\n\r\n");

					gui_DisplayProgressBar(100);

					// upgrade finish 
				//	memset(gNewVersionName.Var, 0, gNewVersionName.VarLen);
				//	gUpgradeFlag.Var[0] = 0;
				//	Ini_WriteToEerpom();

				//	kmy_BeepOnce(BeepNormal);

					LcdClearRow(4, 5);
					LcdWriteRowX(3, 0, "Update successfully!");
					LcdWriteRowX(4, 0, "Reboot...");
					if(Upgrade_Flag[0]=='1')
					{
						Upgrade_Flag[0]='0';
						WriteEepromVariable();
					}

					msleep(5000);

					NVIC_SystemReset();
					msleep(10000);
					// upgrade finish 

					AutoCheckState = AUTOCHECK_EXIT;
				}
			}
			break;
			

		case AUTOCHECK_EXIT:
			printf("AUTOCHECK_EXIT\r\n");
			AutoCheckState = AUTOCHECK_EXIT_ERROR;
			break;

		case AUTOCHECK_EXIT_ERROR:
			printf("AUTOCHECK_EXIT_ERROR\r\n");
			AutoCheckState = AUTOCHECK_WAIT;
			//DebugPrintFlag = 0;

			gui_DisplayProgressBar(0);
			LcdClearRow(4, 5);
			LcdWriteRowX(4, 0, "Update failed,Retry");

			gprs_SendATCmd("+++");//switch to command mode
			msleep(500);
			
			//  "Close gprs connect1"
			memset(pBuff, 0, sizeof(pBuff));
			gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
								 "AT+QICLOSE\r", 
								 "CLOSE OK",
								 "ERROR",
								 2000);
			
			// "Close gprs connect2"
			memset(pBuff, 0, sizeof(pBuff));
			gprs_SendATAndReceive(pBuff, sizeof(pBuff), 
								 "AT+QIDEACT\r", 
								 "DEACT OK",
								 "ERROR",
								 10000);

			gAutoCheckOrderFlag = 0;
		break;

		default:
			// Reset Variable
			AutoCheckState = AUTOCHECK_WAIT;
			break;
	} 	
}







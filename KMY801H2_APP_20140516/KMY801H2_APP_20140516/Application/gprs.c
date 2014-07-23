

#include "gprs.h"

#include "stm32f10x_gpio.h"
#include "key.h"
#include "kmy_Time.h"
//#include "kmy_USART2Drv.h"
#include "kmy_USART3Drv.h"
#include <string.h>
#include "ShuRuFa.h"
#include <stdlib.h>
#include <stdio.h>
#include "mypure_library.h"
#include "kmy_LCDDrv.h"
#include "lcd.h"
#include "my_library.h"
#include "global_extern_variable.h"
#include "setting.h"
#include "kmy_LowPowerDrv.h"




void send_at_cmd(uc8 *cmd)
{
	printf("SendAtCmd-->%s\r\n",cmd);
	kmy_USART3SendString(cmd);
}




unsigned char Gprs_SendDataLen(uc8 *buff,u32 len,int ms,const unsigned char *okstr,const unsigned char *errstr)
{
	unsigned char tempbuff[1024];
	int i,ret,size,pos;
	unsigned char retval;

	retval=TCP_SendAtTimeout;

	kmy_USART3ReceiveReset();

	SetAutoExitTime(0);
	ms/=20;
	kmy_USART3SendData(buff,len);
	for(pos=i=0,size=sizeof(tempbuff);i<ms;i++)
	{
		ret= ReadDatasFromtty(fd_ttyS1,tempbuff+pos,size,0,20000);
		if(ret)
		{
			pos+=ret;
			size-=ret;
		}
		else
		{
			if(strstr((char const*)tempbuff,(char const*)okstr)){retval=TCP_SendOK;break;}
			if(errstr!=NULL){if(strstr((char const*)tempbuff,(char const*)errstr)){retval=TCP_SendFailure;break;}}

		}
	}

	printf("SEND_AT_Re:%s\n",tempbuff);
	return retval;
}

u32 ReadDatasFromtty(int fd,u8 *rcv_buf,u32 size,int sec,int usec)
{
	u32 reclen;

	reclen=kmy_USART3ReceiveData(rcv_buf,size,sec*1000+usec/1000);

	*(rcv_buf+reclen)=0;

	return reclen;
}


unsigned char SendAtCommand_UntillSuccess(unsigned char *cmd)
{
	char tempbuff[100];
	int i,ret,size,pos,ms=2000;
	char cnt=0;

	AgainExecute:
	send_at_cmd(cmd);

	ms/=10;
	for(pos=i=0,size=sizeof(tempbuff);i<ms;i++)
	{
		ret= ReadDatasFromtty(fd_ttyS1,(u8*)(tempbuff+pos),size,0,10000);
		if(ret)
		{
			pos+=ret;
			size-=ret;
		}
		else
		{
			if(strstr(tempbuff,"OK"))return SendAtCommand_OK;
			if(strstr(tempbuff,"ERROR"))return SendAtCommand_ERROR;
		}
	}

	if(cnt++<5)goto AgainExecute;

	return SendAtCommand_ERROR;
}


unsigned char ReadDatasFromSIM300_OKstr(unsigned char *rcv_buf,unsigned long size,unsigned long sec,unsigned char *OKstr)
{
	u8 keyval;
	u8 *p;
	u32 pos;

	kmy_USART3ReceiveReset();

	sec*=1000;	
	pos = 0;
	if(size)size--;	
	else return 5;

	p=rcv_buf;
	while(1)
	{
		pos=ReadDatasFromtty(fd_ttyS1,p,size,0,100*1000);
		p+=pos;
		size-=pos;
		*p=0;
		sec-=100;

		if(strstr((char const*)rcv_buf, (char const*)OKstr))return 0;
		else if(strstr((char const*)rcv_buf,"ERROR"))return 1;

		if(sec<=0)return 3;

		keyval=GetKeyValue();
		if(keyval==KEY_ReturnInitInterface)return 2;
	}
}




unsigned char send_at_cmd_and_receive(unsigned char * CmdBuff,int CmdBuffSize,int Ms,const unsigned char *OkStr)
{
	int i,ret,size,pos;
	unsigned char retval=TCP_SendAtTimeout;


	kmy_USART3ReceiveReset();

	SetAutoExitTime(0);
	Ms/=20;
	send_at_cmd(CmdBuff);
	

	*CmdBuff=0;
	
	for(pos=i=0,size=CmdBuffSize;i<Ms;i++)
	{
		ret= ReadDatasFromtty(fd_ttyS1,CmdBuff+pos,size,0,20000);
		if(ret)
		{
			pos+=ret;
			size-=ret;


		}
		else
		{
			if(strstr((char const*)CmdBuff,(char const*)OkStr))break;
			retval=GetKeyValue();
			if(retval!=KEY_NONE)
			{
				if(retval==KEY_ReturnInitInterface||retval==KEY_ReturnPrevious){retval=TCP_UserSelfReturn;break;}
			}

		}
	}

	if(i<Ms)retval=TCP_OK;

	printf("SendAtCmd receive-->%d %s\r\n",pos,CmdBuff);
	return retval;
}


unsigned char send_at_cmd_and_return(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr)
{
	unsigned char tempbuff[1024];
	int i,ret,size,pos;
	unsigned char retval;

	retval=TCP_SendAtTimeout;

	kmy_USART3ReceiveReset();

	SetAutoExitTime(0);
	ms/=20;
	send_at_cmd(cmd);
	for(pos=i=0,size=sizeof(tempbuff);i<ms;i++)
	{
		ret= ReadDatasFromtty(fd_ttyS1,tempbuff+pos,size,0,20000);
		if(ret)
		{
			pos+=ret;
			size-=ret;
		}
		else
		{
			if(strstr((char const*)tempbuff,(char const*)okstr)){retval=TCP_OK;break;}
			if(errstr!=NULL){if(strstr((char const*)tempbuff,(char const*)errstr)){retval=TCP_ERROR;break;}}
		}
	}

	printf("SEND_AT_Re:%s\n",tempbuff);
	return retval;
}

unsigned char send_at_cmd_and_return_NotResposeKey(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr)
{
	unsigned char tempbuff[1024];
	int i,ret,size,pos;
	unsigned char retval;

	retval=TCP_SendAtTimeout;

	printf("SEND_AT:%s\n",cmd);	//设置加CIP  头,执行了这个指令sim900B  把接收的数据以打包的方式从串口输出。+IPD,1460:数据

	SetAutoExitTime(0);
	ms/=20;
	send_at_cmd(cmd);
	for(pos=i=0,size=sizeof(tempbuff);i<ms;i++)
	{
		ret= ReadDatasFromtty(fd_ttyS1,tempbuff+pos,size,0,20000);
		if(ret)
		{
			pos+=ret;
			size-=ret;
		}
		else
		{
			if(strstr((char const*)tempbuff,(char const*)okstr)){retval=TCP_OK;break;}
			if(strstr((char const*)tempbuff,(char const*)errstr)){retval=TCP_ERROR;break;}
		}
	}

	printf("SEND_AT_Re:%s\n",tempbuff);

	return retval;
}

static unsigned char send_at_cmd_and_return_3errs(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr,const unsigned char *errstr2,const unsigned char *errstr3)
{
	unsigned char tempbuff[200];
	int i,ret,size,pos;
	unsigned char retval;
	unsigned char keyval;

	kmy_USART3ReceiveReset();

	retval=TCP_SendAtTimeout;
	printf("SEND_AT:%s\n",cmd);	//设置加CIP  头,执行了这个指令sim900B  把接收的数据以打包的方式从串口输出。+IPD,1460:数据

	SetAutoExitTime(0);
	ms/=20;
	send_at_cmd(cmd);
	for(pos=i=0,size=sizeof(tempbuff);i<ms;i++)
	{
		ret= ReadDatasFromtty(fd_ttyS1,tempbuff+pos,size,0,20000);
		if(ret)
		{
			pos+=ret;
			size-=ret;
		}
		else
		{

			if(strstr((char const*)tempbuff,(char const*)okstr)){retval=TCP_OK;break;}
			if(strstr((char const*)tempbuff,(char const*)errstr)){retval=TCP_ERROR;break;}
			if(strstr((char const*)tempbuff,(char const*)errstr2)){retval=TCP_ERROR2;break;}
			if(strstr((char const*)tempbuff,(char const*)errstr3)){retval=TCP_ERROR3;break;}

			keyval=GetKeyValue();
			if(keyval==KEY_ReturnInitInterface||keyval==KEY_ReturnPrevious){retval=TCP_UserSelfReturn;break;}
		}
	}

	printf("SEND_AT_Re:%s\n",tempbuff);
	return retval;
}

char SeperateIpAndPort(char *IPandPort, char *IP, char *Port)
{
	char *p;
	
	p=IPandPort;
	if(strlen(IPandPort)<9)return 1;
	
	p=strstr(p,",");
	if(p==NULL) return 1;
	strncat(IP,IPandPort,p-IPandPort);
	p++;
	strcpy(Port,p);
	return 0;//IP correct
}

//参数ipaddr 是要连接的ip  地址，
//其格式是如这样"116.255.144.110,9100"
//连接成功返回宏定义TCP_OK
//连接失败返回其它宏定义
static unsigned char _tcp_Connect(unsigned char *DomainOrIP)
{
	unsigned char tempbuff[150];
	unsigned char serverip[100];
	unsigned char serverport[6];
	unsigned char i;

    if(send_at_cmd_and_return("AT+QISTAT\r",3000,"IP CLOSE","ERROR")!=TCP_OK)
    {
    	if(send_at_cmd_and_return("AT+QIREGAPP\r",60000,"OK","ERROR")!=TCP_OK)
    	{
			return TCP_ERROR;
    	}
		if(send_at_cmd_and_return("AT+QIACT\r",60000,"OK","ERROR")!=TCP_OK)
		{
			return TCP_ERROR;
		}
		if(send_at_cmd_and_return("AT+QILOCIP\r",3000,".","ERROR")!=TCP_OK)
		{
		    return TCP_ERROR;
		}
    }

	memset(serverip,0,sizeof(serverip));	
	memset(serverport,0,sizeof(serverport));

	if(*DomainOrIP<'0'||*DomainOrIP>'9')
    {
    	strcpy((char*)serverip,(char*)DomainOrIP);
		strcpy((char*)serverport,"80");
    }
	else
	{
		SeperateIpAndPort((char *)DomainOrIP,(char *)serverip,(char *)serverport);
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

unsigned char tcp_Connect(unsigned char *DomainOrIP)
{
	unsigned char i;
	unsigned char ret=TCP_ERROR;

	printf("\r\n\r\nTCP_Connect\r\n");
    if(*DomainOrIP<'0'||*DomainOrIP>'9')
    {
    	send_at_cmd_and_return("AT+QIDNSIP=1\r",3000,"OK","ERROR");
    }
	else
	{
    	send_at_cmd_and_return("AT+QIDNSIP=0\r",3000,"OK","ERROR");
	}
	send_at_cmd_and_return("AT+QIHEAD=1\r",2000,"OK","ERROR");

	for(i=0;i<3;i++)
	{
		ret=_tcp_Connect(DomainOrIP);
		
		if(ret==TCP_OK||ret==TCP_UserSelfReturn)
		{
			goto fucexit;
		}
		else
		{
    	    send_at_cmd_and_return("AT+QIDEACT\r",60000,"DEACT OK","ERROR");
		}
	}
	ret=TCP_ConnectFailure;

	fucexit:
	return ret;
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

	printf("\r\n\r\nTCP_Send\r\n");

	strcpy((char *)tempbuff,"AT+QISEND=");
	my_itoa(temp,len);
	strcat((char *)tempbuff,(char const*)temp);
	strcat((char *)tempbuff,"\r");

	i=send_at_cmd_and_return((unsigned char const *)tempbuff,2000,">","ERROR");
	if(i!=TCP_OK){i=TCP_SendFailure;goto fucexit;}
	
	my_charhexprintf_buff("SendBuff=",buff,len);
	
	i=Gprs_SendDataLen(buff, len,2000,"SEND OK","ERROR");
	
	fucexit:
	return i;
}

unsigned char tcp_receive (unsigned char *recBuff, unsigned int recBuffSize, const unsigned char *okstr, unsigned short second)
{
	unsigned int timeout_cnt, ret;
	unsigned char *pRec;
	unsigned int receive_len = 0;
	unsigned char keyval = KEY_NONE;

	pRec = recBuff;
	SetAutoExitTime (0);
	SetBLOverTime (0);
	second = second * 10;

	for (timeout_cnt = 0; timeout_cnt < second; timeout_cnt++)
	{
		keyval = kmy_GetKeyValue();

		if (keyval != KEY_NONE) return TCP_UserSelfReturn;

		ret = ReadDatasFromtty (fd_ttyS1, (unsigned char*) pRec, recBuffSize, 0, 100000);
		pRec += ret;
		recBuffSize -= ret;
		receive_len += ret;
		if(strstr((char*)recBuff,(char const*)okstr))
		{
			ret = ReadDatasFromtty (fd_ttyS1, (unsigned char*) pRec, recBuffSize, 0, 3000000);
    		pRec += ret;
    		recBuffSize -= ret;
    		receive_len += ret;
			return TCP_ReceiveOK;
		}
		if (recBuffSize == 0) return TCP_ReceiveBuffNotEnough;
	}
	
	return TCP_ReceiveTimeOut;
}

//断开tcp  连接关闭tcp  连接
void tcp_DisConnect(void)
{
	send_at_cmd_and_return("AT+QICLOSE\r",3000,"OK","ERROR");
}

unsigned char ReadPINPUKTimes(unsigned char PINTimes[2],unsigned char PUKTimes[3])
{
	char *p,CmdBuff[100];
	unsigned char ret=ERROR;

	strcpy((char *)CmdBuff,"AT+SPIC\r");
	PINTimes[0]=PUKTimes[0]=0;
	send_at_cmd_and_receive((unsigned char*)CmdBuff,sizeof(CmdBuff),3000,"OK\r");	//读取PIN 和PUK 码的剩余次数
	p=strstr((char const *)CmdBuff,"SPIC:");	//+SPIC: 3,3,10,10
	if(p)
	{
		p+=5;
		while(*p<'0'||*p>'9'){p++;if(p>&CmdBuff[99])goto tuichu;}		//0
		PINTimes[0]=*p;PINTimes[1]=0;
		while(*p>='0'&&*p<='9'){p++;if(p>&CmdBuff[99])goto tuichu;}		//,
		while(*p<'0'||*p>'9'){p++;if(p>&CmdBuff[99])goto tuichu;}		// 2
		while(*p>='0'&&*p<='9'){p++;if(p>&CmdBuff[99])goto tuichu;}		// ,

		while(*p<'0'||*p>'9'){p++;if(p>&CmdBuff[99])goto tuichu;}		// 10
		PUKTimes[0]=*p;
		p++;
		if(*p>='0'&&*p<='9'){PUKTimes[1]=*p;PUKTimes[2]=0;}
		else{PUKTimes[1]=0;}
		ret=0;
	}

	tuichu:
	#ifdef DEBUG_PRINTF
	printf("ret=%d %s,%s\n",ret,PINTimes,PUKTimes);
	#endif
	return ret;
}

void PUK_Required(void)
{
	char ret,PinTimes[2],PukTimes[3];
	unsigned char CmdBuff[1024];
	unsigned char PUKin[20];
	unsigned char NewPin[7];
	unsigned char NewPin2[7];
	PasswordInputArgument arg;

	Again:
	ret=ReadPINPUKTimes((unsigned char *)PinTimes,(unsigned char *)PukTimes);
	if(ret==0)
	{
		clear_lcd();
		DrawTitle_to_row_Language2(0,"Input PUK","Input PUK");
		strcpy((char *)CmdBuff,"remain times:");
		
		strcat((char *)CmdBuff,PukTimes);
		DrawTitle_to_row_Language2(1,CmdBuff,CmdBuff);

		arg.clearlcdflag=0;
		arg.DiplayRow=2;
		arg.LocalTitle=NULL;
		arg.OtherTitle=NULL;
		arg.keyval=KEY_NONE;
		arg.receive=PUKin;
		arg.recSize=9;
		arg.xingflag=1;
		ret=PassWordInput(&arg);
		if(ret!=KEY_Enter)goto Again;
		if(strlen((char const*)PUKin)!=8)
		{
			clear_lcd();
			DrawTitle_to_row_Language2(2,"length error","length error");
			sleep(2);
			goto Again;
		}

		arg.clearlcdflag=1;
		arg.DiplayRow=2;
		arg.LocalTitle="Input New PIN";
		arg.OtherTitle="Input New PIN";
		arg.keyval=KEY_NONE;
		arg.receive=NewPin;
		arg.recSize=5;
		arg.xingflag=1;
		ret=PassWordInput(&arg);
		
		if(ret!=KEY_Enter)goto Again;
		if(strlen((char const*)NewPin)!=4)
		{
			clear_lcd();
			DrawTitle_to_row_Language2(1,"length error","length error");
			sleep(2);
			return;
		}

		arg.clearlcdflag=1;
		arg.DiplayRow=2;
		arg.LocalTitle="Again New PIN";
		arg.OtherTitle="Again New PIN";
		arg.keyval=KEY_NONE;
		arg.receive=NewPin2;
		arg.recSize=5;
		arg.xingflag=1;
		ret=PassWordInput(&arg);
		if(ret!=KEY_Enter)goto Again;
		if(strlen((char const*)NewPin)!=4)
		{
			clear_lcd();
			DrawTitle_to_row_Language2(1,"length error","length error");
			sleep(2);
			return;
		}
		if(strcmp((char const*)NewPin,(char const*)NewPin2)!=0)
		{
			clear_lcd();
			DrawTitle_to_row_Language2(2,"Two times input",NULL);
			DrawTitle_to_row_Language2(3,"is not same","takie same");
			sleep(2);
			goto Again;
		}

		strcpy((char *)CmdBuff,"AT+CPIN=\"");		//AT+CPIN="66612458","0907"
		strcat((char *)CmdBuff,(char const*)PUKin);
		strcat((char *)CmdBuff,"\",\"");
		strcat((char *)CmdBuff,(char const*)NewPin);
		strcat((char *)CmdBuff,"\"\r");
		ret=send_at_cmd_and_receive(CmdBuff,sizeof(CmdBuff),3000,"OK\r");
		if(ret!=TCP_OK)
		{
			clear_lcd();
			DrawTitle_to_row_Language2(2,"PUK Error","PUK Error");
			goto Again;
		}
		else
		{
			clear_lcd();
			DrawTitle_to_row_Language2(2,"PUK Right","PUK Right");
			sleep(2);
			return;
		}
	}
	else
	{
		goto Again;
	}
}


char GetIMEI(unsigned char *buff)
{
	char rcv_buf[50];
	char *p;
	int i;
	char retval;

	send_at_cmd("AT+GSN\r");
	retval=ReadDatasFromSIM300_OKstr((u8*)rcv_buf,sizeof(rcv_buf),2,"OK");
	printf("rcv_buf=%s\n",rcv_buf);

	p=rcv_buf;i=0;
	while((*p<'0'||*p>'9')&&i<sizeof(rcv_buf)){p++;i++;}
	for(i=0;i<24;i++){rcv_buf[i]=*p;p++;if(*p<'0'||*p>'9')break;}
	i++;
	rcv_buf[i]=0;

	strcpy((char *)buff,rcv_buf);

	return retval;
}

char GetCCID(unsigned char *buff)
{
	char CmdBuff[200];
	unsigned char retval,i,j,temp[50];

	strcpy(CmdBuff,"AT+QCCID\r");
	retval=send_at_cmd_and_receive((unsigned char*)CmdBuff,sizeof(CmdBuff),2000,"OK");
    if(retval!=TCP_OK)return 0;
	i=0;
	while(CmdBuff[i]<'0'||CmdBuff[i]>'9')
	{
		i++;
		if(i>=200) return 0;
	}
	for(j=0;i<49;i++)
	{
		if(CmdBuff[i]<0x20)break;
		temp[j]=CmdBuff[i];j++;
	}
	temp[j]=0;

	strcpy((char *)buff,(char const*)temp);
	return 1;
}

char set_apn (void)
{
	char Regist[200];
	char retval;
	char cnt = 0;

	clear_lcd();
	DrawTitle_to_row_Language2 (2, "正在设置APN", "Setting APN");
again:
	memset ( (char*) Regist, 0, sizeof (Regist) );
	strcat ( (char*) Regist, "AT+QICSGP=1,\"");
	strcat ( (char*) Regist, (char*) ApnApn);
	strcat ( (char*) Regist, "\",\"");
	strcat ( (char*) Regist, (char*) ApnUsername);
	strcat ( (char*) Regist, "\",\"");
	strcat ( (char*) Regist, (char*) ApnPassword);
	strcat ( (char*) Regist, "\"\r");
	retval = send_at_cmd_and_return ( (const unsigned char*) Regist, 3000, "OK", "ERROR");

	if (retval == TCP_OK)return retval;

	retval = GetKeyValue();

	if (retval == KEY_ReturnInitInterface || retval == KEY_ReturnPrevious) return TCP_ERROR;

	if (cnt++ > 7) return retval;

	retval = send_at_cmd_and_return ("AT+QIDEACT\r", 2000, "OK", "ERROR");

	if (retval == TCP_OK || retval == TCP_ERROR)
	{
		retval = WaitkeyAndAutoExit (3);

		if (retval == KEY_ReturnInitInterface || retval == KEY_ReturnPrevious) return TCP_ERROR;
	}

	goto again;
}









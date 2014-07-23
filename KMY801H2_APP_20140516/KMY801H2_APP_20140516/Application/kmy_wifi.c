#include "kmy_wifi.h"
#include "kmy_WiFi_SPI.h"
#include "kmy_WiFi_USARTDrv.h"
#include "gprs.h"

#include "stm32f10x_gpio.h"
#include "key.h"
#include "kmy_Time.h"
#include "kmy_USART1Drv.h"

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
#include "MenuOperate.h"


static WLAN_SSID Wsisrvy_ssid[MYSSID_SIZE]={0};
WLAN_SSID *DS_ssid;

u32 ReadDatasFromUSART(u8 *rcv_buf,u32 size,int sec,int usec)
{
	u32 reclen;

	reclen=kmy_WiFi_USARTReceiveData(rcv_buf,size,sec*1000+usec/1000);
	*(rcv_buf+reclen)=0;

	return reclen;
}


unsigned char send_wifi_cmd_and_return_NotResposeKey(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr)
{
	unsigned char tempbuff[1024];
	int i,ret,size,pos;
	unsigned char retval;

	retval=TCP_SendAtTimeout;

	//printf("SEND_wifi:%s\n",cmd);	//设置加CIP  头,执行了这个指令sim900B  把接收的数据以打包的方式从串口输出。+IPD,1460:数据

	SetAutoExitTime(0);
	ms/=20;
	send_wifi_cmd(cmd);
	for(pos=i=0,size=sizeof(tempbuff);i<ms;i++){
		ret= ReadDatasFromUSART(tempbuff+pos,size,0,20000);
		if(ret){
			pos+=ret;
			size-=ret;
		}else{
			if(strstr((char const*)tempbuff,(char const*)okstr)){retval=TCP_OK;break;}
			if(strstr((char const*)tempbuff,(char const*)errstr)){retval=TCP_ERROR;break;}
		}
	}

	//printf("SEND_AT_Re:%s\n",tempbuff);

	return retval;
}


unsigned char send_wifi_cmd_and_receive(unsigned char * CmdBuff,int CmdBuffSize,int Ms,const unsigned char *OkStr)
{
	int i,ret,size=CmdBuffSize,pos;
	unsigned char retval=TCP_SendAtTimeout;
	
	kmy_WiFi_USARTReceiveReset();

	SetAutoExitTime(0);
	Ms/=20;
	send_wifi_cmd(CmdBuff);
	*CmdBuff=0;
	for(pos=i=0;i<Ms;i++)
	{
		ret= ReadDatasFromUSART(CmdBuff+pos,size,0,20000);
		if(ret)
		{
			pos+=ret;
			size-=ret;
		}
		else
		{
			if(strstr((char const*)CmdBuff,(char const*)OkStr))
			{
				break;
			}
			retval=GetKeyValue();
			if(retval!=KEY_NONE){
				if(retval==KEY_ReturnInitInterface||retval==KEY_ReturnPrevious){retval=TCP_UserSelfReturn;break;}
			}
		}
	}

	if(i<Ms)retval=TCP_OK;
	printf("WIFI receive-->%s\r\n",CmdBuff);
	return retval;
}

unsigned char send_wifi_cmd_and_norespone(unsigned char * CmdBuff,int CmdBuffSize)
{
	int i,ret,size=CmdBuffSize,pos;
	unsigned char retval=TCP_OK;

	kmy_WiFi_USARTReceiveReset();

	SetAutoExitTime(0);
	send_wifi_cmd(CmdBuff);
	*CmdBuff=0;
	//kmy_USART1ReceiveReset();
	for(pos=i=0;i<2;i++){
		ret= ReadDatasFromUSART(CmdBuff+pos,size,0,20000);
		if(ret){
			pos+=ret;
			size-=ret;
			}
		else{
			retval=GetKeyValue();
			if(retval!=KEY_NONE){
				if(retval==KEY_ReturnInitInterface||retval==KEY_ReturnPrevious){retval=TCP_UserSelfReturn;break;}
			}
		}
	}
	if(*CmdBuff)
		printf("WIFI receive-->%s\r\n",CmdBuff);
	else
		printf("WIFI not need receive");
	return retval;
}


void kmy_WifiInit(void)
{
	kmy_WiFi_USARTInit(9600);
	kmy_SPIInit();
    kmy_WiFiOnOff (1);
	sleep(10);
	kmy_WiFi_USARTReceiveReset();
	GetWiFiInfo();
	Set_wifi_manconn('1');
		printf("\n kmy_WifiInit");
}


void send_wifi_cmd(const unsigned char *cmd)
{
	printf("Send_wifi_Cmd-->%s\r\n",cmd);
	kmy_WiFi_USARTSendString(cmd);
}


unsigned char Set_wifi_cmd(char *cmd,char *prec,int timeout)
{
	
	unsigned char cmdbuf[2048]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"%s\r",cmd);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,timeout,"uart>");
	memcpy(prec,(char *)cmdbuf,strlen((char *)cmdbuf));
	return recval;
}

unsigned char Set_wifi_Reboot(void)
{
	unsigned char cmdbuf[20]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"reboot\r");
	recval = send_wifi_cmd_and_norespone(cmdbuf,sizeof(cmdbuf)-1);
	sleep(10);	
	kmy_WiFi_USARTReceiveReset();
	return recval;
}

unsigned char Set_wifi_IpConfig(void)
{
	unsigned char cmdbuf[20]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"ipconfig\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

unsigned char Set_wifi_Ip(unsigned char *ipAddr)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;
	if(ipAddr == NULL)
		sprintf((char *)cmdbuf,"setip \r");
	else
		sprintf((char *)cmdbuf,"setip %s \r",ipAddr);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}
unsigned char Set_wifi_mask(unsigned char *ipAddr)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(ipAddr == NULL)
		sprintf((char *)cmdbuf,"setmask \r");
	else
		sprintf((char *)cmdbuf,"setmask %s\r",ipAddr);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

unsigned char Set_wifi_gateway(unsigned char *ipAddr)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;


	if(ipAddr == NULL)
		sprintf((char *)cmdbuf,"setgateway \r");
	else
		sprintf((char *)cmdbuf,"setgateway %s\r",ipAddr);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}


unsigned char Set_wifi_dns(unsigned char *ipAddr)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(ipAddr == NULL)
		sprintf((char *)cmdbuf,"setdns \r");
	else
		sprintf((char *)cmdbuf,"setdns %s\r",ipAddr);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

unsigned char Set_wifi_NetAddr(unsigned char *ip,unsigned char *mask,unsigned char *gateway,unsigned char *dns)
{
	unsigned char recval;
	unsigned char *p,tmp[20];

	p=(u8*)strstr((char *)ip,",");
	if(p==NULL)return 0; 
	memset((char *)tmp,0,20);
	memcpy((char *)tmp,ip,p-ip);
	
	Set_wifi_Ip(tmp);
	Set_wifi_srvport(++p);
	Set_wifi_mask(mask);
	Set_wifi_gateway(gateway);
	recval = Set_wifi_dns(dns);
	return recval;	
}

/**************************************
ipaddr: srvip
**************************************/
unsigned char Set_wifi_ConnectAddr(unsigned char *ipaddr)
{
	int i=0;
	unsigned char keyval;
	char tmp[300];
	char *p=NULL;

	p=strstr((char*)ipaddr,",");
	if(p==NULL)return keyval;
	memset(tmp,0,30);
	memcpy(tmp,(char*)ipaddr,(p-(char*)ipaddr));
	i = strlen((char*)tmp);
	Set_wifi_dsthn((u8 *)tmp);	

	memset(tmp,0,20);
	memcpy(tmp,p+1,strlen((char*)ipaddr)-i);
	Set_wifi_dstport((u8*)tmp);
	keyval=Set_wifi_saveconfig();

	return keyval;
}

unsigned char Set_wifi_serialprot(unsigned char *bdr)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;
	
	if(bdr == NULL)
		sprintf((char *)cmdbuf,"serialport \r");
	else
		sprintf((char *)cmdbuf,"serialport %s 3 2 0 2\r",bdr);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}


/**************************************************
mode: 0 server
	 1 client
***************************************************/
unsigned char Set_wifi_mode(unsigned char mode)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(mode == NULL_CHAR)
		sprintf((char *)cmdbuf,"setmode \r");
	else
		sprintf((char *)cmdbuf,"setmode %c\r",mode);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}



unsigned char Set_wifi_srvport(unsigned char *port)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(port == NULL)
		sprintf((char *)cmdbuf,"setsrvport \r");
	else
		sprintf((char *)cmdbuf,"setsrvport %s\r",port);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

unsigned char Set_wifi_dstport(unsigned char *port)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(port == NULL)
		sprintf((char *)cmdbuf,"setdstport \r");
	else
		sprintf((char *)cmdbuf,"setdstport %s\r",port);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

/**************************************************
status: 0 disable
	   1 enable
***************************************************/
unsigned char Set_wifi_dhcpclient(unsigned char status)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;
	
	if(status == NULL_CHAR)
		sprintf((char *)cmdbuf,"dhcpclient \r");
	else
		sprintf((char *)cmdbuf,"dhcpclient %c\r",status);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,1000,WIFIOKSTR);
	return recval;
}

/**************************************************
protocol: 0 tcp
	     1 udp
***************************************************/
unsigned char Set_wifi_connectype(unsigned char protocol)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(protocol == NULL_CHAR)
		sprintf((char *)cmdbuf,"connectype \r");
	else
		sprintf((char *)cmdbuf,"connectype %c\r",protocol);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

/**************************************************
protocol: 0 tcp
	     1 udp
***************************************************/
unsigned char Set_wifi_manconn(unsigned char status)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(status == NULL_CHAR)
		sprintf((char *)cmdbuf,"manconn \r");
	else
		sprintf((char *)cmdbuf,"manconn %c\r",status);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}


unsigned char Set_wifi_saveconfig(void)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"saveconfig\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,20000,WIFIOKSTR);
	return recval;
}

unsigned char Set_wifi_dsthn(unsigned char *IpAddr)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(IpAddr == NULL)
		sprintf((char *)cmdbuf,"setdsthn \r");
	else
		sprintf((char *)cmdbuf,"setdsthn %s\r",IpAddr);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

unsigned char Set_wifi_connstatus(unsigned char *pstr,unsigned char status)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(status == NULL_CHAR)
		sprintf((char *)cmdbuf,"connstatus\r");
	else
		sprintf((char *)cmdbuf,"connstatus %c\r",status);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,200,WIFIOKSTR);
	
	memcpy((char*)pstr,(char*)cmdbuf,strlen((char*)cmdbuf));
	return recval;
} 


unsigned char Set_wifi_ping(unsigned char *IpAddr,unsigned char *pstr)
{
	unsigned char cmdbuf[500]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"ping %s\r",IpAddr);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,5000,WIFIOKSTR);
	strcpy((char*)pstr,(char*)cmdbuf);
	return recval;
}

unsigned char Set_wifi_default(void)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"setdef\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	sleep(10);	
	kmy_WiFi_USARTReceiveReset();
	return recval;
} 

/**************************************************
ssid len must less than 32 characters
***************************************************/
unsigned char Set_wifi_ssid(unsigned char *ssid,unsigned char *retssid)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	unsigned char *p=NULL;
	if(ssid == NULL)
		sprintf((char *)cmdbuf,"setssid \r");
	else
		sprintf((char *)cmdbuf,"setssid %s\r",ssid);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,10000,WIFIOKSTR);
	
	p = (unsigned char *)strstr((char*)cmdbuf,"SSID");
	if(p)
	{
		p += 6;
		while(*p!= 0x0d && *p!= 0x00)
		{
			*retssid = *p;
			retssid++;
			p++;
		}
	}
	return recval;
}

/**************************************************
nt: 0 Infra mode
	1 Ad-hoc mode
***************************************************/
unsigned char Set_wifi_nt(unsigned char nt)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(nt == NULL_CHAR)
		sprintf((char *)cmdbuf,"setnt \r");
	else
		sprintf((char *)cmdbuf,"setnt %c\r",nt);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,20000,WIFIOKSTR);
	return recval;
}


/**************************************************
wp: WPA Passphrase,8~63 characters
***************************************************/
unsigned char Set_wifi_wp(unsigned char *wp)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"setwp %s\r",wp);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,20000,WIFIOKSTR);
	return recval;
}

/**************************************************
et: 0 No security
	1 WEP64
	2 WEP128
	3 TKIP
	4 AES
***************************************************/
unsigned char Set_wifi_et(unsigned char et)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(et == NULL_CHAR)
		sprintf((char *)cmdbuf,"setet \r");
	else
		sprintf((char *)cmdbuf,"setet %c\r",et);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,5000,WIFIOKSTR);
	return recval;
}

/**************************************************
***************************************************/
unsigned char Set_wifi_sisrvy(unsigned char *pssidbuf,int *preclen)
{
	unsigned char cmdbuf[2048]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"sisrvy\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,5000,"Rcv WiFi Connect Command");
	*preclen = strlen((char*)cmdbuf);
	memcpy(pssidbuf,cmdbuf,*preclen);
	return recval;
}

/**************************************************
index: ID in sitesurvey table
***************************************************/
unsigned char Set_wifi_jbss(char* index,unsigned char *pstr)
{
	unsigned char cmdbuf[500]={0};
	unsigned char recval;


	if(index == NULL)
		sprintf((char *)cmdbuf,"jbss \r");
	else
		sprintf((char *)cmdbuf,"jbss %s\r",index);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,10000,"uart>");
	memcpy((char*)pstr,(char*)cmdbuf,strlen((char*)cmdbuf));
	return recval;
}

/**************************************************

***************************************************/
unsigned char Set_wifi_wificonfig(char *prec)
{
	unsigned char cmdbuf[1024]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"wificonfig\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);

	memcpy((char*)prec,(char*)cmdbuf,strlen((char*)cmdbuf));
	return recval;
}

/**************************************************
status: 0 disable
	   1 enable
***************************************************/
unsigned char Set_wifi_dhcpsrv(char status)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	if(status == NULL_CHAR)
		sprintf((char *)cmdbuf,"setdhcpsrv \r");
	else
		sprintf((char *)cmdbuf,"setdhcpsrv %c\r",status);
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	return recval;
}

/**************************************************
status: 0 disable
	   1 enable
***************************************************/
unsigned char Set_wifi_showcounter(char *prec)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"showcounter\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);

	memcpy((char*)prec,(char*)cmdbuf,strlen((char*)cmdbuf));
	return recval;
}


unsigned char Set_wifi_version(char *prec)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"version\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,500,WIFIOKSTR);
	memcpy((char*)prec,(char*)cmdbuf,strlen((char*)cmdbuf));
	return recval;
}

unsigned char wifi_TestResponse(void)
{
	unsigned char cmdbuf[100]={0};
	unsigned char recval;

	sprintf((char *)cmdbuf,"\r");
	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,1000,"\r");
	return recval;
}

unsigned char SearchWLAN(void)
{
	unsigned char ret;
	unsigned char ssidbuf[2048]={0};
	int ilen,i;

	//搜索前，先清空原来的搜索信息
	for(i=0;i<MYSSID_SIZE;i++){
		memset(Wsisrvy_ssid[i].Index,0,sizeof(Wsisrvy_ssid[i].Index));
		memset(Wsisrvy_ssid[i].Bssid,0,sizeof(Wsisrvy_ssid[i].Bssid));
		memset(Wsisrvy_ssid[i].Ssid,0,sizeof(Wsisrvy_ssid[i].Ssid));
		memset(Wsisrvy_ssid[i].Type,0,sizeof(Wsisrvy_ssid[i].Type));
		memset(Wsisrvy_ssid[i].Ch,0,sizeof(Wsisrvy_ssid[i].Ch));
		memset(Wsisrvy_ssid[i].Rssi,0,sizeof(Wsisrvy_ssid[i].Rssi));
		memset(Wsisrvy_ssid[i].Security,0,sizeof(Wsisrvy_ssid[i].Security));
		printf("\nIndex=%s, Bssid=%s",Wsisrvy_ssid[i].Index, Wsisrvy_ssid[i].Bssid);
		printf("\nSsid=%s , Type=%s ,Chan=%s",Wsisrvy_ssid[i].Ssid, Wsisrvy_ssid[i].Type, Wsisrvy_ssid[i].Ch);
		printf("\nRssi=%s,Secu=%s",Wsisrvy_ssid[i].Rssi, Wsisrvy_ssid[i].Security);

	}
	//printf("MYSSID_SIZE=%d",MYSSID_SIZE);
	Set_wifi_sisrvy(ssidbuf,&ilen);
	  	printf("\nssidbuf = %s",ssidbuf);
	ret = WifiSSID_TO_MySSID(ssidbuf);
	printf("\nret = WifiSSID_TO_MySSID(ssidbuf)= %c\n",ret);
	if(ret != TCP_OK)
	{
		printf("WifiSSID_TO_MySSID failed:%d\r\n",ret);
	}
	return ret;	
}

unsigned char WifiSSID_TO_MySSID(unsigned char *pwifissid)
{
	unsigned char *p = NULL;
	unsigned char *p1 = NULL;
	unsigned char i=0;
	
	p = (u8*)strstr((char *)pwifissid,"00");
	
	if(!p) //have no wlan network
	{
	 printf("\n AURO =%c", pwifissid);
		return WIFI_Error_NONET;
	}
	
	for(i=0;i<MYSSID_SIZE;i++)
	{
		if(!(memcmp(p,"Rcv WiFi Connect Command",2)) || *p == 0)//over
		{
			//printf("sisrvy to myssid over\r\n");
			break;
		}
		//index
		memcpy(Wsisrvy_ssid[i].Index,p,2);
		p += 3;
		//bssid
		memcpy(Wsisrvy_ssid[i].Bssid,p,17);
		p += 18;
		//ssid
		memcpy(Wsisrvy_ssid[i].Ssid,p,31);
		p += 33;
		//type
		memcpy(Wsisrvy_ssid[i].Type,p,4);
		p += 5;
		//chanel
		memcpy(Wsisrvy_ssid[i].Ch,p,3);
		p += 4;
		//rssi
		memcpy(Wsisrvy_ssid[i].Rssi,p,4);
		p += 5;
		//secvrty
		p1 = (u8*)strstr((char *)p,"\r\n\r\r");
		if(p1==NULL)
		{
			p1 = (u8*)strstr((char *)p,"\r\n\r");
			if(p1==NULL)return WIFI_Error_NONET;
			else memcpy(Wsisrvy_ssid[i].Security,p,p1-p);
			Clear_space(&Wsisrvy_ssid[i]);
			return TCP_OK;
		}
		else
		{
			memcpy(Wsisrvy_ssid[i].Security,p,p1-p);
		}
		p = p1+4;

		Clear_space(&Wsisrvy_ssid[i]);
	}
	return TCP_OK;
}


void Clear_space(WLAN_SSID *pmyssid)
{
	char *p = pmyssid->Index;
	int i;
	for(i=0;i<76;i++){
		if(*(p+i) == 0x20){
			*(p+i) = 0;
		}
	}			
}


char ConnectSSID(char *Msg)
{

	unsigned char retval;

	retval = ConnectWLAN(DS_ssid);

	return retval;
}

/**************************************
par传递wifi设置参数，一个char标识一个参数
参数依次为
C/S mode;	 dhcpclient-on/off;  workmode;  enctype; connectype; manconn
**************************************/
unsigned char WIFI_Set_WLANPar(void)
{
	unsigned char keyval;

	Set_wifi_nt(workmode[0]);
	Set_wifi_et(enctype[0]);
	Set_wifi_dhcpclient(dhcpclient[0]);
	Set_wifi_mode(CSmode[0]);
	Set_wifi_connectype('0');
	keyval = Set_wifi_manconn('1');
	sleep(3);
	return keyval;
}

/**************************************
连接一个sisrvy搜索之后的网络
**************************************/
unsigned char ConnectWLAN(WLAN_SSID *pmyssid)
{
	unsigned char keyval;
	char tmp[300];

	keyval = Set_wifi_ssid((u8*)pmyssid->Ssid,(u8*)tmp);

	if(keyval == TCP_OK)
	{
		strcpy((char *)WLANSSID,pmyssid->Ssid);
		printf("WLANSSID>>%s\r\n",WLANSSID);
		if (!StorageVariable() )
		{
			printf("Storage falure!\r\n");
		}
	}
	
	return keyval;
}

unsigned char WiFi_Connect(unsigned char *ipaddr)
{
	char retval,i=0;
	char IP[45]={0};
	char Port[6]={0};

	unsigned int start;
    unsigned char connstatus[100]={0};
	SeperateIpAndPort((char*)ipaddr,IP,Port);
	printf("IP:%s\r\n",IP);
	printf("Port:%s",Port);
	retval=Set_wifi_dsthn((unsigned char*)IP);
	if(retval != TCP_OK)
	{
	    msleep(200);
		kmy_WiFi_USARTReceiveReset();
		retval=Set_wifi_dsthn((unsigned char*)IP);
	    if(retval != TCP_OK)return TCP_ConnectFailure;
	}
	retval=Set_wifi_dstport((unsigned char*)Port);
	if(retval != TCP_OK)return TCP_ConnectFailure;
	
	start = my_GetCurrentMs();
	while(1)
	{
		retval = GetKeyValue();
		if(retval==KEY_ReturnInitInterface||retval==KEY_ReturnPrevious ||retval == KEY_Enter)
		{
			return TCP_UserSelfReturn;
		}		
		if(my_GetCurrentMs()-start >1000)
		{
		    Set_wifi_manconn(NULL_CHAR);
		    Set_wifi_connstatus((unsigned char*)connstatus,'1');
			retval=Set_wifi_connstatus((unsigned char*)connstatus,NULL_CHAR);
			if(retval != TCP_OK)return TCP_ConnectFailure;
			if(strstr((char *)connstatus,"CONNECTED"))break;
			i++;
			start = my_GetCurrentMs();
		}
		if(i>10)return TCP_ConnectFailure;
	}
	return retval;
}

unsigned char WiFi_Send(unsigned char *buff,unsigned int len)
{
	unsigned int SendLen = 0;
	printf("Sendbuff:\r\n%s\r\n",buff);
	kmy_SPIReceiveReset();
	SendLen=kmy_SPIWriteData(buff,len);	
    if(SendLen==len)return TCP_SendOK;
	else return TCP_SendFailure;
}

unsigned int ReadDatasFromWiFi(unsigned char *rcv_buf,unsigned int size)
{
	unsigned int reclen;
	
	reclen=kmy_SPIReadData(rcv_buf,size);
	*(rcv_buf+reclen)=0;
	return reclen;
}


unsigned char WiFi_Receive (unsigned char *recBuff, unsigned int recBuffSize, const char *okstr, unsigned long timeout_MilliSeconds)
{
	unsigned int ret;
	unsigned char *pRec;
	unsigned int receive_len = 0;
	unsigned char keyval = KEY_NONE;

	pRec = recBuff;
	SetAutoExitTime (0);
	SetBLOverTime (0);
	
	timeout_MilliSeconds+=kmy_TimeGetMilliseconds();

	while(1)
	{
		keyval = kmy_GetKeyValue();
		if (keyval != KEY_NONE) return TCP_UserSelfReturn;
		
		ret = ReadDatasFromWiFi((unsigned char*) pRec,recBuffSize);
		pRec += ret;
		recBuffSize -= ret;
		receive_len += ret;
		if(strstr((char*)recBuff,(char const*)okstr))
		{
		    sleep(3);
			ret = ReadDatasFromWiFi((unsigned char*) pRec, recBuffSize);
    		pRec += ret;
    		recBuffSize -= ret;
    		receive_len += ret;			
			if (recBuffSize == 0) return TCP_ReceiveBuffNotEnough;		
			return TCP_ReceiveOK;
		}
		if (recBuffSize == 0) return TCP_ReceiveBuffNotEnough;		
		if(kmy_TimeGetMilliseconds()>=timeout_MilliSeconds)return TCP_ReceiveTimeOut;
	}
}

char GetWiFiName(void)
{
	unsigned char Buf[100]={0};
	unsigned char keyval,i;

    for(i=0;i<5;i++)
    {
        memset(Buf,0,sizeof(Buf));
    	keyval = Set_wifi_ssid(NULL,Buf);
    	if(keyval == TCP_OK)
    	{
    	    if(strlen((char*)Buf)==0)break;
    	    printf("Buf:%s\r\n",Buf);
    	    strcpy((char*)WLANSSID,(char*)Buf);
    		printf("Get SSID OK\r\n");
			return 1;
    	}
    }
	memset(WLANSSID,0,sizeof(WLANSSID));
	printf("Get SSID Fail\r\n");
	return 0;
}

char GetGateway(void)
{
	unsigned char Buf[200]={0};
	unsigned char keyval,i;
	char *p1,*p2;

    for(i=0;i<5;i++)
    {
        memset(Buf,0,sizeof(Buf));
        sprintf((char *)Buf,"ipconfig\r");
    	keyval = send_wifi_cmd_and_receive(Buf,sizeof(Buf)-1,500,WIFIOKSTR);
        if(keyval == TCP_OK)
        {
            printf("Buf:%s\r\n",Buf);
    		p1 = strstr((char*)Buf,"Gateway: ");
    		if(p1==NULL)return 0;
    		p1 += 9;
    		p2 = strstr((char*)Buf,"Ok");
    		if(p2==NULL)return 0;
    		p2 -= 3;
			memset(GatewayBuff,0,sizeof(GatewayBuff));
            strncat((char*)GatewayBuff,p1,p2-p1);
    		printf("GatewayBuff:%s\r\n",GatewayBuff);
        	printf("Get Gateway OK\r\n");
    		return 1;
        }
    }
	printf("Get Gateway Fail\r\n");
	return 0;
}

char PingGateway(void)
{
	unsigned char cmdbuf[500]={0};
	unsigned char recval,i;
	
    for(i=0;i<5;i++)
    {
        sprintf((char *)cmdbuf,"ping %s\r",GatewayBuff);
    	recval = send_wifi_cmd_and_receive(cmdbuf,sizeof(cmdbuf)-1,10000,WIFIOKSTR);
    	if(recval == TCP_OK)
    	{
    	    if(strstr((char*)cmdbuf,"Received=0")==NULL) 
			{
				printf("Connected to WiFi\r\n");
				return 1;
			}
    	}
    }
	
	printf("Disconnected to WiFi\r\n");
    return 0;
}


char GetWiFiInfo(void)
{
	unsigned char recval;

	recval = GetWiFiName();
	if(recval != 1)return recval;
	
	recval = GetGateway();	
	if(recval != 1)return recval;
	
	recval = PingGateway();
    return recval;
}


const unsigned char pstrwifiico[6][32]=
{
	//one signal
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},
	//two	signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},
	//three signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	},	

	//four signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00

	},
	//full signal
	{0x00,0x00,0x00,0x00,0x80,0x00,0xE0,0x00,0xF8,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x06,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00
	},
	//no signal
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};



void DisplayWiFiSignal(unsigned char Signal)
{
    if(Signal==0)draw_pic(pstrwifiico[5],0,0,16,12);
	else
	{
		Signal = Signal/10; 
		if(Signal>4) Signal = 4;
		draw_pic(pstrwifiico[Signal],0,0,16,12);
	}
}

char DisplayNetwork(void)
{
 	int i;
	unsigned char tmp,*p=NULL;
	static struct MenuFuc_Language2_Pic Menu[MYSSID_SIZE];
	struct MenuOperate_Language2_Pic MenuOp={Menu,10,0,0,0,4,1,0,0,};

	//printf("init MenuFuc_Language3\r\n");
	for(i=0;i<MYSSID_SIZE;i++){
	if(Wsisrvy_ssid[i].Index[0] == 0)
			break;
		
		//Menu[i].MenuStrLocal = Wsisrvy_ssid[i].Index;
		Menu[i].MenuStrLocal = (u8*)Wsisrvy_ssid[i].Ssid;
		//Menu[i].MenuStrOther= Wsisrvy_ssid[i].Index;
		Menu[i].MenuStrOther = (u8*)Wsisrvy_ssid[i].Ssid;
		Menu[i].MenuFuc = ConnectSSID;

		// 显示wifi信号
		p = (u8 *)Wsisrvy_ssid[i].Rssi;
		my_TwoAscciiToOneHex(&p,&tmp);
		//printf("singal tmp = %d",tmp);
		tmp = tmp/10; if(tmp>4) tmp = 4;
		Menu[i].pPic = pstrwifiico[tmp];
		
	}
	
	MenuOp.MenuTotal = i;//修改菜单的总条数
	if(i==0){//have no wlan network
		printf("no network\r\n");
		return KEY_ReturnPrevious;
		
	}

	text_out_to_row_x_Language2(4,70,"连接","connect");

	
	while(1)
    {
		MenuHandler_Language2_Pic(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter)
		{
			//set connet ssid
			DS_ssid = &(Wsisrvy_ssid[MenuOp.Menucnt]);
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
    		return MenuOp.FucRetVal;
		}
		else
		{
			InitFlag = 1;
			CurProc=CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}


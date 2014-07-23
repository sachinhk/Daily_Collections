#ifndef _KMY_WIFI_H_
#define _KMY_WIFI_H_




#define WIFIOKSTR "Ok"
#define WIFIERROR "Error"

#define WIFI_CONNECTED 0x30
#define WIFI_TCP_CONNECTED 0x3F

#define WIFI_Send_Error 0x31
#define WIFI_Error_NONET 0x32
#define WIFI_TCP_UNCONNECT 0x33

#define WifiDebug

#define NULL_CHAR 	0xff
#define SSID_SIZE   20
#define MYSSID_SIZE 15

//WLAN_SSID
typedef struct 
{
	char Index[3];
	char Bssid[18];
	char Ssid[32];
	char Type[5];
	char Ch[4];
	char Rssi[5];
	char Security[9];
}WLAN_SSID;

//extern WLAN_SSID Wsisrvy_ssid[MYSSID_SIZE];
//extern WLAN_SSID *DS_ssid;


//void kmy_wifidebug(void);

void send_wifi_cmd(const unsigned char *cmd);

unsigned char send_wifi_cmd_and_receive(unsigned char * CmdBuff,int CmdBuffSize,int Ms,const unsigned char *OkStr);
unsigned char send_wifi_cmd_and_return_NotResposeKey(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr);
unsigned char send_wifi_cmd_and_norespone(unsigned char * CmdBuff,int CmdBuffSize);



void kmy_WifiInit(void);
unsigned char wifi_TestResponse(void);

unsigned char Set_wifi_cmd(char *cmd,char *prec,int timeout);
unsigned char Set_wifi_Reboot(void);
unsigned char Set_wifi_IpConfig(void);


unsigned char Set_wifi_Ip(unsigned char *ipAddr);
unsigned char Set_wifi_mask(unsigned char *ipAddr);
unsigned char Set_wifi_gateway(unsigned char *ipAddr);
unsigned char Set_wifi_dns(unsigned char *ipAddr);
unsigned char Set_wifi_NetAddr(unsigned char *ip,unsigned char *mask,unsigned char *gateway,unsigned char *dns);
unsigned char Set_wifi_serialprot(unsigned char *bdr);
unsigned char Set_wifi_mode(unsigned char mode);
unsigned char Set_wifi_srvport(unsigned char *port);
unsigned char Set_wifi_dstport(unsigned char *port);
unsigned char Set_wifi_dhcpclient(unsigned char status);
unsigned char Set_wifi_connectype(unsigned char protocol);
unsigned char Set_wifi_manconn(unsigned char status);
unsigned char Set_wifi_saveconfig(void);
unsigned char Set_wifi_dsthn(unsigned char *IpAddr);
unsigned char Set_wifi_connstatus(unsigned char *pstr,unsigned char status);
unsigned char Set_wifi_ping(unsigned char *IpAddr,unsigned char *pstr);
unsigned char Set_wifi_default(void);
unsigned char Set_wifi_ssid(unsigned char *ssid,unsigned char *retssid);
unsigned char Set_wifi_nt(unsigned char nt);
unsigned char Set_wifi_wp(unsigned char *wp);
unsigned char Set_wifi_et(unsigned char et);
unsigned char Set_wifi_sisrvy(unsigned char *pssidbuf,int *preclen);
unsigned char Set_wifi_jbss(char* ssid,unsigned char *pstr);
unsigned char Set_wifi_wificonfig(char *prec);
unsigned char Set_wifi_dhcpsrv(char status);
unsigned char Set_wifi_showcounter(char *prec);
unsigned char Set_wifi_version(char *prec);


unsigned char SearchWLAN(void);
unsigned char WifiSSID_TO_MySSID(unsigned char *pwifissid);
void Clear_space(WLAN_SSID *pmyssid);

char ConnectSSID(char *Msg);
unsigned char ConnectWLAN(WLAN_SSID *pmyssid);
unsigned char Set_wifi_ConnectAddr(unsigned char *ipaddr);
unsigned char WIFI_Set_WLANPar(void);

unsigned char WiFi_Connect(unsigned char *ipaddr);
unsigned char WiFi_Send(unsigned char *buff,unsigned int len);
unsigned char WiFi_Receive (unsigned char *recBuff, unsigned int recBuffSize, const char *okstr, unsigned long timeout_MilliSeconds);

char GetWiFiName(void);
char GetWiFiInfo(void);
char DisplayNetwork(void);

#endif

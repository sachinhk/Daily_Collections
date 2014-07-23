#ifndef Setting_H
#define Setting_H

#include "my_stdint.h"


#define STORESIZE			27		//if XStor  added entries, the STORESIZE  must added too

#define GPRS    0x47
#define WiFi    0x57


struct storeFile{
	unsigned char *Viarable;		//Viarable
	const unsigned char *initVal;	//init value
};


extern struct storeFile XStor[STORESIZE+1];
extern const u8 efile_Config[];
extern const u8 Configfile_des3key[25];



extern unsigned char beepKG[2];
extern unsigned char LightPercent[4];				//light percent

extern unsigned char ServerIpPortBuf[28];			//ip 地址220.162.237.128,65167
extern unsigned char Upgrade_Flag[2];
extern unsigned char VersionURL[100+1];
extern unsigned char UpgradeURL[100+1];
extern unsigned char ApnApn[21+1];
extern unsigned char ApnUsername[21+1];
extern unsigned char ApnPassword[21+1];

extern unsigned char ScreenOffTimeout[3];			//屏保时间
extern unsigned char EnterAdvancedSetPassWord[22];	//高级设置密码
extern unsigned char StorePIN[5];					//PIN name
extern unsigned char TerminalID[17];				//终端id
extern unsigned char TerminalPassword[17];			//终端密码
extern unsigned char Model[2];	

extern unsigned char NetworkSwitch[2];

//WAN
extern unsigned char LocalIpPortBuf[28];			//本机IP及端口
extern unsigned char GatewayBuff[28];				//网关
extern unsigned char DNSBuff[28];					//DNS服务器
extern unsigned char MaskBuff[28];					//子网掩码

//WLAN
extern unsigned char WLANSSID[30];			//wifi ssid
extern unsigned char WLANWP[30];			//wifi wp

//wifi参数,每个参数占1byte,依次为	
//	0	|C/Smode		|0->server	1->client
//	1	|dhcpclient	|0->disable	1->enable
//	2	|workmode		|0->infra		1->AD-hoc
//	3	|enctype		|0->NoSecurity	1->WEP64	  2->WEP128	3->TKIP 	4->AES
//	4	|connectpe	|0->TCP		1->UDP
//	other  reserve
//extern unsigned char WLANPAR[10];
extern unsigned char CSmode[2];
extern unsigned char dhcpclient[2];
extern unsigned char workmode[2];
extern unsigned char enctype[2];



#define KMY_printer		'0'
#define JieNa_printer	'1'
#define DEFAULT_PIN     ""


char BasicSettings(char *msg);
char PinSet(char *msg);
char MainMenuScreen_Settings(char *msg);
u8 RestoreConfigFile(void);
u8 RestoreGlobalVariable(void);
char PIN_Store(char *msg);//AT+CPWD="SC","0909","0505"
u8 StorageVariable(void);

char WANSet(char *msg);
static char LoclaIPSet(char *msg);
static char GatewaySet(char *msg);
static char MaskSet(char *msg);
static char DNSSet(char *msg);

char WIFISet(char *msg);
static char SearchWLANSet(char *msg);
static char SSIDSet(char *msg);
static char WPManage(char *msg);
static char CSModeSet(char *msg);
static char DhcpSet(char *msg);
static char WifiworkmodeSet(char *msg);
static char EncTypeSet(char *msg);
static char WIFIParStore(char *msg);
static char SetWifiDefault(char *msg);

static char WIFIParmeterSet(char *msg);
static char WIFIConnectTCP(char *msg);
static char LoadParmeter(char *msg);
static char WifiSaveConfig(void);
void DisplayWiFiSignal(unsigned char Signal);

#endif


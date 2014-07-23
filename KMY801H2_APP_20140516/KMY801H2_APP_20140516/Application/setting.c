
#include "setting.h"

#include "kmy_Time.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "key.h"
#include "lcd.h"
#include "main.h"
#include "global_extern_variable.h"
#include "gprs.h"
#include "MenuOperate.h"
#include "SelfTest.h"
#include "des.h"
#include "mypure_library.h"
#include "ShuRuFa.h"
#include "kmy_EEPROMDrv.h"
#include "EepromFileSystem.h"
#include "kmy_Misc.h"
#include "kmy_wifi.h"
#include "kmy_WiFi_USARTDrv.h"
#include "kmy_USART1Drv.h"
#include "kmy_GprsDrv.h"
#include "message.h"


const u8 efile_Config[]={"Config"};
const u8 Configfile_des3key[25]={"106QDabcdefg106QD1234567"};

unsigned char beepKG[2];
unsigned char LightPercent[4];				//light percent

unsigned char ServerIpPortBuf[28];			//ip 地址220.162.237.128,65167
unsigned char NetworkSwitch[2];
unsigned char Upgrade_Flag[2];
unsigned char VersionURL[100+1];
unsigned char UpgradeURL[100+1];
unsigned char ApnApn[21+1];
unsigned char ApnUsername[21+1];
unsigned char ApnPassword[21+1];

unsigned char ScreenOffTimeout[3];			//屏保时间
unsigned char EnterAdvancedSetPassWord[22];	//高级设置密码
unsigned char StorePIN[5];					//PIN name
unsigned char TerminalID[17];				//终端id
unsigned char TerminalPassword[17];			//终端密码
unsigned char Model[2];			            //模式

//WAN
unsigned char LocalIpPortBuf[28];			//本机IP及端口
unsigned char GatewayBuff[28];				//网关
unsigned char DNSBuff[28];					//DNS服务器
unsigned char MaskBuff[28];					//子网掩码

//WLAN
unsigned char WLANSSID[30];			//wifi ssid
unsigned char WLANWP[30];			//wifi wp


//wifi参数,每个参数占1byte,依次为	
//	0	|C/Smode		|0->server	1->client
//	1	|dhcpclient	    |0->disable	1->enable
//	2	|workmode		|0->infra		1->AD-hoc
//	3	|enctype		|0->NoSecurity	1->WEP64	  2->WEP128	3->TKIP 	4->AES
//	4	|connectpe	    |0->TCP		1->UDP
//  5   |mancon         |0->disable 1->enable
//	other  reserve
//unsigned char WLANPAR[10];
unsigned char CSmode[2];
unsigned char dhcpclient[2];
unsigned char workmode[2];
unsigned char enctype[2];



const unsigned char locallanguage[sizeof(Language)]={OTHER_LANGUAGE,0};

struct storeFile XStor[STORESIZE+1]={	//需要保存的变量值
	{beepKG,"1"}, 
	{LightPercent,"44"},
	{Language,locallanguage},
//	{ServerIpPortBuf,"185.28.21.7,80"},	
     {ServerIpPortBuf,"192.168.12.11,5000"},	
	{NetworkSwitch, "G"},
	{Upgrade_Flag, "0"},
	{VersionURL, ""},
	{UpgradeURL, ""},
	{ApnApn,"cmnet"},
	{ApnUsername,""},

	{ApnPassword,""},
	{ScreenOffTimeout,"60"},
	{EnterAdvancedSetPassWord,"000000"},
	{StorePIN,DEFAULT_PIN},
	{TerminalID,"12345678"},
	{TerminalPassword,"123456"},
	{Model,"1"},
	{LocalIpPortBuf,"192.168.12.97,5000"},
	{GatewayBuff,"192.168.12.1"},
	{MaskBuff,"255.255.255.0"},		

	{DNSBuff,"192.168.12.1"},
	{WLANSSID,"Auro 3"},
	{WLANWP,"WPA2"},
	{CSmode,"1"},
    {dhcpclient,"0"},
    {workmode,"0"},
    {enctype,"4"},
	{NULL,NULL} 	//后面不用的务必赋成&SFNULL，否则会引起segmentationfault
};


u8 StorageVariable(void)
{
	static u8 buff[1500];
	u8 i;
	u8 retval;
	struct EepromFile efp;

	buff[0]=0;
	for(i=0;i<STORESIZE;i++){
		strcat((s8*)buff,(sc8*)(XStor[i].Viarable));
		strcat((s8*)buff,"\t");
	}

	retval=EepromFile_Open(efile_Config,&efp);
	if(retval!=EepromFile_OpenOk){
		{printf("[StorageVariable error]=EepromFile_Open\r\n");}
		return 0;
	}

	retval=EepromFile_Write(buff,strlen((sc8*)buff)+1,&efp);
	if(retval!=EepromFile_WriteOk){
		{printf("[StorageVariable error]=EepromFile_Write\r\n");}
		return 0;
	}

	return 1;
}


unsigned char RestoreConfigFile (void)
{
	unsigned char buff[1500]={0};
	unsigned char i;
	unsigned char retval;

	buff[0] = 0;

	for (i = 0; i < STORESIZE; i++)
	{
	    
		strcpy ( (s8*) (XStor[i].Viarable), (sc8*) (XStor[i].initVal) );
		printf("%d=>%s\r\n",i,(XStor[i].initVal));
		strcat ( (s8*) buff, (sc8*) (XStor[i].initVal) );
		strcat ( (s8*) buff, "\t");
	}

	retval = EepromFile_Delete ((unsigned char*)efile_Config);

	if (retval == EepromFile_HaveNotThisFile)
	{
		{
			printf ("EepromFile_HaveNot Config file\r\n");
		}
	}

	retval = EepromFile_WriteData (buff, strlen ( (sc8*) buff), efile_Config);

	if (retval != EepromFile_WriteOk)
	{
		{
			printf ("[RestoreConfigFile error]=EepromFile_Write\r\n");
		}
		return 0;
	}

	return 1;
}


unsigned char RestoreGlobalVariable (void)
{
	unsigned char buff[1500];
	unsigned char i;
	unsigned char retval;
	unsigned char *p;
	char *pt;
	struct EepromFile efp;

	retval = EepromFile_Open ((unsigned char*)efile_Config, &efp);

	if (retval != EepromFile_OpenOk)
	{
		{
			printf ("[RestoreGlobalVariable error]=EepromFile_Open\r\n");
		}
		return 0;
	}

	retval = EepromFile_read (buff, sizeof (buff) - 1, &efp);

	if (retval != EepromFile_ReadOk)
	{
		{
			printf ("[RestoreGlobalVariable error]=EepromFile_read\r\n");
		}
		return 0;
	}
	

	pt = (char*)buff;
	for (i = 0; i < STORESIZE; i++)
	{
		pt=strstr(pt,"\t");
		if(pt==NULL)
		{
			printf("Add new global varible, format Eeprom and restart\r\n");
			return 2;
		}
		pt++;
	}
	pt=strstr(pt,"\t");
	if(pt!=NULL)
	{
		printf("Global varible have been changed, format Eeprom and restart\r\n");
		return 2;
	}
	
	p = buff;
		
	for (i = 0; i < STORESIZE; i++)
	{
		my_strcpy_EncounterHexEnd ( (XStor[i].Viarable), p, 300, '\t');
		my_MovePointToHex (&p, &buff[sizeof (buff) - 1], '\t');
		p++;
		printf ("%d=%s\r\n", i, XStor[i].Viarable);
	}

	return 1;
}


static char ContrastSet(char *msg)	//2.screen constract setting
{
	#if 0
	return(abeyant());
	#else
	signed int NewPercent	=atoi((sc8*)LightPercent);
	signed int 	CurCursor	= 39+((NewPercent-56)/4*3);	
	signed int  OldCursor   = CurCursor;
	unsigned char Buf[50]={'\0'};
	unsigned char keyval;

	clear_lcd();
	text_out_to_row_x_Language2(0,28,"【亮度设定】","Contrast Set");

	text_out(17,25,"-");
	text_out(105,25,"+");
	draw_rect(26,25,77,10, mode_OnlyEdge);
	draw_line(26,30,26+75+1,30,1);

	fill_rect(26+1+CurCursor,26,3,8,mode_Reverse);	//3*8

	memset(Buf,'\0',sizeof(Buf));
	sprintf((s8*)Buf,"%d",NewPercent);
	strcat((s8*)Buf,"%");
	text_out(58,48,Buf);

	while(1)
	{
		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue ;
		KeyEventPost();
		switch(keyval){
			case KEY_UP:
			if(NewPercent+4 <= 100)
			{
				NewPercent += 4;			
				CurCursor += 3;			
			}		
			break;

			case KEY_DOWN:
			if(NewPercent-4 >= 4)
			{
				NewPercent -= 4;
				CurCursor -= 3;			
			}		
			break;

			case KEY_Enter:
			sprintf((s8*)Buf,"%d",NewPercent);
			Buf[3]=0;
			strcpy((s8*)LightPercent,(sc8*)Buf);
			if(!StorageVariable()){
				DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
				WaitkeyAndAutoExit(10);
			}
			return KEY_Enter;

			case KEY_ReturnPrevious:
			NewPercent=atoi((sc8*)LightPercent);
			set_contrast(NewPercent);
			return KEY_ReturnPrevious;

			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
			NewPercent=atoi((sc8*)LightPercent);
			set_contrast(NewPercent);
			return KEY_ReturnInitInterface;

			default:
			break;	
		}
		memset(Buf,'\0',sizeof(Buf));
		text_out(58,48,"    ");
		sprintf((s8*)Buf,"%d",NewPercent);
		strcat((s8*)Buf,"%");
		text_out(58,48,Buf);

		if(OldCursor != CurCursor)
		{
			fill_rect(26+1+OldCursor,26,3,8,mode_Reverse);	//3*8
			fill_rect(26+1+CurCursor,26,3,8,mode_Reverse);	//3*8
			OldCursor = CurCursor;
		}

		#ifdef DEBUG_PRINTF
		printf("NewPercent=%d\n",NewPercent);
		#endif

		set_contrast(NewPercent);		// 24---52
	}
	#endif
}


static char Polish_Display_Keytone_Close(char *msg)
{
	memset(beepKG,0,sizeof(beepKG));
	beepKG[0]='0';
	if(!StorageVariable())
	{
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}

	return 0;
}

static char Polish_Display_Keytone_Open(char *msg)
{
	memset(beepKG,0,sizeof(beepKG));
	beepKG[0]='1';
	if(!StorageVariable())
	{
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}

	return 0;
}

static char Polish_Display_Model_Close(char *msg)
{
	memset(Model,0,sizeof(Model));
	Model[0]='0';
	if(!StorageVariable())
	{
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}

	return 0;
}

static char Polish_Display_Model_Open(char *msg)
{
	memset(Model,0,sizeof(Model));
	Model[0]='1';
	if(!StorageVariable())
	{
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}

	return 0;
}


static char BasicKeyTone(char *msg)	//key tone setting
{
	static unsigned char ToneClose1[20]={"1.关闭   "};
	static unsigned char ToneClose2[20]={"1.Close  "};
	static unsigned char ToneOpen1[20]={"2.打开   "};
	static unsigned char ToneOpen2[20]={"2.Open   "};

	static struct MenuFuc_Language2 Menu[2]={
		{ToneClose1,ToneClose2,Polish_Display_Keytone_Close},
		{ToneOpen1,ToneOpen2,Polish_Display_Keytone_Open}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		2,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	my_strdel(ToneClose1,'*');
	my_strdel(ToneClose2,'*');
	my_strdel(ToneOpen1,'*');
	my_strdel(ToneOpen2,'*');

	if(beepKG[0]=='0'){
		strcat((s8*)ToneClose1,"*");
		strcat((s8*)ToneClose2,"*");
	}else{
		strcat((s8*)ToneOpen1,"*");
		strcat((s8*)ToneOpen2,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(ToneClose1,'*');
				my_strdel(ToneClose2,'*');
				my_strdel(ToneOpen1,'*');
				my_strdel(ToneOpen2,'*');
				if(beepKG[0]=='0'){
					strcat((s8*)ToneClose1,"*");
					strcat((s8*)ToneClose2,"*");
				}else{
					strcat((s8*)ToneOpen1,"*");
					strcat((s8*)ToneOpen2,"*");
				}
			}
		}else{
			return MenuOp.RetVal;
		}
	}
}

static char Model_Set(char *msg)	//Model setting
{
	static unsigned char Model_11[20]={"1.关闭低功耗模式 "};
	static unsigned char Model_12[20]={"1.Close Low Power "};
	static unsigned char Model_21[20]={"2.打开低功耗模式 "};
	static unsigned char Model_22[20]={"2.Open Low Power  "};

	static struct MenuFuc_Language2 Menu[2]={
		{Model_11,Model_12,Polish_Display_Model_Close},
		{Model_21,Model_22,Polish_Display_Model_Open}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		2,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	my_strdel(Model_11,'*');
	my_strdel(Model_12,'*');
	my_strdel(Model_21,'*');
	my_strdel(Model_22,'*');

	if(Model[0]=='0')
	{
		strcat((s8*)Model_11,"*");
		strcat((s8*)Model_12,"*");
	}
	else
	{
		strcat((s8*)Model_21,"*");
		strcat((s8*)Model_22,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(Model_11,'*');
				my_strdel(Model_12,'*');
				my_strdel(Model_21,'*');
				my_strdel(Model_22,'*');
				if(Model[0]=='0'){
					strcat((s8*)Model_11,"*");
					strcat((s8*)Model_12,"*");
				}else{
					strcat((s8*)Model_21,"*");
					strcat((s8*)Model_22,"*");
				}
			}
		}
		else
		{
			return MenuOp.RetVal;
		}
	}
}


static char PowerOffScreenTime (char *msg)	//key tone setting
{
	unsigned char temp[20];
	unsigned char retval;
	PasswordInputArgument arg;

	clear_lcd();

	if (Language[0] == LOCAL_LANGUAGE)
	{
		strcpy ( (s8*) temp, "当前屏保时间:");
	}
	else
	{
		strcpy ( (s8*) temp, "Current time");
	}

	strcat ( (s8*) temp, (sc8*) ScreenOffTimeout);
	DrawTitle_to_row_Language2 (1, temp, temp);

	PassWordArgDeInit (&arg, temp, 2);
	arg.DiplayRow = 2;
	arg.LocalTitle = NULL;
	arg.xingflag = 0;
	arg.clearlcdflag = 0;
	arg.LocalTitle = "【屏保时间设置】";
	arg.OtherTitle = "Screen timeout set";
	retval = PassWordInput_MiddledDisplay (&arg);

	if (retval == KEY_Enter)
	{
		strcpy ( (s8*) ScreenOffTimeout, (sc8*) temp);
		retval = atoi ( (sc8*) temp);

		if (retval == 0) strcpy ( (s8*) ScreenOffTimeout, "30");

		if (!StorageVariable() )
		{
			DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		}
		else
		{
			DrawTitle_to_row_Language2 (3, "更改成功!", "change success!");
		}

		retval = WaitkeyAndAutoExit (10);
	}

	return retval;
}


static char LanguageSet_English(char *msg)
{
	memset(Language,0,sizeof(Language));
	Language[0]=OTHER_LANGUAGE;
	if(!StorageVariable()){
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}
	return 0;
}
static char LanguageSet_Chinese(char *msg)
{
	memset(Language,0,sizeof(Language));
	Language[0]=LOCAL_LANGUAGE;
	if(!StorageVariable()){
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}

	return 0;
}
static char LanguageSet(char *msg)	//key tone setting
{
	static unsigned char ToneClose1[20]={"1.中文   "};
	static unsigned char ToneClose2[20]={"1.Chinese  "};
	static unsigned char ToneOpen1[20]= {"2.英文   "};
	static unsigned char ToneOpen2[20]= {"2.English  "};

	static struct MenuFuc_Language2 Menu[2]={
		{ToneClose1,ToneClose2,LanguageSet_Chinese},
		{ToneOpen1,ToneOpen2,LanguageSet_English}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,2,0,0,0,MaxRowOneScr,1,0,0
	};

	my_strdel(ToneClose1,'*');
	my_strdel(ToneClose2,'*');
	my_strdel(ToneOpen1,'*');
	my_strdel(ToneOpen2,'*');

	if(Language[0]==LOCAL_LANGUAGE){
		strcat((s8*)ToneClose1,"*");
		strcat((s8*)ToneClose2,"*");
	}else{
		strcat((s8*)ToneOpen1,"*");
		strcat((s8*)ToneOpen2,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(ToneClose1,'*');
				my_strdel(ToneClose2,'*');
				my_strdel(ToneOpen1,'*');
				my_strdel(ToneOpen2,'*');
				if(Language[0]==LOCAL_LANGUAGE){
					strcat((s8*)ToneClose1,"*");
					strcat((s8*)ToneClose2,"*");
				}else{
					strcat((s8*)ToneOpen1,"*");
					strcat((s8*)ToneOpen2,"*");
				}
			}
		}else{
			return MenuOp.RetVal;
		}
	}
}



static char DisplayVersion(char *msg)
{
	unsigned char buff[30];
	unsigned char buffother[30];

	strcpy((s8*)buff,"软件版本:");
	strcat((s8*)buff,(sc8*)softwareversion);
	strcpy((s8*)buffother,"softVer:");
	strcat((s8*)buffother,(sc8*)softwareversion);

	clear_lcd();
	DrawTitle_to_row_Language2(0,"终端机信息","Machine Ver");
	text_out_to_row_Language2(1,buff,buffother);
	text_out_to_row_Language2(2,"硬件版本:20140321","HardVer:20140321");
	text_out_to_row_Language2(3,"备注:KMY801H2","Mark:KMY801H2");
	return(WaitkeyAndAutoExit(10));
}



static char DisplayCID(char *msg)
{
	unsigned char CmdBuff[50];
	unsigned char temp[50];
	unsigned char retval;

	if(NetworkSwitch[0]==WiFi)
	{
	    clear_lcd();
		DrawTitle_to_row_Language2 (1, "当前网络为", "Current network is");
		DrawTitle_to_row_Language2 (2, "WiFi请切换", "WiFi,Please change to");
		DrawTitle_to_row_Language2 (3, "为GPRS后操作", "GPRS before operation");
		WaitkeyAndAutoExit (10);
	    return 0;
	}
	
	clear_lcd();
	DrawTitle_to_row_Language2(0,"SIM卡CID码","SIM cid");

	retval = GetCCID(temp);
	if(retval == 0)
	{
		DrawTitle_to_row_Language2 (2, "读取失败", "Failed to read!");		
		WaitkeyAndAutoExit (10);
		return 0;
	}

	if(strlen((sc8*)temp)>16){
		memset(CmdBuff,0,30);
		memcpy(CmdBuff,temp,16);
		DrawTitle_to_row(1,CmdBuff);
		DrawTitle_to_row(2,temp+16);
	}else{
		DrawTitle_to_row(1,temp);
	}

	WaitkeyAndAutoExit(10);
	return 0;
}

static char Display_IMEI(char *msg)
{
	unsigned char buff[50];

	if(NetworkSwitch[0]==WiFi)
	{
	    clear_lcd();
		DrawTitle_to_row_Language2 (1, "当前网络为", "Current network is");
		DrawTitle_to_row_Language2 (2, "WiFi请切换", "WiFi,Please change to");
		DrawTitle_to_row_Language2 (3, "为GPRS后操作", "GPRS before operation");
		WaitkeyAndAutoExit (10);
	    return 0;
	}
	clear_lcd();
	DrawTitle_to_row(0,"IMEI");

	GetIMEI(buff);

	DrawTitle_to_row(2,buff);
	return(WaitkeyAndAutoExit(10));
}

static char Display_ChipId(char *msg)
{
	unsigned char buff[50];

	clear_lcd();
	DrawTitle_to_row_Language2(0,"芯片id","Chip ID");

	kmy_MiscGetChipUniqueId(buff);

	DrawTitle_to_row(3,&buff[12]);
	buff[12]=0;
	DrawTitle_to_row(2,&buff[0]);
	return(WaitkeyAndAutoExit(30));
}


static char Display_RandomNumber(char *msg)
{
	unsigned char buff[21];

	clear_lcd();
	DrawTitle_to_row_Language2(0,"随机数","Random num");

	kmy_MiscGetRandomNumber(buff,sizeof(buff)-1);

	DrawTitle_to_row(2,buff);
	return(WaitkeyAndAutoExit(30));
}

static char Display_KMY_LibVersion(char *msg)
{
	unsigned char buff[40]={0};
	unsigned char desbuff[40]={0};
	char *p=NULL;
	
	p = (char *)buff;
	
	clear_lcd();
	DrawTitle_to_row_Language2(0,"KMY库版本","KMY LibVersion");

	kmy_MiscGetLibraryVersion(buff);

	p=strstr(p,"Library");
	if(p==NULL)return(WaitkeyAndAutoExit(30));

	strcat((char*)desbuff,p);
	
	DrawTitle_to_row(2,desbuff);
	return(WaitkeyAndAutoExit(30));
}

static char BasicApnSetApn(char *msg)
{
	unsigned char buff[sizeof(ApnApn)];
	unsigned char retval;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)ApnApn);
	DrawTitle_to_row(0,"Apn is:");
	text_out_to_row(1,buff);
	text_out_to_row(3,"Change: Enter");
	text_out_to_row(4,"Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("输入Apn","Input Apn",SRF_abc|SRF_ABC|SRF_123,SRF_abc,buff,sizeof(buff),1);

	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	strcpy((s8*)ApnApn,(sc8*)buff);
	if(!StorageVariable()){
		DrawTitle_to_row(3,"Change falure!");
	}else{
		DrawTitle_to_row(2,"setting,pleas wait...");
		retval=set_apn();
		if(retval!=TCP_OK){
			DrawTitle_to_row(3,"Set APN error");
		}else{
			DrawTitle_to_row(3,"Set APN success");
		}
	}

	return (WaitkeyAndAutoExit(10));
}
static char BasicApnSetUsername(char *msg)
{
	unsigned char buff[sizeof(ApnUsername)];
	unsigned char retval;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)ApnUsername);
	DrawTitle_to_row(0,"ApnUsername is:");
	text_out_to_row(1,buff);
	text_out_to_row(3,"Change: Enter");
	text_out_to_row(4,"Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("输入Apn用户名","Input ApnUsername",SRF_abc|SRF_ABC|SRF_123,SRF_abc,buff,sizeof(buff),1);

	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	strcpy((s8*)ApnUsername,(sc8*)buff);
	if(!StorageVariable()){
		DrawTitle_to_row(3,"Change falure!");
	}else{
		DrawTitle_to_row(2,"setting,pleas wait...");
		retval=set_apn();
		if(retval!=TCP_OK){
			DrawTitle_to_row(3,"Set APN error");
		}else{
			DrawTitle_to_row(3,"Set APN success");
		}
	}

	return (WaitkeyAndAutoExit(10));
}
static char BasicApnSetPassword(char *msg)
{
	unsigned char buff[sizeof(ApnPassword)];
	unsigned char retval;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)ApnPassword);
	DrawTitle_to_row(0,"ApnPassword is:");
	text_out_to_row(1,buff);
	text_out_to_row(3,"Change: Enter");
	text_out_to_row(4,"Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("I输入Apn密码","Input ApnPassword",SRF_abc|SRF_ABC|SRF_123,SRF_abc,buff,sizeof(buff),1);

	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	strcpy((s8*)ApnPassword,(sc8*)buff);
	if(!StorageVariable()){
		DrawTitle_to_row(3,"Change falure!");
	}else{
		DrawTitle_to_row(2,"setting,pleas wait...");
		retval=set_apn();
		if(retval!=TCP_OK){
			DrawTitle_to_row(3,"Set APN error");
		}else{
			DrawTitle_to_row(3,"Set APN success");
		}
	}

	return (WaitkeyAndAutoExit(10));
}

static char BasicApnSet(char *msg)
{
	static struct MenuFuc_Language2 Menu[3]={
		{"1.APN","1.APN",BasicApnSetApn},
		{"2.APN用户名","2.User",BasicApnSetUsername},
		{"3.APN密码","3.Password",BasicApnSetPassword},
	};

	struct MenuOperate_Language2 MenuOp={
		Menu,
		3,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_ReturnInitInterface){return KEY_ReturnInitInterface;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return MenuOp.RetVal;
		}
	}
}





char ReadSomeMessage(char *msg)
{
	static struct MenuFuc_Language2 Menu[6]={
		{"1.版本信息","1.Version",DisplayVersion},
		{"2.SIM卡CID码","2.SIM card CID",DisplayCID},
		{"3.IMEI号","3.IMEI number",Display_IMEI},
		{"4.读芯片唯一ID","4.Chip unique ID",Display_ChipId},
		{"5.读随机数","5.Random number",Display_RandomNumber},
		{"6.读KMY库版本","6.KMY_LibVersion",Display_KMY_LibVersion},
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		6,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			CurProc=CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}

unsigned char TranslateKeyToDecimal(unsigned char KeyValue)
{
    switch(KeyValue)
    {
        case KEY0:
            return 0;
         
        case KEY1:
            return 1;
         
        case KEY2:
            return 2;
         
        case KEY3:
            return 3;
         
        case KEY4:
            return 4;
         
        case KEY5:
            return 5;
         
        case KEY6:
            return 6;
         
        case KEY7:
            return 7;
         
        case KEY8:
            return 8;
         
        case KEY9:
            return 9;
         
        default:
            break;
    }
    return 0;
}

unsigned char SetBitOfDecimal(int OldDecimal, unsigned char ValueOfBit, unsigned char bit)
{
    unsigned char NewDecimal=0;
    if(bit == 0)
    {
        NewDecimal = OldDecimal%10;
        NewDecimal += ValueOfBit*10;
    }
    else if(bit == 1)
    {
        NewDecimal = (OldDecimal/10)*10;
        NewDecimal += ValueOfBit;
    }
    return NewDecimal;
}

unsigned char GetBitOfDecimal(unsigned char Decimal, unsigned char bit)
{
    if(bit == 0)
    {
        return ((Decimal/10)%10);
    }
    else if(bit == 1)
    {
        return ((Decimal/1)%10);
    }
    return 0;
}
 
 
 
static char TimeDateSet (char *msg) //1.time data setting
{
    unsigned char FlashFlag = 0x01 | 0x02 | 0x04;
    unsigned int year = 0;
    static unsigned char year_H=0;
    static unsigned char year_L=0;
    static unsigned char month = 0;
    static unsigned char day = 0;
    static unsigned char hour = 0;
    static unsigned char minute = 0;
    static unsigned char second = 0;
    unsigned char sBuff[100]={0};
     
    unsigned char TempKey = KEY_NONE;
    unsigned char Index=0;
    unsigned char MoveFlag=0;
//    unsigned char ret=0;
    #define startx 7
    #define Row 2
    #define MaxArray 14
    char YearofMonth[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
     
    unsigned char CursorArray[MaxArray][4] = {
                                              //Min, x, y, Max
                                              {0, startx+6*0, (Row+1)*13-2, 9},
                                              {0, startx+6*1, (Row+1)*13-2, 9}, 
                                              {0, startx+6*2, (Row+1)*13-2, 9},
                                              {0, startx+6*3, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*5, (Row+1)*13-2, 9},
                                              {0, startx+6*6, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*8, (Row+1)*13-2, 9},
                                              {0, startx+6*9, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*11, (Row+1)*13-2, 9},
                                              {0, startx+6*12, (Row+1)*13-2, 9}, //4
                                               
                                              {0, startx+6*14, (Row+1)*13-2, 9},
                                              {0, startx+6*15, (Row+1)*13-2, 9},
                                               
                                              {0, startx+6*17, (Row+1)*13-2, 9},
                                              {0, startx+6*18, (Row+1)*13-2, 9},
                                             };
    typedef struct
    {
        unsigned char *DestValue;
        unsigned char Subtract;
    }DT;
     
    DT dt[] = {
               {&year_H, 0},
               {&year_H, 0},
               {&year_L, 2},
               {&year_L, 2},
                
               {&month, 4},
               {&month, 4},
                
               {&day, 6},
               {&day, 6},
                
               {&hour, 8},
               {&hour, 8},
                
               {&minute, 10},
               {&minute, 10},
                
               {&second, 12},
               {&second, 12},
              };
     
    kmy_TimeGetTime (&year, &month, &day, &hour, &minute, &second, NULL);
    year_H=year/100;
    year_L=year%100;
    clear_lcd();
    DrawTitle_to_row_Language2 (0, "Time Date", "Time Date");
    kmy_PostKey(KEY_pseudo);
    while (1)
    {
        TempKey = kmy_GetKeyValue();
         
        if (TempKey == KEY_NONE) continue;
         
        switch (TempKey)
        {
            case KEY_pseudo:
                if((FlashFlag & 0x01) == 0x01)
                {
                    kmy_LcdClear();
                    DrawTitle_to_row_Language2 (0, "Time Date", "Time Date");
                      
                    FlashFlag &= ~0x01;
                }
                if((FlashFlag & 0x02) == 0x02)
                {
                    memset(sBuff, 0, sizeof(sBuff));
                    sprintf((char *)sBuff, "%02d%02d-%02d-%02d %02d:%02d:%02d", year_H,year_L, month, day, hour, minute, second);
                    text_out_to_row_x(Row, startx, sBuff);
                     
                    FlashFlag &= ~0x02;
                }
                if((FlashFlag & 0x04) == 0x04)
                {
                    #define CURSOR_HEIGHT 5
                    clear_area(0, CursorArray[Index][2], 132, CursorArray[Index][2]+CURSOR_HEIGHT);
                     
                    draw_rect(CursorArray[Index][1], CursorArray[Index][2], 5, CURSOR_HEIGHT, mode_Black);
                     
                    FlashFlag &= ~0x04;
                }
                if(MoveFlag)
                {
                    kmy_PostKey (KEY_DOWN);//move Index to next
                    MoveFlag = 0;
                }
                break;
            case KEY0:
            case KEY1:
            case KEY2:
            case KEY3:
            case KEY4:
            case KEY5:
            case KEY6:
            case KEY7:
            case KEY8:
            case KEY9:
                if((TranslateKeyToDecimal(TempKey) >= CursorArray[Index][0]) && (TranslateKeyToDecimal(TempKey) <= CursorArray[Index][3]))
                {
                    *dt[Index].DestValue = SetBitOfDecimal(*dt[Index].DestValue, TranslateKeyToDecimal(TempKey),Index-dt[Index].Subtract);
                    FlashFlag |= 0x02;
                    kmy_PostKey(KEY_pseudo);
                }
                MoveFlag = 1;
                break;
            case KEY_UP:
                BeepNormal();
                if ( (Index - 1) >= 0)
                {
                    Index -= 1;
                }
                else
                {
                    Index = MaxArray - 1;
                }
                FlashFlag |= 0x04;
                kmy_PostKey(KEY_pseudo);
                break;
             
            case KEY_DOWN:
                BeepNormal();
                if ( (Index + 1) < MaxArray)
                {
                    Index += 1;
                }
                else
                {
                    Index = 0;
                }
                FlashFlag |= 0x04;
                kmy_PostKey(KEY_pseudo);
                break;
            case KEY_Enter:
                BeepNormal();
                year=year_H*100+year_L;
                 
                if(day>YearofMonth[month])
                {
                    if(month==2)
                    {
                        if(year%4==0&&year%100!=0||year%400==0)
                        {
                            if(day>29)month=0;
                        }
                        else
                        {
                            month=0;
                        }
                    }
                    else
                    {
                        month=0;
                    }
                }
                if(month==0||month>12||hour>24||minute>60||second>60)
                {
                    kmy_LcdClear();
                    DrawTitle_to_row(1,"Time date format");
                    DrawTitle_to_row(2," error");
                    WaitkeyAndAutoExit(2);
                }
                else
                {
                    kmy_TimeSetTime (year, month, day, hour, minute, second);
                    msleep(500);
                }
             
                return KEY_Enter;
            case KEY_ReturnInitInterface:
            case KEY_ReturnPrevious:
                BeepNormal();
                return TempKey;
            default:break;
        }
    }
}

void ChangeNetwork(void)
{
	clear_lcd();
	DrawTitle_to_row_Language2(2,"更改网络","Change Network");
	DrawTitle_to_row_Language2(3,"请等候...","Please wait...");
	if(NetworkSwitch[0]==WiFi)
	{
	    send_at_cmd("AT+QPOWD=1\r");// power down GPRS module
		kmy_GprsOnOff(0);
		kmy_WifiInit();
		kmy_WiFiOnOff (1);
		sleep(10);
		printf("WiFi Mode\r\n");
	}
	else
	{


		kmy_WiFiOnOff (0);
		kmy_GprsInit();
		msleep(1000);
		Sim900B_init(); 
		printf("GPRS Mode\r\n");
	}	
}

typedef struct SettingItem {
	const unsigned char * itemName;
	unsigned short itemLen;
	unsigned char * content;
} SettingItem;

typedef struct StoreItem {
	unsigned char itemName[64];
	unsigned short itemLen;
	unsigned char * content;
} StoreItem;

SettingItem settingItems[] = {
	{"Beep Enable", 1, beepKG},
	{"Web Server IP,Port", 50, ServerIpPortBuf},
	{"Network Selection(G>GPRS;W>WiFi)", 1, NetworkSwitch},
	{"Low Power ON/OFF(0:OFF,1:ON)",1,Model},
	{"Version URL", 150, VersionURL},
	{"Upgrade URL", 150, UpgradeURL},
	{"GPRS APN",50, ApnApn},
	{"GPRS APN UserName",20, ApnUsername},
	{"GPRS APN Password", 20, ApnPassword},
	{"Local IP,Port", 25, LocalIpPortBuf},
	{"Local Gateway", 25, GatewayBuff},
	{"Local Mask", 25, MaskBuff},
	{"Local DNS", 25, DNSBuff},
	{"WiFi SSID", 30, WLANSSID},
	{"WPA Passphrase(8-30 Characters)", 30, WLANWP},
	{"C/S Mode(0:SERVER,1:CLIENT)", 1, CSmode},	
	{"DHCP Client(0:Disable,1:Enable)", 1, dhcpclient},
	{"Network Mode(0:Infra mode,1:Ad-hoc mode)", 1, workmode},
	{"Encryption Type(0>No Security;1>WEP64;2>WEP128;3>TKIP;4>AES)", 1, enctype},
};
unsigned char itemCnt = sizeof(settingItems) / sizeof(settingItems[0]);

unsigned char searchPosInItems(char *itemName)
{
	unsigned char i;
	
	for(i = 0; i < itemCnt; i++)
	{
		if(strcmp((char *)itemName, (char *)settingItems[i].itemName) == 0)
			break;
	}
	return i;		
}

unsigned short GetSum(unsigned char buff[], int len)
{
    unsigned short sum = 0, i = 0;
	
    for (i = 0; i < len; i++)
    {
        sum += buff[i];
    }
	
	return sum;
}

char * NT_itoa(int number, char *buffer, unsigned char base)
{
	if(buffer == NULL)	return NULL;
	
	if(base == 16)		sprintf(buffer, "%x", number);
	else if(base == 8)	sprintf(buffer, "%o", number);
	else 				sprintf(buffer, "%d", number);
	return buffer;
}

#define COLITEM_TAG	"#"
#define SUBITEM_TAG	"*"

char NT_StoreParam(char *cmdString)
{
	StoreItem tempItem;
	char *pfront, *pback, *ptemp;
	char buffer[128] = {0};
	unsigned char pos = 0;

	pfront = pback = cmdString;
	while( *pfront )
	{
		// Get setting name
		pfront = strstr(pback, SUBITEM_TAG);
		if(pfront == NULL)	return 2;
		
		ptemp = pfront++;
		*ptemp = '\0';
		strcpy((char *)tempItem.itemName, (char *)pback);
		pback = pfront;
		
		// Get setting len
		pfront = strstr(pback, SUBITEM_TAG);
		if(pfront == NULL)	return 3;	
		
		ptemp  = pfront++;
		*ptemp = '\0';
		strcpy(buffer, pback);
		tempItem.itemLen = atoi(buffer);
		pback =  pfront;
		memset(buffer, 0, sizeof(buffer)/sizeof(buffer[0]));
		
		// Get setting content
		pfront = strstr(pback, COLITEM_TAG);
		if(pfront == NULL)
			return 4;
		
		ptemp = pfront++;
		*ptemp = '\0';
		tempItem.content = (unsigned char *)malloc(tempItem.itemLen);
		if(tempItem.content == NULL)
		{
			kmy_USART1SendString("Malloc Failed\n");
			return 5;
		}
		strncpy((char *)tempItem.content, (char *)pback, tempItem.itemLen);
		pback = pfront;
		
		pos = searchPosInItems((char *)tempItem.itemName);
		if(pos  < itemCnt)
			strncpy((char *)settingItems[pos].content,(char *)tempItem.content,tempItem.itemLen);

		memset(buffer, 0, sizeof(buffer));

		strcpy(buffer, (char *)tempItem.itemName);
		strcat(buffer, " ");
		strcat(buffer, (char *)tempItem.content);
		strcat(buffer, "\n");
		free(tempItem.content);
	}

	return  !StorageVariable();
}

char * NT_MakeParameter(char sendBuff[])
{
	char * iterator = sendBuff + 2;		// 2 bytes data for  length
	unsigned char pos = 0;
	unsigned char convertBuf[32] = {0}; 
	unsigned short check_sum = 0;
	unsigned short length;
	
	// copy the current config to 
	for(pos = 0; pos < itemCnt; pos++)
	{	
		strcat(iterator, (char *)settingItems[pos].itemName);
		strcat(iterator, SUBITEM_TAG);
		strcat(iterator, (char *)NT_itoa(settingItems[pos].itemLen, (char *)convertBuf, 10));
		strcat(iterator, SUBITEM_TAG);
		strncat(iterator, (char *)settingItems[pos].content,settingItems[pos].itemLen);
		strcat(iterator, COLITEM_TAG);
	}
	
	// the checksum didn't contain length
	length = strlen(iterator);
	check_sum = GetSum((unsigned char*)iterator, length);
	iterator[length] = check_sum >> 8;
	iterator[length+1] = check_sum & 0x00ff;
	iterator[length+2] = 0;
	
	// insert the length into the begining of sendBuff
	sendBuff[0] = length >> 8;
	sendBuff[1] = length & 0x00ff;
	return sendBuff;
}



#define STATE_READ_CMD	0
#define STATE_ACK_CMD	1
#define STATE_READ_LEN	2
#define STATE_READ_DATA	3
#define STATE_UPLOAD	4
#define STATE_UPLOAD_ACK 5
#define STATE_CHECKSUM	6

#define CMD_DOWNLOAD	"DW"
#define CMD_UPLOAD		"UP"
#define ACK_OKSTR		"OK"


char UpdateParameter(char *msg)
{
	unsigned char RecvBuff[1500] = {0}, retval = 0;
	int RecvLen = 0;
	char State = STATE_READ_CMD;
	int ReadLen = 0;
	int ReadLen2 = 0;
	int ReadPos = 0;
	unsigned short CheckSum = 0, localSum = 0;
	unsigned char OldNetwork;

	unsigned char converter[32] = {0};
	
	OldNetwork = NetworkSwitch[0];

	clear_lcd();
	DrawTitle_to_row_Language2(0,"Updating...","Updating...");
	kmy_USART1ReceiveReset();
	
	while(1)
	{
		retval=GetKeyValue();
		if(retval != KEY_NONE)break;

		switch(State)
		{
			case STATE_READ_CMD://read AT
				RecvLen = 0;
				memset(RecvBuff, 0, sizeof(RecvBuff));
				
				RecvLen = kmy_USART1ReceiveData(RecvBuff, 2, 100);
				if(RecvLen == 2)
				{		
					if(strstr((char*)RecvBuff, CMD_UPLOAD))
						State = STATE_UPLOAD;
					
					else if( strstr( (char *)RecvBuff, CMD_DOWNLOAD))// upload parameters list.
						State = STATE_ACK_CMD;
				}
				break;

			case STATE_ACK_CMD://send OK
				kmy_USART1ReceiveReset();
				kmy_USART1SendString(ACK_OKSTR);
				State = STATE_READ_LEN;
				break;

			case STATE_READ_LEN://read 2 bytes of len
				RecvLen = 0;
				memset(RecvBuff, 0, sizeof(RecvBuff));
				
				RecvLen = kmy_USART1ReceiveData(RecvBuff, 2, 100);
				if(RecvLen == 2)
				{
					ReadLen = (RecvBuff[0]<<8) | RecvBuff[1];
					ReadLen2 = ReadLen;
					
					ReadPos = 0;
					ReadPos += 2;
					ReadLen += 2;
					State = STATE_READ_DATA;
				}
				break;

			case STATE_READ_DATA://read "ReadLen" bytes datas
				RecvLen = 0;
				RecvLen = kmy_USART1ReceiveData(RecvBuff + ReadPos, ReadLen, 100);
			
				ReadPos += RecvLen;
				ReadLen -= RecvLen;

				if(ReadLen == 0)	State = STATE_CHECKSUM;
				break;

			case STATE_UPLOAD:
				memset(RecvBuff, 0, sizeof(RecvBuff));
				NT_MakeParameter( (char *)RecvBuff);	
				kmy_USART1SendString( RecvBuff);
				State = STATE_UPLOAD_ACK;
				break;

			case STATE_UPLOAD_ACK:
				RecvLen = 0;
				memset(RecvBuff, 0, sizeof(RecvBuff));
				
				RecvLen = kmy_USART1ReceiveData(RecvBuff, 2, 100);
				if(RecvLen == 2)
				{
					if( strstr( (char *)RecvBuff, ACK_OKSTR))
					{
						State = STATE_READ_CMD;
						clear_lcd();
						DrawTitle_to_row_Language2(1, "Read Config Success", "Read Config Success");

						WaitkeyAndAutoExit(10);
						return 0;
					}		
					else
					{
						clear_lcd();
						DrawTitle_to_row_Language2(1, "Read Config Failed", "Read Config Failed");
						WaitkeyAndAutoExit(10);
						return 1;
					}
					
				}
				break;
			
				
			case STATE_CHECKSUM://check parity, send ok or error
				//有效数据位置是从2开始到ReadLen2的数据
				CheckSum = (RecvBuff[ReadLen2+2] << 8) | RecvBuff[ReadLen2 + 3];
				localSum = GetSum(RecvBuff+2, ReadLen2);

				sprintf((char *)converter, "Len:%u Sum:%u", ReadLen2, CheckSum);
				text_out_to_row(2, converter);
				
				if(CheckSum == localSum)
				{
					kmy_USART1SendString("OK");
					RecvBuff[ReadLen2+2] = '\0';

					retval = NT_StoreParam((char*)RecvBuff+2);
					if(retval == 0)
					{
						clear_lcd();
						DrawTitle_to_row_Language2(1,"Update Success","Update Success");
						
						WaitkeyAndAutoExit(10);
					}
					else 
					{
						clear_lcd();
						if(retval == 1)			DrawTitle_to_row(0,"Update Fail");
						else if(retval == 2)	DrawTitle_to_row(0,"ERROR 2");
						else if(retval == 3)	DrawTitle_to_row(0,"ERROR 3");
						else if(retval == 4)	DrawTitle_to_row(0,"ERROR 4");
						else 					DrawTitle_to_row(0,"Other Error");
						WaitkeyAndAutoExit(10);
					}
					if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
					return 0;
				}
				else
				{	
					kmy_USART1SendString("ERROR");
					if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
					msleep(5000);
					//M: return 0;
					return 0;
				}
		}
	}
	return 0;
}



static char NetworkSet_WiFi (char *msg)
{
	memset (NetworkSwitch, 0, sizeof (NetworkSwitch) );
	NetworkSwitch[0] = WiFi;

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}
	return 0;
}

static char NetworkSet_GPRS (char *msg)
{
	memset (NetworkSwitch, 0, sizeof (NetworkSwitch) );
	NetworkSwitch[0] = GPRS;

	if (!StorageVariable() )
	{
		DrawTitle_to_row_Language2 (3, "更改失败!", "change failure!");
		WaitkeyAndAutoExit (10);
	}
	return 0;
}

static char NetworkSet (char *msg)	//key tone setting
{
	static unsigned char ToneClose1[20] = {"1.GPRS   "};
	static unsigned char ToneClose2[20] = {"1.GPRS   "};
	static unsigned char ToneOpen1[20] = {"2.WiFi   "};
	static unsigned char ToneOpen2[20] = {"2.WiFi   "};
	unsigned char OldNetwork;

	static struct MenuFuc_Language2 Menu[] =
	{
		{ToneClose1, ToneClose2, NetworkSet_GPRS},
		{ToneOpen1, ToneOpen2, NetworkSet_WiFi}
	};
	struct MenuOperate_Language2 MenuOp =
	{
		(struct MenuFuc_Language2*) Menu,
		sizeof(Menu) / sizeof(Menu[0]),
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

    OldNetwork = NetworkSwitch[0];
	
	my_strdel (ToneClose1, '*');
	my_strdel (ToneClose2, '*');
	my_strdel (ToneOpen1, '*');
	my_strdel (ToneOpen2, '*');

	if (NetworkSwitch[0] == GPRS)
	{
		strcat ( (s8*) ToneClose1, "*");
		strcat ( (s8*) ToneClose2, "*");
	}
	else
	{
		strcat ( (s8*) ToneOpen1, "*");
		strcat ( (s8*) ToneOpen2, "*");
	}

	while (1)
	{
		MenuHandler_Language2 (&MenuOp);

		if (MenuOp.RetVal == KEY_Enter)
		{
			MenuOp.FucRetVal = MenuOp.Menu->MenuFuc (msg);

			if (MenuOp.FucRetVal == KEY_WaitTimeout || MenuOp.FucRetVal == KEY_ReturnInitInterface)
			{
				CurProc = CurProc_Return;
				if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
				return MenuOp.FucRetVal;
			}
			else
			{
				clear_lcd();
				MenuOp.flashflag = 1;
				my_strdel (ToneClose1, '*');
				my_strdel (ToneClose2, '*');
				my_strdel (ToneOpen1, '*');
				my_strdel (ToneOpen2, '*');

				if (NetworkSwitch[0] == GPRS)
				{
					strcat ( (s8*) ToneClose1, "*");
					strcat ( (s8*) ToneClose2, "*");
				}
				else
				{
					strcat ( (s8*) ToneOpen1, "*");
					strcat ( (s8*) ToneOpen2, "*");
				}
			}
		}
		else
		{
			if(OldNetwork != NetworkSwitch[0])ChangeNetwork();
			return MenuOp.RetVal;
		}
	}
}

char BasicSettings(char *msg)
{
	static struct MenuFuc_Language2 Menu[10]={
		{"1.屏幕对比度设置","1.Contrast set",ContrastSet},
		{"2.按键音设置","2.Keytone set",BasicKeyTone},
		{"3.模式设置","3.Model set",Model_Set},		
		{"4.时间日期设置","4.DateTime set",TimeDateSet},
		{"5.APN设置","5.APN set",BasicApnSet},
		{"6.屏保时间设置","6.Timeout set",PowerOffScreenTime},
		{"7.语言设置","7.Language set",LanguageSet},
		{"8.读取相关信息","8.Read Misc",ReadSomeMessage},
		{"9.网络设置", "9.Network Settings", NetworkSet},
		{"10.自测","10.Selftest",SelfTest},
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		10,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			CurProc=CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}

static char TerminalIDSet(char *msg)//
{
	unsigned char buff[24];
	unsigned char retval;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)TerminalID);
	DrawTitle_to_row_Language2(0,"【当前终端ID】","Current ID");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按<Enter>","Change: Enter");
	text_out_to_row_Language2(4,"退出:请按<Return>","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("【输入终端ID】","TerminalID edit",SRF_123|SRF_ABC|SRF_abc,SRF_123,buff,sizeof(buff)-1,1);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	if(strlen((sc8*)buff)==0){
		DrawTitle_to_row_Language2(2,"长度不能为0","Length error!");
	}else{
		memset(TerminalID,0,sizeof(TerminalID));
		strcpy((s8*)TerminalID,(sc8*)buff);
		if(!StorageVariable()){
			DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		}else{
			DrawTitle_to_row_Language2(3,"更改成功!","change success!");
		}
	}

	return (WaitkeyAndAutoExit(10));
}

static char TerminalPasswordSet(char *msg)//
{
	unsigned char buff[24];
	unsigned char retval;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)TerminalPassword);
	DrawTitle_to_row_Language2(0,"【当前终端密码】","TerminalPassword");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按<Enter>","Change: Enter");
	text_out_to_row_Language2(4,"退出:请按<Return>","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	retval=GetString_abcABC123PYWuBi_Language2("【输入终端密码】","TerminalPassword",SRF_123|SRF_ABC|SRF_abc,SRF_123,buff,sizeof(buff)-1,1);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	if(strlen((sc8*)buff)==0){
		DrawTitle_to_row_Language2(2,"长度不能为0","Length error!");
	}else{
		memset(TerminalPassword,0,sizeof(TerminalPassword));
		strcpy((s8*)TerminalPassword,(sc8*)buff);
		if(!StorageVariable()){
			DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		}else{
			DrawTitle_to_row_Language2(3,"更改成功!","change success!");
		}
	}

	return (WaitkeyAndAutoExit(10));
}


static char IpAddSet(char *msg)//IP地址设置
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)ServerIpPortBuf);
	DrawTitle_to_row_Language2(0,"【当前IP地址】","Current IP");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按<Enter>","Change: Enter");
	text_out_to_row_Language2(4,"退出:请按<Return>","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc;
	argAa1.LocalTitle="【输入IP地址】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	if(strlen((sc8*)buff)>21||strlen((sc8*)buff)<9){
		DrawTitle_to_row_Language2(2,"错误:IP地址不对!","Error:IP wrong!");
	}else{
		memset(ServerIpPortBuf,0,sizeof(ServerIpPortBuf));
		strcpy((s8*)ServerIpPortBuf,(sc8*)buff);
		if(!StorageVariable()){
			DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		}else{
			DrawTitle_to_row_Language2(3,"更改成功!","change success!");
		}
	}

	return (WaitkeyAndAutoExit(10));
}



static char PIN_OpenClose(char *msg)
{
	#if 0
	char retval,buff[1024];
	char temp[20];
	char PinTimes[2],PukTimes[3];

	loop:
	retval=ReadPINPUKTimes(PinTimes,PukTimes);
	if(retval==0){
		if(PinTimes[0]>'0'){
			clear_lcd();
			DrawTitle_to_row_Language2(0,"输入PIN","Input PIN");
			if(Language[0]=='1'){
				strcpy(buff,"remain times:");
			}else{
				strcpy(buff,"剩余次数:");
			}
			strcat(buff,PinTimes);
			DrawTitle_to_row_Language2(1,buff,buff);
			retval=PassWordInput2(NULL,temp,5,1,0,KEY_NONE,2);
			if(retval!=KEY_Enter)return 0;
			if(strlen(temp)!=4){
				clear_lcd();
				DrawTitle_to_row_Language2(1,"长度错误","length error");
				sleep(2);
				return 0;
			}

			strcpy(buff,"AT+CLCK=\"SC\",");			//AT+CLCK="SC",1,"0907"
			if(RequirePin==NO){
				strcat(buff,"1");
			}else{
				strcat(buff,"0");
			}
			strcat(buff,",\"");
			strcat(buff,temp);
			strcat(buff,"\"\r");

			if(send_at_cmd_and_receive(buff,sizeof(buff),2000,"OK\r")!=TCP_OK){
				clear_lcd();
				DrawTitle_to_row_Language2(2,"PIN 错误","PIN error");
				sleep(2);
				goto loop;
			}else{
				clear_lcd();
				if(RequirePin==0){
					DrawTitle_to_row_Language2(2,"启用pin成功","Enble PIN success");
					RequirePin=1;
				}else{
					DrawTitle_to_row_Language2(2,"禁用pin成功","Disable PIN success");
					RequirePin=0;
				}
				sleep(2);
				return 0;
			}
		}else{
			clear_lcd();
			DrawTitle_to_row_Language2(2,"PIN 被锁","PIN locked");
			sleep(2);
			PUK_Required();
			return 0;
		}
	}

	return 0;
	#else
	return 0;
	#endif
}
char PIN_Store(char *msg)//AT+CPWD="SC","0909","0505"
{
	unsigned char ret;
	unsigned char pin[7];

	clear_lcd();
	strcpy((s8*)pin,(sc8*)StorePIN);
	DrawTitle_to_row_Language2(0,"PIN","PIN");
	text_out_to_row_Language2(1,"****","****");
	text_out_to_row_Language2(3,"修改:请按<Enter>键","change:Enter");
	text_out_to_row_Language2(4,"返回:请按<Return>键","exit:Return");

	ret=WaitkeyAndAutoExit(10);
	if(ret!=KEY_Enter)return ret;

	if(Language[0]=='1'){
		ret=PassWordInput2("Input PIN",pin,6,1,1,KEY_NONE,1);
	}else{
		ret=PassWordInput2("【输入PIN】",pin,6,1,1,KEY_NONE,1);
	}
	if(ret!=KEY_Enter)return ret;

	if(strlen((sc8*)pin)!=4){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"长度错误","longth error");
		return(WaitkeyAndAutoExit(10));
	}

	clear_lcd();
	strcpy((s8*)StorePIN,(sc8*)pin);
	if(!StorageVariable()){
		DrawTitle_to_row_Language2(3,"修改失败","Change falure!");
	}else{
		DrawTitle_to_row_Language2(3,"修改成功","Change success!");
	}

	return (WaitkeyAndAutoExit(10));
}
static char PIN_Change(char *msg)//AT+CPWD="SC","0909","0505"
{
	#if 0
	PasswordInputArgument arg;
	unsigned char oldpassword[5];
	unsigned char newpassword1[5];
	unsigned char newpassword2[200];
	unsigned char retval;

	if(RequirePin==YES){	//启用pin  之后才能改pin
		PassWordArgDeInit(&arg,oldpassword,4);
		arg.LocalTitle="【输入原PIN】";
		retval=BT_PassWordInput_MiddledDisplay(&arg);
		if(retval!=KEY_Enter)return retval;

		again:
		PassWordArgDeInit(&arg,newpassword1,4);
		arg.LocalTitle="【输入新PIN】";
		retval=BT_PassWordInput_MiddledDisplay(&arg);
		if(retval!=KEY_Enter)return retval;

		PassWordArgDeInit(&arg,newpassword2,4);
		arg.LocalTitle="【再次输入新PIN】";
		retval=BT_PassWordInput_MiddledDisplay(&arg);
		if(retval!=KEY_Enter)return retval;

		if(strcmp(newpassword1,newpassword2)!=0){
			clear_lcd();
			DrawTitle_to_row(1,"你两次输入的");
			DrawTitle_to_row(2,"新PIN不一致");
			DrawTitle_to_row(3,"请重新输入");
			retval=WaitkeyAndAutoExit(10);
			if(retval==KEY_Enter)goto again;
			else {return retval;}
		}

		//AT+CPWD="SC","0909","0505"
		strcpy(newpassword2,"AT+CPWD=\"SC\",\"");
		strcat(newpassword2,oldpassword);
		strcat(newpassword2,"\",\"");
		strcat(newpassword2,newpassword1);
		strcat(newpassword2,"\"\r");
		retval=send_at_cmd_and_return(newpassword2,5000,"OK","ERROR");

		clear_lcd();
		if(retval==TCP_OK){
			DrawTitle_to_row(2,"修改成功");
		}
		else{
			DrawTitle_to_row(2,"修改失败");	
		}
		return WaitkeyAndAutoExit(10);
	}
	else{
		clear_lcd();
		DrawTitle_to_row(2,"请先启用PIN");
		return WaitkeyAndAutoExit(10);
	}

	return 0;
	#else
	return 0;
	#endif
}

char PinSet(char *msg)
{
	#if 0
	return(abeyant());
	#else
	static unsigned char BuffLocal[30],BuffOther[30];


	static struct MenuFuc_Language2 Menu[3]={
		{BuffLocal,BuffOther,PIN_OpenClose},
		{"2.修改 PIN","2.Change PIN",PIN_Change},
		{"3.存储 PIN","3.Store PIN",PIN_Store},
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		3,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while(1){
		if(RequirePin==0){
			strcpy((s8*)BuffLocal,"1.禁用 PIN");
			strcpy((s8*)BuffOther,"1.Close PIN");
		}else{
			strcpy((s8*)BuffLocal,"1.启用 PIN");
			strcpy((s8*)BuffOther,"1.Open PIN");
		}
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return MenuOp.RetVal;
		}
	}
	#endif
}

static char RestoreChuChangSet(char *msg)
{
	unsigned char retval;

	clear_lcd();
	DrawTitle_to_row_Language2(0,"特别警告:","Warning:");
	DrawTitle_to_row_Language2(1,"恢复出厂设置将导致,","Are you sure");
	DrawTitle_to_row_Language2(2,"此终端机成为一新机", "reset to the");
	DrawTitle_to_row_Language2(3,"确定恢复:请按<Enter>","factory settings?");
	DrawTitle_to_row_Language2(4,"退出:请按任意键",NULL);

	retval=WaitkeyAndAutoExit(20);
	if(retval!=KEY_Enter)return retval;

	retval=RestoreConfigFile();
	clear_lcd();
	if(retval==1){
		DrawTitle_to_row_Language2(2,"出厂设置恢复成功!","Reset success");
	}else{
		DrawTitle_to_row_Language2(2,"出厂设置恢复失败!","Reset error");
	}
	return (WaitkeyAndAutoExit(10));
}



static char OpPsword(char *msg)//终端操作密码设置，最多20  位密码
{
	unsigned char buff[16],buff2[16];
	char retval;
	PasswordInputArgument arg;

	PassWordArgDeInit(&arg,buff,sizeof(buff));
	arg.LocalTitle="【输入新密码】";
	arg.OtherTitle="Input new password";
	retval=PassWordInput_MiddledDisplay(&arg);
	if(retval!=KEY_Enter)return retval;

	PassWordArgDeInit(&arg,buff2,sizeof(buff2));
	arg.LocalTitle="【再次确认新密码】";
	arg.OtherTitle="Again Input";
	retval=PassWordInput_MiddledDisplay(&arg);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	if(strcmp((sc8*)buff,(sc8*)buff2)==0){
		memset(EnterAdvancedSetPassWord,0,sizeof(EnterAdvancedSetPassWord));
		strcpy((s8*)EnterAdvancedSetPassWord,(sc8*)buff);
		if(!StorageVariable()){
			DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		}else{
			DrawTitle_to_row_Language2(3,"更改成功!","change success!");
		}
	}else{
		DrawTitle_to_row_Language2(2,"错误!你两次输入","Error,Two times");
		DrawTitle_to_row_Language2(3,"的密码不一致!","Input not same");
	}

	return (WaitkeyAndAutoExit(10));
}



static char Delete_log(char *msg)
{
	#if 0
	unsigned char *p;
	unsigned char retval;

	p=(unsigned char *)(malloc(sizeof(JiaoFeiLogPath)+20));

	if(p!=NULL){
		strcpy(p,"rm -rf ");
		strcat(p,JiaoFeiLogPath);

		system(p);

		free(p);
		p=NULL;

		clear_lcd();
		DrawTitle_to_row(2,"日志记录删除成功");
	}
	else{
		clear_lcd();
		DrawTitle_to_row(2,"无法分配内存空间");
	}

	retval=WaitkeyAndAutoExit(10);

	return retval;
	#else
	return 0;
	#endif
}

static char FormatEEPROM(char *msg)
{
	u8 retval;

	clear_lcd();
	text_out_to_row_Language2(1,"格式化EEPROM,将导致","Format EEPROM will");
	text_out_to_row_Language2(2,"EEPROM中的所有数据都","Lose all EEPROM data");
	text_out_to_row_Language2(3,"丢失,确认请按<Enter>","Can you sure Format");
	text_out_to_row_Language2(4,"键","it?");
	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	DrawTitle_to_row_Language2(2,"正在格式化","Formating");
	EepromFile_FileSystemFormat();
	DrawTitle_to_row_Language2(3,"格式化完毕","Format Complete");
	retval=WaitkeyAndAutoExit(10);

	return retval;
}


char AdvancedSet(char *msg)
{
	unsigned char OperateCode[30];
	char retval;
	PasswordInputArgument arg;
	static struct MenuFuc_Language2 Menu[9]={		
		{"1.更新参数", "1.Update Parameter", UpdateParameter},
		{"2.高级设置密码设置","2.This Password set",OpPsword},		//注意:  第一个字符串的长度不能大于MaxByteOneLine-1
		{"3.IP地址设置","3.IP address set",IpAddSet},
		{"4.PIN设置","4.PIN set",PinSet},
		{"5.恢复出厂设置","5.Reset Factory",RestoreChuChangSet},
		{"6.删除日志记录","6.Delete Record",Delete_log},
		{"7.终端ID设置","7.Terminal ID set",TerminalIDSet},
		{"8.终端密码设置","8.Terminal Password",TerminalPasswordSet},
		{"9.格式化EEPROM","9.Format EEPROM",FormatEEPROM},
	};
	struct MenuOperate_Language2 MenuOp={Menu,9,0, 0,0,MaxRowOneScr, 1,0,0};

	PassWordArgDeInit(&arg,OperateCode,16);
	arg.LocalTitle="【请输入密码】";
	retval=PassWordInput_MiddledDisplay(&arg);
	if(retval!=KEY_Enter)return retval;

	if(strcmp((sc8*)EnterAdvancedSetPassWord,(sc8*)OperateCode)!=0){
		DrawTitle_to_row_Language2(2,"密码错误!","Password error");
		return(WaitkeyAndAutoExit(10));
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return (MenuOp.RetVal);
		}
	}
}



char MainMenuScreen_Settings(char *msg)
{
	static struct MenuFuc_Language2 Menu[3]={
		{"1.基本设置","1.BasicSettings",BasicSettings},
		{"2.高级设置","2.AdvancedSet",AdvancedSet},		
		{"3.WiFi设置","3.WiFi Settings",WIFISet}		
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		3,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			CurProc=CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}

char WANSet(char *msg)
{
//	unsigned char OperateCode[30];
//	char retval;
//	PasswordInputArgument arg;
	static struct MenuFuc_Language2 Menu[]={
		{"1.IP地址设置","1.Local IP set",LoclaIPSet},		//注意:  第一个字符串的长度不能大于MaxByteOneLine-1
		{"2.网关设置","2.Gateway set",GatewaySet},
		{"3.子网掩码设置","3.Mask set",MaskSet},
		{"4.DNS设置","4.DNS set",DNSSet},
	};
	//struct MenuOperate_Language2 MenuOp={Menu,4,0, 0,0,MaxRowOneScr, 1,0,0};
	struct MenuOperate_Language2 MenuOp={Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0};

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return (MenuOp.RetVal);
		}
	}
}

static char LoclaIPSet(char *msg)//本地IP地址设置
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)LocalIpPortBuf);
	DrawTitle_to_row_Language2(0,"【当前IP地址】","Current IP");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入IP地址】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();	
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:IP地址不对!","Error:IP wrong!");
	}else{
		memset(LocalIpPortBuf,0,sizeof(LocalIpPortBuf));
		strcpy((s8*)LocalIpPortBuf,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
        	retval = Set_wifi_NetAddr(LocalIpPortBuf,MaskBuff,GatewayBuff,DNSBuff);
			
        	if(retval != TCP_OK){
				clear_lcd();
				DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
        	}
			else
			{
    			retval = Set_wifi_saveconfig();
            	if(retval != TCP_OK)
				{
					clear_lcd();
					DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
				}
				else 
				{
					Set_wifi_Reboot();
					clear_lcd();
					DrawTitle_to_row_Language2(2,"更改成功!","change success!");
				}				
			}
		}
	}

	return (WaitkeyAndAutoExit(10));
}


static char GatewaySet(char *msg)//本地网关地址设置
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)GatewayBuff);
	DrawTitle_to_row_Language2(0,"【当前网关】","Current Gateway");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入网关】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:网关不对!","Error:Gateway wrong!");
	}else{
		memset(GatewayBuff,0,sizeof(GatewayBuff));
		strcpy((s8*)GatewayBuff,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
        	retval = Set_wifi_gateway(GatewayBuff);
			
        	if(retval != TCP_OK){
				clear_lcd();
				DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
        	}
			else
			{
    			retval = Set_wifi_saveconfig();
            	if(retval != TCP_OK)
					{
					clear_lcd();
					DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
				}
				else 
				{
					Set_wifi_Reboot();
					clear_lcd();
					DrawTitle_to_row_Language2(2,"更改成功!","change success!");
				}	
			}
		}
	}

	return (WaitkeyAndAutoExit(10));
}

static char MaskSet(char *msg)//子网掩码
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)MaskBuff);
	DrawTitle_to_row_Language2(0,"【当前子网掩码】","Current Mask");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入子网掩码】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();	
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:子网掩码不对!","Error:Mask wrong!");
	}else{
		memset(MaskBuff,0,sizeof(MaskBuff));
		strcpy((s8*)MaskBuff,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
        	retval = Set_wifi_mask(MaskBuff);
			
        	if(retval != TCP_OK){
				clear_lcd();
				DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
        	}
			else
			{
    			retval = Set_wifi_saveconfig();
            	if(retval != TCP_OK){
					clear_lcd();
					DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
            		}
				else 
				{
					Set_wifi_Reboot();
					clear_lcd();
					DrawTitle_to_row_Language2(2,"更改成功!","change success!");
				}	
			}
		}
	}

	return (WaitkeyAndAutoExit(10));
}


static char DNSSet(char *msg)//dns设置
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	
	strcpy((s8*)buff,(sc8*)DNSBuff);
	DrawTitle_to_row_Language2(0,"【当前DNS地址】","Current DNS");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入DNS地址】";
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	clear_lcd();	
	DrawTitle_to_row_Language2(2,"请等待...","Please Wait...");
	if(strlen((sc8*)buff)>21){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:DNS地址不对!","Error:DNS wrong!");
	}else{
		memset(DNSBuff,0,sizeof(DNSBuff));
		strcpy((s8*)DNSBuff,(sc8*)buff);
		if(!StorageVariable()){
			clear_lcd();
			DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
		}
		else
		{
        	retval = Set_wifi_dns(DNSBuff);
			
        	if(retval != TCP_OK){
				clear_lcd();
				DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
        	}
			else
			{
    			retval = Set_wifi_saveconfig();
            	if(retval != TCP_OK)
            	{
            		clear_lcd();					
					DrawTitle_to_row_Language2(2,"更改失败!","change failure!");
            	}
				else 
				{
					Set_wifi_Reboot();
					clear_lcd();
					DrawTitle_to_row_Language2(2,"更改成功!","change success!");
				}	
			}
		}
	}

	return (WaitkeyAndAutoExit(10));
}


char WIFISet(char *msg)
{
	static struct MenuFuc_Language2 Menu[]={
		{"1.搜索网络","1.Search WLAN Networks",SearchWLANSet},	
		{"2.参数设置","2.Parameter Set",WIFIParmeterSet},
		{"3.本地设置","3.Local settings",WANSet},
		{"4.TCP/IP连接测试","4.TCP/IP Connect Test",WIFIConnectTCP},



		{"5.恢复默认设置","5.Set default",SetWifiDefault}
	};
	struct MenuOperate_Language2 MenuOp={Menu,sizeof(Menu) / sizeof(Menu[0]),0, 0,0,MaxRowOneScr, 1,0,0};

	while(1){
		if(NetworkSwitch[0]!=WiFi)
    	{
			NetworkSwitch[0]=WiFi;
			if (!StorageVariable() )
        	{
        		DrawTitle_to_row_Language2 (3, "网络更改失败!", "Change failure!");
        		WaitkeyAndAutoExit (2);
				return 0;
        	}        	
			ChangeNetwork();
    	}
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter)
		{
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else
			{
				clear_lcd();
				MenuOp.flashflag=1;
			}
		}
		else
		{
			return (MenuOp.RetVal);
		}
	}
}




static char WIFIConnectTCP(char *msg)
{
	unsigned char retval;
	unsigned int start;
	char connstatus[100]={0},i=0;

	
	clear_lcd();
	DrawTitle_to_row_Language2(1,"【连接中,请稍等...】","Connecting...");
	DrawTitle_to_row_Language2(2,"目标地址","Destination address");
	DrawTitle_to_row_Language2(3,ServerIpPortBuf,ServerIpPortBuf);

	start = kmy_TimeGetMilliseconds();
	i=0;
	while(1)
	{
		retval = GetKeyValue();
		printf("\n retval = GetKeyValue();=%d", retval);
		if(retval==KEY_ReturnInitInterface||retval==KEY_ReturnPrevious ||retval == KEY_Enter)
		{
			return retval;
		}		
		if(kmy_TimeGetMilliseconds()-start >1000)
		{
			retval = Set_wifi_connstatus((unsigned char*)connstatus,NULL_CHAR);
			 	printf("\n 	retval = Set_wifi_connstatus();=%d", retval);
			if(retval != TCP_OK){
				goto connectfail;
			}
			if(strstr(connstatus,"CONNECTED"))
			{
				break;
			}

			i++;
			start = kmy_TimeGetMilliseconds();
		}
		if(i>10)
			goto connectfail;
		
	}

	clear_lcd();
	DrawTitle_to_row_Language2(1,"【连接成功!】","Connect success!");

	return WaitkeyAndAutoExit(10);

connectfail:
	clear_lcd();
	DrawTitle_to_row_Language2(1,"【连接失败!】","Connect failed!");
	WaitkeyAndAutoExit(10);
	return KEY_ReturnPrevious;
	
}



static char WIFIParmeterSet(char *msg)
{
	static struct MenuFuc_Language2 Menu[]={
		    {"1.SSID设置","1.SSID Set",SSIDSet},
			{"2.网络密钥管理","2.WP Manage",WPManage},
			{"3.C/S模式设置","3.C/SMode Set",CSModeSet},
			{"4.DHCP开关","4.DHCP Set",DhcpSet},
			{"5.工作模式设置","5.WorkMode Set",WifiworkmodeSet},
			{"6.加密模式设置","6.EncType Set",EncTypeSet},
		};
		struct MenuOperate_Language2 MenuOp={Menu,sizeof(Menu) / sizeof(Menu[0]),0, 0,0,MaxRowOneScr, 1,0,0};
	
		while(1){
			MenuHandler_Language2(&MenuOp);
			if(MenuOp.RetVal==KEY_Enter){
				MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(NULL);
				

				if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface)
				{
					CurProc=CurProc_Return;
					// save wifi saveconfig
					//WifiSaveConfig();
					return MenuOp.FucRetVal;
				} 
				else{clear_lcd();MenuOp.flashflag=1;}
			}else{
				// save wifi saveconfig
				//WifiSaveConfig();
				return (MenuOp.RetVal);
			}
		}


}


static char SetWifiDefault(char *msg)
{ 
	clear_lcd();
	DrawTitle_to_row_Language2(1,"【重置WIFI模块中...】","Reseting...");

	Set_wifi_default();
 	return KEY_ReturnPrevious;
}


static char SSIDSet(char *msg)
{
	unsigned char buff[100];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)WLANSSID);
	DrawTitle_to_row_Language2(0,"【当前网络SSID】","Current SSID");
	text_out_to_row_Language2(1,buff,buff);
	text_out_to_row_Language2(3,"更改:请按确认","Change: Confirm");
	text_out_to_row_Language2(4,"退出:请按取消","Exit: Return");

	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入网络SSID】";
	argAa1.OtherTitle="Input SSID" ;
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	
	if(strlen((sc8*)buff)>30){
		clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:SSID长度不对!","Error:SSID length error!");
	}else{
		memset(WLANSSID,0,sizeof(WLANSSID));
		strcpy((s8*)WLANSSID,(sc8*)buff);
		if(!StorageVariable())
		{
		    clear_lcd();
			DrawTitle_to_row_Language2(2,"失败!","failure!");
		}
		else
	    {
		    retval =Set_wifi_ssid(WLANSSID,buff);
			clear_lcd();
        	if(retval != TCP_OK){
				DrawTitle_to_row_Language2(2,"失败!","failure!");
        	}
			else
			{
    			retval = Set_wifi_saveconfig();
            	if(retval != TCP_OK)DrawTitle_to_row_Language2(2,"失败!","failure!");
				else DrawTitle_to_row_Language2(2,"成功!","success!");				
			}
			DrawTitle_to_row_Language2(2,"成功!","success!");
		}
	}

	return (WaitkeyAndAutoExit(10));
}

static char WifiSaveConfig(void)
{
	clear_lcd();
	DrawTitle_to_row_Language2(2,"【正在配置WiFi...】","Configuring WiFi...");	
	Set_wifi_NetAddr(LocalIpPortBuf,MaskBuff,GatewayBuff,DNSBuff);
    Set_wifi_ConnectAddr(ServerIpPortBuf);
	Set_wifi_Reboot();
	return KEY_ReturnPrevious;	
}


static char SearchWLANSet(char *msg)
{
	unsigned char retval;
	
	clear_lcd();
	DrawTitle_to_row_Language2(2,"【正在搜索网络...】","Searching WLAN...");

	WIFI_Set_WLANPar();

	retval = SearchWLAN();
	if(retval != TCP_OK)
	{
		clear_lcd();
		DrawTitle_to_row_Language2(2,"【搜索失败】","Search WLAN Fail");
		retval = WaitkeyAndAutoExit(10);
		return  retval;
	}
	
	retval = DisplayNetwork();
	if((retval==KEY_ReturnInitInterface)||(retval==KEY_ReturnPrevious))return  retval;
	
	if(retval != TCP_OK)
    {
        clear_lcd();
		DrawTitle_to_row_Language2(2,"WiFi连接失败","Connect WiFi Fail");
        return WaitkeyAndAutoExit(10);
	}
	
	retval = WPManage(NULL);	
	if((retval==KEY_ReturnInitInterface)||(retval==KEY_ReturnPrevious))return retval;
	WifiSaveConfig();
	
	retval = GetWiFiInfo();
	clear_lcd();
	if(retval == 0)
	{	
		DrawTitle_to_row_Language2(2,"WiFi连接失败","Connect WiFi Fail");
	}
	else
	{
		DrawTitle_to_row_Language2(2,"WiFi连接成功","Connect WiFi OK");
	}
	
	WaitkeyAndAutoExit(10);

	return KEY_ReturnPrevious;
}

static char WPManage(char *msg)
{
	unsigned char buff[24];
	unsigned char retval;
	abcABC123PYWuBiArgument argAa1;

	clear_lcd();
	strcpy((s8*)buff,(sc8*)WLANWP);
	DrawTitle_to_row_Language2(0,"【当前网络密钥】","Current WP");
	text_out_to_row_Language2(1,buff,buff);

	abcABC123PYWuBiArgDeInit(&argAa1,buff,28);
	argAa1.srfFlag=SRF_123|SRF_ABC|SRF_abc|SRF_PY|SRF_BiHua;
	argAa1.LocalTitle="【输入网络密钥】";
	argAa1.OtherTitle="Input WP" ;
	argAa1.HaveoldFlag=1;
	retval=GetString_abcABC123PYWuBi2(&argAa1);
	if(retval!=KEY_Enter)return retval;

	
	if(strlen((sc8*)buff)>30)
	{
	    clear_lcd();
		DrawTitle_to_row_Language2(2,"错误:密钥长度不对!","Error:WP length error!");
	}
	else
	{
		memset(WLANWP,0,sizeof(WLANWP));
		strcpy((s8*)WLANWP,(sc8*)buff);
		if(!StorageVariable())
		{
		    clear_lcd();
			DrawTitle_to_row_Language2(2,"失败!","failure!");
		}
		else
	    {
		    retval = Set_wifi_wp(WLANWP);
			clear_lcd();
        	if(retval != TCP_OK){
				DrawTitle_to_row_Language2(2,"失败!","failure!");
        	}
			else
			{
    			retval = Set_wifi_saveconfig();
            	if(retval != TCP_OK)DrawTitle_to_row_Language2(2,"失败!","failure!");
				else DrawTitle_to_row_Language2(2,"成功!","success!");				
			}
			DrawTitle_to_row_Language2(2,"成功!","success!");
		}
	}
	return (WaitkeyAndAutoExit(1));
}

static char WIFIParStore(char *msg)
{
	unsigned char retval;

	if(!StorageVariable()){		
		clear_lcd();
		DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
		WaitkeyAndAutoExit(10);
	}
	else
	{
		retval = WIFI_Set_WLANPar();
		
		if(retval != TCP_OK){			
			clear_lcd();
			DrawTitle_to_row_Language2(3,"更改失败!","change failure!");
			WaitkeyAndAutoExit(10);
		}
		else
		{
			retval = Set_wifi_saveconfig();
			if(retval != TCP_OK)
			{			
			    clear_lcd();
				DrawTitle_to_row_Language2(3,"更改失败!","change failure!");			
				WaitkeyAndAutoExit(10);
			}
		}
	}

	return 0;
	
}

static char CSModeSet(char *msg)
{
	static unsigned char OptionCn1[20]={"1.服务器端  "};
	static unsigned char OptionEn1[20]={"1.Server  "};
	static unsigned char OptionCn2[20]= {"2.客户端   "};
	static unsigned char OptionEn2[20]= {"2.Client  "};

	static struct MenuFuc_Language2 Menu[]={
		{OptionCn1,OptionEn1,WIFIParStore},
		{OptionCn2,OptionEn2,WIFIParStore}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0
	};

	my_strdel(OptionCn1,'*');
	my_strdel(OptionEn1,'*');
	my_strdel(OptionCn2,'*');
	my_strdel(OptionEn2,'*');

	if(CSmode[0]=='0'){
		strcat((s8*)OptionCn1,"*");
		strcat((s8*)OptionEn1,"*");
	}else{
		strcat((s8*)OptionCn2,"*");
		strcat((s8*)OptionEn2,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			CSmode[0] = MenuOp.Menucnt+0x30;
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(OptionCn1,'*');
				my_strdel(OptionEn1,'*');
				my_strdel(OptionCn2,'*');
				my_strdel(OptionEn2,'*');

				if(CSmode[0]=='0'){
					strcat((s8*)OptionCn1,"*");
					strcat((s8*)OptionEn1,"*");
				}else{
					strcat((s8*)OptionCn2,"*");
					strcat((s8*)OptionEn2,"*");
				}
			}
		}else{
			return MenuOp.RetVal;
		}
	}
}


static char DhcpSet(char *msg)
{	
	static unsigned char OptionCn1[20]={"1.关闭Dhcp功能   "};
	static unsigned char OptionEn1[20]={"1.Disable  "};
	static unsigned char OptionCn2[20]= {"2.打开Dhcp功能   "};
	static unsigned char OptionEn2[20]= {"2.Enable  "};

	static struct MenuFuc_Language2 Menu[]={
		{OptionCn1,OptionEn1,WIFIParStore},
		{OptionCn2,OptionEn2,WIFIParStore}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0
	};

	my_strdel(OptionCn1,'*');
	my_strdel(OptionEn1,'*');
	my_strdel(OptionCn2,'*');
	my_strdel(OptionEn2,'*');

	if(dhcpclient[0]=='0'){
		strcat((s8*)OptionCn1,"*");
		strcat((s8*)OptionEn1,"*");
	}else{
		strcat((s8*)OptionCn2,"*");
		strcat((s8*)OptionEn2,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			dhcpclient[0] = MenuOp.Menucnt+0x30;
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){
				CurProc=CurProc_Return;
				return MenuOp.FucRetVal;
			}
			else {
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(OptionCn1,'*');
				my_strdel(OptionEn1,'*');
				my_strdel(OptionCn2,'*');
				my_strdel(OptionEn2,'*');
				
				if(dhcpclient[0]=='0'){
					strcat((s8*)OptionCn1,"*");
					strcat((s8*)OptionEn1,"*");
				} else {
					strcat((s8*)OptionCn2,"*");
					strcat((s8*)OptionEn2,"*");
				}
			}
		}else{
			return MenuOp.RetVal;
		}
	}

}


static char WifiworkmodeSet(char *msg)
{	
	static unsigned char OptionCn1[20]={"1.基础网   "};
	static unsigned char OptionEn1[20]={"1.Infra  "};
	static unsigned char OptionCn2[20]= {"2.自组网   "};
	static unsigned char OptionEn2[20]= {"2.Ad-hoc  "};

	static struct MenuFuc_Language2 Menu[]={
		{OptionCn1,OptionEn1,WIFIParStore},
		{OptionCn2,OptionEn2,WIFIParStore}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0
	};

	my_strdel(OptionCn1,'*');
	my_strdel(OptionEn1,'*');
	my_strdel(OptionCn2,'*');
	my_strdel(OptionEn2,'*');

	if(workmode[0]=='0'){
		strcat((s8*)OptionCn1,"*");
		strcat((s8*)OptionEn1,"*");
	}else{
		strcat((s8*)OptionCn2,"*");
		strcat((s8*)OptionEn2,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			workmode[0] = MenuOp.Menucnt+0x30;
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(OptionCn1,'*');
				my_strdel(OptionEn1,'*');
				my_strdel(OptionCn2,'*');
				my_strdel(OptionEn2,'*');
				
				if(workmode[0]=='0'){
					strcat((s8*)OptionCn1,"*");
					strcat((s8*)OptionEn1,"*");
				}else{
					strcat((s8*)OptionCn2,"*");
					strcat((s8*)OptionEn2,"*");
				}
			}
		}else{
			return MenuOp.RetVal;
		}
	}
}



static char EncTypeSet(char *msg)
{
	static unsigned char OptionCn1[20]={"1.无加密   "};
	static unsigned char OptionEn1[20]={"1.No Security "};
	static unsigned char OptionCn2[20]= {"2.WEP64   "};
	static unsigned char OptionEn2[20]= {"2.WEP64  "};
	static unsigned char OptionCn3[20]= {"3.WEP128   "};
	static unsigned char OptionEn3[20]= {"3.WEP128  "};
	static unsigned char OptionCn4[20]= {"4.TKIP   "};
	static unsigned char OptionEn4[20]= {"4.TKIP  "};
	static unsigned char OptionCn5[20]= {"5.AES   "};
	static unsigned char OptionEn5[20]= {"5.AES  "};

	static struct MenuFuc_Language2 Menu[]={
		{OptionCn1,OptionEn1,WIFIParStore},
		{OptionCn2,OptionEn2,WIFIParStore},
		{OptionCn3,OptionEn3,WIFIParStore},
		{OptionCn4,OptionEn4,WIFIParStore},
		{OptionCn5,OptionEn5,WIFIParStore}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,sizeof(Menu) / sizeof(Menu[0]),0,0,0,MaxRowOneScr,1,0,0
	};

	my_strdel(OptionCn1,'*');
	my_strdel(OptionEn1,'*');
	my_strdel(OptionCn2,'*');
	my_strdel(OptionEn2,'*');
	my_strdel(OptionCn3,'*');
	my_strdel(OptionEn3,'*');
	my_strdel(OptionCn4,'*');
	my_strdel(OptionEn4,'*');
	my_strdel(OptionCn5,'*');
	my_strdel(OptionEn5,'*');


	if(enctype[0]=='0'){
		strcat((s8*)OptionCn1,"*");
		strcat((s8*)OptionEn1,"*");
	}else if(enctype[0]=='1'){
		strcat((s8*)OptionCn2,"*");
		strcat((s8*)OptionEn2,"*");
	}else if(enctype[0]=='2'){
		strcat((s8*)OptionCn3,"*");
		strcat((s8*)OptionEn3,"*");
	}else if(enctype[0]=='3'){
		strcat((s8*)OptionCn4,"*");
		strcat((s8*)OptionEn4,"*");
	}else{
		strcat((s8*)OptionCn5,"*");
		strcat((s8*)OptionEn5,"*");
	}

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			enctype[0] = MenuOp.Menucnt+0x30;
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				my_strdel(OptionCn1,'*');
				my_strdel(OptionEn1,'*');
				my_strdel(OptionCn2,'*');
				my_strdel(OptionEn2,'*');
				my_strdel(OptionCn3,'*');
				my_strdel(OptionEn3,'*');
				my_strdel(OptionCn4,'*');
				my_strdel(OptionEn4,'*');
				my_strdel(OptionCn5,'*');
				my_strdel(OptionEn5,'*');
				
				
				if(enctype[0]=='0'){
					strcat((s8*)OptionCn1,"*");
					strcat((s8*)OptionEn1,"*");
				}else if(enctype[0]=='1'){
					strcat((s8*)OptionCn2,"*");
					strcat((s8*)OptionEn2,"*");
				}else if(enctype[0]=='2'){
					strcat((s8*)OptionCn3,"*");
					strcat((s8*)OptionEn3,"*");
				}else if(enctype[0]=='3'){
					strcat((s8*)OptionCn4,"*");
					strcat((s8*)OptionEn4,"*");
				}else{
					strcat((s8*)OptionCn5,"*");
					strcat((s8*)OptionEn5,"*");
				}
			}
		}
		else{
			return MenuOp.RetVal;
		}
	}	
}



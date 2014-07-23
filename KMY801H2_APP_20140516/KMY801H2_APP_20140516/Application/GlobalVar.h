/***********************************
Author:Jyotiba Vaijinath kadam
Date: 2012-02-02 20:13
Remarks:global structure decleration
LastModified:Date: 2012-02-02 20:13
Remarks:global structure decleration
************************************/
#include <string.h>
#include <stdio.h>
//#include "stm32f10x.h"
#include "my_stdint.h"
#ifndef VAR_DEFS          // Make sure this file is included only once
#define VAR_DEFS 1
#ifndef VAR_DECLS
# define _DECL extern
# define _INIT(x)
#else
# define _DECL
# define _INIT(x)  = x
#endif
_DECL struct active
{	int slno;
	int deactive;
}actobj;
_DECL struct acceptsms
{	int slno;
	char sms[130];
}gacceptsmsobj;
_DECL struct rejectsms
{	int slno;
	char sms[145];
}grejectsmsobj;
_DECL struct reject
{	
	int slno;
	char sms[157];
	char dat[20];
	char tim[20];	
}viewrejsms;
_DECL struct Msgstore
{
int slno;
char mobileno[20];
char sms[161];
char dt[15];
char tm[15];
}obj;
_DECL struct CustomerTransaction
{
	int slno;    
    char custName[20];
	char custID[15];
	char amount[12];
	
}custTra;
_DECL struct accept
{	
	int slno;
	int billno;
	int price;
	int amt;
	float servicetax;
	int quantity;
	char itemid[10];
	char itemname[50];
	char mobileno[20];
	char dat[20];
	char tim[20];
	
};
struct percentage
{
	int slno;
  	float percent;
};
_DECL struct header
{	
	int slno;
	char h1[34];
	char h2[34];
	char h3[34];
	
	
}hobj;
_DECL struct footer
{	
	int slno;
	char f1[34];
	char f2[34];
	char f3[34];
	
}fobj;
_DECL struct adminpass
{ int slno;
  char uname[10];
  char upass[10];
}adp;
_DECL struct CustomerDetails
{
int slno;
char custIDdet[15];
char custNamedet[20];
char custMobdet[10];
char custStatusdet[8];
}custdet;
_DECL char temp[156];
_DECL int A_len;		 
_DECL int pos2 _INIT(0);
_DECL int key_defno;
_DECL int R_Xcoord,R_Ycoord,RFill_Ycoord,RFill_Xcoord,pos1;
_DECL int keyno,D_Xcoord,D_Ycoord;
_DECL char DEAC,buf[2];
_DECL long Gti,start,end,Keydata;
_DECL int fxcor,sxcor,txcor;
_DECL char SuminWords[10][8];
_DECL int chklen _INIT(0);
_DECL int page _INIT(0),printvalid _INIT(0);
_DECL int chklen;
_DECL int xstart _INIT(5);
_DECL int xend _INIT(121);
_DECL int ystart _INIT(12);
_DECL int yend _INIT(48);
_DECL int mainmenupos _INIT(1),numitem _INIT(5),yfill _INIT(15),menucounter _INIT(0),chkdateflag _INIT(0),recnum _INIT(0);
_DECL char menus[15][21];
_DECL char bufmenu[2];
_DECL int mmenu _INIT(0),smenu1 _INIT(0),smenu2 _INIT(0),smenu3 _INIT(0),mfillpos _INIT(15),fillpos _INIT(15),fillpos1 _INIT(15),fillpos2 _INIT(15),rbotton _INIT(0),ok _INIT(1);
_DECL int back _INIT(0);
_DECL int charlen _INIT(0),yfillcurser _INIT(0);
_DECL char File_Data[1000];
#endif
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/
void acceptorder1(struct Msgstore );
void dailyorder();
int compareDates(char *, char *);
void changepass();
removezero(char *);
void PrntCharcreatesms(int ,char txt1[],char txt2[],char txt3[],char txt4[],char txt5[],int);
void ChkForAlphacreatesms(char txt[10],int );
int show(char a[160],char txt[40],int );
void loadsms();
int countspace (char str[200]);
void readinbx(struct Msgstore a);
char *convert (unsigned int, int);
void myprintf (char *frmt, ...);
void saveorder(struct Msgstore );
void rejectorder(struct Msgstore);
void loadmainmenu();
void orderprint(int ,int ,char *);
void clearmemory();
void setdatetime();
void loadmenures();
void keyboard(int,int);
void loadreport();
void loadprintersetting();
void loadsms();
void loadapplication();
//int selectmenu(char *,char *,char *,int );
int selectmenu(char *,char *,int );
void loadmsetting();
void pendingorder();
int header();
void footer();
int headerrec();
int footerrec();
MsgSend(char *,char *, char *);
void signalstrenth(char signaldata[20]);
void percentage();
 
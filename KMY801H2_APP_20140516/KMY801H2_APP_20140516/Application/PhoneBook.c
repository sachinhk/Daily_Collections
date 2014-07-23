
#include "PhoneBook.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "ShuRuFa.h"
#include "MenuOperate.h"
#include "key.h"
#include "lcd.h"
#include "main.h"
#include "mypure_library.h"
#include "my_library.h"
#include "message.h"
#include "global_extern_variable.h"
#include "sim900B.h"
#include "SelfTest.h"
#include "my_stdint.h"
#include "EepromFileSystem.h"
#include "gprs.h"




static const u8 PhoneNameIco[24]={0xf0,0xfe,0xfe,0xff,0x9f,0x0f,0x07,0x07,0x93,0xdf,0x26,0x00,0x00,0x09,0x0f,0x0b,0x08,0x00,0x0a,0x06,0x0a,0x01,0x00,0x00};
static const u8 PhoneNumberIco[24]={0x00,0xfc,0x24,0x64,0x24,0x64,0x24,0x65,0x27,0xfd,0x00,0x00,0x00,0x0f,0x08,0x0d,0x08,0x0d,0x08,0x0d,0x08,0x0f,0x00,0x00};
static const u8 PhoneZuoJiIco[24]={0x06,0xe7,0xf1,0xfd,0xb9,0x59,0xb9,0xfd,0xf1,0xe7,0x06,0x00,0x00,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x00,0x00};
static const u8 PhoneEmailIco[24]={0xfc,0xfe,0x03,0xf9,0xfd,0x05,0x05,0xfd,0xfd,0x02,0xfe,0xfc,0x01,0x03,0x06,0x04,0x05,0x05,0x05,0x05,0x05,0x05,0x01,0x00};
static const u8 PhoneBeiZhuIco[24]={0x00,0x87,0x67,0x1f,0x18,0x28,0xc8,0x08,0x08,0x88,0x68,0x18,0x06,0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x06,0x01,0x00,0x00};
static const u8 PhoneSaveIco[24]={0xff,0xff,0xc3,0x55,0x55,0x55,0x55,0x55,0xc3,0xff,0xff,0x00,0x07,0x07,0x04,0x04,0x05,0x04,0x04,0x04,0x04,0x07,0x07,0x00};


static const u8 con_Chinese_PhoneNumber[]={"手机号码"},con_Chinese_PhoneName[]={"姓名"},con_Chinese_PhoneZuoJi[]={"座机号码"},con_Chinese_PhoneEmail[]={"邮箱"},con_Chinese_PhoneBeiZhu[]={"备注"},con_Chinese_PhoneSave[]={"保存"};
static const u8 con_English_PhoneNumber[]={"Mobile number"},con_English_PhoneName[]={"Name"},con_English_PhoneZuoJi[]={"Telephone"},con_English_PhoneEmail[]={"E-mail"},con_English_PhoneBeiZhu[]={"Remark"},con_English_PhoneSave[]={"Save"};




#define PHONEFILESIZE		1024	//phone book file size

static const u8 file_phonebook[]={"PhoneBook"};


struct XiuGaiArg{	//修改传递的参数
	char num;	  	//哪一条记录
	char position;	//记录中的哪个项
	char *buff;	  	//修改的数据指针
};

static u8 PhoneAddName(u8 *receive,u16 size)
{
	return(GetString_abcABC123PYWuBi_Language2("【姓名编辑】","Name edit",SRF_PY|SRF_BiHua|SRF_abc|SRF_ABC|SRF_123,SRF_abc,receive,size,1));
}


static u8 PhoneAddNumber(u8 *receive,u16 size)
{
	return(GetString_abcABC123PYWuBi_Language2("【手机号编辑】","Mobile num edit",SRF_123,SRF_123,receive,size,1));
}

static u8 PhoneAddZuoJi(u8 *receive,u16 size)
{
	return(GetString_abcABC123PYWuBi_Language2("【座机号编辑】","Telephone edit",SRF_123,SRF_123,receive,size,1));
}


static u8 PhoneAddEmail(u8 *receive,u16 size)
{
	return(GetString_abcABC123PYWuBi_Language2("【Email编辑】","Email edit",SRF_abc|SRF_ABC|SRF_123,SRF_abc,receive,size,1));
}


static u8 PhoneAddBeiZhu(u8 *receive,u16 size)
{
	return(GetString_abcABC123PYWuBi_Language2("【备注编辑】","remark edit",SRF_PY|SRF_BiHua|SRF_abc|SRF_ABC|SRF_123,SRF_abc,receive,size,1));
}




//删除一个记录
static char PhoneDeleteARecord(u8 Record)
{
	char phonebuff[PHONEFILESIZE],*pphonebuffstart,*phonebuffend;
	char temp[11];
	int totalnum;

	temp[0]=EepromFile_ReadData((unsigned char *)phonebuff,sizeof(phonebuff),file_phonebook);

	memcpy(temp,phonebuff,10);
	temp[10]=0;
	totalnum=atoi(temp);
	if(totalnum)totalnum--;

	pphonebuffstart=phonebuff;
	pphonebuffstart+=10+Record*98;
	phonebuffend=pphonebuffstart+98;

	while(phonebuffend<&phonebuff[PHONEFILESIZE-1]){
		*pphonebuffstart++=*phonebuffend++;
	}

	memset(temp,0,10);
	sprintf(temp,"%d",totalnum);
	memcpy(phonebuff,temp,10);
	temp[0]=EepromFile_WriteData((const unsigned char *)phonebuff,sizeof(phonebuff),file_phonebook);

	if(temp[0]==EepromFile_WriteOk){
		return RIGHT_return;
	}

	return ERROR_return;
}

//按拼音排序保存一个记录
static u8 PhoneAddSave(u8 *receive,u16 size)
{
	char phonebuff[4096],*pphonebuff,*endphonebuff;
	char bufftemp[100];
	struct FunctionMenuAddIco *MenuPhoneAdd;
	int num;
	int i;

	clear_lcd();
	DrawTitle_to_row_Language2(2,"正在保存...","saving...");
	MenuPhoneAdd=(struct FunctionMenuAddIco *)receive;

	EepromFile_ReadData((unsigned char *)phonebuff,sizeof(phonebuff)-1,file_phonebook);

	pphonebuff=phonebuff;
	memset(bufftemp,0,sizeof(bufftemp));
	memcpy(bufftemp,pphonebuff,10);
	pphonebuff+=10;
	num=atoi((sc8*)bufftemp);
	endphonebuff=pphonebuff+num*98;	//endphonebuff  指向最后一条记录后面的空白处
	if(endphonebuff>&phonebuff[sizeof(phonebuff)-200]){
		text_out_to_row_x_Language2(3,40,"电话本已满","Phone book full");
		goto tuichu;
	}

	memset(bufftemp,0,50);	//按拼音次序找到要写入记录的位置
	for(i=0;i<num;i++){
		memcpy(bufftemp,pphonebuff,12);
		if(strcmp(bufftemp,(sc8*)MenuPhoneAdd->MenuStr)>0)break;
		pphonebuff+=98;
	}

	while(endphonebuff>pphonebuff){		//把要写入记录位置处的所有记录向后移一个记录的位置
		memcpy(bufftemp,endphonebuff-98,98);
		memcpy(endphonebuff,bufftemp,98);
		endphonebuff-=98;
	}

	memcpy(pphonebuff,MenuPhoneAdd->MenuStr,12);	//读入要写入记录的信息
	MenuPhoneAdd++;pphonebuff+=12;
	memcpy(pphonebuff,MenuPhoneAdd->MenuStr,18);
	MenuPhoneAdd++;pphonebuff+=18;
	memcpy(pphonebuff,MenuPhoneAdd->MenuStr,18);
	MenuPhoneAdd++;pphonebuff+=18;
	memcpy(pphonebuff,MenuPhoneAdd->MenuStr,20);
	MenuPhoneAdd++;pphonebuff+=20;
	memcpy(pphonebuff,MenuPhoneAdd->MenuStr,30);

	num++;
	memset(bufftemp,0,10);
	sprintf(bufftemp,"%d",num);
	memcpy(phonebuff,bufftemp,10);

	i=EepromFile_WriteData((unsigned char*)phonebuff,num*99+100,file_phonebook);
	if(i==EepromFile_WriteOk){
		DrawTitle_to_row_Language2(3,"电话记录保存成功","save success");
	}else{
		DrawTitle_to_row_Language2(3,"电话记录保存失败","save failure");
	}

	tuichu:
	return (WaitkeyAndAutoExit(10));
}



#define PhoneNumberSize	18
#define PhoneNameSize	12
#define PhoneZuoJiSize	18
#define PhoneEmailSize	20
#define PhoneBeiZhuSize	30
#define PhoneSaveSize	5

/************************************光标在姓名处菜单**********************************************/
static s8 PhoneHandleNameXiuGai(s8 *msg)
{
	static u8 PhoneNumber[18],PhoneName[12],PhoneZuoJi[18],PhoneEmail[20],PhoneBeiZhu[30],PhoneSave[5];
	char temp[50];
	int numPhone;
	EFILE efile;

	static struct FunctionMenuAddIco Menu[6]={
		{PhoneNameIco,	PhoneName,	PhoneNameSize,	PhoneAddName},
		{PhoneNumberIco,PhoneNumber,PhoneNumberSize,PhoneAddNumber},
		{PhoneZuoJiIco,	PhoneZuoJi,	PhoneZuoJiSize,	PhoneAddZuoJi},
		{PhoneEmailIco,	PhoneEmail,	PhoneEmailSize,	PhoneAddEmail},
		{PhoneBeiZhuIco,PhoneBeiZhu,PhoneBeiZhuSize,PhoneAddBeiZhu},
		{PhoneSaveIco,	PhoneSave,	PhoneSaveSize,	PhoneAddSave}
	};
	
	struct OperateAddIco MenuOp={Menu,6,0,0,0,MaxRowOneScr,1,0};

	if(Language[0]==LOCAL_LANGUAGE){
		strcpy((char *)PhoneNumber,(const char *)con_Chinese_PhoneNumber);
		strcpy((char *)PhoneName,(const char *)con_Chinese_PhoneName);
		strcpy((char *)PhoneZuoJi,(const char *)con_Chinese_PhoneZuoJi);
		strcpy((char *)PhoneEmail,(const char *)con_Chinese_PhoneEmail);
		strcpy((char *)PhoneBeiZhu,(const char *)con_Chinese_PhoneBeiZhu);
		strcpy((char *)PhoneSave,(const char *)con_Chinese_PhoneSave);
	}
	else{
		strcpy((char *)PhoneNumber,(const char *)con_English_PhoneNumber);
		strcpy((char *)PhoneName,(const char *)con_English_PhoneName);
		strcpy((char *)PhoneZuoJi,(const char *)con_English_PhoneZuoJi);
		strcpy((char *)PhoneEmail,(const char *)con_English_PhoneEmail);
		strcpy((char *)PhoneBeiZhu,(const char *)con_English_PhoneBeiZhu);
		strcpy((char *)PhoneSave,(const char *)con_English_PhoneSave);
	}

	clear_lcd();
	temp[0]=EepromFile_ReadData((unsigned char *)temp,10,file_phonebook);
	if(temp[0]!=EepromFile_ReadOk){		//如果有记录，则先把记录读到phonebuff 。
		clear_lcd();
		text_out_to_row_x_Language2(2,40,"没有记录","No record");
		return KEY_ReturnPrevious;
	}

	EepromFile_Open(file_phonebook,&efile);
	numPhone=*msg;		//如果msg==NULL，则会引起segmentation  fault。
	EepromFile_Seek(&efile,10+numPhone*98,EepromFile_SEEK_SET);
	EepromFile_read(PhoneName,PhoneNameSize,&efile);
	EepromFile_read(PhoneNumber,PhoneNumberSize,&efile);
	EepromFile_read(PhoneZuoJi,PhoneZuoJiSize,&efile);
	EepromFile_read(PhoneEmail,PhoneEmailSize,&efile);
	EepromFile_read(PhoneBeiZhu,PhoneBeiZhuSize,&efile);

	while(1){
		MenuHandler_AddIco(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			if(MenuOp.Menucnt==MenuOp.MenuTotal-1){
				if(PhoneDeleteARecord(numPhone)==RIGHT_return){
					MenuOp.FucRetVal=MenuOp.Menu->MenuFuc((u8 *)Menu,0);
				}
				
				return MenuOp.FucRetVal;
			}else{
				strcpy(temp,(sc8*)(MenuOp.Menu->MenuStr));

				MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(MenuOp.Menu->MenuStr,MenuOp.Menu->size);

				if(strlen((sc8*)(MenuOp.Menu->MenuStr))==0){
					strcpy((s8*)(MenuOp.Menu->MenuStr),temp);
				}
			}
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return MenuOp.RetVal;
		}
	}

	#undef PhoneNumber
	#undef PhoneName
	#undef PhoneZuoJi
	#undef PhoneEmail
	#undef PhoneBeiZhu
	#undef PhoneSave
}


static char PhoneHandleNameDelete(char *msg)
{
	char temp;

	#ifdef DEBUG_PRINTF
	printf("msg=%d\n",*msg);
	#endif

	temp=PhoneDeleteARecord(*msg);
	clear_lcd();
	if(temp==RIGHT_return){
		text_out_to_row_x_Language2(2,46,"已删除","Deleted");
	}else{
		text_out_to_row_x_Language2(2,40,"删除失败","Delete failure");
	}

	return (WaitkeyAndAutoExit(10));
}


static char PhoneHandleName(char *msg)
{
	static struct MenuFuc_Language2 Menu[2]={
		{"1.修改","1.Edit",PhoneHandleNameXiuGai},		//注意:  第一个字符串的长度不能大于MaxByteOneLine-1
		{"2.删除记录","2.Delete record",PhoneHandleNameDelete},
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

	while(1){
		#ifdef DEBUG_PRINTF
		printf("PhoneHandleName\n");
		#endif
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return MenuOp.RetVal;
		}
	}
}

struct phoneleixing{
	int numPhone;
	char leixin;
};

/************************************光标在手机处菜单**********************************************/
static char PhoneHandleCall(char *msg)
{
	u8 phonebuff[20];
	u8 atcmd[50];
	EFILE efile;
	struct phoneleixing *ph;

	ph=(struct phoneleixing *)msg;

	clear_lcd();
	/*fp=fopen("PhoneBook","rb");
	if(fp!=NULL){		//如果有记录，则先把记录读到phonebuff 。
	#ifdef DEBUG_PRINTF
	printf("open PhoneBook success!!\n");
	#endif
	}else{
	#ifdef DEBUG_PRINTF
	printf("open PhoneBook failure!!\n");
	#endif
	return KEY_ReturnPrevious;
	}*/
	phonebuff[0]=EepromFile_Open(file_phonebook,&efile);

	memset(phonebuff,0,sizeof(phonebuff));
	/*if(ph->leixin==0)fseek(fp,10+ph->numPhone*98+12,SEEK_SET);
	else fseek(fp,10+ph->numPhone*98+12+18,SEEK_SET);
	fread(phonebuff,18,1,fp);*/
	if(ph->leixin==0)EepromFile_Seek(&efile,10+ph->numPhone*98+12,EepromFile_SEEK_SET);
	else EepromFile_Seek(&efile,10+ph->numPhone*98+12+18,EepromFile_SEEK_SET);
	EepromFile_read(phonebuff,18,&efile);

	DrawTitle_to_row_Language2(2,"正在拨号...","Dialing...");
	strcpy((char *)atcmd,"ATD");
	strcat((char *)atcmd,(const char *)phonebuff);
	strcat((char *)atcmd,";\r");
	send_at_cmd(atcmd);
	//text_out_to_row_x_Language2(1,25,phonebuff,phonebuff);
	phonebuff[0]=Waitkey_TillKeyPress();

	send_at_cmd("ATH\r");

	return phonebuff[0];
}
static char PhoneHandleSendmassage(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	char receive[60];

	GetString_abcABC123PYWuBi_Language2("短信输入","Msg input",SRF_123|SRF_BiHua|SRF_ABC|SRF_abc|SRF_PY,SRF_BiHua,receive,sizeof(receive),0);

	clear_lcd();
	DrawTitle_to_row_Language2(2,"正在发送短信","Sending message");
	if(SendMessage(receive,"15112475662")==Sim900B_ProccessOK){
		DrawTitle_to_row_Language2(3,"短信已成功发送","Sended");
	}else{
		DrawTitle_to_row_Language2(3,"短信发送失败","Send failure");
	}

	return WaitkeyAndAutoExit(10);
	#endif
}

static char PhoneHandleSendcolormassage(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	char tempbuff[1024];
	char PhoneNumber[30],time[30],context[512];
	char atcmd[20]={"AT+CMGR="};
	static char k=1;
	char j;


	#ifdef DEBUG_PRINTF
	ReceiveMessageDecode("+CMGR: 1,,154\r0891683108705505F04405A10180F60008016002907294238A0500030002015C0A656C76845BA26237FF1A60A85F53524D5E1062374F59989D003100330034002E003700355143FF0C51764E2D57FA672C5E1062374F59989D003100320039002E003400385143FF0C8D6090015E1062374F59989D0035002E003200375143FF0C4E0B6B2167087ED365E54E3A0031003665E5FF0C67096548671F81F3003200300031\rOK",PhoneNumber,time,context,"+CMGL:");
	#endif


	my_itoa(&atcmd[8],k);k++;
	strcat(atcmd,"\r");
	memset(tempbuff,0,sizeof(tempbuff));
	memset(PhoneNumber,0,PhoneNumberSize);
	memset(time,0,sizeof(time));
	memset(context,0,sizeof(context));
	#ifdef DEBUG_PRINTF
	printf("atcmd=%s\n",atcmd);
	#endif
	send_at_cmd(atcmd);
	if(SendATReadDatasFromSIM300(fd_ttyS1,tempbuff,3,0)==OK){
		ReceiveMessageDecode(tempbuff,PhoneNumber,time,context,"+CMGR:");
	}
	#endif
}

static char PhoneHandleIPCall(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}

static char PhoneHandleSetRefuseNumber(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char PhoneHandleNumber(char *msg)
{
	static struct MenuFuc_Language2 Menu[5]={
		{"1.拨号","1.Dial number",PhoneHandleCall},		//注意:  第一个字符串的长度不能大于MaxByteOneLine-1
		{"2.发送短信","2.Send message",PhoneHandleSendmassage},
		{"3.发送彩信","3.Send color msg",PhoneHandleSendcolormassage},
		{"4.IP拨号","4.Ip Dial",PhoneHandleIPCall},
		{"5.设为拒接电话","5.Set refuse num",PhoneHandleSetRefuseNumber}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		5,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};


	//struct phoneleixing ph;
	//ph.leixin=0;
	//ph.numPhone=*msg;

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return MenuOp.RetVal;
		}
	}
}

/************************************光标在座机处菜单**********************************************/
static char PhoneHandleZuoJi(char *msg)
{
	static struct MenuFuc_Language2 Menu[5]={
		{"1.拨号","1.Dial number",PhoneHandleCall},		//注意:  第一个字符串的长度不能大于MaxByteOneLine-1
		{"2.发送短信","2.Send message",PhoneHandleSendmassage},
		{"3.发送彩信","3.Send color msg",PhoneHandleSendcolormassage},
		{"4.IP拨号","4.Ip Dial",PhoneHandleIPCall},
		{"5.设为拒接电话","5.Set refuse num",PhoneHandleSetRefuseNumber}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		5,
		0,
		0,
		0,
		MaxRowOneScr,
		1,
		0,
		0
	};

	struct phoneleixing ph;
	ph.leixin=1;
	ph.numPhone=*msg;

	while(1){
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc((char *)(&ph));
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return MenuOp.RetVal;
		}
	}
}


/************************************光标在Email  处菜单**********************************************/
static char PhoneHandleEmail(char *msg)
{
	FILE *fp;
	u8 dispbuff[20];
	int numPhone;

	clear_lcd();
	fp=fopen("PhoneBook","rb");
	if(fp!=NULL){		//如果有记录，则先把记录读到phonebuff 。
		#ifdef DEBUG_PRINTF
		printf("open PhoneBook success!!\n");
		#endif
		fread(dispbuff,10,1,fp);
	}else{
		text_out_to_row_x_Language2(2,40,"没有记录","没有记录");
		goto tuichu;
	}

	numPhone=*msg;		//如果msg==NULL，则会引起segmentation  fault。
	fseek(fp,10+numPhone*98+12+18+18,SEEK_SET);
	fread(dispbuff,sizeof(dispbuff),1,fp);
	fclose(fp);

	text_out_to_row_x_Language2(0,36,"【Email】","【Email】");
	my_DisplayBuff(dispbuff,1,2);

	tuichu:
	SetBLOverTime(10);
	SetAutoExitTime(10);
	WaitkeyAndAutoExit(10);
	return 0;
}


/************************************光标在备注处菜单**********************************************/
static char PhoneHandleBeiZhu(char *msg)
{
	FILE *fp;
	u8 dispbuff[30];
	int numPhone;

	clear_lcd();
	fp=fopen("PhoneBook","rb");
	if(fp!=NULL){		//如果有记录，则先把记录读到phonebuff 。
		#ifdef DEBUG_PRINTF
		printf("open PhoneBook success!!\n");
		#endif
		fread(dispbuff,10,1,fp);
	}else{
		text_out_to_row_x_Language2(2,40,"没有记录","没有记录");
		goto tuichu;
	}

	numPhone=*msg;		//如果msg==NULL，则会引起segmentation  fault。
	fseek(fp,10+numPhone*98+12+18+18+20,SEEK_SET);
	fread(dispbuff,sizeof(dispbuff),1,fp);
	fclose(fp);
	
	text_out_to_row_x_Language2(0,40,"【备注】","【备注】");
	my_DisplayBuff(dispbuff,1,2);

	tuichu:
	SetBLOverTime(10);
	SetAutoExitTime(10);
	WaitkeyAndAutoExit(10);
	return 0;
}


/************************************记录详情**********************************************/
static char HandlePhoneSelection(char numPhone)
{
	static u8 PhoneNumber[18],PhoneName[12],PhoneZuoJi[18],PhoneEmail[20],PhoneBeiZhu[30];

	EFILE efile;
	char dispbuff[11];
	u8 retval;
	static struct FunctionMenuAddIco2 Menu[5]={
		{PhoneNameIco,	PhoneName,	PhoneHandleName},
		{PhoneNumberIco,PhoneNumber,PhoneHandleNumber},
		{PhoneZuoJiIco,	PhoneZuoJi,	PhoneHandleZuoJi},
		{PhoneEmailIco,	PhoneEmail,	PhoneHandleEmail},
		{PhoneBeiZhuIco,PhoneBeiZhu,PhoneHandleBeiZhu},
	};
	struct OperateAddIco2 MenuOp={Menu,5,0 ,0,0,MaxRowOneScr, 1,0,0};

	if(Language[0]==LOCAL_LANGUAGE){
		strcpy((char *)PhoneNumber,(const char *)con_Chinese_PhoneNumber);
		strcpy((char *)PhoneName,(const char *)con_Chinese_PhoneName);
		strcpy((char *)PhoneZuoJi,(const char *)con_Chinese_PhoneZuoJi);
		strcpy((char *)PhoneEmail,(const char *)con_Chinese_PhoneEmail);
		strcpy((char *)PhoneBeiZhu,(const char *)con_Chinese_PhoneBeiZhu);
		//strcpy((char *)PhoneSave,(const char *)con_Chinese_PhoneSave);
	}
	else{
		strcpy((char *)PhoneNumber,(const char *)con_English_PhoneNumber);
		strcpy((char *)PhoneName,(const char *)con_English_PhoneName);
		strcpy((char *)PhoneZuoJi,(const char *)con_English_PhoneZuoJi);
		strcpy((char *)PhoneEmail,(const char *)con_English_PhoneEmail);
		strcpy((char *)PhoneBeiZhu,(const char *)con_English_PhoneBeiZhu);
		//strcpy((char *)PhoneSave,(const char *)con_English_PhoneSave);
	}

	loop:
	retval=EepromFile_ReadData((unsigned char *)dispbuff,10,file_phonebook);
	if(retval!=EepromFile_ReadOk){		//如果有记录，则先把记录读到phonebuff 。
		clear_lcd();
		text_out_to_row_x_Language2(2,40,"没有记录","No record");
		return WaitkeyAndAutoExit(10);
	}

	retval=EepromFile_Open(file_phonebook,&efile);

	EepromFile_Seek(&efile,10+numPhone*98,EepromFile_SEEK_SET);
	EepromFile_read(PhoneName,PhoneNameSize,&efile);
	EepromFile_read(PhoneNumber,PhoneNumberSize,&efile);
	EepromFile_read(PhoneZuoJi,PhoneZuoJiSize,&efile);
	EepromFile_read(PhoneEmail,PhoneEmailSize,&efile);
	EepromFile_read(PhoneBeiZhu,PhoneBeiZhuSize,&efile);

	while(1){
		MenuHandler_AddIco2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(&numPhone);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;}  
			else{
				clear_lcd();
				MenuOp.flashflag=1;
				goto loop;
			}
		}else{
			return MenuOp.RetVal;
		}
	}

	#undef PhoneNumber
	#undef PhoneName
	#undef PhoneZuoJi
	#undef PhoneEmail
	#undef PhoneBeiZhu
	//#undef PhoneSave
}



/*********************************浏览*************************************/

void my_OperateDisplay_KEY_UP(struct OperateDisplay *MenuOp)
{
	if((*MenuOp).Menucnt){		//未到第一个菜单
		(*MenuOp).Menucnt--;
		if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;}
	}else{		//到了第一个菜单，下一个菜单是最后一个。
		(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
		(*MenuOp).curFlashRow=(*MenuOp).EndLine<(*MenuOp).MenuTotal+(*MenuOp).StartLine ? (*MenuOp).EndLine-1 : (*MenuOp).MenuTotal+(*MenuOp).StartLine-1;
	}
	(*MenuOp).flashflag=1;
}



void my_OperateDisplay_KEY_DOWN(struct OperateDisplay *MenuOp)
{
	if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1){		//未到最后一个菜单
		(*MenuOp).Menucnt++;
		if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;}
	}else{		//到了最后一个菜单，下一个菜单是第一个。
		(*MenuOp).Menucnt=0;
		(*MenuOp).curFlashRow=(*MenuOp).StartLine;
	}
	(*MenuOp).flashflag=1;
}


static char PhoneExplorer(char *msg)
{
	EFILE efile;
	u8 dispbuff[50];
	int i;
	struct OperateDisplay disp={0,0, 0,0,MaxRowOneScr ,1};
	unsigned char keyval;

	loop:
	clear_lcd();
	keyval=EepromFile_ReadData(dispbuff,10,file_phonebook);
	if(keyval==EepromFile_ReadOk){		//如果有记录，则先把记录读到phonebuff 。
		disp.MenuTotal=atoi((sc8*)dispbuff);
		if(disp.MenuTotal==0){
			text_out_to_row_x_Language2(40,2,"没有记录","No record");
			goto tuichu;
		}
	}else{
		text_out_to_row_x_Language2(2,40,"没有记录","No record");
		goto tuichu;
	}

	keyval=EepromFile_Open(file_phonebook,&efile);

	while(1){
		if(disp.flashflag){
			disp.flashflag=0;
			
			for(i=disp.StartLine;i<disp.EndLine&&i<disp.MenuTotal+disp.StartLine;i++){
				EepromFile_Seek(&efile,10+(disp.Menucnt-disp.curFlashRow+i)*98,EepromFile_SEEK_SET);
				EepromFile_read(dispbuff,12,&efile);
				steaddraw_pic_to_row_x(PhoneNameIco,0,i,12,12);
				text_out_to_row_x_fillSpace(i,16,dispbuff);
			}

			DispHighLightRow_to_x(16,disp.curFlashRow);			//高亮
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue;

		KeyEventPost();
		switch(keyval){ 
			case KEY_UP:
			my_OperateDisplay_KEY_UP(&disp);
			break;

			case KEY_DOWN:
			my_OperateDisplay_KEY_DOWN(&disp);
			break;

			case KEY_Enter:
			case KEY_RIGHT:
			if(HandlePhoneSelection(disp.Menucnt)==KEY_ReturnInitInterface){return KEY_ReturnInitInterface;}
			disp.flashflag=1;
			//fclose(fp);
			goto loop;

			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
			//fclose(fp);
			return keyval;
		}

	}

	tuichu:
	return (WaitkeyAndAutoExit(10));
}


/*********************************查找*************************************/
//static char PhoneSearch(char *msg)
char PhoneSearch(char *msg)
{
	EFILE efile;
	u8 receive[12];
	u8 tempbuff[50];
	u8 dispbuff[50];
	int i;
	struct OperateDisplay disp={0,0, 0,0,MaxRowOneScr ,1};
	unsigned char retval;
	unsigned char keyval;

	retval=GetString_abcABC123PYWuBi_Language2("【输入姓名】","Input name",SRF_PY|SRF_BiHua|SRF_abc|SRF_ABC|SRF_123,SRF_abc,receive,sizeof(receive),0);
	if(retval!=KEY_Enter)return retval;
	if(strlen((sc8*)receive)==0)goto tuichu;

	loop:
	clear_lcd();
	//fp=fopen("PhoneBook","rb");
	retval=EepromFile_Open(file_phonebook,&efile);
	retval=EepromFile_ReadData(tempbuff,10,file_phonebook);
	if(retval==EepromFile_ReadOk){		//如果有记录，则先把记录读到phonebuff 。
		disp.MenuTotal=atoi((sc8*)tempbuff);
	}else{
		DrawTitle_to_row_Language2(2,"没有记录","No record");
		goto tuichu;
	}

	retval=EepromFile_Open(file_phonebook,&efile);
	for(i=0;i<disp.MenuTotal;i++){
		EepromFile_Seek(&efile,10+i*98,EepromFile_SEEK_SET);
		EepromFile_read(dispbuff,12,&efile);
		if(my_FFstrstr((sc8*)dispbuff,(sc8*)receive)==1){
			disp.Menucnt=i;
			if(disp.Menucnt<disp.EndLine-disp.StartLine){
				disp.curFlashRow+=disp.Menucnt;
			}else{
				disp.curFlashRow=disp.EndLine-1;
			}
			break;
		}
	}

	if(i>=disp.MenuTotal){DrawTitle_to_row_Language2(2,"找不到此记录","Can't find record");goto tuichu;}

	while(1){
		if(disp.flashflag){
			disp.flashflag=0;

			for(i=disp.StartLine;i<disp.EndLine&&i<disp.MenuTotal+disp.StartLine;i++){
				//fseek(fp,10+(disp.Menucnt-disp.curFlashRow+i)*98,SEEK_SET);
				//fread(dispbuff,12,1,fp);
				EepromFile_Seek(&efile,10+(disp.Menucnt-disp.curFlashRow+i)*98,EepromFile_SEEK_SET);
				EepromFile_read(dispbuff,12,&efile);
				steaddraw_pic_to_row_x(PhoneNameIco,0,i,12,12);
				text_out_to_row_x_fillSpace(i,16,dispbuff);
			}

			DispHighLightRow_to_x(16,disp.curFlashRow);			//高亮
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue;
		KeyEventPost();

		switch(keyval){ 
			case KEY_UP:
			my_OperateDisplay_KEY_UP(&disp);
			break;

			case KEY_DOWN:
			my_OperateDisplay_KEY_DOWN(&disp);
			break;

			case KEY_Enter:
			case KEY_RIGHT:
			if(HandlePhoneSelection(disp.Menucnt)==KEY_ReturnInitInterface){
				//fclose(fp);
				return KEY_ReturnInitInterface;
			}
			disp.flashflag=1;
			disp.curFlashRow=0;
			//fclose(fp);
			goto loop;

			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
			//fclose(fp);
			return keyval;
		}

	}

	tuichu:
	WaitkeyAndAutoExit(10);
	return 0;
}



/*********************************增加*************************************/
static char PhoneAdd(char *msg)
{
	static u8 PhoneNumber[18],PhoneName[12],PhoneZuoJi[18],PhoneEmail[20],PhoneBeiZhu[30],PhoneSave[5];

static struct FunctionMenuAddIco Menu[6]={
		{PhoneNameIco,	PhoneName,	PhoneNameSize,	PhoneAddName},
		{PhoneNumberIco,PhoneNumber,PhoneNumberSize,PhoneAddNumber},
		{PhoneZuoJiIco,	PhoneZuoJi,	PhoneZuoJiSize,	PhoneAddZuoJi},
		{PhoneEmailIco,	PhoneEmail,	PhoneEmailSize,	PhoneAddEmail},
		{PhoneBeiZhuIco,PhoneBeiZhu,PhoneBeiZhuSize,PhoneAddBeiZhu},
		{PhoneSaveIco,	PhoneSave,	PhoneSaveSize,	PhoneAddSave}
	}; 
	
	struct OperateAddIco MenuOp={Menu,6,0, 0,0,MaxRowOneScr, 1,0,0};

	char temp[25];
	
	if(Language[0]==LOCAL_LANGUAGE){
		strcpy((char *)PhoneNumber,(const char *)con_Chinese_PhoneNumber);
		strcpy((char *)PhoneName,(const char *)con_Chinese_PhoneName);
		strcpy((char *)PhoneZuoJi,(const char *)con_Chinese_PhoneZuoJi);
		strcpy((char *)PhoneEmail,(const char *)con_Chinese_PhoneEmail);
		strcpy((char *)PhoneBeiZhu,(const char *)con_Chinese_PhoneBeiZhu);
		strcpy((char *)PhoneSave,(const char *)con_Chinese_PhoneSave);
	}
	else{
		strcpy((char *)PhoneNumber,(const char *)con_English_PhoneNumber);
		strcpy((char *)PhoneName,(const char *)con_English_PhoneName);
		strcpy((char *)PhoneZuoJi,(const char *)con_English_PhoneZuoJi);
		strcpy((char *)PhoneEmail,(const char *)con_English_PhoneEmail);
		strcpy((char *)PhoneBeiZhu,(const char *)con_English_PhoneBeiZhu);
		strcpy((char *)PhoneSave,(const char *)con_English_PhoneSave);
	}

	while(1){
		MenuHandler_AddIco(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			if(MenuOp.Menucnt==MenuOp.MenuTotal-1){
				MenuOp.FucRetVal=MenuOp.Menu->MenuFuc((u8 *)Menu,0);
				return MenuOp.FucRetVal;
			}else{
				strcpy(temp,(sc8*)(MenuOp.Menu->MenuStr));
				memset(MenuOp.Menu->MenuStr,0,10);

				MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(MenuOp.Menu->MenuStr,MenuOp.Menu->size);

				if(strlen((sc8*)(MenuOp.Menu->MenuStr))==0){
					strcpy((s8*)(MenuOp.Menu->MenuStr),temp);
				}
			}
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else{clear_lcd();MenuOp.flashflag=1;}
		}else{
			return MenuOp.RetVal;
		}
	}

	#undef PhoneNumber
	#undef PhoneName
	#undef PhoneZuoJi
	#undef PhoneEmail
	#undef PhoneBeiZhu
	#undef PhoneSave
}


/******************************全部删除************************************/
static char PhoneDeleteAll(char *msg)
{
	char retval;

	clear_lcd();
	DrawTitle_to_row_Language2(2,"确定全部删除吗","Confirm Delete");
	retval=WaitkeyAndAutoExit(10);
	if(retval==KEY_Enter){
		//system("rm PhoneBook");
		EepromFile_Delete(file_phonebook);
		clear_lcd();
		DrawTitle_to_row_Language2(2,"删除成功","Deleted");
		retval=WaitkeyAndAutoExit(10);
	}

	return retval;
}


char MainMenuScreen_ToolBox_PhoneBook(char *msg)
{
	static struct MenuFuc_Language2 Menu[4]={
		{"1.浏览","1.Explorer",PhoneExplorer},
		{"2.查找","2.Search",PhoneSearch},
		{"3.增加","3.Add",PhoneAdd},
		{"4.全部删除","4.DeleteAll",PhoneDeleteAll},
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		4,
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



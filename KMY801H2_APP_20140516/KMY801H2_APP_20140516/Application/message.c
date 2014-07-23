
#include "message.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "key.h"
#include "main.h"
#include "lcd.h"
#include "gprs.h"
#include "FileOperate.h"
#include "sim900B.h"
#include "MenuOperate.h"
#include "mypure_library.h"
#include "my_library.h"
#include "PhoneBook.h"
#include "global_extern_variable.h"
#include "setting.h"
#include "kmy_USART3Drv.h"
#include "kmy_Time.h"
#include "ShuRuFa.h"



//-------------------------------------����-------------------------------------------
static char WriteMessage(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}



static void DisplayMessage(char *msg)
{
	char atcmd[20]={"AT+CMGR="};
	char buff[2048];
	char PhoneNumber[30],time[30],context[1024];

	my_itoa(&atcmd[8],*msg);
	strcat(atcmd,"\r");
	send_at_cmd((unsigned char*)atcmd);
	memset(buff,0,sizeof(buff));
	if(ReadDatasFromSIM300_OKstr((unsigned char*)buff,sizeof(buff),3,"OK\r")==0x09){
		if(ReceiveMessageDecode(buff,PhoneNumber,time,context,"+CMGR:")==0x09){
			DrawTitle_to_row_Language2(0,"���������ݡ�","Msg context");
			memcpy(buff,&time[5],11);
			memset(time,0,sizeof(time));
			memcpy(time,buff,11);
			strcpy(buff,PhoneNumber);
			strcat(buff,"\n");
			strcat(buff,time);
			strcat(buff,"\n");
			strcat(buff,context);
			my_DisplayBuff((unsigned char*)buff,1,MaxRowOneScr);
			return;
		}
	}
}



void GetMessageTotalAndCapacity(unsigned char *Total,unsigned char *Capacity)
{
	unsigned char buff[60];
	char *p;

	*Capacity=*Total=0;
	strcpy((char *)buff,"AT+CPMS?\r");
	if(send_at_cmd_and_receive(buff,sizeof(buff),200,"OK")==TCP_OK){
		p=strstr((char *)buff,"+CPMS:");if(p==NULL)return;
		p=strstr(p,",");if(p==NULL)return;
		p++;
		if(Total!=NULL)*Total=atoi(p);

		p=strstr(p,",");if(p==NULL)return;
		p++;
		if(Capacity!=NULL)*Capacity=atoi(p);
	}
}



//+CMGR: "REC READ","10086","","12/02/25,10:02:19+32"
//740666F4591A79FB52A84E1A52A1FF0C8BF74F7F7528624B673A0063006D00770061007063A5516570B9514D8D39767B5F55005700410050670D52A153850020007700610070002E00670064002E00310030003000380036002E0063006E30024E2D56FD79FB52A85E7F4E1C516C53F83002

//OK





unsigned char GetMessage_state_number_datetime(unsigned char *buff,unsigned char *state,unsigned char *number,unsigned char *datetime)
{
	char *p;

	p=(char *)buff;
	p=strstr((char *)buff,"\"");
	if(p==NULL)return GET_ERROR;
	p++;
	buff=(unsigned char *)p;
	if(state!=NULL){
		p=strstr(p,"REC READ");
		if(p==NULL){*state=MESSAGE_UNREAD;}
		else{*state=MESSAGE_READED;}
	}

	p=strstr((char *)buff,"\",\"");if(p==NULL)return GET_ERROR;
	p+=3;
	if(number!=NULL){
		my_strcpy_EncounterHexEnd(number,(unsigned char *)p,20,'\"');
		//printf("number=%s\r\n",number);
	}

	p=strstr(p,"\",\"");if(p==NULL)return GET_ERROR;
	p+=3;

	p=strstr(p,"\",\"");if(p==NULL)return GET_ERROR;
	p+=3;
	if(datetime!=NULL){
		my_strcpy_EncounterHexEnd(datetime,(unsigned char *)p,20,'\"');
		//printf("datetime=%s\r\n",datetime);
	}

	return GET_OK;
}



unsigned char CheckThisIndexMsgReaded(struct sMessageNode *p_msgNode,unsigned char index,unsigned char totoalreaded)
{
	unsigned char i;

	for(i=0;i<totoalreaded;i++){
		if(p_msgNode->index==index){
			return MESSAGE_READED;
		}
		p_msgNode++;
	}

	return MESSAGE_UNREAD;
}



//�������ܣ��õ����ж���Ϣ�ڵ�
//����������
//		1. p_msgNode: ����Ϣ�ڵ��Žṹ��ָ��
//		2. Total: ����Ϣ����
//		3. Capacity: ����Ϣ����
//		4. readed: �Ѷ�����Ϣ����
//����ֵ����
//ע�����
//		1. 
//		2. 
//		3. 
void GetMessageNode(struct sMessageNode *p_msgNode,unsigned char Total,unsigned char Capacity,unsigned char readed)
{
	unsigned char i;
	unsigned char retval;
	char buff[1024];
	char temp[4];
	unsigned char NeedRead;
	unsigned char datetime[20];
	struct sMessageNode *pm;

	NeedRead=Total-readed;
	pm=p_msgNode+readed;
	for(i=0;i<Capacity;i++){
		if(CheckThisIndexMsgReaded(p_msgNode,i+1,readed)==MESSAGE_READED)continue;
		strcpy(buff,"AT+CMGR=");
		my_itoa(temp,i+1);
		strcat(buff,temp);
		strcat(buff,",1\r");

		if(send_at_cmd_and_receive((unsigned char *)buff,sizeof(buff),3000,"OK\r")==TCP_OK){
			retval=GetMessage_state_number_datetime((unsigned char *)buff,&pm->readflag,pm->Number,datetime);
			if(retval==GET_OK){
				pm->datetime=0;
				pm->index=i+1;
				NeedRead--;pm++;
				if(NeedRead==0)break;
			}
		}
	}
}


void GetOneScreenMessageNumber(unsigned char OneScreenDisplayNumberBuff[4][21],unsigned char *MessageIndex,unsigned char total)
{
	unsigned char i;
	char buff[500];
	char temp[4];
	//unsigned char state;
	//unsigned char datetime[20];

	for(i=0;i<4&&i<total;i++){
		strcat(buff,"AT+CMGR=");
		my_itoa(temp,*MessageIndex);
		MessageIndex++;
		strcat(buff,temp);
		strcat(buff,",1\r");

		if(send_at_cmd_and_receive((unsigned char *)buff,sizeof(buff),3000,"OK\r")==TCP_OK){
			GetMessage_state_number_datetime((unsigned char *)buff,NULL,OneScreenDisplayNumberBuff[i],NULL);
		}
	}
}



#include "my_menuop.h"
#include "ucs2_to_GB2312.h"


#if 0

#define DELETE_ALL			0x01
#define DELETE_ONLY_ONE 	0x02




#define MESSAGE_DELETE_OK			0x01
#define MESSAGE_DELETE_ERROR		0x02


unsigned char Message_delete(unsigned char index,unsigned char delet_flag)
{
	unsigned char buff[50];
	unsigned char temp[4];

	strcat(buff,"AT+CMGD=");
	my_itoa(temp,index);
	strcat(buff,temp);

	if(delet_flag==DELETE_ONLY_ONE){
		strcat(buff,"\r");
	}
	else if(delet_flag==DELETE_ALL){
		strcat(buff,",4\r");
	}
	else{
		return MESSAGE_DELETE_ERROR;
	}

	if(send_at_cmd_and_return(buff,3000,"OK","ERROR")==TCP_OK){
		return MESSAGE_DELETE_OK;
	}

	return MESSAGE_DELETE_ERROR;
}

#endif



char Message_delete_this(char *msg)
{
	struct sMessageNode *pm;
	char buff[50];
	char temp[4];

	pm=(struct sMessageNode *)msg;

	strcat(buff,"AT+CMGD=");
	my_itoa(temp,pm->index);
	strcat(buff,temp);
	strcat(buff,"\r");

	if(send_at_cmd_and_return((unsigned char *)buff,3000,"OK","ERROR")==TCP_OK){
		pm->readflag|=MESSAGE_DELETE_ONE;
	}

	return 0;
}


char Message_delete_all(char *msg)
{
	struct sMessageNode *pm;
	char buff[50];
	char temp[4];

	pm=(struct sMessageNode *)msg;

	strcat(buff,"AT+CMGD=");
	my_itoa(temp,pm->index);
	strcat(buff,temp);
	strcat(buff,",4\r");

	if(send_at_cmd_and_return((unsigned char *)buff,3000,"OK","ERROR")==TCP_OK){
		pm->readflag|=MESSAGE_DELETE_ALL;
	}

	return 0;
}


char MessageDelete(char *msg)
{
	static struct MenuFuc_Language2 Menu[5]={
		{"1.ɾ������","1.delete this",Message_delete_this},
		{"2.ɾ������","2.delete all",Message_delete_all},
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
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter){
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			CurProc=CurProc_Return;
			return MenuOp.FucRetVal;
		}else{
			CurProc=CurProc_Return;
			return MenuOp.RetVal;
		}
	}
}


char MessageDisplayContext(struct sMessageNode *pm)
{
	char *p,*p2;
	unsigned char buff[500];
	unsigned char temp[4];
	unsigned char datetime[20];
	unsigned char Number[20];
	unsigned char retval;

	clear_lcd();
	DrawTitle_to_row(0,"msg context");

	strcpy((char *)buff,"AT+CMGR=");
	my_itoa((char *)temp,pm->index);
	strcat((char *)buff,(char *)temp);
	strcat((char*)buff,",0\r");

	if(send_at_cmd_and_receive(buff,sizeof(buff),3000,"OK\r")!=TCP_OK)return 0;

	GetMessage_state_number_datetime(buff,&pm->readflag,Number,datetime);

	pm->readflag=MESSAGE_READED;

	p=strstr((char*)buff,"CMGR");
	if(p==NULL)return 0;
	p=strstr(p,"\x0d\x0a");
	if(p==NULL)return 0;
	p+=2;

	p2=strstr(p,"\x0d\x0a");
	if(p2==NULL)return 0;

	my_buff_TwoAscciiToOneHex((unsigned char *)p,(unsigned int)(p2-p));
	ucs2str_to_gb2312str((unsigned char *)p,(unsigned int)(p2-p)/2,buff);

	my_string_insert(buff,"\n");
	my_string_insert(buff,datetime);

	my_string_insert(buff,"\n");
	my_string_insert(buff,Number);

	retval=my_DisplayBuff(buff,1,MaxRowOneScr);

	return retval;
}




const unsigned char MessageIco_unread[]=
{
	/*------------------------------------------------------------------------------
	;  ���������룬������ģ��ʽ���ã�ע��ѡ����ȷ��ȡģ������ֽ�λ˳��
	;  Դ�ļ� / ���� : ��ģ
	;  ����ߣ����أ�: 12��12
	;  ��ģ��ʽ/��С : ��ɫ����Һ����ģ������ȡģ���ֽ�����/24�ֽ�
	;  ����ת������  : 2012-2-29 10:47:09
	------------------------------------------------------------------------------*/
	//0x3F,0x30,0x28,0x25,0x22,0x22,0x22,0x25,0x28,0x30,0x3F,0x00,0xE0,0x60,0xA0,0x20,
	//0x20,0x20,0x20,0x20,0xA0,0x60,0xE0,0x00,
	0xfc,0x0c,0x14,0xa4,0x44,0x44,0x44,0xa4,0x14,0x0c,0xfc,0x00,0x07,0x06,0x05,0x04,
	0x04,0x04,0x04,0x04,0x05,0x06,0x07,0x00,
};


const unsigned char MessageIco_readed[]=
{
	/*------------------------------------------------------------------------------
	;  ���������룬������ģ��ʽ���ã�ע��ѡ����ȷ��ȡģ������ֽ�λ˳��
	;  Դ�ļ� / ���� : C:\Documents and Settings\Administrator\����\message_readed.bmp��ģ
	;  ����ߣ����أ�: 12��12
	;  ��ģ��ʽ/��С : ��ɫ����Һ����ģ������ȡģ���ֽ�����/24�ֽ�
	;  ����ת������  : 2012-2-29 10:56:03
	------------------------------------------------------------------------------*/
	//0x0F,0x78,0x48,0x45,0x42,0x42,0x42,0x45,0x48,0x78,0x0F,0x00,0xE0,0x60,0xA0,0x20,
	//0x20,0x20,0x20,0x20,0xA0,0x60,0xE0,0x00,
	0xf0,0x1e,0x12,0xa2,0x42,0x42,0x42,0xa2,0x12,0x1e,0xf0,0x00,0x07,0x06,0x05,0x04,
	0x04,0x04,0x04,0x04,0x05,0x06,0x07,0x00,
};



char ReceiveMailBox(char *msg)//�ռ���
{
	unsigned char i,keyval;
	unsigned char dispbuffLocal[MaxByteOneLine+2];
	struct my_MenuOp op;

	static struct sMessageNode Msg[MAX_MESSAGE_NUMBER];
	static unsigned char messagetotal;

	if(NetworkSwitch[0]==WiFi)
	{
	    clear_lcd();
		DrawTitle_to_row_Language2 (1, "��ǰ����Ϊ", "Current network is");
		DrawTitle_to_row_Language2 (2, "WiFi���л�", "WiFi,Please change to");
		DrawTitle_to_row_Language2 (3, "ΪGPRS�����", "GPRS before operation");
		WaitkeyAndAutoExit (10);
	    return 0;
	}

	if(NetworkSwitch[0]==WiFi)
	{
	    clear_lcd();
		DrawTitle_to_row_Language2 (1, "��ǰ����Ϊ", "Current network is");
		DrawTitle_to_row_Language2 (2, "WiFi���л�", "WiFi,Please change to");
		DrawTitle_to_row_Language2 (3, "ΪGPRS�����", "GPRS before operation");
		WaitkeyAndAutoExit (10);
	    return 0;
	}

	clear_lcd();
	DrawTitle_to_row_Language2(2,"���ڶ�ȡ","reading");
	DrawTitle_to_row_Language2(3,"���Ժ�","please wait...");

	GetMessageTotalAndCapacity(&op.MenuTotal,&keyval);
	printf("op.MenuTotal=%d,keyval=%d\n",op.MenuTotal,keyval);
	if(op.MenuTotal==0||keyval==0||keyval<op.MenuTotal){
		clear_lcd();
		DrawTitle_to_row_Language2(0,"����Ϣ","No Message");
		return WaitkeyAndAutoExit(10);
	}

	if(op.MenuTotal>MAX_MESSAGE_NUMBER){
		printf("Messages is full, please delete some =%d\n",op.MenuTotal);
		op.MenuTotal=MAX_MESSAGE_NUMBER;
	}

	if(op.MenuTotal!=messagetotal){
		GetMessageNode(&Msg[0],op.MenuTotal,keyval,messagetotal);
		messagetotal=op.MenuTotal;
	}

	//GetOneScreenMessageNumber(OneScreenDisplayNumberBuff,MessageIndex,op.MenuTotal);

	//op.MenuTotal;
	op.Menucnt=0;
	op.oldFlashRow=op.curFlashRow=op.StartLine=1;
	op.EndLine=MaxRowOneScr;
	op.flashflag=FlushAllFlag;
	clear_lcd();
	DrawTitle_to_row_Language2(0,"����Ϣ","Message");
	while(1){
		if((op.flashflag&FlushOnlyHightRowFlag)!=0){
			//printf("op.oldFlashRow=%d, op.curFlashRow=%d\n",op.oldFlashRow,op.curFlashRow);
			DispHighLightRow_to_x_width(op.oldFlashRow,16,128-16-12);
			DispHighLightRow_to_x_width(op.curFlashRow,16,128-16-12);
			op.flashflag&=~FlushOnlyHightRowFlag;

			sprintf((char *)dispbuffLocal,"%d/%d  ",op.Menucnt+1,op.MenuTotal);
			my_DisplayMenuPercent(&op);
			text_out_to_row_x(0,128-6*6,dispbuffLocal);
		}
		else if((op.flashflag&FlushAllFlag)!=0){
			//printf("op.flashflag=%d\n",op.flashflag);
			for(i=op.StartLine;i<op.EndLine&&i<op.MenuTotal+op.StartLine;i++){
				if(Msg[op.Menucnt-op.curFlashRow+i].readflag==MESSAGE_UNREAD){steaddraw_pic_to_row_x(MessageIco_unread,0,i,12,12);}
				else {steaddraw_pic_to_row_x(MessageIco_readed,0,i,12,12);}
				my_strcpy_fillspace_bytes(dispbuffLocal,Msg[op.Menucnt-op.curFlashRow+i].Number,17);
				text_out_to_row_x(i,16,dispbuffLocal);
				if(op.Menucnt-op.curFlashRow+i==op.Menucnt+op.MenuTotal-1){
					for(i++;i<op.EndLine&&i<op.MenuTotal+op.StartLine;i++){
						my_strcpy_fillspace(dispbuffLocal," ");
						text_out_to_row(i,dispbuffLocal);
					}
					break;
				}
			}

			sprintf((char *)dispbuffLocal,"%d/%d  ",op.Menucnt+1,op.MenuTotal);
			text_out_to_row_x(0,128-6*6,dispbuffLocal);
			DispHighLightRow_to_x_width(op.curFlashRow,16,128-16-12);
			my_DisplayMenuPercent(&op);
			op.flashflag=0;
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE)continue;

		KeyEventPost();
		switch(keyval){
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
			
			break;

			case KEY_UP:
			MenuOperate_response_keyUp(&op);
			break;

			case KEY_DOWN:
			MenuOperate_response_keyDown(&op);
			break;

			case KEY_RIGHT:
			MenuOperate_response_keyRight(&op);
			break;

			case KEY_LEFT:
			MenuOperate_response_keyLeft(&op);
			break;

			case KEY_Enter:
			keyval=MessageDisplayContext(&Msg[op.Menucnt]);

			if(keyval==KEY_Enter||keyval==KEY_DEL){
				keyval=MessageDelete((char*)(&Msg[op.Menucnt]));
			}

			//����ɹ�ɾ���˱�������readflag
			//����MESSAGE_DELETE_ONE  ��־
			//�ɹ�ɾ�������ж��Ż���MESSAGE_DELETE_ALL ��־
			if(Msg[op.Menucnt].readflag&MESSAGE_DELETE_ONE){	
				for(i=op.Menucnt;i<op.MenuTotal;i++){
					memcpy(&Msg[i],&Msg[i+1],sizeof(struct sMessageNode));
				}

				op.MenuTotal--;
				if(op.Menucnt>=op.MenuTotal)op.Menucnt=op.MenuTotal-1;
				printf("op.curFlashRow=%d\n",op.curFlashRow);
				printf("op.MenuTotal=%d\n",op.MenuTotal);
				
				if(op.curFlashRow>op.MenuTotal-1+op.StartLine)op.curFlashRow=op.MenuTotal-1+op.StartLine;
				messagetotal--;
				if(messagetotal==0)return KEY_ReturnPrevious;
				op.flashflag|=FlushAllFlag;
			}
			else if(Msg[op.Menucnt].readflag&MESSAGE_DELETE_ALL){
				messagetotal=0;
				return KEY_ReturnPrevious;
			}

			clear_lcd();
			DrawTitle_to_row_Language2(0,"����Ϣ","Message");
			op.flashflag|=FlushAllFlag;
			break;

			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
			return keyval;
		}
	}
}


static char MobileQQ(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char SendedMessage(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}

static char SendedBox(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}



static char DraftBox(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char MessageSet(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char MessageCapacity(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}



//-------------------------------------����-------------------------------------------
static char ColorWriteMessage(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}

static char ColorReceiveMailBox(char *msg)//�ռ���
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char ColorSendedMessage(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}

static char ColorSendedBox(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}



static char ColorDraftBox(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char ColorMessageSet(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char ColorMessageCapacity(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}

//-------------------------------------С���㲥-------------------------------------------
static char BroadCastRead(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char BroadCastStart(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}



static char BroadCastChannelSet(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char BroadCastLanguageSelect(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


//-------------------------------------��������-------------------------------------------
static char VoiceBoxReceive(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}


static char VoiceBoxNumberSet(char *msg)
{
	#if 1
	return(abeyant(NULL));
	#else
	
	#endif
}



static char MainMenuScreen_Message_ShortMessage(char *msg)
{
	static struct MenuFuc_Language2 Menu[8]={
		{"1.д����","1.Write Massage",WriteMessage},
		{"2.�ռ���","2.Receive box",ReceiveMailBox},
		{"3.�ƶ�QQ","3.Mobiel QQ",MobileQQ},
		{"4.�ѷ���Ϣ","4.Sended massage",SendedMessage},
		{"5.������","5.Sended box",SendedBox},
		{"6.�ݸ���","6.Draft box",DraftBox},
		{"7.��������","7.Massage set",MessageSet},
		{"8.����","8.Capacity",MessageCapacity},
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		8,
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
static char MainMenuScreen_Message_ColorMessage(char *msg)
{
	static struct MenuFuc_Language2 Menu[7]={
		{"1.д����","1.Write Massage",ColorWriteMessage},
		{"2.�ռ���","2.Receive box",ColorReceiveMailBox},
		{"3.�ѷ���Ϣ","3.Sended massage",ColorSendedMessage},
		{"4.������","4.Sended box",ColorSendedBox},
		{"5.�ݸ���","5.Draft box",ColorDraftBox},
		{"6.��������","6.Massage set",ColorMessageSet},
		{"7.����","7.Capacity",ColorMessageCapacity}
	};
	struct MenuOperate_Language2 MenuOp={
		Menu,
		7,
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

static char MainMenuScreen_Message_XiaoquGuanbo(char *msg)
{
	static struct MenuFuc_Language2 Menu[4]={
		{"1.�Ķ�","1.�Ķ�",BroadCastRead},
		{"2.�����㲥","2.�����㲥",BroadCastStart},
		{"3.�ŵ�����","3.�ŵ�����",BroadCastChannelSet},
		{"4.����ѡ��","4.����ѡ��",BroadCastLanguageSelect}
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

static char MainMenuScreen_Message_VoiceMailbox(char *msg)
{
	static struct MenuFuc_Language2 Menu[2]={
		{"1.������Ϣ","1.������Ϣ",VoiceBoxReceive},
		{"2.��������","2.��������",VoiceBoxNumberSet}
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

char MainMenuScreen_Message(char *msg)
{
	static struct MenuFuc_Language2 Menu[4]={
		{"1.����","1.����",MainMenuScreen_Message_ShortMessage},
		{"2.����","2.����",MainMenuScreen_Message_ColorMessage},
		{"3.С���㲥","3.С���㲥",MainMenuScreen_Message_XiaoquGuanbo},
		{"4.��������","4.��������",MainMenuScreen_Message_VoiceMailbox}
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

//��⵽�¶��ź�ִ�е����ݣ��ɸģ�
//����������һ����Ƶ������ʾ
//Ҳ��������Ļ����ʾ�¶�����ʾͼ��
char NewMessageAct(void)
{
	char buff[1024];
	char *p;
	char totalM;
	unsigned char keyval;

	clear_lcd();
	DrawTitle_to_row_Language2(2,"����Ϣ","new message");
	

	SetAutoExitTime(100);
	loop:
	keyval=GetKeyValue();
	while(keyval == KEY_NONE)goto loop;
	KeyEventPost();
	if(keyval==KEY_Enter){
		memset(buff,0,sizeof(buff));		//��ȡ��������
		send_at_cmd("AT+CPMS?\r");
		if(ReadDatasFromSIM300_OKstr((unsigned char *)buff,sizeof(buff),1,"OK\r")!=0)return 0;		//���ͳɹ���ظ�+CPMS: "SM",21,50,"SM",21,50,"SM",21,50
		if((p=strstr(buff,"+CPMS:"))==NULL)return 0;
		p+=sizeof("+CPMS:")-1;
		if((p=strstr(buff,"\"SM\","))==NULL)return 0;
		p+=sizeof("\"SM\",")-1;
		totalM=atoi(p);
		DisplayMessage(&totalM);
	}

	InitFlag=1;

	return 0;
}




char NullSubs(void)
{
	return 1;
}


#if 0
void cat_my_time(char *Record)
{
	time_t the_time;
	unsigned char i,year[6],month[6],date[6],shifenmiao[20],*p;

	time(&the_time);
	p=ctime(&the_time);

	while(*p>' ')p++;	//�˳�����
	while(*p<=' ')p++;	//�˳��ո�
	memset(month,0,sizeof(month));
	memset(date,0,sizeof(date));
	memset(shifenmiao,0,sizeof(shifenmiao));
	memset(year,0,sizeof(year));
	for(i=0;*p!=' '&&i<5;){month[i]=*p;i++;p++;}		//ȡ��
	while(*p<=' ')p++;	//�˳��ո�
	for(i=0;*p!=' '&&i<5;){date[i]=*p;i++;p++;}			//ȡ��
	while(*p<=' ')p++;	//�˳��ո�
	for(i=0;*p!=' '&&i<19;){shifenmiao[i]=*p;i++;p++;}	//ȡʱ����
	while(*p<=' ')p++;	//�˳��ո�
	for(i=0;*p>' '&&i<5;){year[i]=*p;i++;p++;}			//ȡ��
	my_EnglishMonthToChinesMonth(month);				//��Ӣ�ĵ���ת�������ĵ���
	strcat(Record,year);
	strcat(Record,"/");
	strcat(Record,month);
	strcat(Record,"/");
	strcat(Record,date);
	strcat(Record," ");
	shifenmiao[5]=0;
	strcat(Record,shifenmiao);
}



static void displayVolume(unsigned char vol)
{
	vol-='0';
	if(vol>9)vol=9;

	const unsigned char pstr[10][38]={
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0xF0,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x01,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x0F,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0x00,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x0F,0x00,0x1F,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0x00},
		{0x3C,0x24,0x3C,0x42,0x81,0xFF,0x00,0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x0F,0x00,0x1F,0x00,0x3F,0x10,0x00,0x30,0x00,0x70,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0}
	};

	draw_pic(pstr[vol],109,0,19,12);
}



unsigned char dialnumber(unsigned char *DialStr)
{
	#define VOICEOUT 	1
	#define VOICEIN		0

	unsigned char outVoicestate=VOICEIN,hookstate=HOOK_guaji;
	unsigned char vol[3]={"59"},hook[5],recievebuf[200];
	unsigned char beyondAsweredflag=0,number[2]={"\0\0"};
	unsigned char CallingTimes[10]={"00:00:00"};
	unsigned char Shour,Smin,Ssec,compareSec;
	unsigned char filebuff[2048],Record[100];
	unsigned char keyval;

	FILE *fp;
	struct tm *tm_ptr;
	time_t the_time;

	memset(recievebuf,0,sizeof(recievebuf));
	strcpy(recievebuf,"ATD");
	strcat(recievebuf,DialStr);
	strcat(recievebuf,";\r");

	strcpy(Record,DialStr);					//�ݴ��¼�ĺ����ֶ�
	strcat(Record,"\t");
	cat_my_time(Record);					//�ݴ��¼��ʱ���ֶ�
	strcat(Record,"\t");

	clear_lcd();
	keyval=SendAtCommand_UntillSuccess("AT+CLVL=80\r");
	if(keyval!=SendAtCommand_OK){
		DrawTitle_to_row_Language2(2,"�޷�����","Can't calling");
		return WaitkeyAndAutoExit(10);
	}

	board_sethandfree(1);	//������

	keyval=SendAtCommand_UntillSuccess("AT+CHFA=0\r");
	if(keyval!=SendAtCommand_OK){
		DrawTitle_to_row_Language2(2,"�޷�����","Can't calling");
		return WaitkeyAndAutoExit(10);
	}

	clear_area_to_row(2,2);
	DrawTitle_to_row_Language2(2,"�����С���","Dialing");	//��������ʾ״̬��Ϣ

	displayVolume(vol[0]);

	send_at_cmd(recievebuf);
	/*keyval=SendAtCommand_UntillSuccess(recievebuf);
	if(keyval!=SendAtCommand_OK){
	DrawTitle_to_row_Language2(2,"�޷�����","Can't calling");
	return WaitkeyAndAutoExit(10);
	}*/

	while(1){
		SetBLOverTime(100);
		SetAutoExitTime(100);

		keyval=ReadDatasFromtty(fd_ttyS1,recievebuf,sizeof(recievebuf),0,100000);		//100ms��һ������

		if(strstr(recievebuf,"+CME ERROR")){
			clear_area_to_row(2,2);
			DrawTitle_to_row_Language2(2,"���Ŵ���!","Dial error!");
			break;
		}else if(strstr(recievebuf,"BUSY")){
			clear_area_to_row(2,2);
			DrawTitle_to_row_Language2(2,"��·æ!","BUSY");
			break;
		}else if(strstr(recievebuf,"NO CARRIER")){
			clear_area_to_row(2,2);
			DrawTitle_to_row_Language2(2,"�޷�ͨ�Ż�Է��һ�","NO CARRIER");
			break;
		}else if(strstr(recievebuf,"NO ANSWER")){
			clear_area_to_row(2,2);
			DrawTitle_to_row_Language2(2,"�Է���Ӧ��!","NO ANSWER");
			break;
		}else if(strstr(recievebuf,"OK")){
			clear_area_to_row(2,2);
			DrawTitle_to_row_Language2(2,"ͨ����!","Talking on");
		}

		if(strstr(recievebuf,"+COLP:")){
			beyondAsweredflag=1;
			time(&the_time);
			tm_ptr = gmtime(&the_time);
			Shour=0;
			Smin=0;
			Ssec=0;
			compareSec=tm_ptr->tm_sec;
		}

		if(beyondAsweredflag==1){
			time(&the_time);
			tm_ptr = gmtime(&the_time);
			if(tm_ptr->tm_sec!=compareSec){
				Ssec++;
				if(Ssec>59){
					Ssec=0;
					Smin++;
					if(Smin>59){
						Smin=0;
						Shour++;
					}
				}
				compareSec=tm_ptr->tm_sec;
			}
			sprintf(CallingTimes,"%02d:%02d:%02d",Shour,Smin,Ssec);
			DrawTitle_to_row_Language2(3,CallingTimes,CallingTimes);
		}

		read(fd_read_gpio,hook,5);
		if(outVoicestate==VOICEIN&&hookstate==HOOK_guaji){
			if(hook[1]==HOOK_zaiji){
				//ioctl(fd_gpio, GPB5_L,0);
				board_sethandfree(0);				//�ر�����
				write(fd_ttyS1,"AT+CHFA=0\r",strlen("AT+CHFA=0\r"));
				outVoicestate=VOICEIN;
				hookstate=HOOK_zaiji;
			}
		}

		else if(outVoicestate==VOICEIN&&hookstate==HOOK_zaiji){
			if(hook[1]==HOOK_guaji){goto tuichu;}
		}

		else if(outVoicestate==VOICEOUT&&hookstate==HOOK_guaji){
			if(hook[1]==HOOK_zaiji){
				//ioctl(fd_gpio, GPB5_L,0);
				board_sethandfree(0);				//�ر�����
				write(fd_ttyS1,"AT+CHFA=0\r",strlen("AT+CHFA=0\r"));
				outVoicestate=VOICEIN;
				hookstate=HOOK_zaiji;
			}
		}

		else{
			hookstate=hook[1];
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue ;
		number[0]=0;
		KeyEventPost();
		switch(keyval){
			case KEY0:		number[0]='0';		break;			//
			case KEY1:		number[0]='1';		break;
			case KEY2:		number[0]='2';		break;
			case KEY3:		number[0]='3';		break;
			case KEY4:		number[0]='4';		break;
			case KEY5:		number[0]='5';		break;
			case KEY6:		number[0]='6';		break;
			case KEY7:		number[0]='7';		break;
			case KEY8:		number[0]='8';		break;
			case KEY9:		number[0]='9';		break;
			case KEYXING: 	number[0]='*';		break;
			case KEYJING: 	number[0]='#';		break;

			case KEY_ReturnPrevious:			//ȡ������
			case KEY_ReturnInitInterface:
			goto tuichu;

			case KEY_UP:			//��������
			strcpy(recievebuf,"AT+CLVL=");
			vol[0]+=1;if(vol[0]>'9')vol[0]='9';
			strcat(recievebuf,vol);
			strcat(recievebuf,"\r");
			send_at_cmd(recievebuf);
			displayVolume(vol[0]);
			break;

			case KEY_DOWN:
			strcpy(recievebuf,"AT+CLVL=");
			vol[0]-=1;if(vol[0]<'0')vol[0]='0';
			strcat(recievebuf,vol);
			strcat(recievebuf,"\r");
			send_at_cmd(recievebuf);
			displayVolume(vol[0]);
			break;

			case KEY_HandsFree:			//�����л�
			if(outVoicestate==VOICEIN&&hookstate==HOOK_zaiji){
				//ioctl(fd_gpio, GPB5_H,0);
				board_sethandfree(1);				//������
				write(fd_ttyS1,"AT+CHFA=1\r",strlen("AT+CHFA=1\r"));
				outVoicestate=VOICEOUT;
			}

			else if(outVoicestate==VOICEOUT&&hookstate==HOOK_zaiji){
				//ioctl(fd_gpio, GPB5_L,0);
				board_sethandfree(0);				//�ر�����
				write(fd_ttyS1,"AT+CHFA=0\r",strlen("AT+CHFA=0\r"));
				outVoicestate=VOICEIN;
			}

			else if(outVoicestate==VOICEIN&&hookstate==HOOK_guaji){
				goto tuichu;
			}
			break;
		}

		if(number[0]){
			strcpy(recievebuf,"AT+CLDTMF=");
			strcat(recievebuf,number);
			strcat(recievebuf,",\"1\"\r");
			send_at_cmd(recievebuf);
			usleep(20000);

			strcpy(recievebuf,"AT+VTS=");
			strcat(recievebuf,number);
			strcat(recievebuf,"\r");
			send_at_cmd(recievebuf);
		}
	}

	tuichu:
	clear_area_to_row(2,3);
	DrawTitle_to_row_Language2(2,"����ȡ������!","Canceling");
	keyval=SendAtCommand_UntillSuccess("ATH\r");
	clear_area_to_row(2,3);
	if(keyval==SendAtCommand_OK)DrawTitle_to_row_Language2(2,"��ȡ������!","Canceled");
	else DrawTitle_to_row_Language2(2,"ȡ������ʧ��!","Cancel failure");

	strcat(Record,CallingTimes);	//�ݴ��¼��ͨ��ʱ���ֶ�
	strcat(Record,"\t\n");

	memset(filebuff,0,sizeof(filebuff));
	fp=fopen("DialedPhone","rb");
	if(fp!=NULL){
		fread(filebuff,2048,1,fp);
		fclose(fp);
	}

	fp=fopen("DialedPhone","wb");
	Shour=Insert_A_Record(filebuff,&filebuff[sizeof(filebuff)-1],Record,0);

	if(strlen(filebuff)>sizeof(filebuff)-80){
		Delete_A_Record(filebuff,&filebuff[sizeof(filebuff)-1],Get_Record_Total(filebuff));
	}
	fwrite(filebuff,sizeof(filebuff),1,fp);
	fclose(fp);

	return WaitkeyAndAutoExit(10);
}

#endif



unsigned char CheckMessageComing_method1(unsigned char *buff,unsigned char *index)
{
	static unsigned char magicbuff[20];
	static unsigned char flag=0;
	static unsigned char *psave;

	const unsigned char com[]={"+CMTI"};
	const unsigned char *pcom;

	//\x0D\x0A+CMTI:\x20"SM",1\x0D\x0A

	if(flag==0){
		psave=magicbuff;
		pcom=com;
		while(*buff!=0){
			if(*buff==*pcom){
				*psave=*buff;
				psave++;
				pcom++;
				if(*pcom==0){buff++;flag=1;goto next;}
			}
			else{
				psave=magicbuff;
				pcom=com;
			}

			buff++;
		}
		return CHECK_MESSAGE_NO_MESSAGE;
	}
	else{
		next:
		while(*buff!=0){
			*psave=*buff;
			if(psave>=&magicbuff[19]){
				psave=magicbuff;
				pcom=com;
				flag=0;
				return CHECK_MESSAGE_NO_MESSAGE;
			}
			if(*buff<'\x20'){
				printf("magicbuff=%s\n",magicbuff);
				*psave=0;
				psave=(unsigned char *)strstr((char *)magicbuff,",");
				if(psave==NULL){
					psave=magicbuff;
					pcom=com;
					flag=0;
					return CHECK_MESSAGE_NO_MESSAGE;
				}
				
				psave++;
				*index=atoi((char *)psave);

				psave=magicbuff;
				pcom=com;
				flag=0;
				return CHECK_MESSAGE_HAVE_MESSAGE;
			}
			psave++;
			buff++;
		}

		return CHECK_MESSAGE_NO_MESSAGE;
	}
}



unsigned char CheckMessageComing_method2(unsigned char *buff,unsigned char *index)
{
	static unsigned char total;

	unsigned char temp;

	GetMessageTotalAndCapacity(&temp,NULL);

	if(temp>total){total=temp;return CHECK_MESSAGE_HAVE_MESSAGE;}

	return CHECK_MESSAGE_NO_MESSAGE;
}

unsigned char CheckPhoneComing(unsigned char *buff,unsigned char *phone_number)
{
	// 1. \r\nRING\r\n\r\n+CLIP: "15112475662",129,"",,"",0\r\n
	// 2. \x0D\x0ARING\x0D\x0A\x0D\x0A+CLIP:\x20"15112475662",129,"",,"",0\x0D\x0A	
	return CHECK_PHONE_NO_PHONE;
}

static signed char CheckPINAndPUK (void)
{
	unsigned char CmdBuff[1024]={0};

	RequirePin = 0x02;

	strcpy( (char *)CmdBuff, "AT\r");
	send_at_cmd_and_receive (CmdBuff, sizeof (CmdBuff), 3000, "OK");

	strcpy ( (char *) CmdBuff, "AT+CPIN?\r");
	send_at_cmd_and_receive (CmdBuff, sizeof (CmdBuff), 3000, "OK");	//����Ƿ������˿�������PIN  �롢����Ƿ�װ��SIM  ����
	printf("Send1:AT+CPIN?\r\n->%s\r\n",CmdBuff);
	if (strstr ( (const char*) CmdBuff, "ERROR") )
	{
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "no sim card", "no sim card");
		WaitkeyAndAutoExit (10);
		return 1;
	}

	if (strstr ( (const char*) CmdBuff, "SIM PIN")!=NULL )
	{
		if( strcmp(DEFAULT_PIN, (const char *)StorePIN) != 0)
		{
			RequirePin = 0x01;
			strcpy ( (char *) CmdBuff, "AT+CPIN=\"");
			strcat ( (char *) CmdBuff, (const char*) StorePIN);
			strcat ( (char *) CmdBuff, "\"\r");
			send_at_cmd_and_return (CmdBuff, 6000, "OK\r", "ERROR");

			if ( kmy_USART3CheckBuff("READY") )//strstr((char *)CmdBuff,"READY")!=NULL)
			{
				return 1;
			}
			else
			{
				printf("CmdBuff: %s\r\n", CmdBuff);
				kmy_USART3ShowBuff();
			}
			// delay
			msleep(500);
			
			memset(CmdBuff,0,sizeof(CmdBuff));
			strcpy ( (char *) CmdBuff, "AT+CPIN?\r");
			send_at_cmd_and_receive (CmdBuff, sizeof (CmdBuff), 6000, "OK");
			printf("Send2:AT+CPIN?\r\n%s\r\n",CmdBuff);

			if ( kmy_USART3CheckBuff("READY"))
			{
				return 1;
			}
			else
			{
				printf("CmdBuff: %s\r\n", CmdBuff);
				kmy_USART3ShowBuff();
			}
		}
		
		PassWordInput2 ("Input PIN", StorePIN, 6, 1, 0, KEY_NONE, 1);
		if( strlen((char *)StorePIN) == 0) return 0;
		
		StorageVariable();	
		
		memset(CmdBuff,0,sizeof(CmdBuff));
		strcpy ( (char *) CmdBuff, "AT+CPIN=\"");
		strcat ( (char *) CmdBuff, (const char*) StorePIN);
		strcat ( (char *) CmdBuff, "\"\r");
		send_at_cmd_and_return (CmdBuff, 5000, "OK\r", "ERROR");
		if (strstr((char *)CmdBuff,"READY")==NULL)
		{
			clear_lcd();
			DrawTitle_to_row_Language2 (1, "store pin error", "store pin error");
			DrawTitle_to_row_Language2 (2, "Configed poweron again", "Configed poweron again");
			WaitkeyAndAutoExit (10);
again:
			PIN_Store(NULL);
			clear_lcd();
			DrawTitle_to_row_Language2 (1, "Whether config OK", "Whether config OK");
			DrawTitle_to_row_Language2 (2, "if yes please poweron", "if yes please poweron");
			DrawTitle_to_row_Language2 (3, "again", "again");
			WaitkeyAndAutoExit (50);
			goto again;
			//return 0;
		}
	}

	else if (strstr ( (const char*) CmdBuff, "SIM PUK") )
	{
		clear_lcd();
		DrawTitle_to_row_Language2 (2, "PIN Locked", "PIN Locked");
		DrawTitle_to_row_Language2 (3, "Need PUK", "Need PUK");
		WaitkeyAndAutoExit (10);
		return 0;
	}

	return 1;
}


unsigned char Sim900B_init (void)
{
	unsigned char i;

	send_at_cmd_and_return ("AT\r", 6000, "OK", "ERROR");
    
	i = send_at_cmd_and_return ("AT+IPR=115200&w\r", 3000, "OK", "ERROR");	//set the rate

	if (i != TCP_OK)
	{
	    printf("Set rate error\r\n");
		goto tuichu;
	}
	
	CheckPINAndPUK();

	i = set_apn();

	if (i != TCP_OK)
	{
		clear_lcd();
		DrawTitle_to_row (3, "Set APN error");
		WaitkeyAndAutoExit (10);
	}

	i = send_at_cmd_and_return ("ATE0\r", 3000, "OK\r", "ERROR");	//close echo

	if (i != TCP_OK)
	{
		goto tuichu;
	}

	i = send_at_cmd_and_return ("AT+CMGF=1\r", 3000, "OK", "ERROR");	//���ö���Ϊtextģʽ

	if (i != TCP_OK)
	{
		goto tuichu;
	}

	

	send_at_cmd_and_return ("AT+QIDEACT\r", 1000, "DEACT OK", "ERROR");

	return SIM900B_INIT_SUCCESS;


tuichu:
	clear_lcd();
	DrawTitle_to_row_Language2 (2, "����ģ���ʼ��", "sim module init");
	DrawTitle_to_row_Language2 (3, "ʧ��", "failure!");

	WaitkeyAndAutoExit (10);

	return SIM900B_INIT_FAILURE;
}







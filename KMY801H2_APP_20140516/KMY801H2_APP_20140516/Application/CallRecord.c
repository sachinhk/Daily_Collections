
#include "CallRecord.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "lcd.h"
#include "FileOperate.h"
#include "MenuOperate.h"
#include "key.h"
#include "main.h"
#include "global_extern_variable.h"
#include "my_stdint.h"
#include "EepromFileSystem.h"

static const u8 file_Dialed[]={"DialedPhone"};
static const u8 file_unReceived[]={"unReceived"};
static const u8 file_Received[]={"Received"};


static char DisplayDetailRecvdCalls(u8 curR,u8 *pbuff,u8 *buffEND)
{
	u8 disbuf[MaxByteOneLine+2];

	clear_lcd();
	DrawTitle_to_row_Language2(0,"【记录详情】","【记录详情】");
	Read_A_Record_Segment(pbuff,buffEND,curR,disbuf,sizeof(disbuf)-1,1);
	DrawTitle_to_row_Language2(1,disbuf,disbuf);
	Read_A_Record_Segment(pbuff,buffEND,curR,disbuf,sizeof(disbuf)-1,2);
	DrawTitle_to_row_Language2(2,disbuf,disbuf);
	Read_A_Record_Segment(pbuff,buffEND,curR,disbuf,sizeof(disbuf)-1,3);
	DrawTitle_to_row_Language2(3,disbuf,disbuf);

	return(WaitkeyAndAutoExit(10));
}


static char display_papa(u8 *buff,u8 *Title,u8 *buffEND)
{
	#define STARTROW	1
	#define ENDROW		MaxRowOneScr-1
	u8 disbuf[ENDROW-STARTROW+1][22];
	int i,TotalR,curR,curHR,oldHR,flashflag;
	u8 thenums[8];
	u8 keyval;

	TotalR=Get_Record_Total(buff);
	memset(disbuf,0,sizeof(disbuf));
	for(i=0;i<ENDROW-STARTROW+1&&i<TotalR;i++)
	{
		Read_A_Record_Segment(buff,buffEND,i+1,disbuf[i],sizeof(disbuf)-1,1);
	}

	clear_lcd();
	curR=0;
	curHR=oldHR=STARTROW;
	flashflag=3;
	while(1)
	{
		if(flashflag)
		{
			if(flashflag&0x02){DrawTitle_to_row_Language2(0,Title,Title);}
			memset(thenums,0,sizeof(thenums));
			sprintf((s8*)thenums,"%02d/%02d",curR+1,TotalR);
			text_out_to_row_x_fillSpace(0,96,thenums);
			if(flashflag!=0x04)
			{
				for(i=STARTROW;i<ENDROW+1&&i<TotalR+STARTROW;i++)
				{
					text_out_to_row_fillSpace(i,disbuf[i-STARTROW]);
				}
				DispHighLightRow(curHR);
			}
			flashflag=0;
		}
		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue ;
		KeyEventPost();
		switch(keyval)
		{
			case KEY_UP:
			if(curR)
			{				//current row is not the first row
				curR--;
				if(curHR>STARTROW)
				{					//if is high light row is not the first row, only want to switch the Highlight row
					oldHR=curHR;
					curHR--;
					DispHighLightRow(oldHR);
					DispHighLightRow(curHR);
					flashflag=0x04;
				}
				else
				{
					for(i=ENDROW-STARTROW;i>0;i--)strcpy((s8*)disbuf[i],(sc8*)disbuf[i-1]);

					Read_A_Record_Segment(buff,buffEND,curR+1,disbuf[0],sizeof(disbuf[0])-1,1);
					flashflag=1;
				}
			}
			else
			{				//current row is the first row, the next row is the last row
				if(TotalR<ENDROW-STARTROW+1){
					flashflag=TotalR;
				}
				else
				{
					flashflag=ENDROW-STARTROW+1;
				}
				for(i=0;i<flashflag;i++){Read_A_Record_Segment(buff,buffEND,TotalR-flashflag+i+1,disbuf[i],sizeof(disbuf[0])-1,1);}
				curR=TotalR-1;
				if(TotalR<ENDROW-STARTROW+1)
				{
					curHR=oldHR=STARTROW+TotalR-1;
				}
				else
				{
					curHR=oldHR=ENDROW;
				}
				flashflag=1;
			}
			break;

			case KEY_DOWN:
			if(curR<TotalR-1)
			{				//current row is not the last row
				curR++;
				if(curHR<ENDROW&&curHR<TotalR)
				{					//if is high light row is not the first row, only want to switch the Highlight row
					oldHR=curHR;
					curHR++;
					DispHighLightRow(oldHR);
					DispHighLightRow(curHR);
					flashflag=0x04;
				}
				else
				{
					for(i=0;i<ENDROW-STARTROW+1;i++)strcpy((s8*)disbuf[i],(sc8*)disbuf[i+1]);
					
					Read_A_Record_Segment(buff,buffEND,curR+1,disbuf[ENDROW-STARTROW],sizeof(disbuf[0])-1,1);
					flashflag=1;
				}
				
			}
			else
			{				//current row is the last row, next row is the first row
				for(i=0;i<ENDROW-STARTROW+1&&i<TotalR;i++){Read_A_Record_Segment(buff,buffEND,i+1,disbuf[i],sizeof(disbuf[0])-1,1);}
				curR=0;
				curHR=oldHR=STARTROW;
				flashflag=1;
			}
			break;

			case KEY_Enter:
			case KEY_RIGHT:
			if(DisplayDetailRecvdCalls(curR+1,buff,buffEND)==KEY_ReturnInitInterface)return KEY_ReturnInitInterface;
			clear_lcd();
			flashflag=0x03;
			break;

			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
			return KEY_ReturnInitInterface;

			case KEY_ReturnPrevious:
			return KEY_ReturnPrevious;
		}

	}

	//return(WaitkeyAndAutoExit(40));
}




static char DialledNO(char *msg)
{
	u8 buff[4096];
	u8 retval;

	memset(buff,0,sizeof(buff));
	retval=EepromFile_ReadData(buff,sizeof(buff),file_Dialed);
	if(retval!=EepromFile_ReadOk)
	{	
		clear_lcd();
		DrawTitle_to_row_Language2(2,"没有已拨电话!","No dialed num!");
		return (WaitkeyAndAutoExit(10));
	}

	return display_papa(buff,"已拨电话",buff+sizeof(buff)-1);
}


static char unRecvdCalls(char *msg)
{
	u8 buff[4096];
	u8 retval;

	memset(buff,0,sizeof(buff));
	retval=EepromFile_ReadData(buff,sizeof(buff),file_unReceived);
	if(retval!=EepromFile_ReadOk)
	{
		clear_lcd();
		DrawTitle_to_row_Language2(2,"没有未接电话!","No unreceived num!");
		return (WaitkeyAndAutoExit(10));
	}

	return display_papa(buff,"未接电话",buff+sizeof(buff)-1);
}


static char RecvdCalls(char *msg)
{
	u8 buff[4096];
	u8 retval;

	memset(buff,0,sizeof(buff));
	retval=EepromFile_ReadData(buff,sizeof(buff),file_Received);
	if(retval!=EepromFile_ReadOk)
	{
		clear_lcd();
		DrawTitle_to_row_Language2(2,"没有已接电话!","No received num!");
		return (WaitkeyAndAutoExit(10));
	}

	return display_papa(buff,"已接电话",buff+sizeof(buff)-1);
}



static char Delete_all_record(char *msg)
{
	u8 retval;

	clear_lcd();
	DrawTitle_to_row_Language2(2,"确认全部删除吗","Confirm Delete it?");
	retval=WaitkeyAndAutoExit(10);
	if(retval!=KEY_Enter)return retval;

	EepromFile_Delete(file_Dialed);
	EepromFile_Delete(file_unReceived);
	EepromFile_Delete(file_Received);

	DrawTitle_to_row_Language2(3,"已全部删除","Deleted");
	return(WaitkeyAndAutoExit(10));
}


char MainMenuScreen_Calls(char *msg)
{
	static struct MenuFuc_Language2 Menu[4]={
		{"1.未接电话","1.Unreceived",unRecvdCalls},
		{"2.已接电话","2.Received",RecvdCalls},
		{"3.已拨电话","3.Dialed",DialledNO},
		{"4.全部删除","4.Delete all",Delete_all_record},
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

	while(1)
	{
		MenuHandler_Language2(&MenuOp);
		if(MenuOp.RetVal==KEY_Enter)
		{
			MenuOp.FucRetVal=MenuOp.Menu->MenuFuc(msg);
			if(MenuOp.FucRetVal==KEY_WaitTimeout||MenuOp.FucRetVal==KEY_ReturnInitInterface){CurProc=CurProc_Return;return MenuOp.FucRetVal;} 
			else
			{
				clear_lcd();
				MenuOp.flashflag=1;
			}
		}
		else
		{
			return MenuOp.RetVal;
		}
	}
}


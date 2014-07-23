#include "MenuOperate.h"

#include <stdio.h>
#include <string.h>


#include "key.h"
#include "lcd.h"


#define FlushAllFlag			0x01
#define FlushOnlyHightRowFlag	0x02
#define FlushUpkeyFlag			0x04
#define FlushDownkeyFlag		0x08
#define FlushRightkeyFlag		0x10
#define FlushLeftkeyFlag		0x20


unsigned char GetKeyNumVal(unsigned char keyval)
{
	unsigned char temp=1;

	switch(keyval){
		case KEY1: temp=1;break;
		case KEY2: temp=2;break;
		case KEY3: temp=3;break;
		case KEY4: temp=4;break;
		case KEY5: temp=5;break;
		case KEY6: temp=6;break;
		case KEY7: temp=7;break;
		case KEY8: temp=8;break;
		case KEY9: temp=9;break;
	}

	return temp;
}

void DisplayMenuPercent(struct MenuOperate *MenuOp)
{
	unsigned char TotalLine;
	unsigned char CurLine;
	unsigned char StartLine;
	unsigned char EndLine;
	unsigned char Heigth;
	unsigned char total;
	unsigned char y,restline;

	if((*MenuOp).MenuTotal>(*MenuOp).EndLine-(*MenuOp).StartLine){
		TotalLine=(*MenuOp).MenuTotal;
		CurLine=(*MenuOp).Menucnt;
		StartLine=(*MenuOp).StartLine;
		EndLine=(*MenuOp).EndLine;

		clear_area(ALinePixNum-11,ALineContainDotLine*StartLine,11,(EndLine-StartLine)*ALineContainDotLine);
		fill_rect(ALinePixNum-8,ALineContainDotLine*StartLine,5,2,mode_Black);
		fill_rect(ALinePixNum-8,ALineContainDotLine*EndLine-4,5,2,mode_Black);
		draw_line(ALinePixNum-6,ALineContainDotLine*StartLine,ALinePixNum-6,ALineContainDotLine*EndLine-4,mode_Black);

		total=(EndLine-StartLine)*ALineContainDotLine-8;		// 4+4+1+1
		Heigth=total/TotalLine;
		if(Heigth<1)Heigth=1;
		y=(total/TotalLine*CurLine)+3+ALineContainDotLine*StartLine;		//2+1
		restline=((total%TotalLine)*(CurLine+1))/TotalLine;
		y=y+restline;

		fill_rect(ALinePixNum-8,y,5,Heigth,mode_Black);
	}
}




void MenuOperateDeInit(struct MenuOperate *MenuOp,struct MenuFuc *Menu,unsigned char MenuTotal,unsigned char StartLine,unsigned char EndLine)
{
	(*MenuOp).Menu=Menu;
	(*MenuOp).MenuTotal=MenuTotal;
	(*MenuOp).Menucnt=0;
	(*MenuOp).curFlashRow=StartLine;
	(*MenuOp).StartLine=StartLine;
	(*MenuOp).EndLine=EndLine;
	(*MenuOp).flashflag=1;
	(*MenuOp).RetVal=0;
	(*MenuOp).FucRetVal=0;
}

#if 1
void MenuHandler(struct MenuOperate *MenuOp)
{
	signed char i;
	unsigned char dispbuff[MaxByteOneLine+2];
	unsigned char keyval;
	unsigned char temp;
	unsigned char atendflag=0;

	clear_area_to_row((*MenuOp).StartLine,(*MenuOp).EndLine);
	while(1){
		if(((*MenuOp).flashflag&FlushOnlyHightRowFlag)!=0){
			if(((*MenuOp).flashflag&FlushDownkeyFlag)!=0){
				DispHighLightRow((*MenuOp).curFlashRow-1);
			}else{
				DispHighLightRow((*MenuOp).curFlashRow+1);
			}
			DispHighLightRow((*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);

			(*MenuOp).flashflag=0;
		}
		else if(((*MenuOp).flashflag&FlushAllFlag)!=0){
			temp=(*MenuOp).curFlashRow-(*MenuOp).StartLine;	//得到高亮和高亮之前的行数
			for(i=(*MenuOp).StartLine;i<=(*MenuOp).curFlashRow;i++){	//显示高亮和高亮之前的行
				my_strcpy_fillspace(dispbuff,((*MenuOp).Menu-temp)->MenuStr);
				text_out_to_row(i,dispbuff);
				temp--;
			}

			temp=1;
			for(;i<=(*MenuOp).EndLine-1;i++){				//显示高亮之后的行
				if((*MenuOp).Menucnt+temp<(*MenuOp).MenuTotal){
					my_strcpy_fillspace(dispbuff,((*MenuOp).Menu+temp)->MenuStr);
					text_out_to_row(i,dispbuff);
					temp++;
				}else{
					clear_area_to_row(i,(*MenuOp).EndLine-1);
					break;
				}
			}

			if(atendflag==1&&((*MenuOp).flashflag&FlushRightkeyFlag)==0)atendflag=0;

			DispHighLightRow((*MenuOp).curFlashRow);			//显示高亮
			DisplayMenuPercent((struct MenuOperate *)MenuOp);	//显示右边的百分比

			(*MenuOp).flashflag=0;
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
			i=GetKeyNumVal(keyval);
			if(i<=(*MenuOp).MenuTotal){
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menu+=i-1;
				(*MenuOp).Menucnt=i-1;
				/*if((i-1)<((*MenuOp).EndLine-(*MenuOp).StartLine)-1){
				(*MenuOp).curFlashRow=(*MenuOp).StartLine+i-1;
				}else{
				(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}*/
				(*MenuOp).curFlashRow=(*MenuOp).Menucnt%((*MenuOp).EndLine-(*MenuOp).StartLine)+(*MenuOp).StartLine;
				(*MenuOp).RetVal=KEY_Enter;
				return;
			}
			break;

			case KEY_UP:
			if((*MenuOp).Menucnt){				//未到第一个菜单
				(*MenuOp).Menu--;
				(*MenuOp).Menucnt--;
				if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;}
				else {
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
					(*MenuOp).flashflag=FlushAllFlag;
				}
			}else{				//到了第一个菜单，下一个菜单是最后一个。
				(*MenuOp).Menu=(*MenuOp).Menu+((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				(*MenuOp).curFlashRow=((*MenuOp).MenuTotal-1)%((*MenuOp).EndLine-(*MenuOp).StartLine)+(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag;
			}
			break;

			case KEY_DOWN:
			if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1){				//未到最后一个菜单
				(*MenuOp).Menu++;
				(*MenuOp).Menucnt++;
				if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;}
				else {
					(*MenuOp).curFlashRow=(*MenuOp).StartLine;
					(*MenuOp).flashflag=FlushAllFlag;
				}
			}else{				//到了最后一个菜单，下一个菜单是第一个。
				(*MenuOp).Menu=(*MenuOp).Menu-((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag;
			}
			break;

			case KEY_RIGHT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if(((*MenuOp).Menucnt+i+(*MenuOp).StartLine-(*MenuOp).curFlashRow)<(*MenuOp).MenuTotal){				//可以向下跳一屏
				if((*MenuOp).curFlashRow==(*MenuOp).StartLine){
					(*MenuOp).Menu+=i;
					(*MenuOp).Menucnt+=i;
				}else{
					(*MenuOp).Menu-=((*MenuOp).curFlashRow-(*MenuOp).StartLine);
					(*MenuOp).Menu+=i;
					(*MenuOp).Menucnt-=((*MenuOp).curFlashRow-(*MenuOp).StartLine);
					(*MenuOp).Menucnt+=i;
				}
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushRightkeyFlag;
			}else{											//不够向下跳一屏
				if(!atendflag){
					if((*MenuOp).curFlashRow==(*MenuOp).StartLine){
						(*MenuOp).Menu+=((*MenuOp).MenuTotal-1-(*MenuOp).Menucnt);
						(*MenuOp).curFlashRow=(*MenuOp).MenuTotal-1-(*MenuOp).Menucnt+(*MenuOp).StartLine;
						(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
						(*MenuOp).flashflag=FlushAllFlag|FlushRightkeyFlag;
					}
					atendflag=1;
				}
			}
			break;

			case KEY_LEFT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if((*MenuOp).Menucnt-(*MenuOp).curFlashRow+(*MenuOp).StartLine>=i){		//可以向上跳一屏
				if((*MenuOp).curFlashRow==(*MenuOp).StartLine){
					(*MenuOp).Menu-=i;
					(*MenuOp).Menucnt-=i;
				}else{
					(*MenuOp).Menu-=(*MenuOp).curFlashRow-(*MenuOp).StartLine;
					(*MenuOp).Menu-=i;
					(*MenuOp).Menucnt-=(*MenuOp).curFlashRow-(*MenuOp).StartLine;
					(*MenuOp).Menucnt-=i;
				}
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushLeftkeyFlag;
			}else{							//不够向上跳一屏
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushLeftkeyFlag;
			}
			break;

			case KEY_Enter:
			(*MenuOp).RetVal=KEY_Enter;
			return;

			case KEY_ReturnPrevious:
			(*MenuOp).RetVal=KEY_ReturnPrevious;
			return;

			case KEY_ReturnInitInterface:
			(*MenuOp).RetVal=KEY_ReturnInitInterface;
			return;

			case KEY_WaitTimeout:
			(*MenuOp).RetVal=KEY_WaitTimeout;
			return;
		}

		#if 0
		printf("(*MenuOp).MenuTotal=%d\n",(*MenuOp).MenuTotal);
		printf("(*MenuOp).curFlashRow=%d\n",(*MenuOp).curFlashRow);
		printf("(*MenuOp).Menucnt=%d\n",(*MenuOp).Menucnt);
		printf("*((*MenuOp).Menu).MenuStr=%s\n",(*((*MenuOp).Menu)).MenuStr);
		#endif
	}
}

#else
void MenuHandler(struct MenuOperate *MenuOp)
{
	signed char i;
	signed char dispbuff[MaxByteOneLine+2];
	unsigned char keyval;
	unsigned char temp;
	unsigned char atendflag=0;

	clear_area_to_row((*MenuOp).StartLine,(*MenuOp).EndLine);
	while(1){
		if(((*MenuOp).flashflag&FlushOnlyHightRowFlag)!=0){
			if(((*MenuOp).flashflag&FlushDownkeyFlag)!=0){
				DispHighLightRow((*MenuOp).curFlashRow-1);
			}else{
				DispHighLightRow((*MenuOp).curFlashRow+1);
			}
			DispHighLightRow((*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);

			(*MenuOp).flashflag=0;
		}
		else if(((*MenuOp).flashflag&FlushAllFlag)!=0){
			temp=(*MenuOp).curFlashRow-(*MenuOp).StartLine;	//得到高亮和高亮之前的行数
			for(i=(*MenuOp).StartLine;i<=(*MenuOp).curFlashRow;i++){	//显示高亮和高亮之前的行
				my_strcpy_fillspace(dispbuff,((*MenuOp).Menu-temp)->MenuStr);
				text_out_to_row(i,dispbuff);
				temp--;
			}

			temp=1;
			for(;i<=(*MenuOp).EndLine-1;i++){				//显示高亮之后的行
				if((*MenuOp).Menucnt+temp<(*MenuOp).MenuTotal){
					my_strcpy_fillspace(dispbuff,((*MenuOp).Menu+temp)->MenuStr);
					text_out_to_row(i,dispbuff);
					temp++;
				}else{
					clear_area_to_row(i,(*MenuOp).EndLine-1);
					break;
				}
			}

			if(atendflag==1&&((*MenuOp).flashflag&FlushRightkeyFlag)==0)atendflag=0;

			DispHighLightRow((*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);

			(*MenuOp).flashflag=0;
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
			i=GetKeyNumVal(keyval);
			if(i<=(*MenuOp).MenuTotal){
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menu+=i-1;
				(*MenuOp).Menucnt=i-1;
				if((i-1)<((*MenuOp).EndLine-(*MenuOp).StartLine)-1){
					(*MenuOp).curFlashRow=(*MenuOp).StartLine+i-1;
				}else{
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}
				(*MenuOp).RetVal=KEY_Enter;
				return;
			}
			break;

			case KEY_UP:
			if((*MenuOp).Menucnt){				//未到第一个菜单
				(*MenuOp).Menu--;
				(*MenuOp).Menucnt--;
				if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;}
				else {(*MenuOp).flashflag=FlushAllFlag;}
			}else{				//到了第一个菜单，下一个菜单是最后一个。
				(*MenuOp).Menu=(*MenuOp).Menu+((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine<(*MenuOp).MenuTotal+(*MenuOp).StartLine ? (*MenuOp).EndLine-1 : (*MenuOp).MenuTotal+(*MenuOp).StartLine-1;
				(*MenuOp).flashflag=FlushAllFlag;
			}
			break;

			case KEY_DOWN:
			if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1){				//未到最后一个菜单
				(*MenuOp).Menu++;
				(*MenuOp).Menucnt++;
				if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;}
				else {(*MenuOp).flashflag=FlushAllFlag;}
			}else{				//到了最后一个菜单，下一个菜单是第一个。
				(*MenuOp).Menu=(*MenuOp).Menu-((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag;
			}
			break;

			case KEY_RIGHT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if(((*MenuOp).Menucnt+i)<(*MenuOp).MenuTotal){				//可以向下跳一屏
				(*MenuOp).Menu+=i;
				(*MenuOp).Menucnt+=i;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushRightkeyFlag;
			}else{											//不够向下跳一屏
				//if((*MenuOp).curFlashRow==(*MenuOp).StartLine){
				if(!atendflag){
					(*MenuOp).Menu+=((*MenuOp).MenuTotal-1-(*MenuOp).Menucnt);
					(*MenuOp).curFlashRow=(*MenuOp).MenuTotal-1-(*MenuOp).Menucnt+(*MenuOp).StartLine;
					(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
					(*MenuOp).flashflag=FlushAllFlag|FlushRightkeyFlag;
					atendflag=1;
				}
			}
			break;

			case KEY_LEFT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if((*MenuOp).Menucnt>=i){		//可以向上跳一屏
				(*MenuOp).Menu-=i;
				(*MenuOp).Menucnt-=i;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushLeftkeyFlag;
			}else{							//不够向上跳一屏
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushLeftkeyFlag;
			}
			break;

			case KEY_Enter:
			(*MenuOp).RetVal=KEY_Enter;
			return;

			case KEY_ReturnPrevious:
			clear_lcd();
			(*MenuOp).RetVal=KEY_ReturnPrevious;
			return;

			case KEY_ReturnInitInterface:
			clear_lcd();
			(*MenuOp).RetVal=KEY_ReturnInitInterface;
			return;

			case KEY_WaitTimeout:
			clear_lcd();
			(*MenuOp).RetVal=KEY_WaitTimeout;
			return;
		}

		#if 1
		printf("(*MenuOp).MenuTotal=%d\n",(*MenuOp).MenuTotal);
		printf("(*MenuOp).curFlashRow=%d\n",(*MenuOp).curFlashRow);
		printf("(*MenuOp).Menucnt=%d\n",(*MenuOp).Menucnt);
		printf("*((*MenuOp).Menu).MenuStr=%s\n",(*((*MenuOp).Menu)).MenuStr);
		#endif
	}
}

#endif

void MenuHandler_Language2(struct MenuOperate_Language2 *MenuOp)
{
	unsigned char i;
	unsigned char dispbuffLocal[MaxByteOneLine+2];
	unsigned char dispbuffOther[MaxByteOneLine+2];
	unsigned char keyval;

	clear_area_to_row((*MenuOp).StartLine,(*MenuOp).EndLine);
	while(1)
	{
		if(((*MenuOp).flashflag&FlushOnlyHightRowFlag)!=0)
		{
			#ifdef DEBUG_PRINTF
			printf("Menucnt=%d, curFlashRow=%d,flashflag=%x\n",(*MenuOp).Menucnt,(*MenuOp).curFlashRow,(*MenuOp).flashflag);
			#endif
			if(((*MenuOp).flashflag&FlushDownkeyFlag)!=0)
			{
				DispHighLightRow((*MenuOp).curFlashRow-1);
			}else{
				DispHighLightRow((*MenuOp).curFlashRow+1);
			}
			DispHighLightRow((*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
			(*MenuOp).flashflag=0;
		}
		else if(((*MenuOp).flashflag&FlushAllFlag)!=0)
		{
			#ifdef DEBUG_PRINTF
			printf("Menucnt=%d, curFlashRow=%d\n",(*MenuOp).Menucnt,(*MenuOp).curFlashRow);
			#endif
			(*MenuOp).flashflag=0;
			for(i=(*MenuOp).StartLine;i<(*MenuOp).EndLine&&i<(*MenuOp).MenuTotal+(*MenuOp).StartLine;i++){
				my_strcpy_fillspace(dispbuffLocal,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStrLocal);
				my_strcpy_fillspace(dispbuffOther,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStrOther);
				text_out_to_row_Language2(i,dispbuffLocal,dispbuffOther);
			}

			DispHighLightRow((*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE)continue;
		KeyEventPost();
		switch(keyval)
		{
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
			i=GetKeyNumVal(keyval);
			if(i<=(*MenuOp).MenuTotal)
			{
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menu+=i-1;
				(*MenuOp).Menucnt=i-1;
				if((i-1)<((*MenuOp).EndLine-(*MenuOp).StartLine)-1)
				{
					(*MenuOp).curFlashRow=(*MenuOp).StartLine+i-1;
				}else{
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}
				(*MenuOp).RetVal=KEY_Enter;
				return;
			}
			break;

			case KEY_UP:
			if((*MenuOp).Menucnt)
			{				//未到第一个菜单
				(*MenuOp).Menu--;
				(*MenuOp).Menucnt--;
				if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;}
			}else{				//到了第一个菜单，下一个菜单是最后一个。
				(*MenuOp).Menu=(*MenuOp).Menu+((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine<(*MenuOp).MenuTotal+(*MenuOp).StartLine ? (*MenuOp).EndLine-1 : (*MenuOp).MenuTotal+(*MenuOp).StartLine-1;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_DOWN:
			if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1)
			{				//未到最后一个菜单
				(*MenuOp).Menu++;
				(*MenuOp).Menucnt++;
				if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;}
			}else{				//到了最后一个菜单，下一个菜单是第一个。
				(*MenuOp).Menu=(*MenuOp).Menu-((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_RIGHT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if(((*MenuOp).Menucnt+i)<(*MenuOp).MenuTotal-1)
			{				//可以向下跳一屏
				(*MenuOp).Menu+=i;
				(*MenuOp).Menucnt+=i;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}else
			{											//不够向下跳一屏
				(*MenuOp).Menu+=((*MenuOp).MenuTotal-1-(*MenuOp).Menucnt);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				if(i>(*MenuOp).MenuTotal)(*MenuOp).curFlashRow=(*MenuOp).MenuTotal-1+(*MenuOp).StartLine;
				else{(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;}
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_LEFT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if((*MenuOp).Menucnt>i){		//可以向上跳一屏
				(*MenuOp).Menu-=i;
				(*MenuOp).Menucnt-=i;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushUpkeyFlag;
			}else{							//不够向上跳一屏
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_Enter:
			(*MenuOp).RetVal=KEY_Enter;
			return;

			case KEY_ReturnPrevious:
			(*MenuOp).RetVal=KEY_ReturnPrevious;
			return;

			case KEY_ReturnInitInterface:
			(*MenuOp).RetVal=KEY_ReturnInitInterface;
			return;

			case KEY_WaitTimeout:
			(*MenuOp).RetVal=KEY_WaitTimeout;
			return;
		}
	}
}

void MenuHandler_Language2_Pic(struct MenuOperate_Language2_Pic *MenuOp)
{
	unsigned char i;
	unsigned char dispbuffLocal[MaxByteOneLine+2];
	unsigned char dispbuffOther[MaxByteOneLine+2];
	unsigned char keyval;

	clear_area_to_row((*MenuOp).StartLine,(*MenuOp).EndLine-1);
	//clear_area_to_row(1,5);
	while(1){
		if(((*MenuOp).flashflag&FlushOnlyHightRowFlag)!=0){
			#ifdef DEBUG_PRINTF
			printf("Menucnt=%d, curFlashRow=%d,flashflag=%x\n",(*MenuOp).Menucnt,(*MenuOp).curFlashRow,(*MenuOp).flashflag);
			#endif
			if(((*MenuOp).flashflag&FlushDownkeyFlag)!=0){
				DispHighLightRow((*MenuOp).curFlashRow-1);
			}else{
				DispHighLightRow((*MenuOp).curFlashRow+1);
			}
			DispHighLightRow((*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
			(*MenuOp).flashflag=0;
		}
		else if(((*MenuOp).flashflag&FlushAllFlag)!=0){
			#ifdef DEBUG_PRINTF
			printf("Menucnt=%d, curFlashRow=%d\n",(*MenuOp).Menucnt,(*MenuOp).curFlashRow);
			#endif
			(*MenuOp).flashflag=0;
			for(i=(*MenuOp).StartLine;i<(*MenuOp).EndLine&&i<(*MenuOp).MenuTotal+(*MenuOp).StartLine;i++){
				//my_strcpy_fillspace(dispbuffLocal,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStrLocal);
				//my_strcpy_fillspace(dispbuffOther,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStrOther);

				//text_out_to_row_Language2(i,dispbuffLocal,dispbuffOther);
				my_strcpy_fillspace(dispbuffLocal,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStrLocal);
				my_strcpy_fillspace(dispbuffOther,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStrOther);
				text_out_to_row_x_Language2(i,0,dispbuffLocal,dispbuffOther);
				steaddraw_pic_to_row_x(((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->pPic,104,i,16,12);
			}

			DispHighLightRow((*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
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
			i=GetKeyNumVal(keyval);	 //将对应键值转化为Num
			if(i<=(*MenuOp).MenuTotal){
				(*MenuOp).Menu-=(*MenuOp).Menucnt;	 //回到menu begin
				(*MenuOp).Menu+=i-1; //指向选择的menu
				(*MenuOp).Menucnt=i-1;
				if((i-1)<((*MenuOp).EndLine-(*MenuOp).StartLine)-1){
					(*MenuOp).curFlashRow=(*MenuOp).StartLine+i-1;
				}else{
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}
				(*MenuOp).RetVal=KEY_Enter;
				return;
			}
			break;

			case KEY_UP:
			if((*MenuOp).Menucnt){				//未到第一个菜单
				(*MenuOp).Menu--;
				(*MenuOp).Menucnt--;
				if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;}
			}else{				//到了第一个菜单，下一个菜单是最后一个。
				(*MenuOp).Menu=(*MenuOp).Menu+((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				(*MenuOp).curFlashRow=((*MenuOp).EndLine<(*MenuOp).MenuTotal+(*MenuOp).StartLine) ? (*MenuOp).EndLine-1 : (*MenuOp).MenuTotal+(*MenuOp).StartLine-1;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_DOWN:
			if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1){				//未到最后一个菜单
				(*MenuOp).Menu++;
				(*MenuOp).Menucnt++;
				if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;}
			}else{				//到了最后一个菜单，下一个菜单是第一个。
				(*MenuOp).Menu=(*MenuOp).Menu-((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_RIGHT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if(((*MenuOp).Menucnt+i)<(*MenuOp).MenuTotal-1){				//可以向下跳一屏
				(*MenuOp).Menu+=i;
				(*MenuOp).Menucnt+=i;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}else{											//不够向下跳一屏
				(*MenuOp).Menu+=((*MenuOp).MenuTotal-1-(*MenuOp).Menucnt);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				if(i>(*MenuOp).MenuTotal)
					(*MenuOp).curFlashRow=(*MenuOp).MenuTotal-1+(*MenuOp).StartLine;
				else{
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_LEFT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if((*MenuOp).Menucnt>i){		//可以向上跳一屏
				(*MenuOp).Menu-=i;
				(*MenuOp).Menucnt-=i;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushUpkeyFlag;
			}else{							//不够向上跳一屏
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_Enter:
				(*MenuOp).RetVal=KEY_Enter;
			return;

			case KEY_ReturnPrevious:
				(*MenuOp).RetVal=KEY_ReturnPrevious;
			return;

			case KEY_ReturnInitInterface:
				(*MenuOp).RetVal=KEY_ReturnInitInterface;
			return;

			case KEY_WaitTimeout:
				(*MenuOp).RetVal=KEY_WaitTimeout;
			return;
		}
	}
}


void MenuHandler_AddIco2(struct OperateAddIco2 *MenuOp)
{
	int i;
	unsigned char dispbuff[MaxByteOneLine+2];
	unsigned char keyval;

	clear_area_to_row((*MenuOp).StartLine,(*MenuOp).EndLine);
	while(1){
		if(((*MenuOp).flashflag&FlushOnlyHightRowFlag)!=0){
			if(((*MenuOp).flashflag&FlushDownkeyFlag)!=0){
				DispHighLightRow_to_x(16,(*MenuOp).curFlashRow-1);
			}else{
				DispHighLightRow_to_x(16,(*MenuOp).curFlashRow+1);
			}
			DispHighLightRow_to_x(16,(*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
			(*MenuOp).flashflag=0;
		}
		else if(((*MenuOp).flashflag&FlushAllFlag)!=0){
			(*MenuOp).flashflag=0;

			for(i=(*MenuOp).StartLine;i<(*MenuOp).EndLine&&i<(*MenuOp).MenuTotal+(*MenuOp).StartLine;i++){
				steaddraw_pic_to_row_x(((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->Ico,0,i,12,12);
				my_strcpy_fillspace(dispbuff,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStr);
				text_out_to_row_x(i,16,dispbuff);
			}

			DispHighLightRow_to_x(16,(*MenuOp).curFlashRow);			//高亮
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue ;
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
			i=GetKeyNumVal(keyval);
			if(i<=(*MenuOp).MenuTotal){
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menu+=i-1;
				(*MenuOp).Menucnt=i-1;
				if((i-1)<((*MenuOp).EndLine-(*MenuOp).StartLine)-1){
					(*MenuOp).curFlashRow=(*MenuOp).StartLine+i-1;
				}else{
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}
				(*MenuOp).RetVal=KEY_Enter;
				return;
			}
			break;

			case KEY_UP:
			if((*MenuOp).Menucnt){				//未到第一个菜单
				(*MenuOp).Menu--;
				(*MenuOp).Menucnt--;
				if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;}
			}else{				//到了第一个菜单，下一个菜单是最后一个。
				(*MenuOp).Menu=(*MenuOp).Menu+((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine<(*MenuOp).MenuTotal+(*MenuOp).StartLine ? (*MenuOp).EndLine-1 : (*MenuOp).MenuTotal+(*MenuOp).StartLine-1;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_DOWN:
			if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1){				//未到最后一个菜单
				(*MenuOp).Menu++;
				(*MenuOp).Menucnt++;
				if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;}
			}else{				//到了最后一个菜单，下一个菜单是第一个。
				(*MenuOp).Menu=(*MenuOp).Menu-((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_RIGHT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if(((*MenuOp).Menucnt+i)<(*MenuOp).MenuTotal-1){				//可以向下跳一屏
				(*MenuOp).Menu+=i;
				(*MenuOp).Menucnt+=i;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}else{											//不够向下跳一屏
				(*MenuOp).Menu+=((*MenuOp).MenuTotal-1-(*MenuOp).Menucnt);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				if(i>(*MenuOp).MenuTotal)(*MenuOp).curFlashRow=(*MenuOp).MenuTotal-1+(*MenuOp).StartLine;
				else{(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;}
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_LEFT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if((*MenuOp).Menucnt>i){		//可以向上跳一屏
				(*MenuOp).Menu-=i;
				(*MenuOp).Menucnt-=i;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushUpkeyFlag;
			}else{							//不够向上跳一屏
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_Enter:
			(*MenuOp).RetVal=KEY_Enter;
			return;

			case KEY_ReturnPrevious:
			(*MenuOp).RetVal=KEY_ReturnPrevious;
			return;

			case KEY_ReturnInitInterface:
			(*MenuOp).RetVal=KEY_ReturnInitInterface;
			return;

			case KEY_WaitTimeout:
			(*MenuOp).RetVal=KEY_WaitTimeout;
			return;
		}
	}
}


void MenuHandler_AddIco(struct OperateAddIco *MenuOp)
{
	unsigned char i;
	unsigned char dispbuff[MaxByteOneLine+2];
	unsigned char keyval;

	clear_area_to_row((*MenuOp).StartLine,(*MenuOp).EndLine);
	while(1){
		if(((*MenuOp).flashflag&FlushOnlyHightRowFlag)!=0){
			if(((*MenuOp).flashflag&FlushDownkeyFlag)!=0){
				DispHighLightRow_to_x(16,(*MenuOp).curFlashRow-1);
			}else{
				DispHighLightRow_to_x(16,(*MenuOp).curFlashRow+1);
			}
			DispHighLightRow_to_x(16,(*MenuOp).curFlashRow);
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
			(*MenuOp).flashflag=0;
		}
		else if(((*MenuOp).flashflag&FlushAllFlag)!=0){
			(*MenuOp).flashflag=0;

			for(i=(*MenuOp).StartLine;i<(*MenuOp).EndLine&&i<(*MenuOp).MenuTotal+(*MenuOp).StartLine;i++){
				steaddraw_pic_to_row_x(((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->Ico,0,i,12,12);
				//steaddraw_pic_to_row_x(((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->0,i,12,12);
				my_strcpy_fillspace(dispbuff,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStr);
				text_out_to_row_x(i,16,dispbuff);
			}

			DispHighLightRow_to_x(16,(*MenuOp).curFlashRow);			//高亮
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue ;
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
			i=GetKeyNumVal(keyval);
			if(i<=(*MenuOp).MenuTotal){
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menu+=i-1;
				(*MenuOp).Menucnt=i-1;
				if((i-1)<((*MenuOp).EndLine-(*MenuOp).StartLine)-1){
					(*MenuOp).curFlashRow=(*MenuOp).StartLine+i-1;
				}else{
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}
				(*MenuOp).RetVal=KEY_Enter;
				return;
			}
			break;

			case KEY_UP:
			if((*MenuOp).Menucnt){				//未到第一个菜单
				(*MenuOp).Menu--;
				(*MenuOp).Menucnt--;
				if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;}
			}else{				//到了第一个菜单，下一个菜单是最后一个。
				(*MenuOp).Menu=(*MenuOp).Menu+((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine<(*MenuOp).MenuTotal+(*MenuOp).StartLine ? (*MenuOp).EndLine-1 : (*MenuOp).MenuTotal+(*MenuOp).StartLine-1;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_DOWN:
			if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1){				//未到最后一个菜单
				(*MenuOp).Menu++;
				(*MenuOp).Menucnt++;
				if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;}
			}else{				//到了最后一个菜单，下一个菜单是第一个。
				(*MenuOp).Menu=(*MenuOp).Menu-((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_RIGHT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if(((*MenuOp).Menucnt+i)<(*MenuOp).MenuTotal-1){				//可以向下跳一屏
				(*MenuOp).Menu+=i;
				(*MenuOp).Menucnt+=i;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}else{											//不够向下跳一屏
				(*MenuOp).Menu+=((*MenuOp).MenuTotal-1-(*MenuOp).Menucnt);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				if(i>(*MenuOp).MenuTotal)(*MenuOp).curFlashRow=(*MenuOp).MenuTotal-1+(*MenuOp).StartLine;
				else{(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;}
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_LEFT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if((*MenuOp).Menucnt>i){		//可以向上跳一屏
				(*MenuOp).Menu-=i;
				(*MenuOp).Menucnt-=i;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushUpkeyFlag;
			}else{							//不够向上跳一屏
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_Enter:
			(*MenuOp).RetVal=KEY_Enter;
			return;

			case KEY_ReturnPrevious:
			(*MenuOp).RetVal=KEY_ReturnPrevious;
			return;

			case KEY_ReturnInitInterface:
			(*MenuOp).RetVal=KEY_ReturnInitInterface;
			return;

			case KEY_WaitTimeout:
			(*MenuOp).RetVal=KEY_WaitTimeout;
			return;
		}
	}
}


void my_strcpy_fillspace2(unsigned char *dest,const unsigned char *src)
{
	unsigned char i=0;

	while(i<20&&*src>=' '){*dest=*src;dest++;src++;i++;}
	while(i<23){*dest=' ';dest++;i++;}
	*dest=0;
}

void MenuOperateSameFucHandler(struct MenuOperateSameFuc *MenuOp)
{
	unsigned char i;
	unsigned char dispbuff[MaxByteOneLine+2];
	unsigned char keyval;

	clear_area_to_row((*MenuOp).StartLine,(*MenuOp).EndLine);
	while(1){
		if(((*MenuOp).flashflag&FlushOnlyHightRowFlag)!=0){
			if(((*MenuOp).flashflag&FlushDownkeyFlag)!=0){
				if((*MenuOp).Ico!=NULL){DispHighLightRow_to_x(16,(*MenuOp).curFlashRow-1);}
				else {DispHighLightRow((*MenuOp).curFlashRow-1);}
			}else{
				if((*MenuOp).Ico!=NULL){DispHighLightRow_to_x(16,(*MenuOp).curFlashRow+1);}
				else {DispHighLightRow((*MenuOp).curFlashRow+1);}
			}
			
			if((*MenuOp).Ico!=NULL){DispHighLightRow_to_x(16,(*MenuOp).curFlashRow);}
			else {DispHighLightRow((*MenuOp).curFlashRow);}
			DisplayMenuPercent((struct MenuOperate *)MenuOp);

			(*MenuOp).flashflag=0;
		}
		else if(((*MenuOp).flashflag&FlushAllFlag)!=0){
			(*MenuOp).flashflag=0;

			for(i=(*MenuOp).StartLine;i<(*MenuOp).EndLine&&i<(*MenuOp).MenuTotal+(*MenuOp).StartLine;i++){
				if((*MenuOp).Ico!=NULL){steaddraw_pic_to_row_x((*MenuOp).Ico,0,i,12,12);}
				my_strcpy_fillspace2(dispbuff,((*MenuOp).Menu-(*MenuOp).curFlashRow+i)->MenuStr);
				if((*MenuOp).Ico!=NULL){text_out_to_row_x(i,16,dispbuff);}
				else{text_out_to_row(i,dispbuff);}
			}

			if((*MenuOp).Ico!=NULL){DispHighLightRow_to_x(16,(*MenuOp).curFlashRow);}
			else{DispHighLightRow((*MenuOp).curFlashRow);}
			DisplayMenuPercent((struct MenuOperate *)MenuOp);
		}

		keyval=GetKeyValue();
		if(keyval == KEY_NONE)continue ;
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
			i=GetKeyNumVal(keyval);
			if(i<=(*MenuOp).MenuTotal){
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menu+=i-1;
				(*MenuOp).Menucnt=i-1;
				if((i-1)<((*MenuOp).EndLine-(*MenuOp).StartLine)-1){
					(*MenuOp).curFlashRow=(*MenuOp).StartLine+i-1;
				}else{
					(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				}
				(*MenuOp).RetVal=KEY_Enter;
				return;
			}
			break;

			case KEY_UP:
			if((*MenuOp).Menucnt){				//未到第一个菜单
				(*MenuOp).Menu--;
				(*MenuOp).Menucnt--;
				if((*MenuOp).curFlashRow>(*MenuOp).StartLine){(*MenuOp).curFlashRow--;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;}
			}else{				//到了第一个菜单，下一个菜单是最后一个。
				(*MenuOp).Menu=(*MenuOp).Menu+((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine<(*MenuOp).MenuTotal+(*MenuOp).StartLine ? (*MenuOp).EndLine-1 : (*MenuOp).MenuTotal+(*MenuOp).StartLine-1;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_DOWN:
			if((*MenuOp).Menucnt<(*MenuOp).MenuTotal-1){				//未到最后一个菜单
				(*MenuOp).Menu++;
				(*MenuOp).Menucnt++;
				if((*MenuOp).curFlashRow<(*MenuOp).EndLine-1&&(*MenuOp).curFlashRow<(*MenuOp).MenuTotal+(*MenuOp).StartLine-1){(*MenuOp).curFlashRow++;(*MenuOp).flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;}
			}else{				//到了最后一个菜单，下一个菜单是第一个。
				(*MenuOp).Menu=(*MenuOp).Menu-((*MenuOp).MenuTotal-1);
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
			}
			(*MenuOp).flashflag|=FlushAllFlag;
			break;

			case KEY_RIGHT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if(((*MenuOp).Menucnt+i)<(*MenuOp).MenuTotal-1){				//可以向下跳一屏
				(*MenuOp).Menu+=i;
				(*MenuOp).Menucnt+=i;
				(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}else{											//不够向下跳一屏
				(*MenuOp).Menu+=((*MenuOp).MenuTotal-1-(*MenuOp).Menucnt);
				(*MenuOp).Menucnt=(*MenuOp).MenuTotal-1;
				if(i>(*MenuOp).MenuTotal)(*MenuOp).curFlashRow=(*MenuOp).MenuTotal-1+(*MenuOp).StartLine;
				else{(*MenuOp).curFlashRow=(*MenuOp).EndLine-1;}
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_LEFT:
			i=(*MenuOp).EndLine-(*MenuOp).StartLine;
			if((*MenuOp).Menucnt>i){		//可以向上跳一屏
				(*MenuOp).Menu-=i;
				(*MenuOp).Menucnt-=i;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushUpkeyFlag;
			}else{							//不够向上跳一屏
				(*MenuOp).Menu-=(*MenuOp).Menucnt;
				(*MenuOp).Menucnt=0;
				(*MenuOp).curFlashRow=(*MenuOp).StartLine;
				(*MenuOp).flashflag=FlushAllFlag|FlushDownkeyFlag;
			}
			break;

			case KEY_Enter:
			(*MenuOp).RetVal=KEY_Enter;
			return;

			case KEY_ReturnPrevious:
			(*MenuOp).RetVal=KEY_ReturnPrevious;
			return;

			case KEY_ReturnInitInterface:
			(*MenuOp).RetVal=KEY_ReturnInitInterface;
			return;

			case KEY_WaitTimeout:
			(*MenuOp).RetVal=KEY_WaitTimeout;
			return;
		}
	}
}


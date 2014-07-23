
#include "my_menuop.h"

#include <stdio.h>
#include <string.h>

#include "key.h"
#include "lcd.h"




extern unsigned char GetKeyNumVal(unsigned char keyval);


#if 1
// ���ֲ˵��������: ���1
// ��Щ��ʾ�����������:
// 1. 
// 2. 
// 3. 
// 4. 
void MenuOperate_response_keyLeft(struct my_MenuOp *op)
{
	signed char i;

	i=op->EndLine-op->StartLine;
	i=op->EndLine-op->StartLine;
	if(op->Menucnt>i){		//����������һ��
		op->Menucnt-=i;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->StartLine;
		op->flashflag=FlushAllFlag|FlushUpkeyFlag;
	}else{							//����������һ��
		op->Menucnt=0;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->StartLine;
		op->flashflag=FlushAllFlag|FlushDownkeyFlag;
	}
}


void MenuOperate_response_keyRight(struct my_MenuOp *op)
{
	signed char i;

	i=op->EndLine-op->StartLine;
	if((op->Menucnt+i)<op->MenuTotal-1){				//����������һ��
		op->Menucnt+=i;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->EndLine-1;
		op->flashflag=FlushAllFlag|FlushDownkeyFlag;
	}else{											//����������һ��
		op->Menucnt=op->MenuTotal-1;
		op->oldFlashRow=op->curFlashRow;
		if(i>op->MenuTotal){op->curFlashRow=op->MenuTotal-1+op->StartLine;}
		else{op->curFlashRow=op->EndLine-1;}
		op->flashflag=FlushAllFlag|FlushDownkeyFlag;
	}
}

void MenuOperate_response_keyDown(struct my_MenuOp *op)
{
	//signed char i;

	//i=op->EndLine-op->StartLine;

	if(op->Menucnt<op->MenuTotal-1){				//δ�����һ���˵�
		op->Menucnt++;
		if(op->curFlashRow<op->EndLine-1&&op->curFlashRow<op->MenuTotal+op->StartLine-1){
			op->oldFlashRow=op->curFlashRow;
			op->curFlashRow++;
			op->flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;
		}
	}else{				//�������һ���˵�����һ���˵��ǵ�һ����
		op->Menucnt=0;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->StartLine;
	}
	op->flashflag|=FlushAllFlag;
}

void MenuOperate_response_keyUp(struct my_MenuOp *op)
{
	//signed char i;

	//i=op->EndLine-op->StartLine;

	if(op->Menucnt){				//δ����һ���˵�
		op->Menucnt--;
		if(op->curFlashRow>op->StartLine){
			op->oldFlashRow=op->curFlashRow;
			op->curFlashRow--;
			op->flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;
		}
	}else{				//���˵�һ���˵�����һ���˵������һ����
		op->Menucnt=op->MenuTotal-1;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->EndLine<op->MenuTotal+op->StartLine ? op->EndLine-1 : op->MenuTotal+op->StartLine-1;
	}
	op->flashflag|=FlushAllFlag;
}



unsigned char MenuOperate_response_key1_9(struct my_MenuOp *op,unsigned char keyval)
{
	signed char i;

	i=GetKeyNumVal(keyval);
	if(i<=op->MenuTotal){
		op->Menucnt=i-1;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->Menucnt%(op->EndLine-op->StartLine)+op->StartLine;
		return KEY_Enter;
	}

	return KEY_NONE;
}


#else


// ���ֲ˵��������: ���2
// ��Щ��ʾ�����������:
// 1. ������EndLine �У������°�down ��,  �������StartLine �У�����һ ��
// 2. ������StartLine �У������ϰ�up ��,  �����������StartLine �У�����һ ��
// 3. ���ܸ�������һ�У���right ��,  �������StartLine �У�����һ ��
// 4. ���ܸ�������һ�У���left ��,  �������StartLine �У�����һ ��
void MenuOperate_response_keyLeft(struct my_MenuOp *op)
{
	signed char i;

	i=op->EndLine-op->StartLine;
	if(op->Menucnt-op->curFlashRow+op->StartLine>=i){		//����������һ��
		if(op->curFlashRow==op->StartLine){			//�����ǰ������==��ʾ�������ʼ��
			op->Menucnt-=i;
		}else{
			op->Menucnt-=op->curFlashRow-op->StartLine;
			op->Menucnt-=i;
		}
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->StartLine;
	}else{							//����������һ��
		op->Menucnt=0;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->StartLine;
	}

	op->flashflag=FlushLeftkeyFlag;
	if(op->MenuTotal>i+1)op->flashflag|=FlushAllFlag;
	else {op->flashflag|=FlushOnlyHightRowFlag;}
}


void MenuOperate_response_keyRight(struct my_MenuOp *op)
{
	signed char i;

	i=op->EndLine-op->StartLine;
	if((op->Menucnt+i+op->StartLine-op->curFlashRow)<op->MenuTotal){				//����������һ��
		if(op->curFlashRow==op->StartLine){
			op->Menucnt+=i;
		}else{
			op->Menucnt-=(op->curFlashRow-op->StartLine);
			op->Menucnt+=i;
		}
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->StartLine;
	}else{											//����������һ��
		if(op->MenuTotal>i+1){
			if(op->curFlashRow==op->StartLine){
				op->oldFlashRow=op->curFlashRow;
				op->curFlashRow=op->MenuTotal-1-op->Menucnt+op->StartLine;
				op->Menucnt=op->MenuTotal-1;
			}
		}else{
			op->oldFlashRow=op->curFlashRow;
			op->curFlashRow=op->MenuTotal-1+op->StartLine;
			op->Menucnt=op->MenuTotal-1;
		}
	}

	op->flashflag=FlushRightkeyFlag;
	if(op->MenuTotal>i+1)op->flashflag|=FlushAllFlag;
	else {op->flashflag|=FlushOnlyHightRowFlag;}
}

void MenuOperate_response_keyDown(struct my_MenuOp *op)
{
	signed char i;

	i=op->EndLine-op->StartLine;

	if(op->Menucnt<op->MenuTotal-1){				//δ�����һ���˵�
		op->Menucnt++;
		if(op->curFlashRow<op->EndLine-1&&op->curFlashRow<op->MenuTotal+op->StartLine-1){
			op->oldFlashRow=op->curFlashRow;
			op->curFlashRow++;
			op->flashflag=FlushOnlyHightRowFlag|FlushDownkeyFlag;
		}
		else {
			op->oldFlashRow=op->curFlashRow;
			op->curFlashRow=op->StartLine;
			if(op->MenuTotal>i+1)op->flashflag|=FlushAllFlag;
			else {op->flashflag|=FlushOnlyHightRowFlag;}
		}
	}else{				//�������һ���˵�����һ���˵��ǵ�һ����
		op->Menucnt=0;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->StartLine;
		if(op->MenuTotal>i+1)op->flashflag|=FlushAllFlag;
		else {op->flashflag|=FlushOnlyHightRowFlag;}
	}
}

void MenuOperate_response_keyUp(struct my_MenuOp *op)
{
	signed char i;

	i=op->EndLine-op->StartLine;

	if(op->Menucnt){				//δ����һ���˵�
		op->Menucnt--;
		if(op->curFlashRow>op->StartLine){
			op->oldFlashRow=op->curFlashRow;
			op->curFlashRow--;
			op->flashflag=FlushOnlyHightRowFlag|FlushUpkeyFlag;
		}
		else {
			op->oldFlashRow=op->curFlashRow;
			op->curFlashRow=op->EndLine-1;
			if(op->MenuTotal>i+1)op->flashflag|=FlushAllFlag;
			else {op->flashflag|=FlushOnlyHightRowFlag;}
		}
	}else{				//���˵�һ���˵�����һ���˵������һ����
		op->Menucnt=op->MenuTotal-1;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=(op->MenuTotal-1)%(op->EndLine-op->StartLine)+op->StartLine;
		if(op->MenuTotal>i+1)op->flashflag|=FlushAllFlag;
		else {op->flashflag|=FlushOnlyHightRowFlag;}
	}
}



unsigned char MenuOperate_response_key1_9(struct my_MenuOp *op,unsigned char keyval)
{
	signed char i;

	i=GetKeyNumVal(keyval);
	if(i<=op->MenuTotal){
		op->Menucnt=i-1;
		op->oldFlashRow=op->curFlashRow;
		op->curFlashRow=op->Menucnt%(op->EndLine-op->StartLine)+op->StartLine;
		return KEY_Enter;
	}

	return KEY_NONE;
}

#endif


void my_DisplayMenuPercent(struct my_MenuOp *op)
{
	unsigned char TotalLine;
	unsigned char CurLine;
	unsigned char StartLine;
	unsigned char EndLine;
	unsigned char Heigth;
	unsigned char total;
	unsigned char y,restline;

	if(op->MenuTotal>op->EndLine-op->StartLine){
		TotalLine=op->MenuTotal;
		CurLine=op->Menucnt;
		StartLine=op->StartLine;
		EndLine=op->EndLine;

		clear_area(ALinePixNum-11,ALineContainDotLine*StartLine,11,(EndLine-StartLine)*ALineContainDotLine);
		fill_rect(ALinePixNum-8,ALineContainDotLine*StartLine,5,2,mode_Black);
		fill_rect(ALinePixNum-8,ALineContainDotLine*EndLine-4,5,2,mode_Black);
		draw_line(ALinePixNum-6,ALineContainDotLine*StartLine,ALinePixNum-6,ALineContainDotLine*EndLine-4,1);

		total=(EndLine-StartLine)*ALineContainDotLine-8;		// 4+4+1+1
		Heigth=total/TotalLine;
		if(Heigth<1)Heigth=1;
		y=(total/TotalLine*CurLine)+3+ALineContainDotLine*StartLine;		//2+1
		restline=((total%TotalLine)*(CurLine+1))/TotalLine;
		y=y+restline;

		fill_rect(ALinePixNum-8,y,5,Heigth,mode_Black);
	}
}


void my_MenuHandler(struct my_MenuOperate *MenuOp)
{
	unsigned char i;
	unsigned char dispbuffLocal[MaxByteOneLine+2];
	unsigned char keyval;
	struct my_MenuFuc *pM;

	clear_area_to_row((*MenuOp).op.StartLine,(*MenuOp).op.EndLine);
	(*MenuOp).Menu-=(*MenuOp).op.Menucnt;

	while(1){
		if(((*MenuOp).op.flashflag&FlushOnlyHightRowFlag)!=0){
			DispHighLightRow((*MenuOp).op.oldFlashRow);
			DispHighLightRow((*MenuOp).op.curFlashRow);
			my_DisplayMenuPercent(&(*MenuOp).op);

			(*MenuOp).op.flashflag=0;
		}
		else if(((*MenuOp).op.flashflag&FlushAllFlag)!=0){
			pM=(*MenuOp).Menu+(*MenuOp).op.Menucnt;	//pM ָ��ǰ����������struct my_MenuFuc

			(*MenuOp).op.flashflag=0;
			for(i=(*MenuOp).op.StartLine;i<(*MenuOp).op.EndLine&&i<(*MenuOp).op.MenuTotal+(*MenuOp).op.StartLine;i++){
				my_strcpy_fillspace(dispbuffLocal,(pM-(*MenuOp).op.curFlashRow+i)->MenuStr);
				text_out_to_row(i,dispbuffLocal);
				if(pM-(*MenuOp).op.curFlashRow+i==(*MenuOp).Menu+(*MenuOp).op.MenuTotal-1){
					for(i++;i<(*MenuOp).op.EndLine&&i<(*MenuOp).op.MenuTotal+(*MenuOp).op.StartLine;i++){
						my_strcpy_fillspace(dispbuffLocal," ");
						text_out_to_row(i,dispbuffLocal);
					}
					break;
				}
			}

			DispHighLightRow((*MenuOp).op.curFlashRow);
			my_DisplayMenuPercent(&(*MenuOp).op);
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
			(*MenuOp).RetVal=MenuOperate_response_key1_9(&(*MenuOp).op,keyval);
			if((*MenuOp).RetVal==KEY_Enter){
				(*MenuOp).Menu+=(*MenuOp).op.Menucnt;
				return;
			}
			break;

			case KEY_UP:
			MenuOperate_response_keyUp(&(*MenuOp).op);
			break;

			case KEY_DOWN:
			MenuOperate_response_keyDown(&(*MenuOp).op);
			break;

			case KEY_RIGHT:
			MenuOperate_response_keyRight(&(*MenuOp).op);
			break;

			case KEY_LEFT:
			MenuOperate_response_keyLeft(&(*MenuOp).op);
			break;

			case KEY_Enter:
			(*MenuOp).Menu+=(*MenuOp).op.Menucnt;
			(*MenuOp).RetVal=keyval;
			return;

			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
			case KEY_WaitTimeout:
			(*MenuOp).RetVal=keyval;
			return;
		}
	}
}








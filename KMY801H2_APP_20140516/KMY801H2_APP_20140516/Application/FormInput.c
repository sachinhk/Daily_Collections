
#include "FormInput.h"



#include <string.h>
#include <stdio.h>
#include "key.h"
#include "lcd.h"
#include "Cursor.h"
#include "my_library.h"
#include "kmy_Time.h"





//�������ܣ�ͨ��abc �ķ�ʽ��һ��ascii �뵽pRead
//����������
//		1. pRead: ����ascii���ŵ�ָ��
//		2. key: ����ֵ
//		3. x: ��ĸ��ʾ��x  ��λ��
//		4. y: ��ĸ��ʾ��y  ��λ��
//����ֵ��
//		1. ������ļ�ֵ��key0��key9��KEYXING������!=key,�򷵻ش˼�ֵ
//		2. �������ȫ������0.
//ע�����
//		1. �˺������ػ������ʾ��ĸ��ʾ�ĵط�
//		2. ֻҪkey�����ֵ��key0��key9��KEYXING������֮һ��
//		    �˺��������pRead д��һ���ַ�
//		3. x����<=128,y����<=64, ������ĸ�޷���ʾ����Ļ��
unsigned char abcReadCode(unsigned char *pRead,unsigned char key,unsigned char x,unsigned char y)
{
	char prekey=0,curkey=0,keyflag;
	int PreviousMs,CurrentMs;
	unsigned char keyval;
	char flag=1;
	char curpos=0;

	const char *ABCcode[10]={
		" 0",
		",.@1/?:;\'\"|\\{[}]=+-_)(*&^%$#!~`<>",
		"abc2",
		"def3",
		"ghi4",
		"jkl5",
		"mno6",
		"pqrs7",
		"tuv8",
		"wxyz9"
	};

	PreviousMs=CurrentMs=TimeGetMilliseconds();

	while(1)
	{
		CurrentMs=TimeGetMilliseconds();
		if(CurrentMs-PreviousMs>=500)
		{
			clear_area(x,y,6,12);
			return 0;
		}

		if(flag==1)
		{
			flag=0;
			keyval=key;
		}
		else
		{
			keyval=GetKeyValue();
		}
		if(keyval==KEY_NONE)continue;

		PreviousMs=CurrentMs;
		KeyEventPost();

		switch(keyval)
		{
			case KEY0:		keyflag=1;prekey=curkey;curkey='0';	break;
			case KEY1:		keyflag=1;prekey=curkey;curkey='1';	break;
			case KEY2:		keyflag=1;prekey=curkey;curkey='2';	break;
			case KEY3:		keyflag=1;prekey=curkey;curkey='3';	break;
			case KEY4:		keyflag=1;prekey=curkey;curkey='4';	break;
			case KEY5:		keyflag=1;prekey=curkey;curkey='5';	break;
			case KEY6:		keyflag=1;prekey=curkey;curkey='6';	break;
			case KEY7:		keyflag=1;prekey=curkey;curkey='7';	break;
			case KEY8:		keyflag=1;prekey=curkey;curkey='8';	break;
			case KEY9:		keyflag=1;prekey=curkey;curkey='9';	break;
			case KEYXING:	keyflag=1;prekey=curkey;curkey='1';	break;

			case KEY_Enter:
			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
			clear_area(x,y,7,11);
			return 0;
		}

		if(keyflag==1)
		{
			//printf("curpos=%d\n",curpos);
			if(prekey!=0&&prekey!=curkey){clear_area(x,y,6,12);return keyval;}
			*pRead=*(ABCcode[curkey-'0']+curpos);
			curpos++;				//"abc2" ֮���������λ��
			if(curpos>strlen(ABCcode[curkey-'0'])-1)curpos=0;
			*(pRead+1)=0;
			text_out(x,y,(const unsigned char*)pRead);
			reverse_area(x,y,6,12);
		}

	}
}


//�������ܣ�ͨ��ABC �ķ�ʽ��һ��ascii �뵽pRead
//����������
//		1. pRead: ����ascii���ŵ�ָ��
//		2. key: ����ֵ
//		3. x: ��ĸ��ʾ��x  ��λ��
//		4. y: ��ĸ��ʾ��y  ��λ��
//����ֵ��
//		1. ������ļ�ֵ��key0��key9��KEYXING������!=key,�򷵻ش˼�ֵ
//		2. �������ȫ������0.
//ע�����
//		1. �˺������ػ������ʾ��ĸ��ʾ�ĵط�
//		2. ֻҪkey�����ֵ��key0��key9��KEYXING������֮һ��
//		    �˺��������pRead д��һ���ַ�
//		3. x����<=128,y����<=64, ������ĸ�޷���ʾ����Ļ��
unsigned char ABCReadCode(unsigned char *pRead,unsigned char key,unsigned char x,unsigned char y)
{
	char prekey=0,curkey=0,keyflag;
	int PreviousMs,CurrentMs;
	unsigned char keyval;
	char flag=1;
	char curpos=0;

	const char *ABCcode[10]={
		" 0",
		",.@1/?:;\'\"|\\{[}]=+-_)(*&^%$#!~`<>",
		"ABC2",
		"DEF3",
		"GHI4",
		"JKL5",
		"MNO6",
		"PQRS7",
		"TUV8",
		"WXYZ9"
	};

	PreviousMs=CurrentMs=TimeGetMilliseconds();

	while(1){
		CurrentMs=TimeGetMilliseconds();
		if(CurrentMs-PreviousMs>=500){clear_area(x,y,6,12);return 0;}

		if(flag==1)
		{
			flag=0;
			keyval=key;
		}
		else
		{
			keyval=GetKeyValue();
		}
		if(keyval==KEY_NONE)continue;

		PreviousMs=CurrentMs;
		KeyEventPost();

		switch(keyval)
		{
			case KEY0:		keyflag=1;prekey=curkey;curkey='0';	break;
			case KEY1:		keyflag=1;prekey=curkey;curkey='1';	break;
			case KEY2:		keyflag=1;prekey=curkey;curkey='2';	break;
			case KEY3:		keyflag=1;prekey=curkey;curkey='3';	break;
			case KEY4:		keyflag=1;prekey=curkey;curkey='4';	break;
			case KEY5:		keyflag=1;prekey=curkey;curkey='5';	break;
			case KEY6:		keyflag=1;prekey=curkey;curkey='6';	break;
			case KEY7:		keyflag=1;prekey=curkey;curkey='7';	break;
			case KEY8:		keyflag=1;prekey=curkey;curkey='8';	break;
			case KEY9:		keyflag=1;prekey=curkey;curkey='9';	break;
			case KEYXING:	keyflag=1;prekey=curkey;curkey='1';	break;

			case KEY_Enter:
			case KEYJING:
			case KEY_WaitTimeout:
			case KEY_ReturnPrevious:
			case KEY_ReturnInitInterface:
			clear_area(x,y,7,11);
			return 0;
		}

		if(keyflag==1)
		{
			//printf("curpos=%d\n",curpos);
			if(prekey!=0&&prekey!=curkey){clear_area(x,y,6,12);return keyval;}
			*pRead=*(ABCcode[curkey-'0']+curpos);
			curpos++;				//"abc2" ֮���������λ��
			if(curpos>strlen(ABCcode[curkey-'0'])-1)curpos=0;
			*(pRead+1)=0;
			text_out(x,y,(const unsigned char*)pRead);
			reverse_area(x,y,6,12);
		}

	}
}


void _123ReadCode(unsigned char *pRead,unsigned char keyval)
{
	switch(keyval)
	{
		case KEY0:		pRead[0]='0';	break;
		case KEY1:		pRead[0]='1';	break;
		case KEY2:		pRead[0]='2';	break;
		case KEY3:		pRead[0]='3';	break;
		case KEY4:		pRead[0]='4';	break;
		case KEY5:		pRead[0]='5';	break;
		case KEY6:		pRead[0]='6';	break;
		case KEY7:		pRead[0]='7';	break;
		case KEY8:		pRead[0]='8';	break;
		case KEY9:		pRead[0]='9';	break;
		case KEYXING:	pRead[0]='.';	break;
	}
}



//�������ܣ��л����뷨
//����������
//		1. VolidInputMode: ��Ч�����뷨
//		2. currentInputMode: ��ǰʹ�õ����뷨
//����ֵ��
//		1. �л�һ��֮������뷨
//ע�����
//		VolidInputMode�������������|�ķ�ʽ���룬����INPUT_MODE_123|INPUT_MODE_abc
//		�����뷨��INPUT_MODE_123��INPUT_MODE_abc֮���л�
unsigned char Switch_InputMode(unsigned char VolidInputMode,unsigned char currentInputMode)
{
	if(currentInputMode==INPUT_MODE_PY)
	{
		if(VolidInputMode&INPUT_MODE_123)return INPUT_MODE_123;
		else if(VolidInputMode&INPUT_MODE_abc)return INPUT_MODE_abc;
		else if(VolidInputMode&INPUT_MODE_ABC)return INPUT_MODE_ABC;
		else if(VolidInputMode&INPUT_MODE_BiHua)return INPUT_MODE_BiHua;
		else return INPUT_MODE_PY;
	}
	else if(currentInputMode==INPUT_MODE_123)
	{
		if(VolidInputMode&INPUT_MODE_abc)return INPUT_MODE_abc;
		else if(VolidInputMode&INPUT_MODE_ABC)return INPUT_MODE_ABC;
		else if(VolidInputMode&INPUT_MODE_BiHua)return INPUT_MODE_BiHua;
		else if(VolidInputMode&INPUT_MODE_PY)return INPUT_MODE_PY;
		else return INPUT_MODE_123;
	}
	else if(currentInputMode==INPUT_MODE_abc)
	{
		if(VolidInputMode&INPUT_MODE_ABC)return INPUT_MODE_ABC;
		else if(VolidInputMode&INPUT_MODE_BiHua)return INPUT_MODE_BiHua;
		else if(VolidInputMode&INPUT_MODE_PY)return INPUT_MODE_PY;
		else if(VolidInputMode&INPUT_MODE_123)return INPUT_MODE_123;
		else return INPUT_MODE_abc;
	}
	else if(currentInputMode==INPUT_MODE_ABC)
	{
		if(VolidInputMode&INPUT_MODE_BiHua)return INPUT_MODE_BiHua;
		else if(VolidInputMode&INPUT_MODE_PY)return INPUT_MODE_PY;
		else if(VolidInputMode&INPUT_MODE_123)return INPUT_MODE_123;
		else if(VolidInputMode&INPUT_MODE_abc)return INPUT_MODE_abc;
		else return INPUT_MODE_ABC;
	}
	else if(currentInputMode==INPUT_MODE_BiHua)
	{
		if(VolidInputMode&INPUT_MODE_PY)return INPUT_MODE_PY;
		else if(VolidInputMode&INPUT_MODE_123)return INPUT_MODE_123;
		else if(VolidInputMode&INPUT_MODE_abc)return INPUT_MODE_abc;
		else if(VolidInputMode&INPUT_MODE_ABC)return INPUT_MODE_ABC;
		else return INPUT_MODE_BiHua;
	}
	else
	{
		return INPUT_MODE_abc;
	}
}



unsigned char *GetinputModeString(unsigned char inputMode)
{
	if(inputMode==INPUT_MODE_123)return "123 ";
	else if(inputMode==INPUT_MODE_abc)return "abc ";
	else if(inputMode==INPUT_MODE_ABC)return "ABC ";
	else if(inputMode==INPUT_MODE_PY)return "ƴ��";
	else if(inputMode==INPUT_MODE_BiHua)return "�ʻ�";
	else return "abc ";
}


void Form_text_out(unsigned char x,unsigned char y,unsigned char *buff,unsigned char OneLineCanDisplayBytes)
{
	unsigned char len;

	len=strlen((const char*)buff);
	if(len>OneLineCanDisplayBytes)len-=OneLineCanDisplayBytes;
	else len=0;

	text_out_length(x,y,(buff+len),OneLineCanDisplayBytes);
}



//�������ܣ���ʾ���б���ַ��������
//����������
//		1. pt_display: ���б���ַ�����ʼָ��
//		2. OneLineCanDisplayBytes: ����ұ�����������ɵ�bytes ��Ŀ
//		3. result_xpos: ����ұ��������ʾ����ʼ��ַ
//����ֵ����
//ע�����
//		�����������ʾ��pt_display ��ʼ�����б������
void Form_display(struct sFormInput *pt_display,unsigned char OneLineCanDisplayBytes,unsigned char result_xpos)
{
	unsigned char i;

	for(i=0;i<=3;i++)
	{
		text_out(1,1+i*13,pt_display->Title);
		Form_text_out(result_xpos,1+i*13,pt_display->TitleResult,OneLineCanDisplayBytes);
		pt_display++;
	}
}


//�������ܣ�������б���ַ�����
//����������
//		1. firststr_x: ��߱������ʼ��ʾx  ��
//		2. secondstr_x: �ұ��������ʼ��ʾx  ��
//		3. OneLineCanDisplayBytes: ����ұ�����������ɵ�bytes ��Ŀ
//����ֵ����
//ע�������
void Form_ClearDisplayErea(unsigned char firststr_x,unsigned char secondstr_x,unsigned char OneLineCanDisplayBytes)
{
	unsigned char i;
	const unsigned char space[21]={"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"};

	for(i=0;i<=3;i++)
	{
		text_out_length(firststr_x,1+i*13,space,(secondstr_x-firststr_x)/6);
		text_out_length(secondstr_x,1+i*13,space,OneLineCanDisplayBytes);
	}
}



//�������ܣ��������
//����������
//		1. pFI: �������ṹ��ָ��
//		2. TitleNumber: �������Ŀ
//����ֵ��
//		1. ���ͣ�ڵ�һ��������ť����ȷ�ϼ�����FORMINPUT_FIRST_CONFIRM_RETURN
//		2. ���ͣ�ڵڶ���������ť����ȷ�ϼ�����FORMINPUT_SECOND_CONFIRM_RETURN
//		3. ��KEY_ReturnPrevious������FORMINPUT_KEY_ReturnPrevious
//		4. ��KEY_ReturnInitInterface������FORMINPUT_KEY_ReturnInitInterface
//		5. �������С��3 ����FORMINPUT_TITLE_NUMBER_ERROR
//		6. ������̫��(����16��ascii ��)����FORMINPUT_TITLE_TO_LONG
//ע�����
//		1. �˺���������������������Ϊ������ť����ʾ�����һ��
//		2. �ṹ���ԱMaxInputSize ����С�ڽṹ���ԱTitleResult ��������С
//		3. ���غ�����������ַ�����ڣ��ṹ���ԱTitleResult  ��
//		4. ���뺯��ʱ����ѳ�ԱTitleResult ��ʾ�ڱ���ڣ����н���֮ǰ
//		    ����Գ�ԱTitleResult  ���г�ʼ��
unsigned char FormInput(struct sFormInput *pFI,unsigned char TitleNumber)
{
	struct sFormInput *pt;
	struct sFormInput *pt_display;
	unsigned int i,cnt,temp;
	unsigned int cursor_x,cursor_y;
	unsigned char keyval;
	//unsigned char *pdisplay;	//��ʾ��ʼ�ĵ�ַ
	unsigned char *pMove;	//��괦buff �ĵ�ַ
	unsigned char OneLineCanDisplayBytes;	//һ�п�����ʾ���ֽ���
	unsigned char inputMode=INPUT_MODE_ABC;
	unsigned char buff[3];
	unsigned char InputModeSwitchFalg=0;
	unsigned char result_xpos;

	if(TitleNumber<3)return FORMINPUT_TITLE_NUMBER_ERROR;
	pt=pFI;
	for(cnt=i=0;i<TitleNumber-2;i++)
	{	//�õ��Title ��bytes ��С
		temp=strlen((const char*)(pt->Title));
		if(cnt<temp)cnt=temp;
		pt++;
	}
	cnt*=6;
	cnt+=1;
	if(cnt>128-6*5)return FORMINPUT_TITLE_TO_LONG;
	cursor_x=cnt+1;	//������
	OneLineCanDisplayBytes=(126-cursor_x)/6;
	cursor_y=1;

	clear_lcd();

	draw_line(0,0,128,0,1);	//�����к���
	for(i=0;i<4&&i<TitleNumber-2;i++)
	{
		temp=(i+1)*13;if(temp>63)temp=63;
		draw_line(0,temp,128,temp,1);
	}
	//����������
	if(TitleNumber>6)i=6;
	else i=TitleNumber;
	i=(i-2)*13;
	draw_line(0,0,0,i,1);
	draw_line(cnt,0,cnt,i,1);
	draw_line(127,0,127,i,1);

	pt=pFI;	//��ʾ�����ÿ�������Ӧ������
	//ptEnd=pFI+TitleNumber;
	result_xpos=cnt+1;
	for(i=0;i<=3&&i<TitleNumber-2;i++)
	{
		text_out(1,1+i*13,pt->Title);
		Form_text_out(result_xpos,1+i*13,pt->TitleResult,OneLineCanDisplayBytes);
		pt++;
	}


	//if(TitleNumber<=6){		//��ʾ���������ť
	pt=pFI+TitleNumber-2;
	text_out(((128/2)-strlen((const char*)(pt->Title))*6)/2,1+i*13,(const unsigned char*)pt->Title);
	pt++;
	text_out(((128/2)-strlen((const char*)(pt->Title))*6)/2+128/2,1+i*13,(const unsigned char*)pt->Title);
	//}

	pt=pFI;
	temp=strlen((const char*)pt->TitleResult)*6+cursor_x;
	if(temp>128)temp=128;
	Cursor_Init(temp,cursor_y,2,12,500);
	//pdisplay=pt->TitleResult;
	pMove=pt->TitleResult+strlen((const char*)(pt->TitleResult));
	while(1)
	{
		Cursor_Twinkle();

		keyval=GetKeyValue();
		if(keyval == KEY_NONE)continue;

		if(InputModeSwitchFalg==1)
		{
			InputModeSwitchFalg=0;
			Form_text_out(cursor_x,cursor_y,pt->TitleResult,OneLineCanDisplayBytes);
		}
		KeyEventPost();
		switch(keyval)
		{
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
			case KEYXING:
			if(strlen((const char*)(pt->TitleResult))>=pt->MaxInputSize)continue;

			if(inputMode==INPUT_MODE_123)
			{
				temp=strlen((const char*)(pt->TitleResult));
				if(temp>=pt->MaxInputSize)continue;
				_123ReadCode(buff,keyval);
				buff[1]=0;
			}
			else if(inputMode==INPUT_MODE_ABC)
			{
				againABC:
				cnt=temp=strlen((const char*)(pt->TitleResult));
				if(temp>=pt->MaxInputSize)continue;
				if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes-1;
				temp=temp*6+cursor_x;
				if(cnt>=OneLineCanDisplayBytes)
				{
					Form_text_out(cursor_x,cursor_y,pt->TitleResult+cnt-OneLineCanDisplayBytes+1,OneLineCanDisplayBytes);
				}
				keyval=ABCReadCode(buff,keyval,temp,cursor_y);
				buff[1]=0;
				if(keyval!=0)
				{
					my_strcat_insert(pMove,buff);
					pMove++;

					temp=strlen((const char*)(pt->TitleResult));
					if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes;
					temp=temp*6+cursor_x;
					Cursor_SetX(temp);
					Form_text_out(cursor_x,cursor_y,pt->TitleResult,OneLineCanDisplayBytes);
					goto againABC;
				}
			}
			else if(inputMode==INPUT_MODE_abc)
			{
				againabc:
				cnt=temp=strlen((const char*)(pt->TitleResult));
				if(temp>=pt->MaxInputSize)continue;
				if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes-1;
				temp=temp*6+cursor_x;
				if(cnt>=OneLineCanDisplayBytes)
				{
					Form_text_out(cursor_x,cursor_y,pt->TitleResult+cnt-OneLineCanDisplayBytes+1,OneLineCanDisplayBytes);
				}
				keyval=abcReadCode(buff,keyval,temp,cursor_y);
				buff[1]=0;
				if(keyval!=0)
				{
					my_strcat_insert(pMove,buff);
					pMove++;

					temp=strlen((const char*)(pt->TitleResult));
					if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes;
					temp=temp*6+cursor_x;
					Cursor_SetX(temp);
					Form_text_out(cursor_x,cursor_y,pt->TitleResult,OneLineCanDisplayBytes);
					goto againabc;
				}
			}
			else
			{
				buff[0]=0;
			}

			my_strcat_insert(pMove,buff);
			pMove++;

			temp=strlen((const char*)(pt->TitleResult));
			if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes;
			temp=temp*6+cursor_x;
			Cursor_SetX(temp);
			Form_text_out(cursor_x,cursor_y,pt->TitleResult,OneLineCanDisplayBytes);
			break;

			case KEY_UP:
			if(pt==pFI+TitleNumber-1)
			{
				if(TitleNumber>=6)i=4;
				else
				{
					i=TitleNumber-2;
				}
				Cursor_OnOff(CURSOR_OFF);
				temp=strlen((const char*)(pt->Title))*6;
				cnt=128/2-temp;
				cnt/=2;cnt+=128/2;
				reverse_area(cnt,i*13+1,temp,12);
				pt--;
				temp=strlen((const char*)(pt->Title))*6;
				cnt=128/2-temp;
				cnt/=2;
				reverse_area(cnt,i*13+1,temp,12);
			}
			else if(pt==pFI+TitleNumber-2)
			{
				if(TitleNumber>=6)i=4;
				else{i=TitleNumber-2;}
				temp=strlen((const char*)(pt->Title))*6;
				cnt=128/2-temp;
				cnt/=2;
				reverse_area(cnt,i*13+1,temp,12);
				pt--;
				Cursor_OnOff(CURSOR_ON);
			}
			else if(pt!=pFI)
			{
				if(pt<=pFI+3)
				{	//�����÷���
					pt--;
					cursor_y-=13;
					Cursor_SetY(cursor_y);
					temp=strlen((const char*)(pt->TitleResult));
					pMove=pt->TitleResult+temp;
					if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes;
					temp=temp*6+cursor_x;
					Cursor_SetX(temp);
					if(TitleNumber>6)pt_display=pFI+TitleNumber-2-4;
				}
				else
				{					//Ҫ������
					pt--;
					pt_display--;
					temp=strlen((const char*)(pt->TitleResult));
					pMove=pt->TitleResult+temp;
					if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes;
					temp=temp*6+cursor_x;
					Cursor_SetX(temp);
					Cursor_OnOff(CURSOR_OFF);
					Form_ClearDisplayErea(1,result_xpos,OneLineCanDisplayBytes);
					Form_display(pt_display,OneLineCanDisplayBytes,result_xpos);
					Cursor_OnOff(CURSOR_ON);
				}
			}
			break;

			case KEY_DOWN:
			if(pt<pFI+TitleNumber-3)
			{
				if(pt<=pFI+2)
				{	//�����÷���
					pt++;
					cursor_y+=13;
					Cursor_SetY(cursor_y);
					temp=strlen((const char*)(pt->TitleResult));
					pMove=pt->TitleResult+temp;
					if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes;
					temp=temp*6+cursor_x;
					Cursor_SetX(temp);
					pt_display=pFI;
				}
				else
				{	//Ҫ������
					pt++;
					pt_display++;
					temp=strlen((const char*)(pt->TitleResult));
					pMove=pt->TitleResult+temp;
					if(temp>=OneLineCanDisplayBytes)temp=OneLineCanDisplayBytes;
					temp=temp*6+cursor_x;
					Cursor_SetX(temp);
					Cursor_OnOff(CURSOR_OFF);
					Form_ClearDisplayErea(1,result_xpos,OneLineCanDisplayBytes);
					Form_display(pt_display,OneLineCanDisplayBytes,result_xpos);
					Cursor_OnOff(CURSOR_ON);
				}
			}
			else if(pt==pFI+TitleNumber-3)
			{
				if(TitleNumber>=6)i=4;
				else{i=TitleNumber-2;}
				Cursor_OnOff(CURSOR_OFF);
				pt++;
				temp=strlen((const char*)(pt->Title))*6;
				cnt=128/2-temp;
				cnt/=2;
				reverse_area(cnt,i*13+1,temp,12);
			}
			else if(pt==pFI+TitleNumber-2)
			{
				if(TitleNumber>=6)i=4;
				else{i=TitleNumber-2;}
				Cursor_OnOff(CURSOR_OFF);
				temp=strlen((const char*)(pt->Title))*6;
				cnt=128/2-temp;
				cnt/=2;
				reverse_area(cnt,i*13+1,temp,12);
				pt++;
				temp=strlen((const char*)(pt->Title))*6;
				cnt=128/2-temp;
				cnt/=2;cnt+=128/2;
				reverse_area(cnt,i*13+1,temp,12);
			}
			break;

			case KEY_DEL:
			temp=strlen((const char*)(pt->TitleResult));
			if(temp)
			{
				temp--;
				pMove--;
				(pt->TitleResult)[temp]=0;
				
				Form_text_out(cursor_x,cursor_y,pt->TitleResult,OneLineCanDisplayBytes);

				if(temp>=OneLineCanDisplayBytes)
				{
					temp=OneLineCanDisplayBytes;
					temp=temp*6+cursor_x;
				}
				else
				{
					temp=temp*6+cursor_x;
					clear_area(temp,cursor_y,6,12);
				}
				Cursor_SetX(temp);
			}
			break;

			case KEYJING:
			inputMode=Switch_InputMode(INPUT_MODE_123|INPUT_MODE_ABC|INPUT_MODE_abc,inputMode);
			text_out(cursor_x,cursor_y,GetinputModeString(inputMode));
			reverse_area(cursor_x,cursor_y,24,12);
			InputModeSwitchFalg=1;
			break;

			case KEY_RIGHT:
			
			break;

			case KEY_LEFT:
			
			break;

			case KEY_Enter:
			if(pt==pFI+TitleNumber-2)return FORMINPUT_FIRST_CONFIRM_RETURN;
			else if(pt==pFI+TitleNumber-1)return FORMINPUT_SECOND_CONFIRM_RETURN;
			break;

			case KEY_ReturnPrevious:
			return FORMINPUT_KEY_ReturnPrevious;

			case KEY_ReturnInitInterface:
			return FORMINPUT_KEY_ReturnInitInterface;
		}
	}
}











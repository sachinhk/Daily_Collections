
#include "FormInput.h"



#include <string.h>
#include <stdio.h>
#include "key.h"
#include "lcd.h"
#include "Cursor.h"
#include "my_library.h"
#include "kmy_Time.h"





//函数功能：通过abc 的方式读一个ascii 码到pRead
//函数参数：
//		1. pRead: 所读ascii码存放的指针
//		2. key: 按键值
//		3. x: 字母显示的x  轴位置
//		4. y: 字母显示的y  轴位置
//返回值：
//		1. 如果按的键值是key0到key9和KEYXING，并且!=key,则返回此键值
//		2. 其它情况全部返回0.
//注意事项：
//		1. 此函数返回会清除显示字母显示的地方
//		2. 只要key传入的值在key0到key9和KEYXING的其中之一，
//		    此函数便会在pRead 写入一个字符
//		3. x必须<=128,y必须<=64, 否则字母无法显示在屏幕上
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
			curpos++;				//"abc2" 之类的轮流的位置
			if(curpos>strlen(ABCcode[curkey-'0'])-1)curpos=0;
			*(pRead+1)=0;
			text_out(x,y,(const unsigned char*)pRead);
			reverse_area(x,y,6,12);
		}

	}
}


//函数功能：通过ABC 的方式读一个ascii 码到pRead
//函数参数：
//		1. pRead: 所读ascii码存放的指针
//		2. key: 按键值
//		3. x: 字母显示的x  轴位置
//		4. y: 字母显示的y  轴位置
//返回值：
//		1. 如果按的键值是key0到key9和KEYXING，并且!=key,则返回此键值
//		2. 其它情况全部返回0.
//注意事项：
//		1. 此函数返回会清除显示字母显示的地方
//		2. 只要key传入的值在key0到key9和KEYXING的其中之一，
//		    此函数便会在pRead 写入一个字符
//		3. x必须<=128,y必须<=64, 否则字母无法显示在屏幕上
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
			curpos++;				//"abc2" 之类的轮流的位置
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



//函数功能：切换输入法
//函数参数：
//		1. VolidInputMode: 生效的输入法
//		2. currentInputMode: 当前使用的输入法
//返回值：
//		1. 切换一次之后的输入法
//注意事项：
//		VolidInputMode这个参数必须以|的方式传入，比如INPUT_MODE_123|INPUT_MODE_abc
//		则输入法在INPUT_MODE_123和INPUT_MODE_abc之间切换
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
	else if(inputMode==INPUT_MODE_PY)return "拼音";
	else if(inputMode==INPUT_MODE_BiHua)return "笔画";
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



//函数功能：显示四行表格字符到表格中
//函数参数：
//		1. pt_display: 四行表格字符的起始指针
//		2. OneLineCanDisplayBytes: 表格右边输入框能容纳的bytes 数目
//		3. result_xpos: 表格右边输入框显示的起始地址
//返回值：无
//注意事项：
//		这个函数会显示从pt_display 开始的四行表格数据
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


//函数功能：清除四行表格字符区域
//函数参数：
//		1. firststr_x: 左边标题框起始显示x  轴
//		2. secondstr_x: 右边输入框起始显示x  轴
//		3. OneLineCanDisplayBytes: 表格右边输入框能容纳的bytes 数目
//返回值：无
//注意事项：无
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



//函数功能：表格输入
//函数参数：
//		1. pFI: 表格输入结构体指针
//		2. TitleNumber: 表格总数目
//返回值：
//		1. 表格停在第一个高亮按钮处按确认键返回FORMINPUT_FIRST_CONFIRM_RETURN
//		2. 表格停在第二个高亮按钮处按确认键返回FORMINPUT_SECOND_CONFIRM_RETURN
//		3. 按KEY_ReturnPrevious键返回FORMINPUT_KEY_ReturnPrevious
//		4. 按KEY_ReturnInitInterface键返回FORMINPUT_KEY_ReturnInitInterface
//		5. 表格总数小于3 返回FORMINPUT_TITLE_NUMBER_ERROR
//		6. 表格标题太长(超过16个ascii 码)返回FORMINPUT_TITLE_TO_LONG
//注意事项：
//		1. 此函数把最后两个表格数据作为高亮按钮，显示在最后一行
//		2. 结构体成员MaxInputSize 必须小于结构体成员TitleResult 缓冲区大小
//		3. 返回后，所有输入的字符存放于，结构体成员TitleResult  内
//		4. 进入函数时，会把成员TitleResult 显示于表格内，所有进入之前
//		    必须对成员TitleResult  进行初始化
unsigned char FormInput(struct sFormInput *pFI,unsigned char TitleNumber)
{
	struct sFormInput *pt;
	struct sFormInput *pt_display;
	unsigned int i,cnt,temp;
	unsigned int cursor_x,cursor_y;
	unsigned char keyval;
	//unsigned char *pdisplay;	//显示开始的地址
	unsigned char *pMove;	//光标处buff 的地址
	unsigned char OneLineCanDisplayBytes;	//一行可以显示的字节数
	unsigned char inputMode=INPUT_MODE_ABC;
	unsigned char buff[3];
	unsigned char InputModeSwitchFalg=0;
	unsigned char result_xpos;

	if(TitleNumber<3)return FORMINPUT_TITLE_NUMBER_ERROR;
	pt=pFI;
	for(cnt=i=0;i<TitleNumber-2;i++)
	{	//得到最长Title 的bytes 大小
		temp=strlen((const char*)(pt->Title));
		if(cnt<temp)cnt=temp;
		pt++;
	}
	cnt*=6;
	cnt+=1;
	if(cnt>128-6*5)return FORMINPUT_TITLE_TO_LONG;
	cursor_x=cnt+1;	//计算光标
	OneLineCanDisplayBytes=(126-cursor_x)/6;
	cursor_y=1;

	clear_lcd();

	draw_line(0,0,128,0,1);	//画所有横线
	for(i=0;i<4&&i<TitleNumber-2;i++)
	{
		temp=(i+1)*13;if(temp>63)temp=63;
		draw_line(0,temp,128,temp,1);
	}
	//画三条竖线
	if(TitleNumber>6)i=6;
	else i=TitleNumber;
	i=(i-2)*13;
	draw_line(0,0,0,i,1);
	draw_line(cnt,0,cnt,i,1);
	draw_line(127,0,127,i,1);

	pt=pFI;	//显示标题和每个标题对应的内容
	//ptEnd=pFI+TitleNumber;
	result_xpos=cnt+1;
	for(i=0;i<=3&&i<TitleNumber-2;i++)
	{
		text_out(1,1+i*13,pt->Title);
		Form_text_out(result_xpos,1+i*13,pt->TitleResult,OneLineCanDisplayBytes);
		pt++;
	}


	//if(TitleNumber<=6){		//显示最后两个按钮
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
				{	//还不用翻屏
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
				{					//要翻屏了
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
				{	//还不用翻屏
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
				{	//要翻屏了
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











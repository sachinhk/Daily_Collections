#include "Tool.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kmy_Time.h"

#include "key.h"
#include "lcd.h"
#include "main.h"
#include "my_library.h"
#include "gprs.h"
#include "MenuOperate.h"

#include "PhoneBook.h"
#include "global_extern_variable.h"


static void DeleteAbuff0000(char *abuff)
{
	char i,*t;
	char flag=0;

	t=abuff;
	for(i=0;i<50;i++){
		if((*t<'0'||*t>'9')&&*t!='.')break;
		if(*t=='.')flag=1;
		t++;
	}
	t--;

	for(;i>0&&flag;i--){
		if(*t=='0')*t=0;
		else{
			if(*t=='.')*t=0;
			break;
		}
		t--;
	}
}

static void calculate_result(unsigned char * abuff,const char *bbuff,const char *jjcc)
{
	double a,b;

	a=atof((char const *)abuff);
	b=atof((char const *)bbuff);
	if(jjcc[0]=='+')a=a+b;
	else if(jjcc[0]=='-')a=a-b;
	else if(jjcc[0]=='*')a=a*b;
	else a=a/b;

	clear_area_to_row(1,3);
	#ifdef DEBUG_PRINTF
	printf("a=%f\n",a);
	#endif
	sprintf((char *)abuff,"%f",a);
	abuff[49]=0;
	DeleteAbuff0000((char *)abuff);
	//text_out_to_row_fillSpace(1,abuff);
	//my_DisplayBuff(abuff,1,3);
	clear_area_to_row(1,3);
	_my_DisplayBuff(abuff,1,3);
}

static char Calculator(char *msg)//2.计算器
{
	unsigned char abuff[50];	//因为abuff  要做结果存储，所以要申请大一点，否则如果数值太大，会segmentation fault
	unsigned char bbuff[22];
	signed char jjcc[2]={0,0};
	signed char input09,inputcnt;
	signed char inputFlag;
	unsigned char keyval;
	
	enum {WaitFirst,WaitFirst_inputed,WaitSecond,WaitSecond_inputed}ope;

	ope=WaitFirst;
	inputcnt=0;
	clear_lcd();
	text_out_to_row_x_Language2(0,34,"【计算器】","Calculator");
	draw_line(0,12,132,12,1);
	draw_line(0,11,132,11,1);
	text_out_to_row_fillSpace(1,"0");

	while(1){
		keyval=GetKeyValue();
		if(keyval==0)continue;
		KeyEventPost();
		inputFlag=0;
		switch(keyval){
			case KEY0:		inputFlag=1;	input09= '0';break;			//号码键
			case KEY1:		inputFlag=1;	input09= '1';break;	
			case KEY2:		inputFlag=1;	input09= '2';break;
			case KEY3:		inputFlag=1;	input09= '3';break;
			case KEY4:		inputFlag=1;	input09= '4';break;
			case KEY5:		inputFlag=1;	input09= '5';break;
			case KEY6:		inputFlag=1;	input09= '6';break;
			case KEY7:		inputFlag=1;	input09= '7';break;
			case KEY8:		inputFlag=1;	input09= '8';break;
			case KEY9:		inputFlag=1;	input09= '9';break;
			case KEYJING:	inputFlag=1;	input09= '.';break;			//小数点键

			case KEY_UP:		inputFlag=2;	input09= '+';break;			//加
			case KEY_DOWN:		inputFlag=2;	input09= '-';break;			//减
			case KEY_LEFT:		inputFlag=2;	input09= '*';break;			//乘
			case KEY_RIGHT:		inputFlag=2;	input09= '/';break;			//除
			case KEYXING:	inputFlag=2;	input09= '&';break;

			case KEY_Enter:	//等于键
			if(ope==WaitSecond_inputed){
				if(strlen((char const *)abuff)<sizeof(bbuff)-3){
					calculate_result(abuff,(char const *)bbuff,(char const *)jjcc);
				}else{
					clear_area_to_row(1,3);
					text_out_to_row_fillSpace(2,"数据太大!无法计算!");
				}
				ope=WaitFirst_inputed;
				inputcnt=0;
			}else{				//如果只是输入了第一个数据，按确认则清0。
				clear_area_to_row(1,3);
				text_out_to_row_fillSpace(1,"0");
				ope=WaitFirst;
				inputcnt=0;
			}
			break;

			case KEY_DEL:	//删除返回键
			if(inputcnt){
				inputcnt--;
				if(ope==WaitFirst_inputed){abuff[inputcnt]=0;text_out_to_row_fillSpace(1,abuff);}
				else if(ope==WaitSecond_inputed){bbuff[inputcnt]=0;text_out_to_row_fillSpace(3,bbuff);}
			}else{
				//return 1;
			}

			case KEY_ReturnInitInterface:
			case KEY_ReturnPrevious:
			return keyval;
		}

		if(inputFlag==1&&inputcnt<sizeof(bbuff)-3){			//如果有0到9.输入并且没有超过规定的位数
			if(inputcnt==0&&input09=='.'){
				if(ope==WaitFirst||ope==WaitFirst_inputed){ope=WaitFirst_inputed;abuff[inputcnt++]='0';abuff[inputcnt++]='.';abuff[inputcnt]=0;text_out_to_row_fillSpace(1,abuff);}
				else if(ope==WaitSecond||ope==WaitSecond_inputed){ope=WaitSecond_inputed;bbuff[inputcnt++]='0';bbuff[inputcnt++]='.';bbuff[inputcnt]=0;text_out_to_row_fillSpace(3,bbuff);}
			}else if(inputcnt!=0||input09!='0'){				//如果输入的第一个数不为0
				if(ope==WaitFirst||ope==WaitFirst_inputed){ope=WaitFirst_inputed;abuff[inputcnt++]=input09;abuff[inputcnt]=0;text_out_to_row_fillSpace(1,abuff);}
				else if(ope==WaitSecond||ope==WaitSecond_inputed){ope=WaitSecond_inputed;bbuff[inputcnt++]=input09;bbuff[inputcnt]=0;text_out_to_row_fillSpace(3,bbuff);}
			}
		}

		if(inputFlag==2){
			if(ope==WaitFirst_inputed||ope==WaitSecond){				//如果输入了第一个并在等待第二个时，可以更换运算符。
				if(input09=='&'){if(jjcc[0]=='+')jjcc[0]='-';else if(jjcc[0]=='-')jjcc[0]='*';else if(jjcc[0]=='*')jjcc[0]='/';else jjcc[0]='+';}
				else jjcc[0]=input09;
				text_out_to_row_x_fillSpace(2,6,(unsigned char const *)jjcc);
				ope=WaitSecond;
				inputcnt=0;
			}else if(ope==WaitSecond_inputed){				//如果输入了第二个数据，则计算出结果，并可以连算。
				calculate_result(abuff,(char const *)bbuff,(char const *)jjcc);
				if(input09=='&')jjcc[0]='+';
				else jjcc[0]=input09;
				text_out_to_row_x_fillSpace(2,6,(unsigned char const *)jjcc);
				ope=WaitSecond;
				inputcnt=0;
			}
		}

		#ifdef DEBUG_PRINTF
		printf("inputFlag=%d, inputcnt=%d\n",inputFlag,inputcnt);
		#endif

	}
}


static unsigned char WeekDay(unsigned int year,unsigned char mouth, unsigned char day)
{
	unsigned char i,weekday,ruinian;
	unsigned int totaldays;
	unsigned char mouthTab[]={
		31,		//1月
		28,		//2月
		31,		//3月
		30,		//4月
		31,		//5月
		30,		//6月
		31,		//7月
		31,		//8月
		30,		//9月
		31,		//10月
		30,		//11月
		31 		//12月
	};

	if((year%4==0&&year%100!=0)||year%400==0)ruinian=1;
	else ruinian=0;
	for(totaldays=0,i=0;i<mouth-1;i++){
		totaldays+=mouthTab[i];
	}
	totaldays+=365*(year-1970)+(year-1970+2)/4+day-1;	//1970 年1 月1 日到现在的天数。星期四
	if(mouth<3&&ruinian)totaldays+=3;
	else totaldays+=4;

	weekday=totaldays%7;
	weekday<<=5;

	if(ruinian)mouthTab[1]=29;
	if(mouth<1)mouth=1;
	if(mouth>12)mouth=12;
	weekday|=mouthTab[mouth-1];

	return weekday;
}




//bug  如果有闰月，会返回两个闰月
static int LunarCalendar(int year,int month,int day,int *LunarCalendarDay)
{
	const int MonthAdd[12]={0,31,59,90,120,151,181,212,243,273,304,334};
	const unsigned int LunarCalendarTable[199]={
		0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A,		/*1901-1910*/
		0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754,		/*1911-1920*/
		0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E,		/*1921-1930*/
		0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48,		/*1931-1940*/    
		0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51,		/*1941-1950*/ 
		0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x6AD53C,		/*1951-1960*/ 
		0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46,		/*1961-1970*/ 
		0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50,		/*1971-1980*/
		0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB,		/*1981-1990*/
		0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,0x05AC53,0x0AB647,0x5936BC,0x092E50,0x0C9645,		/*1991-2000*/
		0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E,		/*2001-2010*/
		0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9,		/*2011-2020*/
		0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43,		/*2021-2030*/
		0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C,		/*2031-2040*/
		0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37,		/*2041-2050*/
		0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42,		/*2051-2060*/
		0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B,		/*2061-2070*/
		0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6,		/*2071-2080*/
		0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E,		/*2081-2090*/
		0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5		  		/*2091-2099*/
	};
	int Spring_NY,Sun_NY,StaticDayCount;	//Spring_NY 记录春节离当年元旦的天数。
	int index,flag;	//Sun_NY 记录阳历日离当年元旦的天数。
	//StaticDayCount记录大小月的天数 29 或30
	//index 记录从哪个月开始来计算。
	//flag 是用来对闰月的特殊处理。

	if(((LunarCalendarTable[year-1901] & 0x0060) >> 5) == 1){Spring_NY = (LunarCalendarTable[year-1901] & 0x001F) - 1;}
	else{Spring_NY = (LunarCalendarTable[year-1901] & 0x001F) - 1 + 31;}
	Sun_NY = MonthAdd[month-1] + day - 1;
	if( (!(year % 4)) && (month > 2))Sun_NY++;

	if (Sun_NY >= Spring_NY){		//阳历日在春节后（含春节那天）
		Sun_NY -= Spring_NY;
		month = 1;
		index = 1;
		flag = 0;
		if((LunarCalendarTable[year-1901]&(0x80000>>(index-1)))==0)StaticDayCount = 29; 
		else{StaticDayCount = 30;}

		while(Sun_NY >= StaticDayCount){
			Sun_NY -= StaticDayCount;
			index++;
			if(month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20) ){
				flag = ~flag;
				if(flag == 0)month++;
			}else{month++;}

			if((LunarCalendarTable[year - 1901] & (0x80000 >> (index-1)) ) ==0)StaticDayCount=29;
			else{StaticDayCount=30;}
		}
		day = Sun_NY + 1;
	}else{		//阳历日在春节前
		Spring_NY -= Sun_NY;
		year--;
		month = 12;
		if(((LunarCalendarTable[year - 1901] & 0xF00000) >> 20) == 0)index = 12;
		else{index = 13;}
		flag = 0;
		if((LunarCalendarTable[year - 1901]&(0x80000 >>(index-1)))==0)StaticDayCount = 29;
		else{StaticDayCount = 30;}
		while(Spring_NY > StaticDayCount){
			Spring_NY -= StaticDayCount;
			index--;
			if(flag == 0)month--;
			if(month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20))flag = ~flag;
			if( ( LunarCalendarTable[year - 1901] & (0x80000 >> (index-1)) ) ==0)StaticDayCount = 29;
			else StaticDayCount = 30;
		}
		day = StaticDayCount - Spring_NY + 1;
	}

	*LunarCalendarDay |= day;
	*LunarCalendarDay |= (month << 6);
	if(month==((LunarCalendarTable[year - 1901] & 0xF00000) >> 20))return 1;
	else{return 0;}
}


static char Calendar(char *msg)//3.万年历
{
	unsigned char Hxianmu=0,i,x,y;
	unsigned char ri1xinqi;	//一号是星期几
	unsigned char mondays;	//一个月有多少天
	unsigned char temp[MaxByteOneLine];
	unsigned char keyval;
	unsigned int year;
	unsigned char month,day;
	int LunarCalendarDay;
	const unsigned char shuxianYear[12][5]={"马年","羊年","猴年","鸡年","狗年","猪年","鼠年","牛年","虎年","兔年","龙年","蛇年"};
	const char *ChDay[]={"*",
		"初一","初二","初三","初四","初五",
		"初六","初七","初八","初九","初十",
		"十一","十二","十三","十四","十五",
		"十六","十七","十八","十九","二十",
		"廿一","廿二","廿三","廿四","廿五", 
		"廿六","廿七","廿八","廿九","三十"
	};
	const char *ChMonth[]={"*","正","二","三","四","五","六","七","八","九","十","十一","腊"};
	const unsigned char NumData[10][5]={		//宽×高（像素）: 4×5
		{0x1f,0x11,0x1f,0x00},	//_0
		{0x00,0x1f,0x00,0x00},	//_1
		{0x1d,0x15,0x17,0x00},	//_2
		{0x15,0x15,0x1f,0x00},	//_3
		{0x07,0x04,0x1f,0x00},	//_4
		{0x17,0x15,0x1d,0x00},	//_5
		{0x1f,0x15,0x1d,0x00},	//_6
		{0x01,0x01,0x1f,0x00},	//_7
		{0x1f,0x15,0x1f,0x00},	//_8
		{0x17,0x15,0x1f,0x00}	//_9
	};
	const unsigned char ZHNumData[180]={		//90*9，字模III，纵向取模，字节正序
		/*0xFF,0xFF,0xFF,0x80,0xB6,0xB6,0xB6,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,
		0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xDD,0xDD,0xDD,
		0xDD,0xDD,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xBE,0xB6,0xB6,0xB6,0xBE,0xFE,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0xBA,0x86,0xBE,0x86,0xBA,0x80,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xBE,0xAE,0xA8,0x86,0xAE,0xA0,0xBE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xEE,0xED,0xAB,0xCF,0xEB,0xED,0xEE,0xFF,0xFF,0xFF,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80*/

		0xff,0xff,0xff,0x01,0x6d,0x6d,0x6d,0x01,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,
		0xef,0xef,0xef,0xef,0xef,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xbf,0xbb,0xbb,0xbb,
		0xbb,0xbb,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0x7f,0x7d,0x6d,0x6d,0x6d,0x7d,0x7f,
		0xff,0xff,0xff,0xff,0xff,0xff,0x01,0x5d,0x61,0x7d,0x61,0x5d,0x01,0xff,0xff,0xff,
		0xff,0xff,0xff,0x7d,0x75,0x15,0x61,0x75,0x05,0x7d,0xff,0xff,0xff,0xff,0xff,0xff,
		0x77,0xb7,0xd5,0xf3,0xd7,0xb7,0x77,0xff,0xff,0xff,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,
	};

	/*struct tm *tm_ptr;
	time_t the_time;

	(void)time(&the_time);
	tm_ptr = gmtime(&the_time);
	year=1900+tm_ptr->tm_year;
	month=1+tm_ptr->tm_mon;
	day=tm_ptr->tm_mday;*/
	kmy_TimeGetTime(&year,&month,&day,NULL,NULL,NULL,NULL);

	clear_lcd();
	text_out_to_row_x_Language2(0,98,"农历","农历");
	draw_line(92,12,132,12,1);	//右边上两横
	draw_line(92,13,132,13,1);
	draw_line(90,0,90,66,1);	//中间两竖
	draw_line(91,0,91,66,1);
	draw_line(92,48,132,48,1);	//右边下两横
	draw_line(92,49,132,49,1);
	draw_pic(ZHNumData,0,12,90,9);	//星期图标

	PostKey(KEY_pseudo);
	while(1){
		keyval=GetKeyValue();
		if(keyval == KEY_NONE) continue ;
		if(keyval!=KEY_pseudo){
			KeyEventPost();
		}
		switch(keyval){
			case KEY_LEFT:	if(Hxianmu)Hxianmu--;else Hxianmu=2;break;
			case KEY_RIGHT: Hxianmu++;Hxianmu%=3;break;
			case KEY_UP:
			if(Hxianmu==0){		//高亮年处
				if(year<2099)year++;
				else year=2000;
			}else if(Hxianmu==1){				//高亮月处
				month++;
				if(month>12)month=1;
			}else{				//高亮日处
				if(month==2){
					if((year%4==0&&year%100!=0)||year%400==0){if(day<29)day++;}
					else{if(day<28)day++;else day=1;}
				}else if(month==4||month==6||month==9||month==11){
					if(day<30)day++;else day=1;
				}else{
					if(day<31)day++;else day=1;
				}
			}
			break;
			case KEY_DOWN:	
			if(Hxianmu==0){		//高亮年处
				if(year>2000)year--;
				else year=2099;
			}else if(Hxianmu==1){				//高亮月处
				if(month>1)month--;
				else month=12;
			}else{				//高亮日处
				if(month==2){
					if((year%4==0&&year%100!=0)||year%400==0){if(day>1)day--;else day=29;}
					else{if(day>1)day--;else day=28;}
				}else if(month==4||month==6||month==9||month==11){
					if(day>1)day--;else day=30;
				}else{
					if(day>1)day--;else day=31;
				}
			}
			break;

			case KEY_ReturnInitInterface:
			case KEY_ReturnPrevious:
			return keyval;
		}

		ri1xinqi=WeekDay(year,month,1);
		mondays=ri1xinqi&0x1f;
		ri1xinqi>>=5;
		sprintf((char *)temp,"%04d年%02d月%02d日",year,month,day);
		text_out_to_row_Language2(0,temp,temp);

		clear_area(0,21,90,49);		//清除日子的第一行
		for(x=3+ri1xinqi*13,y=22,i=1;i<=mondays;i++){
			if(i<10){
				draw_pic(NumData[i],x,y,4,5);
				if(i==day){reverse_area(x-1,y-1,6,7);}
			}else{
				draw_pic(NumData[i/10],x,y,4,5);
				draw_pic(NumData[i%10],x+4,y,4,5);
				if(i==day){reverse_area(x-1,y-1,10,7);}
			}
			x+=13;
			if(x>=82){x=3;y+=7;}
		}
		//text_out_to_row_x_Language2(4,98,shuxianYear[(year-1930)%12],shuxianYear[(year-1930)%12]);

		if(Hxianmu==0){
			reverse_area(0,0,24,11);
		}else if(Hxianmu==1){
			reverse_area(36,0,12,11);
		}else{
			reverse_area(60,0,12,11);
		}

		LunarCalendarDay=0;
		if(LunarCalendar(year,month,day,&LunarCalendarDay)){
			strcpy((char *)temp,"闰");
			strcat((char *)temp,ChMonth[(LunarCalendarDay & 0x3C0) >> 6]);
		}else strcpy((char *)temp,ChMonth[(LunarCalendarDay & 0x3C0) >> 6]);
		strcat((char *)temp,"月");

		//clear_area(92,18,36,13);
		clear_area(92,18,36,53);
		text_out_to_row_x_Language2(4,98,shuxianYear[(year-1930)%12],shuxianYear[(year-1930)%12]);
		draw_line(92,48,132,48,1);	//右边下两横
		draw_line(92,49,132,49,1);

		if(strlen((char const *)temp)>4)text_out(92,18,temp);
		else text_out(98,18,temp);

		strcpy((char *)temp,ChDay[LunarCalendarDay & 0x1F]);
		text_out(98,34,temp);

	}
}




char MainMenuScreen_ToolBox(char *msg)
{
	static struct MenuFuc_Language2 Menu[5]={
		{"1.电话本","1.PhoneBook",MainMenuScreen_ToolBox_PhoneBook},
		{"2.计算器","2.Calculator",Calculator},
		{"3.万年历","3.Calendar",Calendar},
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


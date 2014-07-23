
#include "Cursor.h"


#include "my_library.h"
#include "lcd.h"
#include "kmy_Time.h"




//光标的属性有
// 1. 闪动频率2. x坐标3. y坐标4.宽度5.高度
struct sCursor
{
	int TwinkleFrequecy_ms;
	int Cursor_x;
	int Cursor_y;

	unsigned int PreviousMs;
	unsigned int CurrentMs;

	char Cursor_Width;
	char Cursor_High;
	char TwinkleFlag;
};



#define Twinkle_Enable		0x01	//容许光标显示标志
#define Twinkle_ON			0x02	//光标在显示标志
#define Twinkle_OFF			~0x02	//光标不在显示标志


static struct sCursor cursor={0,0,0,0,0,0,0,~(Twinkle_Enable|Twinkle_ON)};



//函数功能：光标开关
//函数参数：
//		1. Flag: 开关标志，Flag==CURSOR_ON 开，其它关
//返回值：无
//注意事项：无
void Cursor_OnOff(char Flag)
{
	if(Flag==CURSOR_ON)
	{
		cursor.TwinkleFlag|=Twinkle_Enable;
		if((cursor.TwinkleFlag&Twinkle_ON)==0)
		{
			reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
		}
		cursor.TwinkleFlag|=Twinkle_ON;
	}
	else
	{
		cursor.TwinkleFlag&=~Twinkle_Enable;
		if((cursor.TwinkleFlag&Twinkle_ON)!=0)
		{
			reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
		}
		cursor.TwinkleFlag&=~Twinkle_ON;
	}
}

//函数功能：光标闪烁
//函数参数：无
//返回值：无
//注意事项：
//		光标闪烁的精度跟调用此函数的频率有关
//		1ms 调用1次，精度为+-1ms; 1S 调用1次，精度为+-1S
void Cursor_Twinkle(void)
{
	if(cursor.TwinkleFlag&Twinkle_Enable)
	{
		cursor.CurrentMs=TimeGetMilliseconds();
		if(cursor.CurrentMs>cursor.PreviousMs&&cursor.CurrentMs-cursor.PreviousMs>=cursor.TwinkleFrequecy_ms)
		{
			cursor.PreviousMs=cursor.CurrentMs;
			reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
			if(cursor.TwinkleFlag&Twinkle_ON)cursor.TwinkleFlag&=~Twinkle_ON;
			else cursor.TwinkleFlag|=Twinkle_ON;
		}
	}
}


//函数功能：设置光标x 轴
//函数参数：
//		Cursor_x: x 轴坐标值
//返回值：无
//注意事项：无
void Cursor_SetX(int Cursor_x)
{
	if(cursor.TwinkleFlag&Twinkle_ON)
	{
		reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	}

	cursor.Cursor_x=Cursor_x;
	reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	cursor.TwinkleFlag|=Twinkle_ON;
	cursor.PreviousMs=TimeGetMilliseconds();
}


//函数功能：设置光标y 轴
//函数参数：
//		Cursor_y: y 轴坐标值
//返回值：无
//注意事项：无
void Cursor_SetY(int Cursor_y)
{
	if(cursor.TwinkleFlag&Twinkle_ON)
	{
		reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	}

	cursor.Cursor_y=Cursor_y;
	reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	cursor.TwinkleFlag|=Twinkle_ON;
	cursor.PreviousMs=TimeGetMilliseconds();
}

//函数功能：设置光标宽度
//函数参数：
//		1. Cursor_Width: 光标宽度值
//返回值：无
//注意事项：无
void Cursor_SetWidth(char Cursor_Width)
{
	if(cursor.TwinkleFlag&Twinkle_ON)
	{
		reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	}

	cursor.Cursor_Width=Cursor_Width;
	reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	cursor.TwinkleFlag|=Twinkle_ON;
	cursor.PreviousMs=TimeGetMilliseconds();
}

//函数功能：设置光标高度
//函数参数：
//		1. Cursor_High: 光标高度值
//返回值：无
//注意事项：无
void Cursor_SetHigh(char Cursor_High)
{
	if(cursor.TwinkleFlag&Twinkle_ON)
	{
		reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	}

	cursor.Cursor_High=Cursor_High;
	reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	cursor.TwinkleFlag|=Twinkle_ON;
	cursor.PreviousMs=TimeGetMilliseconds();
}


//函数功能：设置光标闪烁频率
//函数参数：
//		1. TwinkleFrequecy_ms: 光标光标闪烁频率ms 值
//返回值：无
//注意事项：无
void Cursor_SetTwinkle_ms(int TwinkleFrequecy_ms)
{
	if(cursor.TwinkleFlag&Twinkle_ON)
	{
		reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	}

	cursor.TwinkleFrequecy_ms=TwinkleFrequecy_ms;
	reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	cursor.TwinkleFlag|=Twinkle_ON;
	cursor.PreviousMs=TimeGetMilliseconds();
}


//函数功能：光标初始化
//函数参数：
//		1. Cursor_x: x 轴坐标值
//		2. Cursor_y: y 轴坐标值
//		3. Cursor_Width: 光标宽度值
//		4. Cursor_High: 光标高度值
//		5. TwinkleFrequecy_ms: 光标光标闪烁频率ms 值
//返回值：无
//注意事项：无
void Cursor_Init(int Cursor_x,int Cursor_y,int Cursor_Width,int Cursor_High,int TwinkleFrequecy_ms)
{
	//if(cursor.TwinkleFlag&Twinkle_ON){
	//reverse_area(cursor.Cursor_x,cursor.Cursor_y,cursor.Cursor_Width,cursor.Cursor_High);
	//}

	cursor.Cursor_x=Cursor_x;
	cursor.Cursor_y=Cursor_y;
	cursor.Cursor_Width=Cursor_Width;
	cursor.Cursor_High=Cursor_High;
	cursor.TwinkleFrequecy_ms=TwinkleFrequecy_ms;
	cursor.PreviousMs=cursor.CurrentMs=TimeGetMilliseconds();
	cursor.TwinkleFlag=Twinkle_Enable|Twinkle_OFF;
}




#include "Cursor.h"


#include "my_library.h"
#include "lcd.h"
#include "kmy_Time.h"




//����������
// 1. ����Ƶ��2. x����3. y����4.���5.�߶�
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



#define Twinkle_Enable		0x01	//��������ʾ��־
#define Twinkle_ON			0x02	//�������ʾ��־
#define Twinkle_OFF			~0x02	//��겻����ʾ��־


static struct sCursor cursor={0,0,0,0,0,0,0,~(Twinkle_Enable|Twinkle_ON)};



//�������ܣ���꿪��
//����������
//		1. Flag: ���ر�־��Flag==CURSOR_ON ����������
//����ֵ����
//ע�������
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

//�������ܣ������˸
//������������
//����ֵ����
//ע�����
//		�����˸�ľ��ȸ����ô˺�����Ƶ���й�
//		1ms ����1�Σ�����Ϊ+-1ms; 1S ����1�Σ�����Ϊ+-1S
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


//�������ܣ����ù��x ��
//����������
//		Cursor_x: x ������ֵ
//����ֵ����
//ע�������
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


//�������ܣ����ù��y ��
//����������
//		Cursor_y: y ������ֵ
//����ֵ����
//ע�������
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

//�������ܣ����ù����
//����������
//		1. Cursor_Width: �����ֵ
//����ֵ����
//ע�������
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

//�������ܣ����ù��߶�
//����������
//		1. Cursor_High: ���߶�ֵ
//����ֵ����
//ע�������
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


//�������ܣ����ù����˸Ƶ��
//����������
//		1. TwinkleFrequecy_ms: �������˸Ƶ��ms ֵ
//����ֵ����
//ע�������
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


//�������ܣ�����ʼ��
//����������
//		1. Cursor_x: x ������ֵ
//		2. Cursor_y: y ������ֵ
//		3. Cursor_Width: �����ֵ
//		4. Cursor_High: ���߶�ֵ
//		5. TwinkleFrequecy_ms: �������˸Ƶ��ms ֵ
//����ֵ����
//ע�������
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



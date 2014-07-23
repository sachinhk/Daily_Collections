

#ifndef Cursor_h
#define Cursor_h



#define CURSOR_OFF		0x01
#define CURSOR_ON		0x02




void Cursor_OnOff(char Flag);
void Cursor_Twinkle(void);
void Cursor_SetX(int Cursor_x);
void Cursor_SetY(int Cursor_y);
void Cursor_SetWidth(char Cursor_Width);
void Cursor_SetHigh(char Cursor_High);
void Cursor_SetTwinkle_ms(int TwinkleFrequecy_ms);
void Cursor_Init(int Cursor_x,int Cursor_y,int Cursor_Width,int Cursor_High,int TwinkleFrequecy_ms);





#endif


#ifndef Key_H
#define Key_H


#include "kmy_KeyDrv.h"



#define GetKeyValue kmy_GetKeyValue



extern void KeyEventPost(void);
extern unsigned char WaitkeyAndAutoExit(unsigned int timeSecond);
unsigned char Waitkey_TillKeyPress(void);

//extern unsigned char GetKeyValue(void);
//void PostKey(unsigned char key);
void KeyEventPost_nobeep(void);
//unsigned char WaitkeyAndAutoExit2(unsigned int light,unsigned char beep,unsigned char autoexit);
//unsigned char WaitkeyAndAutoExit3(unsigned int light,unsigned char beep,unsigned char autoexit);
//unsigned char WaitkeyAndAutoExit_BT(unsigned int timeSecond);

void SetAutoExitTime(unsigned int second);
void SetBLOverTime(unsigned int second);

void BeepNormal(void);

void PostKey(unsigned char key);


#endif


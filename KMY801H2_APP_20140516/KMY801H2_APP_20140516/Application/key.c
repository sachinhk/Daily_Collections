#include "key.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kmy_LCDDrv.h"
#include "kmy_BeepDrv.h"
#include "kmy_KeyDrv.h"
#include "kmy_Time.h"
#include "setting.h"




void BeepNormal(void)
{
	kmy_BeepOn();
	msleep(60);
	kmy_BeepOff();
}


void PostKey(unsigned char key)
{
	kmy_PostKey(key);
}

void KeyEventPost(void)
{
	if(beepKG[0]=='1'){BeepNormal();}
	kmy_LcdBlackLightOn();
	kmy_AutoOffLcdLight(120); 
	kmy_AutoPostKey(120); 
}


unsigned char WaitkeyAndAutoExit(unsigned int timeSecond)
{
	char keyval;

	kmy_LcdBlackLightOn();
	kmy_AutoOffLcdLight(timeSecond);
	kmy_AutoPostKey(timeSecond);
	while(1){
		keyval=GetKeyValue();
		if(keyval==KEY_NONE)continue;
		else break;	
//		if(keyval==KEY_Enter||keyval==KEY_ReturnInitInterface||keyval==KEY_ReturnPrevious||keyval==KEY_WaitTimeout)break;
	}

	KeyEventPost();
	return keyval;
}

unsigned char Waitkey_TillKeyPress(void)
{
	char keyval;

	kmy_LcdBlackLightOn();
	kmy_AutoOffLcdLight(0);
	kmy_AutoPostKey(0);
	while(1){
		keyval=GetKeyValue();
		if(keyval==KEY_NONE)continue;
		else break;
	}

	KeyEventPost();
	return keyval;
}



void KeyEventPost_nobeep(void)
{
	kmy_LcdBlackLightOn();
	kmy_AutoOffLcdLight(10);
	kmy_AutoPostKey(10);
}

void SetAutoExitTime(unsigned int second)
{
	kmy_AutoPostKey(second);
}

void SetBLOverTime(unsigned int second)
{
	kmy_AutoOffLcdLight(second);
}


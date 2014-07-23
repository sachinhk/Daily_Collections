#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FactoryTest.h"
#include "kmy_KeyDrv.h"
#include "kmy_LEDDrv.h"
#include "kmy_BeepDrv.h"
#include "kmy_MagcardPADrv.h"
#include "kmy_USART1Drv.h"
#include "kmy_USART2Drv.h"
#include "kmy_USART3Drv.h"
#include "kmy_PrinterDrv.h"
#include "kmy_EEPROMDrv.h"
#include "kmy_FlashDrv.h"
#include "kmy_Time.h"
#include "ShuRuFa.h"
#include "lcd.h"
#include "global_extern_variable.h"
#include "message.h"
#include "gprs.h"
#include "my_library.h"
#include "key.h"
#include "setting.h"

static struct strTestData gTestResult[17];
static u8 gTestCommentbuf[256];
static u16 gTestBufIndex = 0;
static u8 gLedTestStep = LED_BEEP;
static u8 gTestMode = TEST_FACTORY;


void print_item(int item)
{
	unsigned char printbuf[256];

	kmy_PrinterSetcommand(WidthZoom1,sizeof(WidthZoom1));
	kmy_PrinterSetcommand(HighZoom1,sizeof(HighZoom1));

	memset(printbuf, 0x00, sizeof(printbuf));
	if(Language[0]==OTHER_LANGUAGE)
	{
			sprintf((char*)printbuf, "test item:%s  ", gTestResult[item].m_title);
			kmy_PrinterString(printbuf);
			if(gTestResult[item].m_result == 0)
			{
				sprintf((char*)printbuf, "result:pass\r\n");
				kmy_PrinterString(printbuf);
			}
			else if(gTestResult[item].m_result == 1)
			{
				sprintf((char*)printbuf, "result:failure\r\n");
				kmy_PrinterString(printbuf);
			}
			else
			{
				sprintf((char*)printbuf, "result:unknow\r\n");
				kmy_PrinterString(printbuf);
			}
			if(gTestResult[item].m_commentlen)
			{
				kmy_PrinterString("comment:\r\n");
				memset(printbuf, 0x00, sizeof(printbuf));
				memcpy(printbuf, &gTestCommentbuf[gTestResult[item].m_commentindex],
					gTestResult[item].m_commentlen);
				strcpy((char*)printbuf, "\r\n");
				kmy_PrinterString(printbuf);
			}
	}
	else
	{
			sprintf((char*)printbuf, "测试项:%s   ", gTestResult[item].m_title);
			if(gTestResult[item].m_result == 0)
			{
				strcat((char*)printbuf, "测试结果:通过\r\n");
				kmy_PrinterString(printbuf);
			}
			else if(gTestResult[item].m_result == 1)
			{
				strcat((char*)printbuf, "测试结果:失败\r\n");
				kmy_PrinterString(printbuf);
			}
			else
			{
				strcat((char*)printbuf, "测试结果:未知\r\n");
				kmy_PrinterString(printbuf);
			}
			if(gTestResult[item].m_commentlen)
			{
				kmy_PrinterString("注释:\r\n");
				memset(printbuf, 0x00, sizeof(printbuf));
				memcpy(printbuf, &gTestCommentbuf[gTestResult[item].m_commentindex], gTestResult[item].m_commentlen);
				strcat((char*)printbuf, "\r\n");
				kmy_PrinterString(printbuf);
			}
	}
	kmy_PrinterString("-----------------------------\r\n");
}

char print_test_result(char *msg)
{
	u16 i = 0;
	u8 IMEI[32];
	u8 PrinterVersion[50];
	u8 buf[128];
	unsigned char mark;
	
	kmy_LcdClear();
	DrawTitle_to_row_Language2(0,"测试完成","test complete");
	DrawTitle_to_row_Language2(1,"输出结果","output result");
	CurProc = NULL;

	/*Get wireless module IMEI*/
	kmy_USART2ReceiveReset();
	kmy_USART2SendData("AT+GSN\r",  strlen("AT+GSN\r"));
	memset(IMEI, 0x00, sizeof(IMEI));
	kmy_USART2ReceiveData(IMEI, sizeof(IMEI) - 1, 1000);
	/*delete string "OK"*/
	if(strstr((char*)IMEI, "OK"))
	{
		memset(strstr((char*)IMEI, "OK"), 0x00, 2);
	}
	/*delete string "\r\n"*/
	if(strstr((char*)IMEI, "\r\n"))
	{
		i = strstr((char*)IMEI, "\r\n") - (char*)IMEI;
		memset(&IMEI[i], 0x00, 2);
		while(i < sizeof(IMEI) - 2)
		{
			IMEI[i] = IMEI[i + 2];
			i++;
		}
	}

	/*Get printer version*/
	memset(PrinterVersion, 0x00, sizeof(PrinterVersion));
	kmy_USART3ReceiveReset();
	kmy_USART3SendData("\x1B\x46",  2);
	kmy_USART3ReceiveData(PrinterVersion, sizeof(PrinterVersion) - 1, 1000);
	
	kmy_PrinterSetcommand(WidthZoom1,sizeof(WidthZoom1));
	kmy_PrinterSetcommand(HighZoom1,sizeof(HighZoom1));
//	kmy_PrinterBitmap(BoyaLogo,320,245,(384-320)/2);

	sprintf((char*)buf, "IMEI: %s", IMEI);
	kmy_PrinterString(buf);
	if(Language[0]==OTHER_LANGUAGE)
	{
		sprintf((char*)buf, "Printer version: %s", PrinterVersion);
	}
	else
	{
		sprintf((char*)buf, "打印机控制板程序版本信息: \r\n%s\r\n", PrinterVersion);
	}
	kmy_PrinterString(buf);
	for(i = 0; i < TESTITEMCOUNT - 1; i++)
	{
		print_item(i);
	}
	
	/*mark for reset to verify rtc*/
	mark='*';
	kmy_EepromWritePage(64 * 1024 - 1,&mark, 1);
	kmy_LcdClear();
	DrawTitle_to_row_Language2(0,"测试RTC功能","RTC test");
	DrawTitle_to_row_Language2(1,"请先关机","Please power off");	
	DrawTitle_to_row_Language2(2,"并拔掉电源和电池","And unplug the power");
	DrawTitle_to_row_Language2(3,"重新接入电源后再开机","and battery,then plug");	
	DrawTitle_to_row_Language2(4,"","the power and restart");
	while(1);
}

char test_rtc(char *msg)
{
	gTestResult[8].m_result = 2;
	gTestResult[8].m_commentindex = gTestBufIndex;
	gTestResult[8].m_commentlen = 0;
	kmy_TimeSetTime(2014, 3, 27, 10, 10, 10);
	CurProc = print_test_result;
	return 0;
}

char test_font_display(char *msg)
{
	int i = 0;
	u8  key;
	unsigned char fonttemp[512]={
		"20-27 \x20\x21\x22\x23\x24\x25\x26\x27\n28-2f \x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\n"
		"30-37 \x30\x31\x32\x33\x34\x35\x36\x37\n38-3f \x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\n"
		"40-47 \x40\x41\x42\x43\x44\x45\x46\x47\n48-4f \x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\n"
		"50-57 \x50\x51\x52\x53\x54\x55\x56\x57\n58-5f \x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\n"
		"60-67 \x60\x61\x62\x63\x64\x65\x66\x67\n68-6f \x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\n"
		"70-77 \x70\x71\x72\x73\x74\x75\x76\x77\n78-7f \x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\n"
		"80-87 \x80\x81\x82\x83\x84\x85\x86\x87\n88-8f \x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\n"
		"90-97 \x90\x91\x92\x93\x94\x95\x96\x97\n98-9f \x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\n"
		"a0-a7 \xa0\xa1\xA2\xA3\xA4\xa5\xa6\xa7\na8-af \xa8\xa9\xaa\xab\xac\xad\xae\xaf\n"
		"b0-b7 \xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\nb8-bf \xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\n"
		"c0-c7 \xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\nc8-cf \xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\n"
		"d0-d7 \xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\nd8-df \xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\n"
		"e0-e7 \xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\ne8-ef \xe8\xe9\xea\xeb\xec\xed\xee\xef\n"
		"f0-f7 \xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\nf8-ff \xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\n"
	};

	//my_charhexprintf_buff("aa",fonttemp,strlen(fonttemp));

	clear_lcd();
	DrawTitle_to_row_Language2(0,"字体测试","Font test");
	my_DisplayBuff(fonttemp,1,MaxRowOneScr);
	if(gTestMode != TEST_FACTORY)
	{
		CurProc = CurProc_Return;
		return WaitkeyAndAutoExit(1);
	}
	gTestResult[7].m_result = 0;
	gTestResult[7].m_commentindex = gTestBufIndex;
	gTestResult[7].m_commentlen = 0;
	CurProc = test_rtc;
	clear_lcd();
	DrawTitle_to_row_Language2(2,"通过[确认]", "success[Enter]");
	DrawTitle_to_row_Language2(3,"不通过[取消]", "failure[Return]");
	for(i = 0; i < 1500000; i++)
		{
		key = kmy_GetKeyValue();
	if(key == KEY_Enter)
	{
		gTestResult[7].m_result = 0;
		break;
	}
	if(key == KEY_ReturnPrevious)
	{
		gTestResult[7].m_result = 1;
		break;
	}
	else if(key == KEY_ReturnInitInterface)
			{
				CurProc = NULL;
				return key;
			}
		}
	return 0;
}

char test_call(char *msg)
{
	PasswordInputArgument arg;
	unsigned char keyval;
	unsigned char phonenumber[21];
	unsigned char atcmd[30];
	int i = 0;
	u8 key;

	arg.clearlcdflag=1;
	arg.DiplayRow=2;
	arg.keyval=KEY_NONE;
	arg.LocalTitle="输入号码";
	arg.OtherTitle="Input Number";
	arg.receive=phonenumber;
	arg.recSize=21;
	arg.xingflag=0;
	keyval=PassWordInput_MiddledDisplay(&arg);
	if(keyval!=KEY_Enter)goto FuctionExit;

	strcpy((char *)atcmd,"ATD");
	strcat((char *)atcmd,(const char *)phonenumber);
	strcat((char *)atcmd,";\r");
	//send_at_cmd(atcmd);
	gTestResult[6].m_result = 2;
	gTestResult[6].m_commentindex = gTestBufIndex;
	gTestResult[6].m_commentlen = 0;
	clear_lcd();
	DrawTitle_to_row_Language2(2,"正在拨号...","dialing...");
	
	gTestResult[6].m_result = 1;
	gTestResult[6].m_commentindex = gTestBufIndex;
	gTestResult[6].m_commentlen = 0;
	if(gTestMode == TEST_FACTORY)
	{
	clear_lcd();
	DrawTitle_to_row_Language2(2,"通过[确认]", "success[Enter]");
	DrawTitle_to_row_Language2(3,"不通过[取消]", "failure[Return]");
	send_at_cmd_and_receive(atcmd, sizeof(atcmd) - 1, 5000, "NO");
	for(i = 0;; i++)
		{
		key = kmy_GetKeyValue();
	if(key == KEY_ReturnPrevious)
	{
		gTestResult[6].m_result = 1;
		break;
	}
	else if(key == KEY_Enter)
	{
		gTestResult[6].m_result = 0;
		break;
	}
	else if(key == KEY_ReturnInitInterface)
	{
				CurProc = NULL;
				return key;
	}
	}
	}
	else
		{
	send_at_cmd_and_receive(atcmd, sizeof(atcmd) - 1, 2000, "NO");
	//clear_area_to_row(2,3);
	WaitkeyAndAutoExit(30);
		}
	DrawTitle_to_row_Language2(2,"正在取消拨号!","Canceling");
	keyval=SendAtCommand_UntillSuccess("ATH\r");
	clear_area_to_row(2,3);
	if(keyval==SendAtCommand_OK)DrawTitle_to_row_Language2(2,"已取消拨号!","Canceled");
	else DrawTitle_to_row_Language2(2,"取消拨号失败!","Cancel failure");

	if(gTestMode != TEST_FACTORY)
	{

		CurProc = CurProc_Return;
		return WaitkeyAndAutoExit(3);
	}
	keyval=WaitkeyAndAutoExit(3);

	CurProc = test_font_display;
	

	FuctionExit:

	CurProc = test_font_display;
	
	return keyval;
}

char test_batvol(char *msg)
{
	unsigned char Buf[100];
	unsigned char retval;
	char *p;
	int Electricity;
	int seconds;
	u32 timetmp;

	
	clear_lcd();
	DrawTitle_to_row_Language2(2,"电池电压","BatVoltage");

	seconds=0;
	retval = 0;

	gTestResult[5].m_result = 1;
	gTestResult[5].m_commentindex = gTestBufIndex;
	gTestResult[5].m_commentlen = 0;
	timetmp=kmy_TimeGetMilliseconds();
	while(1)
	{
		Electricity=kmy_TimeGetRtcSeconds();
		if(seconds!=Electricity)
		{
			seconds=Electricity;
			
			strcpy((char*)Buf,"AT+QADC?\r");	//先通过AT 指令取得电量
			retval=send_at_cmd_and_receive(Buf,sizeof(Buf)-3,100,"OK");
			if(retval == TCP_OK)
			{
				printf("%s\r\n",Buf);
				p=strstr((char *)Buf,"+QADC:");	//+QADC:1,2088
				if(p!=NULL)
				{
					p=strstr(p,",");
					if(p!=NULL)
					{
						p++;
						Electricity=atoi(p);
						Electricity*=4;
						sprintf((char *)Buf,"%d mV",Electricity);
						strcpy((char*)&gTestCommentbuf[gTestBufIndex],(char*)Buf);
						gTestResult[5].m_commentlen = strlen((char*)Buf);
						gTestResult[5].m_result = 0;
						clear_area_to_row(3,3);
						DrawTitle_to_row_Language2(3,Buf,Buf);
					}
				}
			}
		}

		if(gTestMode == TEST_FACTORY && kmy_TimeGetMilliseconds() - timetmp > 2000)
		{
			// 3秒后退出
			break;
		}
		retval=kmy_GetKeyValue();
		if(retval==KEY_ReturnPrevious||retval==KEY_Enter)
		{
			break;
		}
		else if(retval == KEY_ReturnInitInterface)
			{
				CurProc = NULL;
				return retval;
			}
	}

	if(gTestMode != TEST_FACTORY)
	{
		CurProc = CurProc_Return;
		return WaitkeyAndAutoExit(1);
	}
	
	gTestBufIndex += gTestResult[5].m_commentlen;
	CurProc = test_call;
	return retval;
}

char test_magcard(char *msg)//3
{
	unsigned char tempbuff[300];
	unsigned char Magbuff2[80];
	unsigned char Magbuff3[170];
	u8  key;

	unsigned char retval;

	clear_lcd();
	DrawTitle_to_row_Language2(0,"刷卡测试","Brush card");
	memset(tempbuff,0,sizeof(tempbuff));
	memset(Magbuff2,0,sizeof(Magbuff2));
	memset(Magbuff3,0,sizeof(Magbuff3));
	retval=kmy_MagcardRead(NULL,Magbuff2,Magbuff3,15);
	if(retval!=ReadMagCardSuccess)
	{
		DrawTitle_to_row_Language2(2,"读卡失败","Read card Failure");
	}
	else
	{
		strcpy((char *)tempbuff,"Trace 1:");
		strcat((char *)tempbuff,"\nTrace 2:");
		strcat((char *)tempbuff,(char *)Magbuff2);
		strcat((char *)tempbuff,"\nTrace 3:");
		strcat((char *)tempbuff,(char *)Magbuff3);
		strcat((char *)tempbuff,"\n");
		my_charhexprintf_buff("tempbuff=",tempbuff,strlen((char *)tempbuff));
		retval=my_DisplayBuff(tempbuff,1,MaxRowOneScr);
		CurProc = test_batvol;
	}

	if(gTestMode != TEST_FACTORY)
	{

		CurProc = CurProc_Return;
		return WaitkeyAndAutoExit(1);
	}
	
	CurProc = test_batvol;

	kmy_LcdClear();
	text_out_to_row_Language2(1, "磁卡测试",  "Mag test");
	DrawTitle_to_row_Language2(2,"通过[确认]", "success[Enter]");
	DrawTitle_to_row_Language2(3,"不通过[取消]", "failure[Return]");

	while(1)
	{
		key=kmy_GetKeyValue();
		if(key == KEY_Enter)
		{
			kmy_LcdClear();
			DrawTitle_to_row_Language2(2,"通过", "success");
			gTestResult[0].m_result = 0;
			break;
		}
		else if(key == KEY_ReturnPrevious)
		{
			kmy_LcdClear();
			DrawTitle_to_row_Language2(2,"不通过", "failure");
			gTestResult[0].m_result = 1;
			break;
		}
		else if(key == KEY_ReturnInitInterface)
		{
			CurProc = NULL;
			return key;
		}
	}
	WaitkeyAndAutoExit(1);
	CurProc = test_batvol;
	gTestResult[4].m_commentindex = 0;
	gTestResult[4].m_commentlen = 0;
	return WaitkeyAndAutoExit(1);
}

char test_led(char *msg)
{
	u32 tmptime;
	u32 nowtime;
	u8 key;
	u8 showtext[16];
	LED_test_fun fun_on;
	LED_test_fun fun_off;
	u8 nextstep;
	u8 index;

	printf("1232a");
	
	switch(gLedTestStep)
	{
		case LED_BEEP:
			strcpy((char*)showtext, "Beep");
			fun_on = kmy_BeepOn;
			fun_off = kmy_BeepOff;
			nextstep = LED_DATA_LED;
			index = 1;
			break;

		case LED_DATA_LED:
			strcpy((char*)showtext, "Data Led");
			fun_on = kmy_LedDataLedOn;
			fun_off = kmy_LedDataLedOff;
			nextstep = LED_PAPER_LED;
			index = 2;
			break;

		case LED_PAPER_LED:
			strcpy((char*)showtext, "Paper Led");
			fun_on = kmy_LedPaparLedOn;
			fun_off = kmy_LedPaparLedOff;
			nextstep = LED_COMPLETE;
			index = 3;
			break;

		default:
			break;
	}
	gTestResult[index].m_result = 0;

	kmy_LcdClear();
	text_out_to_row_Language2(1, showtext, showtext);
	if(gTestMode == TEST_FACTORY)
	{
		DrawTitle_to_row_Language2(2,"通过[确认]", "success[Confirm]");
		DrawTitle_to_row_Language2(3,"不通过[取消]", "failure[Return]");
	}

	nowtime = kmy_TimeGetMilliseconds();


	while(1)
	{
		fun_on();
		tmptime = kmy_TimeGetMilliseconds();
		while(kmy_TimeGetMilliseconds() < tmptime + 60)
		{
			key = kmy_GetKeyValue();
			if(key == KEY_Enter&& gTestMode == TEST_FACTORY)
			{
				//通过
				CurProc = test_led;
				gLedTestStep = nextstep;
				if(LED_COMPLETE == nextstep)
				{
					CurProc = test_magcard;
				}
				kmy_LcdClear();
				text_out_to_row_Language2(1, showtext, showtext);
				DrawTitle_to_row_Language2(3,"通过", "success");
				fun_off();
				WaitkeyAndAutoExit(1);
				gTestResult[index].m_result = 0;
				gTestResult[index].m_commentindex = gTestBufIndex;
				gTestResult[index].m_commentlen = 0;
				return 0;
			}
			else if(key == KEY_ReturnPrevious&& gTestMode == TEST_FACTORY)
			{
				//不通过
				CurProc = test_led;
				gLedTestStep = nextstep;
				if(LED_COMPLETE == nextstep)
				{
					CurProc = test_magcard;
				}
				kmy_LcdClear();
				text_out_to_row_Language2(1,showtext, showtext);
				DrawTitle_to_row_Language2(3,"不通过", "failure");
				fun_off();
				WaitkeyAndAutoExit(1);
				gTestResult[index].m_result = 1;
				gTestResult[index].m_commentindex = gTestBufIndex;
				gTestResult[index].m_commentlen = 0;
				return 1;
			}
			else if(key == KEY_ReturnInitInterface)
			{
				CurProc = NULL;
				return key;
			}
			if(gTestMode != TEST_FACTORY
			&& (key != KEY_NONE))
			{
				return key;
			}
		}
		fun_off();
		tmptime = kmy_TimeGetMilliseconds();
		
		while(kmy_TimeGetMilliseconds()< tmptime + 500)
		{
			key = kmy_GetKeyValue();
			if(key == KEY_Enter&& gTestMode == TEST_FACTORY)
			{
				//通过
				CurProc = test_led;
				gLedTestStep = nextstep;
				if(LED_COMPLETE == nextstep)
				{
					CurProc = test_magcard;
				}
				kmy_LcdClear();
				text_out_to_row_Language2(1, showtext, showtext);
				DrawTitle_to_row_Language2(3,"通过", "success");
				fun_off();
				WaitkeyAndAutoExit(1);
				gTestResult[index].m_result = 0;
				gTestResult[index].m_commentindex = gTestBufIndex;
				gTestResult[index].m_commentlen = 0;
				return 0;
			}
			else if(key == KEY_ReturnPrevious&& gTestMode == TEST_FACTORY)
			{
				//不通过
				CurProc = test_led;
				gLedTestStep = nextstep;
				if(LED_COMPLETE == nextstep)
				{
					CurProc = test_magcard;
				}
				kmy_LcdClear();
				text_out_to_row_Language2(1, showtext, showtext);
				DrawTitle_to_row_Language2(3,"不通过", "failure");
				fun_off();
				WaitkeyAndAutoExit(1);
				gTestResult[index].m_result = 1;
				gTestResult[index].m_commentindex = gTestBufIndex;
				gTestResult[index].m_commentlen = 0;
				return 1;
			}
			else if(key == KEY_ReturnInitInterface)
			{
				CurProc = NULL;
				return key;
			}
			if(gTestMode != TEST_FACTORY
			&& (key != KEY_NONE))
			{
				return key;
			}
		}

		if((nowtime + 5000 < kmy_TimeGetMilliseconds())&& gTestMode == TEST_FACTORY)
		{
			/*5 second automaticly go the next step, and it it regarded as pass*/
			CurProc = test_led;
			gLedTestStep = nextstep;
			if(LED_COMPLETE == nextstep)
			{
				CurProc = test_magcard;
			}
			kmy_LcdClear();
			text_out_to_row_Language2(1, showtext, showtext);
			DrawTitle_to_row_Language2(3,"通过", "success");
			fun_off();
			WaitkeyAndAutoExit(1);
			gTestResult[index].m_result = 0;
			gTestResult[index].m_commentindex = gTestBufIndex;
			gTestResult[index].m_commentlen = 0;
			return 0;
		}
	}
}

char test_key(char *msg)
{
	unsigned char keyval;
	u8 iscomplete = 0;
	kmy_LcdClear();
	DrawTitle_to_row_Language2(0,"按键测试","Key Test");

	iscomplete = 0;
	while(1){
		keyval=kmy_GetKeyValue();
		if(keyval == KEY_NONE) continue;
		KeyEventPost();
		switch(keyval){
			case KEY0:				text_out_to_row_Language2(1,"Key is 0      ","Key is 0      ");		break;
			case KEY1:				text_out_to_row_Language2(1,"Key is 1      ","Key is 1      ");		break;
			case KEY2:				text_out_to_row_Language2(1,"Key is 2      ","Key is 2      ");		break;
			case KEY3:				text_out_to_row_Language2(1,"Key is 3      ","Key is 3      ");		break;
			case KEY4:				text_out_to_row_Language2(1,"Key is 4      ","Key is 4      ");		break;
			case KEY5:				text_out_to_row_Language2(1,"Key is 5      ","Key is 5      ");		break;
			case KEY6:				text_out_to_row_Language2(1,"Key is 6      ","Key is 6      ");		break;
			case KEY7:				text_out_to_row_Language2(1,"Key is 7      ","Key is 7      ");		break;
			case KEY8:				text_out_to_row_Language2(1,"Key is 8      ","Key is 8      ");		break;
			case KEY9:				text_out_to_row_Language2(1,"Key is 9      ","Key is 9      ");		break;
			case KEYXING:			text_out_to_row_Language2(1,"Key is *      ","Key is *      ");		break;
			case KEYJING:			text_out_to_row_Language2(1,"Key is #      ","Key is #      ");		break;

			case KEY_UP:			text_out_to_row_Language2(1,"Key is UP     ","Key is UP     ");		break;
			case KEY_DOWN:			text_out_to_row_Language2(1,"Key is DOWN   ","Key is DOWN   ");		break;
			case KEY_LEFT:			text_out_to_row_Language2(1,"Key is LEFT   ","Key is LEFT   ");		break;
			case KEY_RIGHT:			text_out_to_row_Language2(1,"Key is RIGHT  ","Key is RIGHT  ");		break;

			case KEY_L1:			text_out_to_row_Language2(1,"Key is L1     ","Key is L1     ");		break;
			case KEY_L2:			text_out_to_row_Language2(1,"Key is L2     ","Key is L2     ");		break;

			case KEY_R1:			text_out_to_row_Language2(1,"Key is R1     ","Key is R1     ");		break;
			case KEY_R2:			text_out_to_row_Language2(1,"Key is R2     ","Key is R2     ");		iscomplete = 1;break;
			case KEY_R3:			text_out_to_row_Language2(1,"Key is R3     ","Key is R3     ");		break;
			case KEY_R4:			text_out_to_row_Language2(1,"Key is R4     ","Key is R4     ");		break;
			case KEY_R5:			text_out_to_row_Language2(1,"Key is R5     ","Key is R5     ");		break;
			case KEY_R6:			text_out_to_row_Language2(1,"Key is R6     ","Key is R6     ");		break;

			default: break;	
		}
		if(iscomplete)
		{
			break;
		}
		KeyIsResponse();
	}
	
	if(gTestMode != TEST_FACTORY)
	{

		CurProc = CurProc_Return;
		return WaitkeyAndAutoExit(1);
	}
	
	kmy_LcdClear();
	text_out_to_row_Language2(1, "按键测试",  "Key test");
	DrawTitle_to_row_Language2(2,"通过[确认]", "success[Enter]");
	DrawTitle_to_row_Language2(3,"不通过[取消]", "failure[Return]");

	while(1)
	{
		keyval=kmy_GetKeyValue();
		if(keyval == KEY_Enter)
		{
			kmy_LcdClear();
			DrawTitle_to_row_Language2(2,"通过", "success");
			gTestResult[0].m_result = 0;
			break;
		}
		if(keyval == KEY_ReturnPrevious)
		{
			kmy_LcdClear();
			DrawTitle_to_row_Language2(2,"不通过", "failure");
			gTestResult[0].m_result = 1;
			break;
		}
	}
	CurProc = test_led;
	gTestResult[0].m_commentindex = 0;
	gTestResult[0].m_commentlen = 0;
	return WaitkeyAndAutoExit(1);
}

void init_test_data(void)
{
	memset(gTestResult, 0x00, sizeof(gTestResult));
	gTestBufIndex = 0;
	if(Language[0]==OTHER_LANGUAGE)
	{
		strcpy((char*)gTestResult[0].m_title, "key");
		strcpy((char*)gTestResult[1].m_title, "beep");
		strcpy((char*)gTestResult[2].m_title, "Data Led");
		strcpy((char*)gTestResult[3].m_title, "Paper Led");
		strcpy((char*)gTestResult[4].m_title, "Mag Card");
		strcpy((char*)gTestResult[5].m_title, "Voltage");
		strcpy((char*)gTestResult[6].m_title, "Call");
		strcpy((char*)gTestResult[7].m_title, "Font test");
		strcpy((char*)gTestResult[8].m_title, "RTC");
	}
	else
	{
		strcpy((char*)gTestResult[0].m_title, "按键");
		strcpy((char*)gTestResult[1].m_title, "蜂鸣器");
		strcpy((char*)gTestResult[2].m_title, "Data Led");
		strcpy((char*)gTestResult[3].m_title, "Paper Led");
		strcpy((char*)gTestResult[4].m_title, "磁卡");
		strcpy((char*)gTestResult[5].m_title, "电源电压");
		strcpy((char*)gTestResult[6].m_title, "拨号");
		strcpy((char*)gTestResult[7].m_title, "字体");
		strcpy((char*)gTestResult[8].m_title, "时钟");
	}
}

void check_rtc(void)
{
	unsigned int year;
	unsigned char month, day, hour, minute, second, weekday;
	unsigned char mark;
	unsigned char oldlanguage;

	kmy_EepromReadSequent(64 * 1024 - 1, &mark, 1);
	if(mark!='*')
	{
		return;
	}
	init_test_data();
	oldlanguage=Language[0];
	Language[0] = LOCAL_LANGUAGE;
	mark = 0;
	kmy_EepromWritePage(64 * 1024 - 4, &mark, 4);
	kmy_TimeGetTime(&year, &month, &day, 
		&hour, &minute, &second, &weekday);
	kmy_LcdClear();
	if(year >= 2014)
	{
		gTestResult[TESTITEMCOUNT - 1].m_result = 0;
		DrawTitle_to_row_Language2(2,"RTC测试通过","RTC test ok");
		print_item(TESTITEMCOUNT - 1);
		kmy_PrinterString("\r\n\r\n");
		WaitkeyAndAutoExit(3);
	}
	else
	{
		gTestResult[TESTITEMCOUNT - 1].m_result = 1;
		DrawTitle_to_row_Language2(2,"RTC测试未通过","RTC test fail");
		DrawTitle_to_row_Language2(3,"请检测纽扣电池","Check cell battery");
		print_item(TESTITEMCOUNT - 1);
		kmy_PrinterString("\r\n\r\n");
		WaitkeyAndAutoExit(3);
		mark = 0;
		kmy_EepromWritePage(64 * 1024 - 4, &mark, 4);
	}
	Language[0] = oldlanguage;
}

char start_factory_test(char *msg)
{	
	unsigned char keyval;
	
	kmy_LcdClear();	
	if(NetworkSwitch[0]==WiFi)
	{
		DrawTitle_to_row_Language2 (1, "当前网络为", "Current network is");
		DrawTitle_to_row_Language2 (2, "WiFi请切换", "WiFi,Please change to");
		DrawTitle_to_row_Language2 (3, "为GPRS后操作", "GPRS before operation");
		keyval=WaitkeyAndAutoExit (10);
		CurProc=ProcStandbyKey;
		InitFlag = 1;
		return keyval;
	}

	DrawTitle_to_row_Language2(0,"工厂测试","Factory test");
	DrawTitle_to_row_Language2(2,"按确认键继续","Press Confirm to");	
	DrawTitle_to_row_Language2(3,"","continue");
	keyval=WaitkeyAndAutoExit(60);
	if(keyval!=KEY_Enter)
	{
		CurProc=ProcStandbyKey;
		InitFlag = 1;
		return keyval;
	}
	gTestMode = TEST_FACTORY;
	Language[0] = LOCAL_LANGUAGE;/*factory test it it better to use local language*/
	DrawTitle_to_row_Language2(2,"即将开始....","Ready to start...");
	DrawTitle_to_row_Language2(3,"按键测试","Key test");
	WaitkeyAndAutoExit(1);
	init_test_data();
	check_rtc();
	CurProc = test_key;
	return 0;
}



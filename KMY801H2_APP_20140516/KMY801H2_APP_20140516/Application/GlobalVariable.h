/***********************************
Author:Satish
Date:2011-12-15 20:32
Remarks:global variable decleration 
LastModified:Date:2011-12-15 20:32
Remarks:global variable decleration
************************************/
#ifndef VAR_DEFSS          // Make sure this file is included only once
#define VAR_DEFSS 1
#ifndef VAR_DECLSS
# define _DECLL extern
# define _INIT(Y)
#else
# define _DECLL
# define _INIT(y)  = y
#endif 
_DECLL char Modem_data[1024],c; 
_DECLL int Front_End _INIT(0),Rear_End _INIT(0);  
_DECLL int RMessage_ID[25],RMIDFend _INIT(0),RMIDRend _INIT(0);
_DECLL int DMessage_ID[25],DMIDFend,DMIDRend;
_DECLL char SMessage_ID[25][180];
_DECLL char SMessage_MobNo[25][15];
_DECLL  int SMIDFend,SMIDRend _INIT(0);
_DECLL  int MDETFend,MDETRend,Modem_Process_Status;
_DECLL char MSGRead_Status[20],MSGDelete_Status[20],MSGSend_Status[20];
_DECLL char CALL_Status[20],REC_Status[10],CSQ_Status[20],CMGL_Status[20],CBC_Status[20],CLV_Status[20];
_DECLL  char MSC_Status[10];
_DECLL  char Dial_Status[20],Dial_No[30],GMessageCNo[15];
_DECLL  int RingCallStatus _INIT(0),GIMTTimeOut _INIT(0);
_DECLL  int ERROR_Count _INIT(0),Test_Variable _INIT(0),GSpeakerVolume _INIT(0);
_DECLL  int DataCTranStatus _INIT(0);
_DECLL  char Sig_status[50],Bat_status[50],GRecDCallData[20],GDataTOSend[160];

_DECLL  char data[1024],*ptrtemp;// =new char[1000];
_DECLL  int pos _INIT(0);
_DECLL  char MSGReadData[1000];
_DECLL  char num[13];
_DECLL  char Last_Command[50];
_DECLL	int LError_Count _INIT(0),SlnSlrPos,LCallStatusTC _INIT(0),LRECStatusTC _INIT(0),LMSGReadTC _INIT(0),LMSGDeleteTC _INIT(0),LMSGSendTC _INIT(0);
_DECLL  char Lc;
_DECLL  int Li _INIT(0),VarCOLP _INIT(0),speaker _INIT(0); 
_DECLL	char Proc_RecvBuf[6];
	     
	   
_DECLL  char datethread[15];

_DECLL int fd_Receiver;
_DECLL int LCommandLineCount _INIT(0),LUMNTimeC _INIT(0);


_DECLL int LsTime _INIT(0),GRFMFlag _INIT(0),MMSt _INIT(1);

//_DECLL int fd_ttySS1 _INIT(0);
#define		fd_ttySS1	3




_DECLL struct Auro_time
{
	unsigned int  year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min; 
	unsigned char sec;
	unsigned char weekday;
};


 
_DECLL int Ghs,Gms,Gss,Ghe,Gme,Gse;
//_DECLL 
 
_DECLL long LMDstat;
_DECLL long LMDend;
_DECLL long GStartTime,GEndTime;
_DECLL long CMGLSTime,CMGLETime,CSQSTime, CSQETime,CBCSTime, CBCETime,CALLTime;


#endif
int ReadDatasFromtty1(int fd,unsigned char *rcv_buf,int sec,int usec);
void write(int fd,char *str,int leng);
long difftime1(long et,long st);


long AuroGetTime();
void AuroGetDateTime(struct Auro_time *t);
 
 
char* Itoa(int value, char* str, int radix);

//static void FlashWaitForWriteEnd(void);
/*void kmy_TimeGetTime(unsigned int *year, unsigned char *mon,
unsigned char *day, unsigned char *hour,
unsigned char *min, unsigned char *sec,unsigned char *weekday);
function: get rtc time
parameter: *year:
*mon:
*day:
*hour:
*min:
*sec:
*weekday:
return value:no*/

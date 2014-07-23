#ifndef Message_H
#define Message_H




#define	RIGHT_return		0
#define	ERROR_return		1
#define	ERROR_Phonefull		2


#define	HOOK_zaiji			1
#define	HOOK_guaji			0


#define SendAtCommand_OK		0x01
#define SendAtCommand_ERROR	0x02



#define MESSAGE_UNREAD			0x01
#define MESSAGE_READED			0x02
#define MESSAGE_DELETE_ONE		0x04
#define MESSAGE_DELETE_ALL		0x08



#define MAX_MESSAGE_NUMBER	50



#define GET_OK		0x61
#define GET_ERROR	0x62





#define CHECK_MESSAGE_NO_MESSAGE		0x01
#define CHECK_MESSAGE_HAVE_MESSAGE		0x02
#define CHECK_PHONE_NO_PHONE			0x03
#define CHECK_PHONE_HAVE_PHONE			0x04

#define SIM900B_INIT_SUCCESS			0x05
#define SIM900B_INIT_FAILURE			0x06




struct sMessageNode{
	unsigned char index;		//消息索引号
	unsigned char readflag;		//已读标志
	unsigned char Number[21];	//消息日期
	unsigned int  datetime;		//消息日期
};



extern char NewMessageAct(void);
extern char NullSubs(void);
extern char abeyant(char *msg);
extern unsigned char dialnumber(unsigned char *DialStr);
extern char MainMenuScreen_Message(char *msg);



unsigned char GetMessage_state_number_datetime(unsigned char *buff,unsigned char *state,unsigned char *number,unsigned char *datetime);
char MessageDisplayContext(struct sMessageNode *pm);


unsigned char CheckMessageComing_method1(unsigned char *buff,unsigned char *index);
unsigned char CheckMessageComing_method2(unsigned char *buff,unsigned char *index);
unsigned char CheckPhoneComing(unsigned char *buff,unsigned char *phone_number);

char ReceiveMailBox(char *msg);//收件箱
unsigned char Sim900B_init(void);


#endif


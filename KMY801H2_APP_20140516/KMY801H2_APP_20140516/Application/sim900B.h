#ifndef sim900B_H
#define sim900B_H


#define Sim900B_ProccessOK		1
#define Sim900B_ProccessERROR	2





char SendMessage(char *Msg,const char *Phone);
char ReceiveMessageDecode_onlyPhoneNumber(const char *codebuff,char *PhoneNumber,const char *type);
char ReceiveMessageDecode(const char *codebuff,char *PhoneNumber,char *time,char *context,const char *type);

//成功发送返回0，错误返回1
unsigned char SendMessage_TextMode(const unsigned char *Context,const unsigned char *PhoneNumber);

#endif


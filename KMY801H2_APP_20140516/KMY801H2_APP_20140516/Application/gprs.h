#ifndef __GPRS_H
#define __GPRS_H



//#define		fd_ttyS1	2
#define		fd_ttyS1	3


#define SendAtCommand_OK		0x01
#define SendAtCommand_ERROR		0x02


#define TCP_OK							0x10
#define TCP_UserSelfReturn				0x14
#define TCP_SendAtTimeout				0x17
#define TCP_CONNECT_AND_RIGHT_RECEIVE	0x18
#define TCP_ERROR						0x21
#define TCP_ERROR2						0x22
#define TCP_ERROR3						0x23


#define TCP_ConnectFailure				0x50
#define TCP_ConnectOK					0x51
#define TCP_SendFailure					0x52
#define TCP_SendOK						0x53
#define TCP_ReceiveBuffNotEnough		0x54
#define TCP_ReceiveTimeOut				0x56
#define TCP_ReceiveOK					0x58

char SeperateIpAndPort(char *IPandPort, char *IP, char *Port);
unsigned int ReadDatasFromtty(int fd,unsigned char *rcv_buf,unsigned int size,int sec,int usec);
extern void send_at_cmd(const unsigned char *cmd);
extern unsigned char SendAtCommand_UntillSuccess(unsigned char *cmd);
extern unsigned char send_at_cmd_and_receive(unsigned char * CmdBuff,int CmdBuffSize,int Ms,const unsigned char *OkStr);
extern unsigned char send_at_cmd_and_return(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr);
extern unsigned char send_at_cmd_and_return_NotResposeKey(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr);
extern unsigned char tcp_Connect(unsigned char *ipaddr);
extern unsigned char tcp_send(const unsigned char *buff,int len);
extern unsigned char tcp_receive (unsigned char *recBuff, unsigned int recBuffSize, const unsigned char *okstr, unsigned short second);
extern void tcp_DisConnect(void);
extern unsigned char ReadPINPUKTimes(unsigned char PINTimes[2],unsigned char PUKTimes[3]);
extern void PUK_Required(void);
extern char GetIMEI(unsigned char *buff);
extern char GetCCID(unsigned char *buff);

extern unsigned char ReadDatasFromSIM300_OKstr(unsigned char *rcv_buf,unsigned long size,unsigned long sec,unsigned char *OKstr);
extern char set_apn(void);



#endif

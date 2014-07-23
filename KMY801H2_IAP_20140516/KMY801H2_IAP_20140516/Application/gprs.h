#ifndef __GPRS_H
#define __GPRS_H



#define GPRS_SendATOK					0x30
#define GPRS_SendATERROR				0x31
#define GPRS_SendATRecvTimeout			0x32

//  error for tcp connect
#define GPRS_TCPConnectAddressInvalid	0x33
#define GPRS_TCPConnectOK				0x34
#define GPRS_TCPConnectAlready			0x35
#define GPRS_TCPConnectPDPDEACT			0x36
#define GPRS_TCPConnectFail				0x37

// error for tcp send
#define GPRS_TCPSendError				0x38
#define GPRS_TCPSendFail				0x39
#define GPRS_TCPSendOK					0x3A

				
//   error for tcp receive
#define GPRS_TcpRecvOK					0x3B
#define GPRS_TcpRecvTimeout				0x3C
#define GPRS_TcpRecvBuffFull			0x3D
#define GPRS_TcpRecvClosed				0x3E

//5 for dial
#define GPRS_DialOK						0x3F
#define GPRS_DialBusy			   		0x40
#define GPRS_DialNoDialTone				0x41
#define GPRS_DialNoCarrier				0x42
#define GPRS_DialNoAnswer				0x43

//6 for non-block function
#define AT_StateIsNonBlock				0x44
#define AT_StateIsBlock					0x45
#define	AT_BuffFull						0x46
#define AT_IndexInvalid					0x47
#define	AT_RecvFinish					0x48
#define	AT_RecvUnFinish					0x49
#define	AT_RecvTimeOut					0x4A



#define TCP_OK							0x10
#define TCP_RECEIVE_FAILUE				0x13
#define TCP_UserSelfReturn				0x14
#define TCP_NotINITIALState				0x15
#define TCP_CanNotSetCIPHEAD			0x16
#define TCP_SendAtTimeout				0x17
#define TCP_CONNECT_AND_RIGHT_RECEIVE	0x18
#define TCP_ErrorUnknow					0x19
#define TCP_XOR_FAILUE					0x20
#define TCP_ERROR						0x21
#define TCP_ERROR2						0x22
#define TCP_ERROR3						0x23


#define TCP_ConnectFailure				0x50
#define TCP_ConnectOK					0x51
#define TCP_SendFailure					0x52
#define TCP_SendOK						0x53
#define TCP_ReceiveBuffNotEnough		0x54
#define TCP_ReceiveMallocFailure		0x55
#define TCP_ReceiveTimeOut				0x56
#define TCP_ReceiveIPDHeadError			0x57
#define TCP_ReceiveOK					0x58

#define GPRS                            0x47
#define WiFi                            0x57


char GPRSmenu(void);

extern void send_at_cmd(const unsigned char *cmd);
extern unsigned char send_at_cmd_and_receive(unsigned char * CmdBuff,int CmdBuffSize,int Ms,const unsigned char *OkStr);
extern unsigned char send_at_cmd_and_return(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr);
//extern unsigned char send_at_cmd_and_return_NotResposeKey(const unsigned char * cmd,int ms,const unsigned char *okstr,const unsigned char *errstr);
extern unsigned char tcp_Connect(unsigned char *ipaddr);
extern unsigned char tcp_send(const unsigned char *buff,int len);

unsigned char ReadDatasFromSIM300_OKstr(unsigned char *rcv_buf,unsigned long size,unsigned long Msec,unsigned char *OKstr);


void GPRS_Upgrade(void);
char WiFi_Upgrade(void);

char InputUpgradeURL(void);
char InputUpgradeIPandPort(void);
char InputUpgradeAPN(void);
char InputUpgradeApnUsername(void);
char InputUpgradeApnPassword(void);

void GetEepromVariable(void);
char WriteEepromVariable(void);


extern char Upgrade_Flag[];



#endif

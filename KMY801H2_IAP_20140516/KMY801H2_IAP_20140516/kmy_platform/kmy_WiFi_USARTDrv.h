#ifndef __KMY_WIFI_USARTDRV_H
#define __KMY_WIFI_USARTDRV_H


				   

void kmy_WiFi_USARTInit(unsigned long BaudRate);
//函数功能：串口初始化
//函数参数：brd: 波特率
//返回值：无
//注意事项：无


void kmy_WiFiOnOff (unsigned char OnOff);
//函数功能：WiFi开关
//函数参数：OnOff；1-->开，0-->关
//返回值：无
//注意事项：无



void kmy_WiFi_USARTSendString(const unsigned char *Buff);
//函数功能： 发送字符串
//函数参数：
//	Buff: 发送字符串的buff
//返回值：无
//注意事项：无




void kmy_WiFi_USARTSendData(const unsigned char *Buff, unsigned int SendSize);
//函数功能：  发送数据
//函数参数：
//	Buff:  发送数据的buff
//	SendSize:发送数据的长度
//返回值：无
//注意事项：无





void kmy_WiFi_USARTReceiveReset(void);
//函数功能： 把WiFi_USART已接受的所有字节丢弃
//函数参数：无
//返回值：无
//注意事项：无




unsigned int kmy_WiFi_USARTReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);
//函数功能： 接受串口的数据
//函数参数：
//	Buff: 接受数据存放的地方
//	RecSize: 接受多少个字节
//	timeout_MilliSeconds: 超时的毫秒数
//返回值：返回接受的总字节数
//注意事项：无

char kmy_WiFi_USARTCheckBuff(char *ExpectString);
void kmy_WiFi_USARTShowBuff(void);
unsigned int kmy_WiFi_USARTReadToEnd(char *Buff, unsigned int BuffSize);
int kmy_WiFi_USARTReadTo(char *Buff, short int BuffSize, char *EndString);

#endif

#ifndef __KMY_USART1DRV_H
#define __KMY_USART1DRV_H




void kmy_USART1Init(unsigned long BaudRate);
//函数功能：串口初始化
//函数参数：
//	brd: 波特率
//返回值：无
//注意事项：无




void kmy_USART1SendString(const unsigned char *Buff);
//函数功能： 发送字符串
//函数参数：
//	Buff: 发送字符串的buff
//返回值：无
//注意事项：无



void kmy_USART1SendData(const unsigned char *Buff, unsigned int SendSize);
//函数功能：  发送数据
//函数参数：
//	Buff:  发送数据的buff
//	SendSize:发送数据的长度
//返回值：无
//注意事项：无





//void kmy_USART1ReceiveReset(void);
//函数功能： 把USART1 已接受的所有字节丢弃
//函数参数：无
//返回值：无
//注意事项：无




//unsigned int kmy_USART1ReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);
//函数功能： 接受串口的数据
//函数参数：
//	Buff: 接受数据存放的地方
//	RecSize: 接受多少个字节
//	timeout_MilliSeconds: 超时的毫秒数
//返回值：返回接受的总字节数
//注意事项：无


#endif

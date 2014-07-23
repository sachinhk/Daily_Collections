#ifndef __KMY_USART3DRV_H
#define __KMY_USART3DRV_H





void kmy_USART3Init(unsigned long BaudRate);
//函数功能：串口初始化
//函数参数：brd: 波特率
//返回值：无
//注意事项：无



void kmy_USART3SendString(const unsigned char *Buff);
//函数功能： 发送字符串
//函数参数：Buff: 发送字符串的buff
//返回值：无
//注意事项：无





void kmy_USART3SendData(const unsigned char *Buff, unsigned int SendSize);
//函数功能：  发送数据
//函数参数：
//	Buff:  发送数据的buff
//	SendSize:发送数据的长度
//返回值：无
//注意事项：无





void kmy_USART3ReceiveReset(void);
//函数功能： 把USART3 已接受的所有字节丢弃
//函数参数：无
//返回值：无
//注意事项：无





unsigned int kmy_USART3ReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);
//函数功能： 接受串口的数据
//函数参数：
//	Buff: 接受数据存放的地方
//	RecSize: 接受多少个字节
//	timeout_MilliSeconds: 超时的毫秒数
//返回值：返回接受的总字节数
//注意事项：无





char kmy_USART3CheckBuff(char *ExpectString);
//函数功能： 检查USART3 接收的数据是否包含字符串ExpectString
//函数参数：
//	ExpectString: 检查字符串
//返回值：包含字符串返回      1
//        不包含字符串则返回  0
//注意事项：无





void kmy_USART3ShowBuff(void);
//函数功能： 显示USART3 接收缓冲区的所有内容
//函数参数：无
//返回值：无
//注意事项：无





unsigned int kmy_USART3ReadToEnd(char *Buff, unsigned int BuffSize);
//函数功能： 读出USART3 的数据
//函数参数：
//	Buff: 读出数据存放的地方
//	BuffSize: 读出最大字节数
//返回值：返回读出的总字节数
//注意事项：无





int kmy_USART3ReadTo(char *Buff, short int BuffSize, char *EndString);
//函数功能： 读USART3 的数据直到字符串EndString 结束
//函数参数：
//	Buff: 读出数据存放的地方
//	BuffSize: 读出最大字节数
//	EndString: 结束字符串
//返回值：成功   返回接受的总字节数
//        失败   返回负值
//注意事项：无

#endif

#ifndef __KMY_Time_H
#define __KMY_Time_H








void kmy_TimeInit(void);
//函数功能： 时间初始化
//函数参数：无
//返回值：无
//注意事项：无


void kmy_RCCInit(void);
//函数功能： 系统RCC  初始化
//函数参数：无
//返回值：无
//注意事项：无



unsigned long kmy_TimeGetMilliseconds(void);
//函数功能： 取当前系统(从开机到现在)  的毫秒数
//函数参数：无
//返回值：当前毫秒数
//注意事项：无


//unsigned long kmy_TimeGetRtcSeconds(void);
//函数功能： 取当前系统RTC 时间
//函数参数：无
//返回值：当前RTC 的秒数
//注意事项：无

/*
void kmy_TimeGetTime(unsigned int *year, unsigned char *mon,
unsigned char *day, unsigned char *hour,
unsigned char *min, unsigned char *sec,unsigned char *weekday);
*/
//函数功能： 得到rtc时间
//函数参数：
//	year: 年
//	mon: 月
//	day: 日
//	hour: 时
//	min: 分
//	sec: 秒
//	weekday:星期
//返回值：无
//注意事项：无


/*
void kmy_TimeSetTime(const unsigned int year, const unsigned char mon,
const unsigned char day, const unsigned char hour,
const unsigned char min, const unsigned char sec);
*/
//函数功能： 设置rtc时间
//函数参数：
//	year: 年
//	mon: 月
//	day: 日
//	hour: 时
//	min: 分
//	sec: 秒
//返回值：无
//注意事项：无





void sleep(unsigned long seconds);
//函数功能： 等待延时
//函数参数：
//seconds: 延时的秒数
//返回值：无
//注意事项：延时精度为+-2ms




void msleep(unsigned long ms);
//函数功能： 等待延时
//函数参数：
//ms: 延时的毫秒数
//返回值：无
//注意事项：延时精度为-1到ms数比如
//msleep(1) 实际延时时间为0.01-1ms，
//msleep(2)实际延时时间为1.01-2ms




//void usleep(unsigned long us);
//函数功能： 等待延时
//函数参数：
//us: 延时的微秒数
//返回值：无
//注意事项：延时精度为+-2us





#endif

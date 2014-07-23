#ifndef __KMY_Time_H
#define __KMY_Time_H






/*
    function��time initialization
    parameter��no
    return value��no
*/
void kmy_TimeInit(void);


/*
    function��system RCC initialization
    parameter��no
    return value��no
*/
void kmy_RCCInit(void);


/*
    function��get millisecond number
    parameter��no
    return value��current millisecond numer
*/
unsigned long kmy_TimeGetMilliseconds(void);


/*
    function��get current RTC time
    parameter��no
    return value��currrent RTC second number
*/
unsigned long kmy_TimeGetRtcSeconds(void);


/*
    function�� get rtc time
    parameter�� *year: 
    		   *mon: 
    		   *day: 
    		   *hour: 
    		   *min: 
    	   	   *sec: 
    		   *weekday:
    return value��no
*/
void kmy_TimeGetTime(unsigned int *year, unsigned char *mon,
unsigned char *day, unsigned char *hour,
unsigned char *min, unsigned char *sec,unsigned char *weekday);


/*
    function�� set rtc time
    parameter�� year: 
    		   mon: 
    	  	   day: 
    		   hour: 
    		   min: 
    		   sec: 
    return value��no
*/
void kmy_TimeSetTime(const unsigned int year, const unsigned char mon,
const unsigned char day, const unsigned char hour,
const unsigned char min, const unsigned char sec);


/*
    function��wait delay
    parameter��seconds: delay time in second
    return value��no
    note��delay accurate ��2ms
*/
void sleep(unsigned long seconds);


/*
    function��wait delay
    parameter��ms: delay time in millisecond
    return value��no
    note��delay accurate from -1 to ms
    		  msleep(1) actual delay time is 0.01-1ms��
    		  msleep(2) actual delay time is 1.01-2ms
*/
void msleep(unsigned long ms);


/*
    function��wait delay
    parameter��us: delay time in millisecond
    return value��no
    note��delay time accurate ��2us
*/
void usleep(unsigned long us);





#endif

#ifndef __KMY_Time_H
#define __KMY_Time_H






/*
    function£ºtime initialization
    parameter£ºno
    return value£ºno
*/
void kmy_TimeInit(void);


/*
    function£ºsystem RCC initialization
    parameter£ºno
    return value£ºno
*/
void kmy_RCCInit(void);


/*
    function£ºget millisecond number
    parameter£ºno
    return value£ºcurrent millisecond numer
*/
unsigned long kmy_TimeGetMilliseconds(void);


/*
    function£ºget current RTC time
    parameter£ºno
    return value£ºcurrrent RTC second number
*/
unsigned long kmy_TimeGetRtcSeconds(void);


/*
    function£º get rtc time
    parameter£º *year: 
    		   *mon: 
    		   *day: 
    		   *hour: 
    		   *min: 
    	   	   *sec: 
    		   *weekday:
    return value£ºno
*/
void kmy_TimeGetTime(unsigned int *year, unsigned char *mon,
unsigned char *day, unsigned char *hour,
unsigned char *min, unsigned char *sec,unsigned char *weekday);


/*
    function£º set rtc time
    parameter£º year: 
    		   mon: 
    	  	   day: 
    		   hour: 
    		   min: 
    		   sec: 
    return value£ºno
*/
void kmy_TimeSetTime(const unsigned int year, const unsigned char mon,
const unsigned char day, const unsigned char hour,
const unsigned char min, const unsigned char sec);


/*
    function£ºwait delay
    parameter£ºseconds: delay time in second
    return value£ºno
    note£ºdelay accurate ¡À2ms
*/
void sleep(unsigned long seconds);


/*
    function£ºwait delay
    parameter£ºms: delay time in millisecond
    return value£ºno
    note£ºdelay accurate from -1 to ms
    		  msleep(1) actual delay time is 0.01-1ms£¬
    		  msleep(2) actual delay time is 1.01-2ms
*/
void msleep(unsigned long ms);


/*
    function£ºwait delay
    parameter£ºus: delay time in millisecond
    return value£ºno
    note£ºdelay time accurate ¡À2us
*/
void usleep(unsigned long us);





#endif

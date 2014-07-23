#ifndef __KMY_KEYDRV_H
#define __KMY_KEYDRV_H


// for kmy801h--key define
#define KEY_DEL				        0x01	// Delete
#define KEY3				        0x02	// 3
#define KEYXING			            0x05	// *
#define KEY7				        0x04	// 7
#define KEY5				        0x03	// 5

#define KEY_UP				        0x08	// UP
#define KEY_R3				        0x07	// M1
#define KEY8				        0x0a	// 8
#define KEY0				        0x0b	// 0
#define KEY6				        0x09	// 6

#define KEY_R4				        0x0d	// M2
#define KEY1				        0x0E	// 1
#define KEY_DOWN			        0x0F	// DOWN
#define KEY9				        0x10	// 9
#define KEYJING			            0x11	// #

#define KEY_ReturnInitInterface	    0x13    // KEY_EXIT
#define KEY2				        0x14	// 2
#define KEY4				        0x15	// 4
#define KEY_ReturnPrevious	        0x16	// Return
#define KEY_Enter			        0x17	// ENTER

#define KEY_NONE			        0x00
#define	KEY_R1	                    KEY_Enter
#define	KEY_R2	                    KEY_ReturnInitInterface

#define KEY_pseudo					255			//pseudo key
#define KEY_WaitTimeout				254			//wait timeout pseudo key

#define KEY_LEFT			        KEY_R3
#define KEY_RIGHT			        KEY_R4
#define KEY_R6				        0x19



/*¡¡¡¡
    Function: key initialization
    Function parameter: no
    Return value: no
*/
void kmy_KeyInit(void);


/*
    Function: get key value
    Function parameter:no
    Return value: key value macro definition
*/
unsigned char kmy_GetKeyValue(void);


/*
    Function: set LCD light turn off time
    Function parameter: set seconds
    Return value: no
    Notice: 1 if seconds==0,cancel time set
*/
void kmy_AutoOffLcdLight(unsigned long seconds);
           

/*
    Function: set wait time to generate KEY_WaitTimeout
    Function parameter:seconds
    Return value: no
    Notice: 1 if seconds==0,cancel time set
            2 After waiting seconds,using kmy_KeyGetKeyValue() to read key value,will show KEY_WaitTimeout's keyvalue
*/
void kmy_AutoPostKey(unsigned long seconds);


/*
    Function: Post key
    Function parameter:key: key value macro definition
    Return value: no
    Notice: 1 After executing this function ,use kmy_KeyGetKeyValue() read keyvalue ,will show formal parameter key's keyvalue
    	    2 seldom used function
*/
void kmy_PostKey(unsigned char key);


/*
    Function: Enter Power Down Mode
    Function parameter:key: no
    Return value: no
    Notice: 1 After executing this function , there will be no display screen.
            2 Press any key exit of power down mode.
*/
void kmy_EnterPowerDownMode(void);


#endif

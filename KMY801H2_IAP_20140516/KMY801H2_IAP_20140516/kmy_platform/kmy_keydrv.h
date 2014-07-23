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


void kmy_KeyInit(void);
//函数功能：key 初始化
//函数参数：无
//返回值：无
//注意事项：无


unsigned char kmy_GetKeyValue(void);
//函数功能：取按键值
//函数参数：无
//返回值：
//	上面按键宏定义
//注意事项：无


void kmy_AutoOffLcdLight(unsigned long seconds);
//函数功能：设置多少秒后关闭lcd 的背光
//函数参数：
//	seconds: 时间的秒数
//返回值：无
//注意事项：
// 	1 如果seconds==0取消设置的秒数
// 	2 如果没到时间重新设置这个秒数
// 	之前设置的秒数被取消，以本次的
// 	的秒数重新计算


void kmy_AutoPostKey(unsigned long seconds);
//函数功能：设置多少秒后自动产生KEY_WaitTimeout
//函数参数：
//	seconds: 时间的秒数
//返回值：无
//注意事项：
// 	1 如果seconds==0取消设置的秒数
// 	2 如果没到时间重新设置这个秒数
// 	之前设置的秒数被取消，以本次的
// 	的秒数重新计算
// 	3 当过完这个秒数后，用kmy_GetKeyValue()函数
// 	读按键，马上会得到KEY_WaitTimeout 按键值


void kmy_PostKey(unsigned char key);
//函数功能：直接置键值
//函数参数：
//	key: 按键值，见此文件的宏定义
//返回值：无
//注意事项：
//	1 当执行这个函数之后，用kmy_GetKeyValue()函数
// 	读按键，马上会得到形参key 的 按键值
// 	2 这个函数在一些特殊的场合用，一般用户很少用到



void kmy_EnterPowerDownMode(void);
//函数功能：进入低功耗的停止模式
//函数参数：无
//返回值：无
//注意事项：
//	进入此函数后屏幕会无任何显示
//	按任意键退出低功耗的停止模式


#endif

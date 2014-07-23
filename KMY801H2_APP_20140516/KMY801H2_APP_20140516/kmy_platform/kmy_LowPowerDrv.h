#ifndef _kmy_LowPowerDrv_
#define _kmy_LowPowerDrv_

#include "stm32f10x.h"

#define SWITCHPOWER

#define VCC3_CTR 	GPIO_Pin_12
#define VCC5_CTR 	GPIO_Pin_13


#define CTR_POWER 	GPIO_Pin_14		 //PD14
#define PWSW_CHECK 	GPIO_Pin_7		 //PE7

#define SW_On	1
#define SW_Off  0

#define GSM_DTR 	GPIO_Pin_15		 //PD15

void kmy_VCC3V5V_Init(void);

void kmy_VCC3On(void);

void kmy_VCC3Off(void);

void kmy_VCC5On(void);

void kmy_VCC5Off(void);

void kmy_PowerOn(void);

void kmy_PowerOff(void);

uint8_t kmy_PowerCheck(void);

void KMY_WakeUpGPIOInit(void);

void KMY_GPIODeInit(void);

#endif


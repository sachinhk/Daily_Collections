
#ifndef __KMY_GPRSDRV_H
#define __KMY_GPRSDRV_H

#define GSM_DTR 	GPIO_Pin_15		 //PD15
#define AT_SLEEP	"AT+QSCLK=1\r"	//进入睡眠模式(801H)

void kmy_GprsInit (void);

void kmy_GprsOnOff (unsigned char OnOff);

void GsmSleep(void);

void GsmWakeup(void);


#endif









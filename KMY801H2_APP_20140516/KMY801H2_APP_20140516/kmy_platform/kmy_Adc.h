#ifndef _kmy_adc_h
#define _kmy_adc_h

#include "stm32f10x.h"





/*
����Function��ADC initialization
����Function parameter��No
����Return value��No
*/
void kmyAdcInit(void);


/*
����Function��Get sampling value
����Function parameter��No
����Return value��Sampling value
*/
uint16_t kmyGetAdc(void);


/*
����Function��Get voltage value
����Function parameter��No
����Return value��Voltage value
*/
uint16_t kmyGetVoltage(void);


#endif

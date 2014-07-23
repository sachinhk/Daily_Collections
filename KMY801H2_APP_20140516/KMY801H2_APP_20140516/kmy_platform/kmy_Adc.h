#ifndef _kmy_adc_h
#define _kmy_adc_h

#include "stm32f10x.h"





/*
¡¡¡¡Function£ºADC initialization
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmyAdcInit(void);


/*
¡¡¡¡Function£ºGet sampling value
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºSampling value
*/
uint16_t kmyGetAdc(void);


/*
¡¡¡¡Function£ºGet voltage value
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºVoltage value
*/
uint16_t kmyGetVoltage(void);


#endif

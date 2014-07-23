#ifndef __KMY_BATTARTDRV_H
#define __KMY_BATTARTDRV_H



typedef enum
{ ChargeStatus_Charge = 1,
	ChargeStatus_DisCharge,
}CHARGESTATUS;


typedef enum
{ AcStatus_Plug = 1,
	AcStatus_UnPlug,
}ACSTATUS;


/*
¡¡¡¡function£ºbattery test function initialization
¡¡¡¡parameter£ºno
¡¡¡¡return value£ºno
*/
void kmy_BatInit(void);


/*
    function£ºget charge status
    parameter£ºno
    return value£ºcharinging(macro definition)ChargeStatus_Charge
    		charge finished(macro definition)ChargeStatus_DisCharge
*/
unsigned char kmy_BatGetChargeStatus(void);


/*
    function£ºget external power plug status
    parameter£ºno
    return value£ºpluged(macro definition)AcStatus_Plug
    			unpluged(macro definition)AcStatus_UnPlug
*/
unsigned char kmy_BatGetACStatus(void);




#endif

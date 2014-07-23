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
����function��battery test function initialization
����parameter��no
����return value��no
*/
void kmy_BatInit(void);


/*
    function��get charge status
    parameter��no
    return value��charinging(macro definition)ChargeStatus_Charge
    		charge finished(macro definition)ChargeStatus_DisCharge
*/
unsigned char kmy_BatGetChargeStatus(void);


/*
    function��get external power plug status
    parameter��no
    return value��pluged(macro definition)AcStatus_Plug
    			unpluged(macro definition)AcStatus_UnPlug
*/
unsigned char kmy_BatGetACStatus(void);




#endif

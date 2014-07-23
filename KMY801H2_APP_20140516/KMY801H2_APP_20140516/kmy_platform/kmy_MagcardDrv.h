#ifndef __KMY_MAGCARDDRV_H
#define __KMY_MAGCARDDRV_H



#define ReadMagCardReturnInit		0x02
#define ReadMagCardReturnPrev		0x03
#define ReadMagCardTimeOut			0x04
#define ReadMagCardDateError		0x05
#define ReadMagCardReturnEnter		0x06

#define ReadMagCardSuccess			0x0a






/*
    Function: Magnetic card driver initialization
    Function parameter:no
    Return value: no
*/
void kmy_MagcardInit(void);


/*
    Function: read magnetic card data
    Function parameter:track1: Track 1 buffer 
    			        track2: Track 2 buffer	
    			        track3: Track 3 buffer
    Return value: 	read success return(macro definition)ReadMagCardSuccess
    			   	Press EXIT key return(macro definition)ReadMagCardReturnInit
    			   	Press Return key return(macro definition)ReadMagCardReturnPrev
    			   	Press Confirm key return(macro definition)ReadMagCardReturnEnter
    				Time-out return(macro definition)ReadMagCardTimeOut
    				Card data Error return(macro definition)ReadMagCardDateError
    Notice:
    　　      1. track1: Should be more than 200bytes.
    　　      2. track2: Should be more than 200bytes.
    　　      3. track3: Should be more than 200bytes.
*/
char kmy_MagcardRead(unsigned char *track1,unsigned char *track2, unsigned char *track3, unsigned int timeout_Seconds);





#endif

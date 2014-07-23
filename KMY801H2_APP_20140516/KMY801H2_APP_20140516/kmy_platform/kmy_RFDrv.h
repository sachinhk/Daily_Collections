#ifndef __KMY_RFDRV_H
#define __KMY_RFDRV_H

//------2013-9-29--zj---------



/*
¡¡¡¡Function£ºRF initialization
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_RFInit(void);


/*
¡¡¡¡Function£ºconfig RF
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£º0  Success
                  1  Fail
*/
unsigned char KMY_RFSet(void);


/*
¡¡¡¡Function£º To find RF cards
¡¡¡¡Function parameter£ºMode: 0   idle
                              1   all
                        TagtypeBuff: Store card type.
¡¡¡¡Return value£º2       Find RF card
                  other   No card
*/
unsigned char kmy_RFRequest(unsigned char Mode,unsigned char* TagtypeBuff);


/*
¡¡¡¡Function£ºAnti collision
¡¡¡¡Function parameter£ºUIDBuff: Store UID data
¡¡¡¡Return value£ºUID Length
                  < 4      Fail
                  >=4      Success
*/
unsigned char kmy_RFAntiColl(unsigned char* UIDBuff);


/*
¡¡¡¡Function£º Select card
¡¡¡¡Function parameter£ºSAKBuff: Store SAK data.
¡¡¡¡Return value£º0       Success
                  other   Fail
*/
unsigned char kmy_RFSelect_Card(unsigned char *SAKBuff);


/*
¡¡¡¡Function£º load key
¡¡¡¡Function parameter£ºmifare_key: the key.
¡¡¡¡Return value£º0       Success
                  1       Fail
*/
unsigned char kmy_RFmifare_loadkey(unsigned char *mifare_key);


/*
¡¡¡¡Function£º The Mifare1 authentication
¡¡¡¡Function parameter£ºmode: 0    KEYA authentication
                              1    KEYB authentication
                        sector: The authentication sector number
¡¡¡¡Return value£º0       Success
                  1       Fail
*/
unsigned char kmy_RFmifare_auth(unsigned char mode,unsigned char sector);


/*
¡¡¡¡Function£ºRead block data
¡¡¡¡Function parameter£ºblock: block number
                        Buff: Store block data
¡¡¡¡Return value£ºData Length
                  < 4      Fail
                  >=4      Success
*/
unsigned char kmy_RFblockread(unsigned char block,unsigned char *Buff);


/*
¡¡¡¡Function£ºWrite block data
¡¡¡¡Function parameter£ºblock: block number
                        Buff: Store write data(Need 16 bytes data)
¡¡¡¡Return value£º0       Success
                  1       Fail
*/
unsigned char kmy_RFblockwrite(unsigned char block,unsigned char *buff);


/*
¡¡¡¡Function£ºAdd Value
¡¡¡¡Function parameter£ºblock: block number
                        Buff: Store write data(Need 16 bytes data)
¡¡¡¡Return value£º0       Success
                  1       Fail
*/
unsigned char kmy_RFblockinc(unsigned char block,unsigned char *buff);


/*
¡¡¡¡Function£ºReduce Value
¡¡¡¡Function parameter£ºblock: block number
                        Buff: Store write data(Need 16 bytes data)
¡¡¡¡Return value£º0       Success
                  1       Fail
*/
unsigned char kmy_RFblockdec(unsigned char block,unsigned char *buff);


/*
¡¡¡¡Function£ºTYPEA CPU Card reset
¡¡¡¡Function parameter£ºATRBuff: Store ATR data
¡¡¡¡Return value£ºATR Length
                  < 2      Fail
                  >=2      Success
*/
unsigned char kmy_RFATS(unsigned char *ATRBuff);


/*
¡¡¡¡Function£ºSend APDU command
¡¡¡¡Function parameter£ºlen: APDU length
                        buff: APDU command
                        Recvbuff: Recieve APDU response data
¡¡¡¡Return value£ºAPDU response data Length
                  < 2      Fail
                  >=2      Success
*/
unsigned char kmy_RFCPUcommand(unsigned char len,unsigned char *buff, unsigned char *Recvbuff);


/*
¡¡¡¡Function£ºClose RF
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_RFHalt(void);


#endif   



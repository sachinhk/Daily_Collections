#ifndef __KMY_RFDRV_H
#define __KMY_RFDRV_H

//------2013-9-29--zj---------



/*
����Function��RF initialization
����Function parameter��No
����Return value��No
*/
void kmy_RFInit(void);


/*
����Function��config RF
����Function parameter��No
����Return value��0  Success
                  1  Fail
*/
unsigned char KMY_RFSet(void);


/*
����Function�� To find RF cards
����Function parameter��Mode: 0   idle
                              1   all
                        TagtypeBuff: Store card type.
����Return value��2       Find RF card
                  other   No card
*/
unsigned char kmy_RFRequest(unsigned char Mode,unsigned char* TagtypeBuff);


/*
����Function��Anti collision
����Function parameter��UIDBuff: Store UID data
����Return value��UID Length
                  < 4      Fail
                  >=4      Success
*/
unsigned char kmy_RFAntiColl(unsigned char* UIDBuff);


/*
����Function�� Select card
����Function parameter��SAKBuff: Store SAK data.
����Return value��0       Success
                  other   Fail
*/
unsigned char kmy_RFSelect_Card(unsigned char *SAKBuff);


/*
����Function�� load key
����Function parameter��mifare_key: the key.
����Return value��0       Success
                  1       Fail
*/
unsigned char kmy_RFmifare_loadkey(unsigned char *mifare_key);


/*
����Function�� The Mifare1 authentication
����Function parameter��mode: 0    KEYA authentication
                              1    KEYB authentication
                        sector: The authentication sector number
����Return value��0       Success
                  1       Fail
*/
unsigned char kmy_RFmifare_auth(unsigned char mode,unsigned char sector);


/*
����Function��Read block data
����Function parameter��block: block number
                        Buff: Store block data
����Return value��Data Length
                  < 4      Fail
                  >=4      Success
*/
unsigned char kmy_RFblockread(unsigned char block,unsigned char *Buff);


/*
����Function��Write block data
����Function parameter��block: block number
                        Buff: Store write data(Need 16 bytes data)
����Return value��0       Success
                  1       Fail
*/
unsigned char kmy_RFblockwrite(unsigned char block,unsigned char *buff);


/*
����Function��Add Value
����Function parameter��block: block number
                        Buff: Store write data(Need 16 bytes data)
����Return value��0       Success
                  1       Fail
*/
unsigned char kmy_RFblockinc(unsigned char block,unsigned char *buff);


/*
����Function��Reduce Value
����Function parameter��block: block number
                        Buff: Store write data(Need 16 bytes data)
����Return value��0       Success
                  1       Fail
*/
unsigned char kmy_RFblockdec(unsigned char block,unsigned char *buff);


/*
����Function��TYPEA CPU Card reset
����Function parameter��ATRBuff: Store ATR data
����Return value��ATR Length
                  < 2      Fail
                  >=2      Success
*/
unsigned char kmy_RFATS(unsigned char *ATRBuff);


/*
����Function��Send APDU command
����Function parameter��len: APDU length
                        buff: APDU command
                        Recvbuff: Recieve APDU response data
����Return value��APDU response data Length
                  < 2      Fail
                  >=2      Success
*/
unsigned char kmy_RFCPUcommand(unsigned char len,unsigned char *buff, unsigned char *Recvbuff);


/*
����Function��Close RF
����Function parameter��No
����Return value��No
*/
void kmy_RFHalt(void);


#endif   



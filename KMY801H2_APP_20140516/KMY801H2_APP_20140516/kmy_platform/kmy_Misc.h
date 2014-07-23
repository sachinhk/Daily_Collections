#ifndef __KMY_MISC_H
#define __KMY_MISC_H






/*
¡¡¡¡Function£ºMiscellaneous functions initialization
¡¡¡¡Function parameter£ºNo
¡¡¡¡Return value£ºNo
*/
void kmy_MiscInit(void);


/*
    Function£ºget the only chip id
    Function parameter£ºbuff: chip id stores buff 
¡¡¡¡Return value£ºNo
*/
void kmy_MiscGetChipUniqueId(unsigned  char *buff);


/*
    Function£ºGet random number
    Function parameter£ºbuff:  random number stores buff
¡¡¡¡Return value£ºNo
*/
void kmy_MiscGetRandomNumber(unsigned  char *buff,unsigned int len);


/*
    Function£ºGet Library Version
¡¡¡¡Function parameter£ºlibrary version number
¡¡¡¡Return value£ºNo
    Note£ºPlease ensure that buff size is equal or greater than 20 bytes
*/
void kmy_MiscGetLibraryVersion(unsigned  char *buff);




#endif

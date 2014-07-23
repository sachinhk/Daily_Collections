#ifndef __KMY_MISC_H
#define __KMY_MISC_H






/*
����Function��Miscellaneous functions initialization
����Function parameter��No
����Return value��No
*/
void kmy_MiscInit(void);


/*
    Function��get the only chip id
    Function parameter��buff: chip id stores buff 
����Return value��No
*/
void kmy_MiscGetChipUniqueId(unsigned  char *buff);


/*
    Function��Get random number
    Function parameter��buff:  random number stores buff
����Return value��No
*/
void kmy_MiscGetRandomNumber(unsigned  char *buff,unsigned int len);


/*
    Function��Get Library Version
����Function parameter��library version number
����Return value��No
    Note��Please ensure that buff size is equal or greater than 20 bytes
*/
void kmy_MiscGetLibraryVersion(unsigned  char *buff);




#endif

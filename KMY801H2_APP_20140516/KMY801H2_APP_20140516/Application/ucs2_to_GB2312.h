#ifndef ucs2_to_GB2312_H
#define ucs2_to_GB2312_H


void ucs2_to_gb2312(unsigned short  ucs2,  // the ucs2 code that need to be converted.
unsigned char  *gb2312_ptr) ; // store the gb2312 code


unsigned short ucs2str_to_gb2312str(// return the length of gb2312_ptr
unsigned char  *ucs_ptr,        // the ucs2 stringcode that need to be converted.
unsigned short   length,         //the unicode length      //@steven  cr8219 2004 03 29 , see cr8040
unsigned char  *gb2312_ptr) ;    // store the gb2312 string code


#endif 

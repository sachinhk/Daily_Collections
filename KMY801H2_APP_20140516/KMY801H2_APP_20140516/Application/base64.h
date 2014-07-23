
#ifndef Base64_h
#define Base64_h



//src: 需要编码的数据buff
//dest: 编码后存放的buff, 注意buff的长度必需大于srcLen/3*4+3
//srcLen: 需要编码数据的长度
//返回编码后字符串的长度
unsigned int Base64_Encode(const unsigned char *src,unsigned char *dest, unsigned int srcLen);	


//src: 需要解码的buff
//dest: 解码后存放的buff
//srcLen: 需要解码数据的长度
//返回解码后数据的长度
unsigned int Base64_Decode(const unsigned char *src,unsigned char *dest,unsigned int srcLen);




#endif


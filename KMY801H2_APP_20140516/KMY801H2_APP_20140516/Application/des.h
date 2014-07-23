#ifndef Des_H
#define Des_H



unsigned int MyDes(unsigned char *inPlaintext,unsigned int PlaintextLen, unsigned char *outCiphertext, const unsigned char *key);
unsigned int MyUnDes(unsigned char *inCiphertext,unsigned int CiphertextLen,unsigned char *outPlaintext, const unsigned char *key);



//inPlaintext: 需要加密的明文数据
//PlaintextLen:加密明文数据的长度
//outCiphertext:加密之后密文存放的地方
//key:加密的密钥，固定24byte
//返回密文数据长度。
extern unsigned int MyDes3(unsigned char *inPlaintext,unsigned int PlaintextLen, unsigned char *outCiphertext, const unsigned char *key);

//inCiphertext: 需要解密的密文数据
//CiphertextLen:解密密文数据的长度
//outPlaintext:解密之后明文存放的地方
//key:解密的密钥，固定24byte
//返回明文数据长度。
extern unsigned int MyUnDes3(unsigned char *inCiphertext,unsigned int CiphertextLen,unsigned char *outPlaintext, const unsigned char *key);


#endif


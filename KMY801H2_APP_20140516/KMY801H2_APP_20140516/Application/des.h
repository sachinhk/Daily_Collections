#ifndef Des_H
#define Des_H



unsigned int MyDes(unsigned char *inPlaintext,unsigned int PlaintextLen, unsigned char *outCiphertext, const unsigned char *key);
unsigned int MyUnDes(unsigned char *inCiphertext,unsigned int CiphertextLen,unsigned char *outPlaintext, const unsigned char *key);



//inPlaintext: ��Ҫ���ܵ���������
//PlaintextLen:�����������ݵĳ���
//outCiphertext:����֮�����Ĵ�ŵĵط�
//key:���ܵ���Կ���̶�24byte
//�����������ݳ��ȡ�
extern unsigned int MyDes3(unsigned char *inPlaintext,unsigned int PlaintextLen, unsigned char *outCiphertext, const unsigned char *key);

//inCiphertext: ��Ҫ���ܵ���������
//CiphertextLen:�����������ݵĳ���
//outPlaintext:����֮�����Ĵ�ŵĵط�
//key:���ܵ���Կ���̶�24byte
//�����������ݳ��ȡ�
extern unsigned int MyUnDes3(unsigned char *inCiphertext,unsigned int CiphertextLen,unsigned char *outPlaintext, const unsigned char *key);


#endif


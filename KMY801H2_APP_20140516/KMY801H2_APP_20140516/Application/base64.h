
#ifndef Base64_h
#define Base64_h



//src: ��Ҫ���������buff
//dest: ������ŵ�buff, ע��buff�ĳ��ȱ������srcLen/3*4+3
//srcLen: ��Ҫ�������ݵĳ���
//���ر�����ַ����ĳ���
unsigned int Base64_Encode(const unsigned char *src,unsigned char *dest, unsigned int srcLen);	


//src: ��Ҫ�����buff
//dest: ������ŵ�buff
//srcLen: ��Ҫ�������ݵĳ���
//���ؽ�������ݵĳ���
unsigned int Base64_Decode(const unsigned char *src,unsigned char *dest,unsigned int srcLen);




#endif


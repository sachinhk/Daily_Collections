#ifndef FileOperate_h
#define FileOperate_h




#define Buffile_OperateOK				1
#define Buffile_CantFindRecord			2
#define Buffile_BuffSpaceNotEnough		3






//��Record  ���뵽buffile  �ļ��������ĵ�num  ���ֶ��ϣ�
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ����ɹ�����Buffile_OperateOK
//num==0��������1  ��ǰ�棬����Ϊ��num  ������
extern unsigned char Insert_A_Record(unsigned char *buffile,unsigned char *buffileEND,const unsigned char *Record,unsigned short num);

//ɾ��buffile  �еĵ�num ����¼
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ɾ���ɹ�����Buffile_OperateOK
//num==0||num==1����ɾ����1  ��������Ϊɾ����num  ��
extern unsigned char Delete_A_Record(unsigned char *buffile,unsigned char *buffileEND,unsigned short num);

//��buffile  �еĵ�num ����¼��Record
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ���ɹ�����Buffile_OperateOK
//num==0||num==1�������1  ��������Ϊ����num  ��
extern unsigned char Read_A_Record(unsigned char *buffile,unsigned char *buffileEND,unsigned char *Record,unsigned int Readlen,unsigned short num);

//��buffile  �еĵ�num ����¼�еĵ�Snum  ��Segment
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ���ɹ�����Buffile_OperateOK
//num==0||num==1�������1  ��������Ϊ����num  ��
//Snum==0||Snum==1�������1  �Σ������������Snum  ��
extern unsigned char Read_A_Record_Segment(unsigned char *buffile,unsigned char *buffileEND,unsigned char Snum,unsigned char *Segment,unsigned int Readlen,unsigned short num);

//�õ�buffile  �к��ж�������¼
extern unsigned short Get_Record_Total(const unsigned char *buffile);

#endif


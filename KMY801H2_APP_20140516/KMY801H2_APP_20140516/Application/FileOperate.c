
#include <string.h>


#include "FileOperate.h"
#include "my_stdint.h"


//this is I define file operate fuction format
//  1.Field and between fields with \ n division 
//  2.Record and records use between \ r division 
//  3.Contents of the columns are all visible asccii yards or GB2312 unsigned characters 
//  4.The rest not part with fully 0 fills
//  5.All function wrong buffer cross-border judgment, by applications judge for yourself.
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |
//   | segment1\t | segment2\t | segment3\t | segment4\t | segmentN\t\n |


#define SegmentSeparator	'\t'
#define RecordSeparator		'\n'


//��pinsert  �Ƶ�buffile  �ļ��������ĵ�num  ����¼���棬
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//num==0�����1  ��ǰ�棬����Ϊ��num  ������
static unsigned char move_point_to_record(unsigned char **pinsert,unsigned char *buffile,unsigned char *buffileEND,unsigned short num)
{
	unsigned short i;

	for(i=0;i<num;i++)
	{
		while(*buffile!=RecordSeparator)
		{
			if(buffile>buffileEND){return Buffile_CantFindRecord;}
			if(*buffile=='\0'){return Buffile_CantFindRecord;}
			buffile++;
		}
		buffile++;
	}

	*pinsert=buffile;

	return Buffile_OperateOK;
}


//��pinsert  �Ƶ�buffile  �ļ��������ĵ�num  �κ��棬
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//num==0�����1  ��ǰ�棬����Ϊ��num  �κ���
static unsigned char move_point_to_segment(unsigned char **pinsert,unsigned char *buffile,unsigned char *buffileEND,unsigned short num)
{
	unsigned short i;

	for(i=0;i<num;i++)
	{
		while(*buffile!=SegmentSeparator)
		{
			if(*buffile==RecordSeparator){return Buffile_CantFindRecord;}
			if(*buffile=='\0'){return Buffile_CantFindRecord;}
			if(buffile>buffileEND){return Buffile_CantFindRecord;}
			buffile++;
		}
		buffile++;
	}

	*pinsert=buffile;

	return Buffile_OperateOK;
}


//��Record  ���뵽buffile  �ļ��������ĵ�num  ����¼֮ǰ��
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ����ɹ�����Buffile_OperateOK
//num==0��������1  ��ǰ�棬����Ϊ��num  ������
unsigned char Insert_A_Record(unsigned char *buffile,unsigned char *buffileEND,const unsigned char *Record,unsigned short num)
{
	unsigned char *pinsert;
	unsigned char *pmove;
	unsigned char *pmoveTo;
	unsigned char retval;
	int len;

	retval=move_point_to_record(&pinsert,buffile,buffileEND,num);
	if(retval!=Buffile_OperateOK)return retval;

	len=strlen((sc8*)Record);
	pmove=pinsert+strlen((sc8*)pinsert);
	pmoveTo=pmove+len;
	*pmoveTo=0;
	if(pmoveTo>buffileEND)return Buffile_BuffSpaceNotEnough;
	while(pmove>pinsert)
	{
		pmoveTo--;
		pmove--;
		*pmoveTo=*pmove;
	}

	memcpy(pinsert,Record,len);

	return Buffile_OperateOK;
}

//ɾ��buffile  �еĵ�num ����¼
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ɾ���ɹ�����Buffile_OperateOK
//num==0||num==1����ɾ����1  ��������Ϊɾ����num  ��
unsigned char Delete_A_Record(unsigned char *buffile,unsigned char *buffileEND,unsigned short num)
{
	unsigned char *pinsert;
	unsigned char *pmove;
	unsigned char retval;

	if(num!=0)num--;
	retval=move_point_to_record(&pinsert,buffile,buffileEND,num);
	if(retval!=Buffile_OperateOK)return retval;
	retval=move_point_to_record(&pmove,buffile,buffileEND,num+1);
	if(retval!=Buffile_OperateOK)return retval;

	while(*pmove!=0)
	{
		*pinsert=*pmove;
		pinsert++;
		pmove++;
	}

	while(pinsert!=pmove)
	{
		*pinsert=0;
		pinsert++;
	}

	return Buffile_OperateOK;
}

//��buffile  �еĵ�num ����¼��Record
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ���ɹ�����Buffile_OperateOK
//num==0||num==1�������1  ��������Ϊ����num  ��
//Readlen Ϊ�����ݵĳ��ȣ����ƻ������ã��»����������
unsigned char Read_A_Record(unsigned char *buffile,unsigned char *buffileEND,unsigned char *Record,unsigned int Readlen,unsigned short num)
{
	unsigned char *pinsert;
	unsigned char retval;

	if(num!=0)num--;
	retval=move_point_to_record(&pinsert,buffile,buffileEND,num);
	if(retval!=Buffile_OperateOK)return retval;

	while(*pinsert!=RecordSeparator&&Readlen!=0)
	{
		*Record=*pinsert;
		Record++;
		pinsert++;
		Readlen--;
	}
	*Record=0;

	return Buffile_OperateOK;
}

//��buffile  �еĵ�num ����¼�еĵ�Snum  ��Segment
//buffileEND Ϊbuffile����߽���Ϊ��ֹ�����������
//�Ҳ�����¼����Buffile_CantFindRecord, ���ɹ�����Buffile_OperateOK
//num==0||num==1�������1  ��������Ϊ����num  ��
//Snum==0||Snum==1�������1  �Σ������������Snum  ��
//Readlen Ϊ�����ݵĳ��ȣ����ƻ������ã��»����������
unsigned char Read_A_Record_Segment(unsigned char *buffile,unsigned char *buffileEND,unsigned char Snum,unsigned char *Segment,unsigned int Readlen,unsigned short num)
{
	unsigned char *pinsert;
	unsigned char retval;

	if(num!=0)num--;
	if(Snum!=0)Snum--;
	retval=move_point_to_record(&pinsert,buffile,buffileEND,num);
	if(retval!=Buffile_OperateOK)return retval;
	retval=move_point_to_segment(&pinsert,pinsert,buffileEND,Snum);
	if(retval!=Buffile_OperateOK)return retval;

	while(*pinsert!=SegmentSeparator&&*pinsert!=RecordSeparator&&Readlen!=0)
	{
		*Segment=*pinsert;
		Segment++;
		pinsert++;
		Readlen--;
	}
	*Segment=0;

	return Buffile_OperateOK;
}


//�õ�buffile  �к��ж�������¼
unsigned short Get_Record_Total(const unsigned char *buffile)
{
	unsigned short Total;

	Total=0;
	while(*buffile!=0)
	{
		if(*buffile==RecordSeparator)Total++;
		buffile++;
	}

	return Total;
}






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


//把pinsert  移到buffile  文件缓冲区的第num  条记录后面，
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//num==0代表第1  条前面，其它为第num  条后面
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


//把pinsert  移到buffile  文件缓冲区的第num  段后面，
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//num==0代表第1  段前面，其它为第num  段后面
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


//把Record  插入到buffile  文件缓冲区的第num  条记录之前，
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 插入成功返回Buffile_OperateOK
//num==0代表插入第1  条前面，其它为第num  条后面
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

//删除buffile  中的第num 条记录
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 删除成功返回Buffile_OperateOK
//num==0||num==1代表删除第1  条，其它为删除第num  条
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

//读buffile  中的第num 条记录到Record
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 读成功返回Buffile_OperateOK
//num==0||num==1代表读第1  条，其它为读第num  条
//Readlen 为读数据的长度，限制缓冲区用，怕缓冲区溢出。
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

//读buffile  中的第num 条记录中的第Snum  到Segment
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 读成功返回Buffile_OperateOK
//num==0||num==1代表读第1  条，其它为读第num  条
//Snum==0||Snum==1代表读第1  段，其它代表读第Snum  段
//Readlen 为读数据的长度，限制缓冲区用，怕缓冲区溢出。
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


//得到buffile  中含有多少条记录
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





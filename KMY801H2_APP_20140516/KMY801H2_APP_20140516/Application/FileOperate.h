#ifndef FileOperate_h
#define FileOperate_h




#define Buffile_OperateOK				1
#define Buffile_CantFindRecord			2
#define Buffile_BuffSpaceNotEnough		3






//把Record  插入到buffile  文件缓冲区的第num  条字段上，
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 插入成功返回Buffile_OperateOK
//num==0代表插入第1  条前面，其它为第num  条后面
extern unsigned char Insert_A_Record(unsigned char *buffile,unsigned char *buffileEND,const unsigned char *Record,unsigned short num);

//删除buffile  中的第num 条记录
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 删除成功返回Buffile_OperateOK
//num==0||num==1代表删除第1  条，其它为删除第num  条
extern unsigned char Delete_A_Record(unsigned char *buffile,unsigned char *buffileEND,unsigned short num);

//读buffile  中的第num 条记录到Record
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 读成功返回Buffile_OperateOK
//num==0||num==1代表读第1  条，其它为读第num  条
extern unsigned char Read_A_Record(unsigned char *buffile,unsigned char *buffileEND,unsigned char *Record,unsigned int Readlen,unsigned short num);

//读buffile  中的第num 条记录中的第Snum  到Segment
//buffileEND 为buffile的最边界作为防止缓冲区溢出用
//找不到记录返回Buffile_CantFindRecord, 读成功返回Buffile_OperateOK
//num==0||num==1代表读第1  条，其它为读第num  条
//Snum==0||Snum==1代表读第1  段，其它代表读第Snum  段
extern unsigned char Read_A_Record_Segment(unsigned char *buffile,unsigned char *buffileEND,unsigned char Snum,unsigned char *Segment,unsigned int Readlen,unsigned short num);

//得到buffile  中含有多少条记录
extern unsigned short Get_Record_Total(const unsigned char *buffile);

#endif


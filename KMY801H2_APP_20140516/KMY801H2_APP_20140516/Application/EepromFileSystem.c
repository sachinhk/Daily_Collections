
//����ļ�ϵͳ��˼����������:

//��0 ҳ��ų����ļ�ͷ��Ϣ��������ʱֻ����һ���ֽ�
//��0 ҳ��һ���ֽ�(���ʼ���Ǹ��ֽ�)���������ļ���

//��1����5ҳ�ܹ�5*64byte=320byte  �����ļ�����ͷ��Ϣ��ŵ�
//�ط���һ��EFILE����ͷռ��16�ֽ�(��EFILE �ṹ��)�����ܹ�
//������320byte/16=20���ļ���һҳΪ64byte������һҳ���Դ�
//��4���ļ�����ͷ��Ϣ����һҳ���4���ļ�����ͷ��2ҳ
//Ҳ���4���ļ�����ͷ���Դ����ơ�Ȼ���ļ�ͷ�ṹ���
//StartPage ��¼�ļ��Ŀ�ʼҳ. ��ֻҪ������ļ���ʼҳ����
//Ϣ���ٸ��ݺ����ҳ��ҳ֮��Ĺ�����ϵ�Ϳ����ҵ�һ
//���ļ���������Ϣ��Ȼ���ļ�ͷ��Size ��¼�ļ��Ĵ�С��
//Size Ϊshort �ͣ������ļ����Ϊ64k

//��6ҳ����20 ҳ��¼ҳ��ҳ֮��Ĺ�����ϵ���ܹ�15*64byte
//=960byte, ���Կ��Ա�ʾ480ҳÿ�����ֽڴ���һ��ҳ�Ĺ�ϵ:
//�������������ϵ�ֽ�: �����ֽڷ�Χ(0x0000----0x0014)
//0x0000--->�����(��δʹ��ҳ)     0x0001--->��������βҳ
//0x000f--->����ҳ  0x0015 �����ϴ�����һҳΪ0x0015ҳ
//����: �����6  ҳ�ĵ�1�͵�2�ֽ�Ϊ0x00,0x00����˵����21ҳ
//Ϊδʹ��ҳ�������6  ҳ�ĵ�3 �͵�4 �ֽ�Ϊ0x00,0x01����
//˵����22ҳΪһ���ļ���βҳ�������6  ҳ�ĵ�5 �͵�6��
//��Ϊ0x00,0x0f����˵����23ҳΪһ����ҳ�������6  ҳ�ĵ�
//7 �͵�8 �ֽ�Ϊ0x00,0x64����˵����24ҳΪһ���ļ����м�ҳ
//��������һҳΪ��100ҳ��



#define EepromFile_PageIdle		0x0000
#define EepromFile_PageTail		0x0001
#define EepromFile_PageBroken		0x000f
#define EepromFile_PageMaxKey		0x0014

#define EepromFile_MaxFiles			20


#define EEPROM_TOTALPAGE			500		//�˴�����512ҳ���У���Ϊ15x32=480+21=501,���д˴������501
#define EEPROM_PAGESIZE				64		//eeprom ҳ��С


#include "EepromFileSystem.h"


#include "kmy_EEPROMDrv.h"
#include <string.h>
#include <stdio.h>


#define MAXEEPROMFILELONGTH		11 //eeprom �ļ�������ֽ���


const u8 InitKong[64]={
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const u8 Buff0xff[64]={
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};



void EepromFile_DeleteAllPageArea(pEFILE efp);
u16 EepromFile_GetPageRelationAreaTwoByte(u16 RelationTwoByte);
u8 EepromFile_CheckWhetherHaveThisFile(uc8 Name[12],u8 *filenumber,u8 *TheFirstIdlePosition);

//buff:Ҫ�������ݴ�ŵ�buff��M24128 ÿҳ64byte��M2416,M2432,M2464��32byte
//page: ���ڼ�ҳ(128*1024/8)/64=256ҳ���ܹ�256ҳ��������page���EEPROM_TOTALPAGE��
u8 EepromFile_readPage(u8 *buff,u16 page)
{
	return kmy_EepromReadSequent(page*64,buff,64);
}
//buff:Ҫ�������ݴ�ŵ�buff��M24128 ÿҳ64byte��16,32,64��32byte
//page: ���ڼ�ҳ(128*1024/8)/64=256ҳ���ܹ�256ҳ��������page���EEPROM_TOTALPAGE��
u8 EepromFile_WritePage(uc8 *buff,u16 page)
{
	return kmy_EepromWritePage(page*64,buff,64);
}

//efp: efile ָ�룬
//filenumber: �ļ��Ĵ����(0x00--0x13)
void EepromFile_GetFileStruct(pEFILE efp,u8 filenumber)
{
	u8 buff[64];
	u8 bytes;
	u8 page;

	bytes=filenumber%4; 	//ȷ���ļ����ڵ�ҳ���ֽ���
	bytes*=16;		
	page=1;	//ȷ���ļ����ڵ�ҳ��
	page+=filenumber/4;

	EepromFile_readPage(buff,page);
	strcpy((s8 *)efp->Name,(sc8*)&buff[bytes]);
	efp->Size=(buff[bytes+12]<<8)+buff[bytes+12+1];
	efp->StartPage=(buff[bytes+12+2]<<8)+buff[bytes+12+1+2];
}

//�������ļ�����
u8 EepromFile_GetTotalFiles(void)
{
	u8 len;

	kmy_EepromReadSequent(0x0000,&len,1);

	return len;
}

//д���ļ�����
void EepromFile_SetTotalFiles(u8 number)
{
	kmy_EepromWriteByte(0x0000,number);
}


//���ؿ��������ֽ���
u16 EepromFile_GetIdleAreaSize(void)
{
	u8 buff[64];
	u8 i,j;
	u16 page=0;
	u16 pagecnt=0;

	for(i=6;i<=20;i++)
	{
		EepromFile_readPage(buff,i);	//��һ��ҳ������
		for(j=0;j<64;j+=2)
		{
			if(buff[j]==0x00&&buff[j+1]==0x00)
			{
				page++;	//����ҳ++
			}
			pagecnt++;	//�ܹ���ѯ��ҳ++
			if(pagecnt>=EEPROM_TOTALPAGE-21)
			{
				return page*64;
			}
		}
	}

	return 0;
}


//Name[12]: �����ļ���
//*filenumber: ����ɹ��ҵ�����ļ������ļ��Ĵ���������*filenumber
//����1: *filenumber==0 ������ΪName ������ļ�������ͷ��
//��1  ҳ�ĵ�0�ֽڵ���15 �ֽڡ�
//����2: *filenumber==4 ������ΪName ������ļ�������ͷ��
//��2 ҳ�ĵ�0�ֽڵ���15 �ֽڡ�
//*TheFirstIdlePosition: ��һ�������ļ�ͷ���Ե�λ��,������ֵΪ0xff����û�п���λ����
//����: �д��ļ�����EepromFile_HaveThisFile
//û������ļ�����EepromFile_HaveNotThisFile
u8 EepromFile_CheckWhetherHaveThisFile(uc8 Name[12],u8 *filenumber,u8 *TheFirstIdlePosition)
{
	u8 i;
	u8 buff[64];
	u8 *p;
	u8 flag=0;

	//Name[11]=0;

	for(i=1;i<=5;i++)
	{
		EepromFile_readPage(buff,i);
		p=buff;
		if(*p==0)
		{
			if(flag==0)
			{
				if(TheFirstIdlePosition!=NULL)*TheFirstIdlePosition=(i-1)*4+0;
				flag=1;
			}
		}
		else
		{
			if(strcmp((sc8*)p,(sc8*)Name)==0)
			{
				if(filenumber!=NULL)*filenumber=(i-1)*4+0;
				return EepromFile_HaveThisFile;
			}
		}

		p+=sizeof(struct EepromFile)-2;
		if(*p==0)
		{
			if(flag==0)
			{
				if(TheFirstIdlePosition!=NULL)*TheFirstIdlePosition=(i-1)*4+1;
				flag=1;
			}
		}
		else
		{
			if(strcmp((sc8*)p,(sc8*)Name)==0)
			{
				if(filenumber!=NULL)*filenumber=(i-1)*4+1;
				return EepromFile_HaveThisFile;
			}
		}

		p+=sizeof(struct EepromFile)-2;
		if(*p==0)
		{
			if(flag==0)
			{
				if(TheFirstIdlePosition!=NULL)*TheFirstIdlePosition=(i-1)*4+2;
				flag=2;
			}
		}
		else
		{
			if(strcmp((sc8*)p,(sc8*)Name)==0)
			{
				if(filenumber!=NULL)*filenumber=(i-1)*4+2;
				return EepromFile_HaveThisFile;
			}
		}

		p+=sizeof(struct EepromFile)-2;
		if(*p==0)
		{
			if(flag==0)
			{
				if(TheFirstIdlePosition!=NULL)*TheFirstIdlePosition=(i-1)*4+3;
				flag=1;
			}
		}
		else
		{
			if(strcmp((sc8*)p,(sc8*)Name)==0)
			{
				if(filenumber!=NULL)*filenumber=(i-1)*4+3;
				return EepromFile_HaveThisFile;
			}
		}
	}

	if(flag==0)
	{
		if(TheFirstIdlePosition!=NULL)*TheFirstIdlePosition=0xff;
	}

	return EepromFile_HaveNotThisFile;
}

static u8 EepromFile_SaveEfpAttibute(pEFILE efp)
{
	u8 filenumber;
	u8 TheFirstIdlePosition;
	u8 retval;
	u8 page;
	u8 buff[64];

	retval=EepromFile_CheckWhetherHaveThisFile(efp->Name,&filenumber,&TheFirstIdlePosition);
	if(retval==EepromFile_HaveNotThisFile)
	{
		printf("[EepromFile error]=EepromFile_SaveEfpAttibute\r\n");
		return retval;
	}

	if(filenumber>=20)
	{
		printf("[EepromFile error]=EepromFile_SaveEfpAttibute\r\n");
		return EepromFile_Error_FilesMax20;
	}

	page=1;
	page+=filenumber/4;	//�õ��ļ�����ͷ���ڵ�ҳ

	filenumber%=4;		//�õ��ļ�����ͷ����ҳ���ֽ�ƫ����
	filenumber*=16;

	EepromFile_readPage(buff,page);	//�����ļ�����ͷ��ҳ

	memcpy((s8*)(&buff[filenumber]),(sc8*)(efp->Name),12);	//д�ļ�����ͷ��buff  ���ڵ�λ���ֽ�
	buff[filenumber+12]=(u8)(efp->Size>>8);
	buff[filenumber+12+1]=(u8)(efp->Size&0x00ff);
	buff[filenumber+12+2]=(u8)(efp->StartPage>>8);
	buff[filenumber+12+2+1]=(u8)(efp->StartPage&0x00ff);

	EepromFile_WritePage(buff,page);	//��д�ļ�����ͷ��ҳ

	return EepromFile_OperateNormal;
}



static void EepromFile_TotalFileAdd1(void)
{
	u8 total;

	total=EepromFile_GetTotalFiles();
	if(total<EepromFile_MaxFiles)total++;
	EepromFile_SetTotalFiles(total);
}



static u16 EepromFile_GetPhysicalPage(u16 startpage,u16 offset)
{
	for(;offset!=0;offset--)
	{
		startpage=EepromFile_GetPageRelationAreaTwoByte(startpage);
	}

	return startpage;
}


static void EepromFile_readFileCurrentPage(u8 *buffer,pEFILE efp)
{
	u16 pages;

	//�����ǰҪ����ҳ������ڿ�ʼҳ��ƫ��
	pages=efp->CurPos/64;	//

	//��Ϊpages ������������Ҫ��λ�����pageҳ
	pages=EepromFile_GetPhysicalPage(efp->StartPage,pages);

	//�������ҳ��buffer
	EepromFile_readPage(buffer,pages);
}

//���page ҳ�ǲ��ǻ�ҳ
u8 EepromFile_checkThisPageWhetherOk(u16 page)
{
	u8 i,Buff[EEPROM_PAGESIZE];

	EepromFile_WritePage(Buff0xff,page);
	EepromFile_readPage(Buff,page);

	for(i=0;i<sizeof(Buff);i++)
	{
		if(Buff[i]!=Buff0xff[i])return 1;
	}

	return 0;
}


//����ҳ��־
void EepromFile_SetPageArea(u16 thispage,u16 nextpage)
{
	u16 pages;
	u16 bytes;
	u8 buff[64];

	if(thispage<=EepromFile_PageMaxKey){printf("[EepromFile error]=SetPageArea\r\n");}
	pages=thispage-0x0015;
	bytes=pages*2;
	pages=6+bytes/64;	//�õ�thispage ����ҳ��������ҳ
	bytes%=64;			//�õ�thispage ����ҳ������ҳ���ֽ�ƫ��
	
	EepromFile_readPage(buff,pages);

	buff[bytes]=(u8)(nextpage>>8);
	buff[bytes+1]=(u8)(nextpage&0x00ff);

	EepromFile_WritePage(buff,pages);
}

//����һ�����е�ҳ
//����1: ����0x0015  ��0x0015ҳΪ���У�
//ע�ⲻ���ܷ���С��0x0015��ҳ
//����2: ����0xffff  ��û�п���ҳ
u16 EepromFile_GetAIdlePage(void)
{
	u8 buff[64];
	u8 i,j;
	u16 page=0;
	u16 pagecnt=0;

	for(i=6;i<=20;i++)
	{
		EepromFile_readPage(buff,i);	//��һ��ҳ������
		for(j=0;j<64;j+=2){
			if(buff[j]==0x00&&buff[j+1]==0x00)
			{
				page=0x0015+(i-6)*32+j/2;
				if(EepromFile_checkThisPageWhetherOk(page)==1)
				{	//����ҳ�ǲ��ǻ�ҳ
					//����ǻ�ҳ���û�ҳ��־��ҳ��������
					EepromFile_SetPageArea(page,EepromFile_PageBroken);
				}
				else
				{
					return page;
				}
			}

			pagecnt++;
			if(pagecnt>EEPROM_TOTALPAGE-21)return 0xffff;
		}
	}

	return 0xffff;
}




u16 EepromFile_GetPageRelationAreaTwoByte(u16 RelationTwoByte)
{
	//Ϊ����߶��ٶȣ�����������̬����
	static u8 buff[64];
	static u16 ReadedPage=0xffff;

	u16 page;		//�ļ������ֽڵ���ʼλ��
	u16 nextpage;

	RelationTwoByte-=0x0015;	//������6  ҳ�����λ��
	RelationTwoByte*=2;			//������6  ҳ������ֽ�λ��

	page=6;
	page+=RelationTwoByte/64;	//�õ�RelationTwoByte ���ڵ�ҳ
	RelationTwoByte%=64;		//�õ�RelationTwoByte ����ҳ���ֽ�ƫ��

	if(ReadedPage!=page)
	{	//Ϊ����߶��ٶȣ��Ӵ����
		EepromFile_readPage(buff,page);
		ReadedPage=page;
	}

	nextpage=(buff[RelationTwoByte]<<8)+buff[RelationTwoByte+1];

	return nextpage;
}


//RelationTwoByte: Ҫ����ҳ�Ĺ����ֽ�
//����RelationTwoByte λ�õĹ����ֽ�
//�õ��ļ��������ֽ�
//����1: RelationTwoByte==0x0015���򷵻�(��6  ҳ�ĵ�1�ֽ�<<8)+��6  ҳ�ĵ�2�ֽ�
//����2: RelationTwoByte==0x0016���򷵻�(��6  ҳ�ĵ�3�ֽ�<<8)+��6  ҳ�ĵ�4�ֽ�
//����3: RelationTwoByte==0x0035���򷵻�(��7  ҳ�ĵ�1�ֽ�<<8)+��6  ҳ�ĵ�2�ֽ�
//����4: RelationTwoByte==0x0036���򷵻�(��7  ҳ�ĵ�3�ֽ�<<8)+��6  ҳ�ĵ�4�ֽ�
//����5: RelationTwoByte==0x0055���򷵻�(��9  ҳ�ĵ�1�ֽ�<<8)+��6  ҳ�ĵ�2�ֽ�
u16 EepromFile_GetPageRelationAreaTwoByte_AndDelete(u16 RelationTwoByte)
{
	u16 page;		//�ļ������ֽڵ���ʼλ��
	u8 buff[64];
	u16 nextpage;

	RelationTwoByte-=0x0015;	//������6  ҳ�����λ��
	RelationTwoByte*=2;			//������6  ҳ������ֽ�λ��

	page=6;
	page+=RelationTwoByte/64;	//�õ�RelationTwoByte ���ڵ�ҳ
	RelationTwoByte%=64;		//�õ�RelationTwoByte ����ҳ���ֽ�ƫ��

	EepromFile_readPage(buff,page);

	nextpage=(buff[RelationTwoByte]<<8)+buff[RelationTwoByte+1];

	//Delete �����ֽ�
	buff[RelationTwoByte]=0x00;
	buff[RelationTwoByte+1]=0x00;
	EepromFile_WritePage(buff,page);

	return nextpage;
}

void EepromFile_DeleteAllPageArea(pEFILE efp)
{
	u16 RelationTwoByte;

	RelationTwoByte=efp->StartPage;
	if(RelationTwoByte<=EepromFile_PageMaxKey){return;}

	//����ļ��м�ҳ�ֽڱ�־
	while(RelationTwoByte>EepromFile_PageMaxKey)
	{
		RelationTwoByte=EepromFile_GetPageRelationAreaTwoByte_AndDelete(RelationTwoByte);
	}

	//���β�ֽڱ�־
	EepromFile_GetPageRelationAreaTwoByte_AndDelete(RelationTwoByte);
}

u8 EepromFile_DeleteEfpAttibute(pEFILE efp)
{
	u8 filenumber;
	u8 page;
	u8 retval;
	u8 buff[64];

	retval=EepromFile_CheckWhetherHaveThisFile(efp->Name,&filenumber,&page);
	if(retval==EepromFile_HaveNotThisFile)return retval;

	if(filenumber>=20){printf("[EepromFile error]=DeleteEfpAttibute\r\n");}

	page=1;
	page+=filenumber/4;	//�õ��ļ�����ͷ���ڵ�ҳ

	filenumber%=4;		//�õ��ļ�����ͷ����ҳ���ֽ�ƫ����
	filenumber*=16;

	EepromFile_readPage(buff,page);	//�����ļ�����ͷ��ҳ

	memset(&buff[filenumber],0,16);		//����ļ�����ͷ���ڵ�buffλ���ֽ�

	EepromFile_WritePage(buff,page);	//��д�ļ�����ͷ��ҳ

	return EepromFile_OperateNormal;
}

void EepromFile_TotalFilePlus1(void)
{
	u8 total;

	total=EepromFile_GetTotalFiles();
	if(total)total--;
	EepromFile_SetTotalFiles(total);
}

void __EepromFile_Delete(pEFILE efp)
{
	EepromFile_DeleteAllPageArea(efp);
	EepromFile_DeleteEfpAttibute(efp);
	EepromFile_TotalFilePlus1();
}




//-------------------------------------����ΪAPI-------------------------------------------

void EepromFile_FileSystemFormat(void)
{
	unsigned char i;

	for(i=0;i<21;i++)
	{
		EepromFile_WritePage(InitKong,i);
	}
}

u8 EepromFile_Create(uc8 Name[12],pEFILE efp)
{
	u8 buff[64];
	u8 i;
	u8 page;

	if(strlen((sc8*)Name)>MAXEEPROMFILELONGTH)return EepromFile_FileNameToLong;

	i=EepromFile_GetTotalFiles();
	if(i>=EepromFile_MaxFiles)
	{	//page 0: ��һ���ֽ�Ϊ�ļ��������������ܴ���20��
		return EepromFile_Error_FilesMax20;
	}
	if(EepromFile_CheckWhetherHaveThisFile(Name,&i,&page)==EepromFile_HaveThisFile)
	{
		return EepromFile_HaveThisFile;
	}

	if(page==0xff){return EepromFile_Error_FilesMax20;}
	// 1��5ҳ���ļ���Ϣ
	i=page%4;	//ȷ���ļ����ڵ�ҳ��λ����(0--3)
	page/=4;	//ȷ���ļ����ڵ�ҳ��
	page+=1;	//�ļ�����ͷ��1ҳ��ʼ���Լ�1
	EepromFile_readPage(buff,page);
	//Name[11]=0;
	strcpy((s8*)&buff[i*16],(sc8*)Name);	//���ļ���д��buff
	buff[i*16+12]=0;	//�Ѵ�Сд��buff
	buff[i*16+12+1]=0;
	buff[i*16+12+2]=0;	//����ʼҳд��buff
	buff[i*16+12+2+1]=0;
	EepromFile_WritePage(buff,page);

	strcpy((s8*)(efp->Name),(sc8*)Name);	//���ļ���д�����ص�ָ��
	memset((s8*)(&efp->Size),0,2);	//�Ѵ�Сд�����ص�ָ��
	memset((s8*)(&efp->StartPage),0,2);	//����ʼҳд�����ص�ָ��

	EepromFile_TotalFileAdd1();

	return EepromFile_CreateOK;
}

u8 EepromFile_Open(uc8 Name[12],pEFILE efp)
{
	u8 i;
	u8 TheFirstIdlePosition;

	if(strlen((sc8*)Name)>MAXEEPROMFILELONGTH)return EepromFile_FileNameToLong;

	if(EepromFile_CheckWhetherHaveThisFile(Name,&i,&TheFirstIdlePosition)==EepromFile_HaveNotThisFile)
	{
		return EepromFile_HaveNotThisFile;
	}

	efp->CurPos=0;

	EepromFile_GetFileStruct(efp,i);
	return EepromFile_OpenOk;
}

u8 EepromFile_Seek(pEFILE efp,s16 size,u8 flag)
{
	s16 temp;

	if(flag==EepromFile_SEEK_CUR)
	{
		temp=efp->CurPos+size;
		if(temp>efp->Size||temp<0)
		{
			return EepromFile_Error_SeekOutofArea;
		}
		else
		{
			efp->CurPos=temp;
		}
	}
	else if(flag==EepromFile_SEEK_SET)
	{
		temp=size;
		if(temp>efp->Size||temp<0)
		{
			return EepromFile_Error_SeekOutofArea;
		}
		else
		{
			efp->CurPos=temp;
		}
	}
	else if(flag==EepromFile_SEEK_END)
	{
		temp=efp->Size+size;
		if(temp>efp->Size||temp<0)
		{
			return EepromFile_Error_SeekOutofArea;
		}
		else
		{
			efp->CurPos=temp;
		}
	}
	else
	{
		return EepromFile_Error_SeekFlag;
	}

	return EepromFile_SeekOK;
}

u8 EepromFile_read(u8 *buffer, u16 size,  pEFILE efp)
{
	//u16 pages;
	u8 buff[64];
	u8 temp;
	u8 pos;

	//����ļ�����ʼҳΪ0�����ļ�Ϊֻ�����˶��ѣ�û������
	if(efp->StartPage==0x0000){return EepromFile_Error_NoContext;}

	//���Ҫ���Ĵ�С���ڵ�ǰλ�õ��ļ�β�����ܴ�С��
	//ֻ������ǰλ�õ��ļ�β�����ܴ�С��
	if(size>efp->Size-efp->CurPos)size=efp->Size-efp->CurPos;

	//pages=efp->CurPos/64;	//������һҳ
	EepromFile_readFileCurrentPage(buff,efp);	//������ǰλ�õ�����ҳ��buff
	pos=efp->CurPos%64;		//�õ�Ҫ������һҳbuff��λ��
	temp=64-pos;			//�õ�Ҫ�������ֽ���
	if(temp>size)temp=size;	//�����һҳ�����Ĵ�С> ʵ��Ҫ�������ֽ�������ֻ����Ҫ�������ֽ���
	memcpy(buffer,&buff[pos],temp);
	buffer+=temp;		//�Կ�������Ŀ��buff  ����ƫ��
	size-=temp;			//�ܹ�Ҫ�����ֽ�-�ѿ����ֽ�
	efp->CurPos+=temp;	//�Ե�ǰλ��ƫ��

	while(size>=64)
	{	//�����м��ҳ
		EepromFile_readFileCurrentPage(buff,efp);
		memcpy(buffer,buff,64);
		efp->CurPos+=64;
		size-=64;
		buffer+=64;
	}

	EepromFile_readFileCurrentPage(buff,efp);	//���������ֽ�ҳ
	memcpy(buffer,buff,size);
	efp->CurPos+=size;

	return EepromFile_ReadOk;
}

//
u8 EepromFile_Write(uc8 *buffer, u16 size,  pEFILE efp)
{
	u16 pages;
	u16 temp;

	if(EepromFile_GetIdleAreaSize()+efp->Size<size){return EepromFile_Error_AreaNotEnough;}

	//��ɾ������ļ�������ռ�õ��ļ���������
	EepromFile_DeleteAllPageArea(efp);

	pages=EepromFile_GetAIdlePage();	//�Ȱѵ�һҳ��λ��д���ļ�����ͷ
	efp->StartPage=pages;
	efp->Size=size;
	EepromFile_SaveEfpAttibute(efp);

	//�Ȱѿ�ʼ����ҳ��ҳ����������Ϊβҳ
	//�Ա�������EepromFile_GetAIdlePage �еĺ���
	//�ֶ�����ҳ
	EepromFile_SetPageArea(pages,EepromFile_PageTail);

	while(size>=64)
	{
		EepromFile_WritePage(buffer,pages);
		buffer+=64;
		size-=64;
		if(size==0)
		{
			EepromFile_SetPageArea(pages,EepromFile_PageTail);
			break;
		}
		else
		{
			temp=EepromFile_GetAIdlePage();
			EepromFile_SetPageArea(pages,temp);
			EepromFile_SetPageArea(temp,EepromFile_PageTail);
			pages=temp;	//��һ������ҳ��ֵ��pages ������д�µ�ҳ
		}
	}

	if(size!=0)
	{
		//EepromFile_WritePage(buffer,pages);
		kmy_EepromWritePage(pages*64,buffer,size);
		EepromFile_SetPageArea(pages,EepromFile_PageTail);
	}

	return EepromFile_WriteOk;
}


//	ɾ���ɹ�����EepromFile_DeleteOk
//	û������ļ�����EepromFile_HaveNotThisFile
u8 EepromFile_Delete(uc8 *Name)
{
	u8 filenumber;
	u8 TheFirstIdlePosition;
	u8 retval;
	struct EepromFile efp;

	retval=EepromFile_CheckWhetherHaveThisFile(Name,&filenumber,&TheFirstIdlePosition);
	if(retval==EepromFile_HaveNotThisFile)return retval;

	EepromFile_Open(Name,&efp);

	__EepromFile_Delete(&efp);

	return EepromFile_DeleteOk;
}



//-----------------------------------------��һ����װAPI------------------------------------------
//filename: �ļ���
//����ֵ: 
//�д��ļ�����EepromFile_HaveThisFile
//û������ļ�����EepromFile_HaveNotThisFile
//�ļ����ַ�����>11byte,����EepromFile_FileNameToLong
u8 EepromFile_CheckFileExist(uc8 *name)
{
	if(strlen((sc8*)name)>MAXEEPROMFILELONGTH)return EepromFile_FileNameToLong;

	return EepromFile_CheckWhetherHaveThisFile(name,NULL,NULL);
}


//buff: Ҫд��Դbuff
//wLen: Ҫд�ĳ���
//filename: �ļ���
//д�ɹ�����EepromFile_WriteOk��
//���ɹ����صĺ궨��������:
//EepromFile_FileNameToLong---->�����ļ���̫��
//EepromFile_Error_FilesMax20---->�����Ѿ���������20���ļ�������
//EepromFile_Error_AreaNotEnough---->����û���㹻�Ŀռ��ŵ�ǰ���ļ�
u8 EepromFile_WriteData(const unsigned char *buff,unsigned int wLen,const unsigned char *filename)
{
	unsigned int retval;
	struct EepromFile efp;

	retval=EepromFile_Open(filename,&efp);
	if(retval==EepromFile_FileNameToLong)return retval;

	if(retval==EepromFile_HaveNotThisFile)
	{
		{printf("ERROR-->EepromFile_WriteData-->EepromFile_Open\r\n");}

		retval=EepromFile_Create(filename,&efp);
		if(retval!=EepromFile_CreateOK)
		{
			{printf("ERROR-->EepromFile_WriteData-->EepromFile_Create\r\n");}
			return retval;
		}
	}

	//ReadEepromPage(NULL);
	retval=EepromFile_Write(buff,wLen,&efp);
	if(retval!=EepromFile_WriteOk)
	{
		{printf("ERROR-->EepromFile_WriteData-->EepromFile_Write\r\n");}
		
	}

	return retval;
}


//����buff :ΪҪ�����ļ��Ļ�����
//����rLen:ΪҪ�����ļ�����
//����filename: �ļ���
//���ɹ�����EepromFile_ReadOk��
//���ɹ����صĺ궨��������:
//EepromFile_FileNameToLong---->�����ļ���̫��
//EepromFile_HaveNotThisFile---->�����ļ�������
//EepromFile_Error_NoContext---->�����ļ�û������
u8 EepromFile_ReadData(unsigned char *buff,unsigned int rLen,const unsigned char *filename)
{
	u8 retval;
	struct EepromFile efp;

	retval=EepromFile_Open(filename,&efp);
	if(retval!=EepromFile_OpenOk)
	{
		{printf("ERROR-->CheckTamper-->EepromFile_Open\r\n");}
		return retval;
	}

	retval=EepromFile_read(buff,rLen,&efp);
	if(retval!=EepromFile_ReadOk)
	{
		{printf("ERROR-->CheckTamper-->EepromFile_read\r\n");}
	}

	return retval;
}


void EepromFile_GetFileName(u8 *Name,u8 number)
{
	u8 i;
	u8 buff[64];
	u8 *p;

	*Name=0;

	for(i=1;i<=5;i++)
	{
		EepromFile_readPage(buff,i);

		p=buff;
		if(*p!=0)
		{
			if(number)number--;
			else{strcpy((char*)Name,(char*)p);return;}
		}

		p+=sizeof(struct EepromFile)-2;
		if(*p!=0)
		{
			if(number)number--;
			else{strcpy((char*)Name,(char*)p);return;}
		}

		p+=sizeof(struct EepromFile)-2;
		if(*p!=0)
		{
			if(number)number--;
			else{strcpy((char*)Name,(char*)p);return;}
		}

		p+=sizeof(struct EepromFile)-2;
		if(*p!=0)
		{
			if(number)number--;
			else{strcpy((char*)Name,(char*)p);return;}
		}
	}
}


//#include "Flash_Write.h"
#include "my_stdint.h"
#include "GlobalVar.h"
#include <string.h>
#include <stdio.h>
int footerrec()
{
	struct footer obj;
 	int count=-1;
	obj.slno=1;
	while(count<=256)
	{
	 	ReadFooter(&obj,obj.slno);
		printf("\n pageno==%d obj==%d\n",count,obj.slno);
		if(obj.slno==0||obj.slno==-1)
		break;
		count=obj.slno;
		obj.slno++;
		fobj=obj;
	}
	printf("\n ###pageno==%d obj==%d\n",count,fobj.slno);
	if(count==0||count==-1)
		count=1;
		else
		count=count+1;
	   return count;
			
}
int InsertHeader(struct header hd, int pageno)
{
	   if(pageno>256)
	   {
	   kmy_FlashEraseSector(0x10000);
	   pageno=1;
	   }
	   kmy_FlashWriteBuffer((char *)&hd,0x10000+pageno*256,sizeof(hd));
	   kmy_FlashWaitForWriteEnd();
	   return;
}
int ReadHeader(struct header *hd,int pageno)
{
   kmy_FlashReadBuffer((char *)hd,0x10000+pageno*256,sizeof(hd));
   return;
}
int InsertFooter(struct footer hd,int pageno)
{
   if(pageno>256)
	{
	   kmy_FlashEraseSector(0x10000);
	   pageno=1;
	}
   kmy_FlashWriteBuffer((char *)&hd,0x20000+pageno*256,256);
   kmy_FlashWaitForWriteEnd();
}

void ReadFooter(struct footer *hd,int pageno)
{
	 kmy_FlashReadBuffer((char *)hd,0x20000+pageno*256,256);
}
void InsertAccMsg(struct acceptsms hd,int pageno)
{
	kmy_FlashWriteBuffer((char *)&hd,0x30000+pageno*256,256);
	kmy_FlashWaitForWriteEnd();
}
void ReadAccMsg(struct acceptsms *hd,int pageno)
{
	 kmy_FlashReadBuffer((char *)hd,0x30000+pageno*256,256);
}

void InsertRejMsg(struct rejectsms hd,int pageno)
{
	kmy_FlashWriteBuffer((char *)&hd,0x40000+pageno*256,256);
	kmy_FlashWaitForWriteEnd();
}

void ReadRejMsg(struct rejectsms *hd,int pageno)
{
	 kmy_FlashReadBuffer((char *)hd,0x40000+pageno*256,256);
}

/*void InsertAct(struct Header hd,u16 pageno)
{
	kmy_FlashWriteBuffer((char *)&hd,0x0+pageno*256,256);
	kmy_FlashWaitForWriteEnd();
}

void UpReStatusOrdAccDet(struct countorder hd,u16 pageno)
{
	kmy_FlashWriteBuffer((char *)&hd,0x0+pageno*256,256);
	kmy_FlashWaitForWriteEnd();
}
void UpReStatusItemDet(struct countitems hd,u16 pageno)
{
	kmy_FlashWriteBuffer((char *)&hd,0x0+pageno*256,256);
	kmy_FlashWaitForWriteEnd();
}
void UpReStatusMsgDet(struct countsms hd,int pageno)
{
    //printf("\n in write header %s",hd.name);
//        printf("\n in write header %d",hd.count);
	struct countsms LCsms;     
	int i=1;
	while(i<256)
	{
	  pageno=i;
     kmy_FlashReadBuffer((char *)&LCsms,0x0+pageno*256,256);
	 if(LCsms.count<=0 && (i==1 || i==255))
	 {
	 	kmy_FlashEraseSector(0x0);
		
		break;
	 }
	 else if(LCsms.count<=0)
	 break;
	 i++;
	}
	pageno=i;
	printf("\n Pageno=%i",pageno);
	kmy_FlashWriteBuffer((char *)&hd,0x0+pageno*256,256);
	kmy_FlashWaitForWriteEnd();
}
void  GetTotalSMS(struct countsms *Prevs,int pageno)
{
    int i=1;
   struct countsms LCsms;     
	while(i<256)
	{
	  pageno=i;
     kmy_FlashReadBuffer((char *)&LCsms,0x0+pageno*256,256);
	 if(LCsms.count<=0 )
	 {
	 	break;
	 }
	 Prevs->count=LCsms.count;	  
	 i++;
	}
	  printf("\n Read Pageno=%i",pageno);
	  
	  printf("\n Value add Reaf Pageno=%i",pageno);
	 //kmy_FlashReadBuffer((char *)hd,0x0+pageno*256,256);

}
void GetTotalOrder(struct countorder *hd,u16 pageno)
{
	 kmy_FlashReadBuffer((char *)hd,0x0+pageno*256,256);
}

void GetTotalItems(struct countitems *hd,u16 pageno)
{
	 kmy_FlashReadBuffer((char *)hd,0x0+pageno*256,256);	
}



 
void InsertOrder(struct AccOrder order,u16 pageno)
{
   kmy_FlashWriteBuffer((char *)&order,0x0+pageno*256,256);
   kmy_FlashWaitForWriteEnd();
}

void ReadOrder(struct AccOrder *order,u16 pageno)
{
	 kmy_FlashReadBuffer((char *)order,0x0+pageno*256,256);
}*/


#include "GlobalVar.h"
#include "popup.h"
#include "EepromFileSystem.h"
#include "kmy_EEPROMDrv.h"
int header()
{
	struct header obj;
 	int count=-1;
	obj.slno=1;
	//kmy_FlashEraseSector(0x10000);
	kmy_FlashWriteEnable();
	memset (hobj.h1, '\0', sizeof (hobj.h1));
	memset (hobj.h2, '\0', sizeof (hobj.h2));
	memset (hobj.h3, '\0', sizeof (hobj.h3));
	obj.slno=1;
	while(count<=256)
	{
	 	kmy_FlashReadBuffer((char *)&obj,0x10000+obj.slno*256,sizeof(obj));
	    myprintf("\n reading slno==%d h1==%s,h2==%s h3==%s\n",obj.slno,obj.h1,obj.h2,obj.h3);
		if(obj.slno==0||obj.slno==-1)
			break;
		count=obj.slno;
		hobj=obj;
		obj.slno++;
		myprintf("\n @@@@@@reading slno==%d h1==%s,h2==%s h3==%s\n",hobj.slno,hobj.h1,hobj.h2,hobj.h3);
		
	}
	myprintf("\n count==%d obj==%d\n",count,obj.slno);
	if(count==0||count==-1)
	{
		hobj.slno=1;
		memset (hobj.h1, '\0', sizeof (hobj.h1));
		memset (hobj.h2, '\0', sizeof (hobj.h2));
		memset (hobj.h3, '\0', sizeof (hobj.h3));
	}else
	{
		hobj.slno=count+1;
	}
	myprintf("\n h1 ==%d,h1==%s h2==%s h3==%s\n",hobj.slno,hobj.h1,hobj.h2,hobj.h3);
	R_Xcoord = 10, R_Ycoord = 25, RFill_Ycoord = 115, RFill_Xcoord =15;
	clear_area (5, 12, 121, 48);
	memset(temp,'\0',sizeof(temp));
	chklen = strlen (hobj.h1);
	A_len = chklen - 1;
	memset (temp, '\0', sizeof (temp));
	strcpy (temp, hobj.h1);
	showdetails (hobj.h1, "ENTER HEADER 1", sizeof (hobj.h1));
	memset (hobj.h1, '\0', sizeof (hobj.h1));
	strcpy (hobj.h1, temp);
	R_Xcoord = 10, R_Ycoord = 25, RFill_Ycoord = 115, RFill_Xcoord =15;
	clear_area (5, 12, 121, 48);
	memset(temp,'\0',sizeof(temp));
	chklen = strlen (hobj.h2);
	A_len = chklen - 1;
	memset (temp, '\0', sizeof (temp));
	strcpy (temp,hobj.h2);
	showdetails (hobj.h2, "ENTER HEADER 2", sizeof (hobj.h2));
	memset (hobj.h2, '\0', sizeof (hobj.h2));
	strcpy (hobj.h2, temp);
	R_Xcoord = 10, R_Ycoord = 25, RFill_Ycoord = 115, RFill_Xcoord =15;
	clear_area (5, 12, 121, 48);
	memset(temp,'\0',sizeof(temp));
	chklen = strlen (hobj.h3);
	A_len = chklen - 1;
	memset (temp, '\0', sizeof (temp));
	strcpy (temp,hobj.h3);
	showdetails (hobj.h3, "ENTER HEADER 3", sizeof (hobj.h3));
	memset (hobj.h3, '\0', sizeof (hobj.h3));
	strcpy (hobj.h3, temp);
	myprintf("\n Writing lastpage count ==%d pageno==%d\n",count,hobj.slno);
	//myprintf("\n h1==%s,h2==%s h3==%s\n",hobj.h1,hobj.h2,hobj.h3);
	//InsertHeader(&hobj,hobj.slno);
	if(hobj.slno>256)
	{
	hobj.slno=1;
	kmy_FlashEraseSector(0x10000);
	}
	kmy_FlashWriteBuffer((char *)&hobj,0x10000+hobj.slno*256,sizeof(hobj));
	kmy_FlashWaitForWriteEnd();
//	popup("  HEADER IS SET");
	return 1;
	
}

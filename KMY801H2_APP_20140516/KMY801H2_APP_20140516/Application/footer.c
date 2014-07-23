#include "GlobalVar.h"
#include "popup.h"
#include "EepromFileSystem.h"
#include "kmy_EEPROMDrv.h"
void footer()
{
	 struct footer obj;
 	 int count=-1;
	 obj.slno=1;
	 kmy_FlashWriteEnable();
	 memset (fobj.f1, '\0', sizeof (fobj.f1));
	 memset (fobj.f2, '\0', sizeof (fobj.f2));
	 memset (fobj.f3, '\0', sizeof (fobj.f3));
	 obj.slno=1;
	 while(count<=256)
	{
	 	kmy_FlashReadBuffer((char *)&obj,0x20000+obj.slno*256,sizeof(obj));
	    myprintf("\n reading slno==%d h1==%s,h2==%s h3==%s\n",obj.slno,obj.f1,obj.f2,obj.f3);
		if(obj.slno==0||obj.slno==-1)
			break;
		count=obj.slno;
		fobj=obj;
		obj.slno++;
		myprintf("\n @@@@@@reading slno==%d h1==%s,h2==%s h3==%s\n",fobj.slno,fobj.f1,fobj.f2,fobj.f3);
		
	}
	myprintf("\n count==%d obj==%d\n",count,obj.slno);
	if(count==0||count==-1)
	{
		fobj.slno=1;
		memset (fobj.f1, '\0', sizeof (fobj.f1));
	 	memset (fobj.f2, '\0', sizeof (fobj.f2));
	 	memset (fobj.f3, '\0', sizeof (fobj.f3));
	}else
	{
		fobj.slno=count+1;
	}
	myprintf("\n h1 ==%d,h1==%s h2==%s h3==%s\n",fobj.slno,fobj.f1,fobj.f2,fobj.f3);
	 R_Xcoord = 10, R_Ycoord = 25, RFill_Ycoord = 115, RFill_Xcoord =15;
	 clear_area (5, 12, 121, 48);
	 chklen = strlen (fobj.f1);
	 A_len = chklen - 1;
	 memset (temp, '\0', sizeof (temp));
	 strcpy (temp, fobj.f1);
	 showdetails (fobj.f1, "ENTER FOOTER 1", sizeof (fobj.f1));
	 memset (fobj.f1, '\0', sizeof (fobj.f1));
	 strcpy (fobj.f1, temp);
	 R_Xcoord = 10, R_Ycoord = 25, RFill_Ycoord = 115, RFill_Xcoord =15;
	 clear_area (5, 12, 121, 48);
	 chklen = strlen (fobj.f2);
	 A_len = chklen - 1;
	 memset (temp, '\0', sizeof (temp));
	 strcpy (temp, fobj.f2);
	 showdetails (fobj.f2, "ENTER FOOTER 2", sizeof (fobj.f2));
	 memset (fobj.f2, '\0', sizeof (fobj.f2));
	 strcpy (fobj.f2, temp);
	
	 R_Xcoord = 10, R_Ycoord = 25, RFill_Ycoord = 115, RFill_Xcoord =15;
	 clear_area (5, 12, 121, 48);
	 chklen = strlen (fobj.f3);
	 A_len = chklen - 1;
	 myprintf("\n A_len==%d",A_len);
	 memset (temp, '\0', sizeof (temp));
	 strcpy (temp, fobj.f3);
	 showdetails (fobj.f3, "ENTER FOOTER 3", sizeof (fobj.f3));
	 memset (fobj.f3, '\0', sizeof (fobj.f3));
	 strcpy (fobj.f3, temp);
	 if(fobj.slno>256)
	 {
	  fobj.slno=1;
	  kmy_FlashEraseSector(0x20000);
	  }
	 kmy_FlashWriteBuffer((char *)&fobj,0x20000+fobj.slno*256,sizeof(fobj));
	kmy_FlashWaitForWriteEnd();
//	popup("  FOOTER IS SET");
	
	}

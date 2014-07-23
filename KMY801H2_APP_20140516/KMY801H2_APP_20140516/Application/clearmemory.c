#include "GlobalVar.h"
#include "popup.h"
#include "key.h"
#include "EepromFileSystem.h"
#include "kmy_EEPROMDrv.h"
void clearmemory()
{
		  int count=-1,i=0;
		  struct adminpass obj;
		  //kmy_FlashEraseSector(0x060000);
		  memset(&adp,'\0',sizeof(adp));
		  memset(&obj,'\0',sizeof(obj));
		  obj.slno=1;
		  while(count<256)
		{
			kmy_FlashReadBuffer((char *)&obj,0x060000+obj.slno*256,sizeof(obj));
			myprintf("\n reading itemid==%d itme==%s price==%s\n",obj.slno,obj.uname,obj.upass);
			if(obj.slno==0||obj.slno==-1)
				break;
			count=obj.slno;
			adp=obj;
			obj.slno++;
		}
		  memset(temp,'\0',sizeof(temp));
		  clear_area (5, 12, 121, 48);
	      A_len = -1;
	      D_Ycoord = 20;
	      D_Xcoord = -6;
	     ChkForAlpha ("ENTER USER NAME", 15);
		if(strcmp(adp.uname,temp)==0)
		 {
		  memset(temp,'\0',sizeof(temp));
		  clear_area (5, 12, 121, 48);
	      A_len = -1;
	      D_Ycoord = 20;
	      D_Xcoord = -6;
	      ChkForAlpha ("ENTER PASSWORD", 15);
		   if(strcmp(adp.upass,temp)==0)
		   {
					//	popup2("   PLEASE WAIT","CLEARING MEMORY");
						for(i=0;i<7936;i=i+260)
						{
							 kmy_FlashEraseSector(0x010000+(i*256));
						}
			
		   }else
		   {		clear_area (5, 12, 121, 48);
		   			//popup("  WRONG PASSWORD");
					
		   }
		   }else
		   {		clear_area (5, 12, 121, 48);
					//popup("  WRONG USER NAME");
		   }

}
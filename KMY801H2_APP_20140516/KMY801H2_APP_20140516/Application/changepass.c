/***********************************
Author:Jyotiba Vaijinath kadam
Date:2011-09-29 20:00
Remarks:change administrator user and password 
LastModified:Date:2011-09-29 20:00
Remarks:change administrator user and password 
************************************/
#include "GlobalVar.h"
void changepass()
{
  int count=-1;
  struct adminpass obj;
  //kmy_FlashEraseSector(0x060000);
  
  memset(&adp,'\0',sizeof(adp));
  memset(&obj,'\0',sizeof(obj));
  obj.slno=1;
  while(count<256)
		{
			kmy_FlashReadBuffer((char *)&obj,0x060000+obj.slno*256,sizeof(obj));
			printf("\n reading itemid==%d itme==%s price==%s\n",obj.slno,obj.uname,obj.upass);
			if(obj.slno==0||obj.slno==-1)
				break;
			count=obj.slno;
			adp=obj;
			obj.slno++;
		}
	myprintf("\n Count==%d adp==%d\n",count,adp.slno);
  if (count>0)
    {
      clear_area (5, 12, 121, 48);
      A_len = -1;
      D_Ycoord = 20;
      D_Xcoord = -6;
      ChkForAlpha ("CURRENT USER NAME", 15);
      if (strcmp (adp.uname, temp) == 0)
	{
	  clear_area (5, 12, 121, 48);
	  A_len = -1;
	  D_Ycoord = 20;
	  D_Xcoord = -6;
	  ChkForAlpha ("CURRENT PASSWORD", 8);
	  if (strcmp (adp.upass, temp) == 0)
	    {
	      clear_area (5, 12, 121, 48);
	      A_len = -1;
	      D_Ycoord = 20;
	      D_Xcoord = -6;
	      ChkForAlpha ("NEW USER NAME", 15);
	      strcpy (adp.uname, temp);
	      if (strlen (adp.uname) <= 0)
		{
		  popup ("INVALID USER NAME");
		  return;
		}
	      clear_area (5, 12, 121, 48);
	      A_len = -1;
	      D_Ycoord = 20;
	      D_Xcoord = -6;
	      ChkForAlpha ("NEW PASSWORD", 15);
	      strcpy (adp.upass, temp);
	     if (strlen (adp.upass) <= 0)
		{
		  popup ("INVALID PASSWORD");
		  return;
		}
			  if(count==0||count==-1)
			{
						adp.slno=1;
			}else
			{
						adp.slno=count+1;
			}
			if(adp.slno>255||adp.slno==1)
			{
			   kmy_FlashEraseSector(0x060000);
			   adp.slno=1;
			}
		  kmy_FlashWriteBuffer((char *)&adp,0x060000+adp.slno*256,sizeof(adp));
	      popup ("   PASSWORD SET");
	   }			//if end for password check
	  else
	    {
	      popup ("  WRONG PASSWORD");
	      
	    }
	}			//if end for user name check
      else
	{
	  popup (" WRONG USER NAME");
	 
	}
    }				//if end for file not nul
  else
    {
	  memset(&adp,'\0',sizeof(adp));
      clear_area (5, 12, 121, 48);
      A_len = -1;
      D_Ycoord = 20;
      D_Xcoord = -6;
      ChkForAlpha ("NEW USER NAME", 15);
      strcpy (adp.uname, temp);
      if (strlen (adp.uname) <= 0)
	{
	  popup ("INVALID USER NAME");
	  return;
	}
      clear_area (5, 12, 121, 48);
      A_len = -1;
      D_Ycoord = 20;
      D_Xcoord = -6;
      ChkForAlpha ("NEW PASSWORD", 15);
      strcpy (adp.upass, temp);
      if (strlen (adp.upass) <= 0)
	{
	  popup ("INVALID PASSWORD");
	  return;
	}
       if(count==0||count==-1)
		{
			adp.slno=1;
			
		}else
		{
			adp.slno=count+1;
		}
		if(adp.slno>255||adp.slno==1)
		{
		   kmy_FlashEraseSector(0x060000);
		   adp.slno=1;
		}
		myprintf("\n Writting name==%s pass==%s\n",adp.uname,adp.upass);
		kmy_FlashWriteBuffer((char *)&adp,0x060000+adp.slno*256,sizeof(adp));
	    popup ("   PASSWORD SET");
     
    }
  
}

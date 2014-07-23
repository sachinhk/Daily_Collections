/***********************************
Author:Jyotiba Vaijinath kadam
Date:2012-01-01 14:00
Remarks:send massage to sever
LastModified:Date:2012-01-01 14:00
Remarks:send massage to sever
************************************/
#include "GlobalVariable.h"
MsgSend (char header[20], char test[156], char mob[15])
{
  //  myprintf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");	
    myprintf ("mobile no in Msgsend=%s\n", mob);
    if (SMIDRend == 50)
     {
         if (SMIDFend == 0)
	 {
	     //System.out.println("Que Full");
	    //continue;
	 }
      else
	{
	      SMIDRend = 0;
	}
     }
   if ((SMIDRend + 1) == SMIDFend)
    {
      
	//System.out.println("Que Full");
	//continue;
    }

  strcpy (SMessage_MobNo[SMIDRend], mob);
  SMessage_MobNo[SMIDRend][14] = '\0';
  myprintf ("SMessage_MobNo[SMIDFend]=%s\n", SMessage_MobNo[SMIDRend]);
  strcpy (SMessage_ID[SMIDRend], header);
  strcat (SMessage_ID[SMIDRend], " ");
  strcat (SMessage_ID[SMIDRend], test);
  // strcat(SMessage_ID[SMIDRend],GSep);
  //  strcat(SMessage_ID[SMIDRend],";");
  printf ("\nmsgsend end He=%s,%s,%d\n",header,SMessage_ID[SMIDRend],SMIDRend);
  SMIDRend++;
 


}

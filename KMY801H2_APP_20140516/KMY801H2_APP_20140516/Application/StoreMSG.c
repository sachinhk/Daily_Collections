								 
#include <string.h>
#include "Globalvar.h"
//#include "saveorder.c"
//void saveorder(FILE *fp,struct Msgstore);
void StoreMSGToFile(char *mob,char *data)
 {
    int z=0,x=0;
	int count=-1;
	char Lsms[157];
	struct Msgstore b;
	while(mob[z]!='\0')
    {
	     if(mob[z]!='\"')
	     {
		    b.mobileno[x]=mob[z];
		    x=x+1;
	     }
    z=z+1;
    }
       b.mobileno[x]='\0';
       b.mobileno[15]='\0';
	   strcpy(b.sms,data);
	   myprintf("\n Mobileno==%s  data==%s\n",b.mobileno,b.sms);
	   if(actobj.deactive==0)
	   {
	   	//	saveorder(b);
	   }else
	   {
			 	grejectsmsobj.slno=1;
			  count=-1;
			  while(count<=256)
			{
					 	kmy_FlashReadBuffer((char *)&grejectsmsobj,0x040000+grejectsmsobj.slno*256,sizeof(grejectsmsobj));
					    myprintf("\n reading itemid==%d\n",grejectsmsobj.slno);
						if(grejectsmsobj.slno==0||grejectsmsobj.slno==-1)
						break;
						count=grejectsmsobj.slno;
						grejectsmsobj.slno++;
						
			
			}
			if(count==0 || count==-1)
			{
			 	 grejectsmsobj.slno=1;
			}else
			{
				grejectsmsobj.slno=count;
			}
			kmy_FlashReadBuffer((char *)&grejectsmsobj,0x040000+grejectsmsobj.slno*256,sizeof(grejectsmsobj));
		//	MsgSend("-",grejectsmsobj.sms,b.mobileno);
	   }
  
  
}
   

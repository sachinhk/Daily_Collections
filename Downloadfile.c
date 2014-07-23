#include "Downloadfile.h"
#include "lcd.h"
#include "gprs.h"
#include <string.h>
#include "setting.h"
#include <stdio.h>
#include "key.h"
#include "kmy_wifi.h"
#include "GlobalVar.h"


char Download_File()
//char Download_File(char *msg)
{
    unsigned char keyval;
	unsigned int pos,temp;	
	int count=-1,devcount=-1;   
	char Temp_Data[50];
	int i=0,j=0,k=0,lineno=0;
	 memset(&custdet,'\0',sizeof(custdet));
	  memset (custdet.custIDdet, '\0', sizeof (custdet.custIDdet));
		memset (custdet.custNamedet, '\0', sizeof (custdet.custNamedet));
		memset (custdet.custMobdet, '\0', sizeof (custdet.custMobdet));
		memset (custdet.custStatusdet, '\0', sizeof (custdet.custStatusdet));
	clear_lcd();
	text_out_to_row_Language2 (0, "现在你可以通过串", "now,you can download");
	text_out_to_row_Language2 (1, "口1下载文件", "file by serial1");
	text_out_to_row_Language2 (2, "串口设置如下", "use pc serial tool");
	text_out_to_row_Language2 (3, "115200,N,8,1", "115200,N,8,1");

	keyval = GetKeyValue();
	kmy_USART1ReceiveReset();

	pos = 0;
	memset(File_Data,0,sizeof(File_Data));

	while (1)
	{
		temp = kmy_USART1ReceiveData (File_Data + pos, sizeof (File_Data)-pos, 10);
		
		pos += temp;
		
		keyval = GetKeyValue();

		if (keyval == KEY_ReturnPrevious || keyval == KEY_ReturnInitInterface)  break;
		
	}
	printf("File size=<%d> File_Data\r\n%s\r\n",pos,File_Data);
	
	 kmy_FlashEraseSector(0x070000);
	   	custdet.slno=1;
		  while(count<256)
		{
				 	kmy_FlashReadBuffer((char *)&custdet,0x070000+custdet.slno*256,sizeof(custdet));
				    printf("\n reading in Transaction :-\n slno==%d ID==%s Name==%s Mobile==%s Status==%s\n",custdet.slno,custdet.custIDdet ,custdet.custNamedet, custdet.custMobdet, custdet.custStatusdet);
					if(custdet.slno==0||custdet.slno==-1)
					break;
					count=custdet.slno;
					custdet.slno++;
					
		
		}
		printf("\n 0x070000 COUNT==%d",count);

		 kmy_FlashEraseSector(0x040000);
	   	 devdet.slno=1;
		  while(devcount<256)
		{
				 	kmy_FlashReadBuffer((char *)&devdet,0x040000+devdet.slno*256,sizeof(devdet));
				    printf("\n reading in Transaction :-\n slno==%d ID==%s Name==%s Mobile==%s Status==%s\n",custdet.slno,custdet.custIDdet ,custdet.custNamedet, custdet.custMobdet, custdet.custStatusdet);
					if(devdet.slno==0||devdet.slno==-1)
					break;
					devcount=devdet.slno;
					devdet.slno++;
					
		
		}
		printf("\n 0x040000 COUNT==%d",devcount);
		//---------------- copy from File_Data[] to sector---------------
		for(i=0;File_Data[i]!='\0';i++)
	    {
			if(File_Data[i]=='\n')
	         {

	    	/*	Temp_Data[j]='\0';
	  			printf("Temp_Data=%s\n",Temp_Data);
				strcpy(custdet.custStatusdet,Temp_Data);
				j=0;
				k=0; */
				
			     if(lineno==0)
				 {

				  	Temp_Data[j]='\0';
	  			    //printf("Temp_Data=%s\n",Temp_Data);
					 printf("IN lineno 0 Line No=%d\n",lineno);
				    strcpy(devdet.devstatus,Temp_Data);
				    j=0;
				    k=0;
					lineno=1;
				   //printf("\n count in write =%d",count);
		  			if(devcount==0||devcount==-1)
						{
							devdet.slno=1;
							devcount=1;
						   	printf("\n devdet.slno in if=%d",devdet.slno);
						}
					 else
						{
						//	custdet.slno=count+1;
							devdet.slno=devcount;
				   		//	printf("\n devdet.slno in else=%d",devdet.slno);
						}

				       if(strlen(devdet.devID)>0 && strlen(devdet.devmob)>0 && strlen(devdet.devempID)>0 && strlen(devdet.devrout)>0 && strlen(devdet.devstatus)>0)
				        {
				 	    if(devdet.slno>255)
					     {
					     //  printf("\ndevdet.slno in >255=%d",devdet.slno);
					       devdet.slno=1;
					       kmy_FlashEraseSector(0x040000);
					     }
					 printf("\n Befor writing sl==%d ID==%s Mobile==%s EmpID==%s Route=%s Status==%s\n",devdet.slno,devdet.devID,devdet.devmob,devdet.devempID,devdet.devrout,devdet.devstatus);
				   //kmy_FlashWriteBuffer((char *)&custTra,0x050000+custTra.slno*256,sizeof(custTra));
		    	     kmy_FlashWriteBuffer((char *)&devdet,0x040000+devdet.slno*256,sizeof(devdet));
					 kmy_FlashWaitForWriteEnd();
					 devcount++;
				//	popup (" File DONLOADED");
		      }
				 
				 } //lineno==0
				 
				 if(lineno>0)
				 {	       
				    Temp_Data[j]='\0';
	  		     //	printf("Temp_Data=%s\n",Temp_Data);
				 printf("IN lineno>0 Line No=%d\n",lineno);
				    strcpy(custdet.custStatusdet,Temp_Data);
				    j=0;
				    k=0; 
				//	printf("\n count in write =%d",count);
		  			if(count==0||count==-1)
						{
							custdet.slno=1;
							count=1;
					//	   	printf("\n custdet.slno in if=%d",custdet.slno);
						}
					 else
						{
						//	custdet.slno=count+1;
							custdet.slno=count;
				   	//		printf("\n custdet.slno in else=%d",custdet.slno);
						}

				       if(strlen(custdet.custIDdet)>0 && strlen(custdet.custNamedet)>0 && strlen(custdet.custMobdet)>0 && strlen(custdet.custStatusdet)>0)
				        {
				 	    if(custdet.slno>255)
					     {
					   //    printf("\n custdet.slno in >255=%d",custdet.slno);
					       custdet.slno=1;
					       kmy_FlashEraseSector(0x070000);
					     }
					 printf("\n Befor writing sl==%d ID==%s Name==%s Mobile==%s Status==%s\n",custdet.slno,custdet.custIDdet,custdet.custNamedet,custdet.custMobdet,custdet.custStatusdet);
					  //kmy_FlashWriteBuffer((char *)&custTra,0x050000+custTra.slno*256,sizeof(custTra));
		    	     kmy_FlashWriteBuffer((char *)&custdet,0x070000+custdet.slno*256,sizeof(custdet));
					 kmy_FlashWaitForWriteEnd();
					   count++;
				//	popup (" File DONLOADED");
		      }
			 }//lineno>0
	}
	  
	else if(File_Data[i]=='#')
	  {

	     Temp_Data[j]='\0';
		 if(lineno==0)
		 {
	   	 //printf("\n 0x040000 COUNT in #==%d",devcount);
	  	// printf("Temp_Data=%s\n",Temp_Data);
		  printf("Line 0 Temp_Data=%s\n",Temp_Data);
		 if(k==0)
		 strcpy(devdet.devID,Temp_Data);
		 if(k==1)
		 strcpy(devdet.devmob,Temp_Data);
		 if(k==2)
		 strcpy(devdet.devempID,Temp_Data);
		 if(k==3)
		 strcpy(devdet.devrout,Temp_Data);
		 k++;
		 j=0;
		 }

		 if(lineno>0)
		 {
	   	// printf("\n 0x070000 COUNT in #==%d",count);
	  	 printf("Line>0 Temp_Data=%s\n",Temp_Data);
		 if(k==0)
		 strcpy(custdet.custIDdet,Temp_Data);
		 if(k==1)
		 strcpy(custdet.custNamedet,Temp_Data);
		 if(k==2)
		 strcpy(custdet.custMobdet,Temp_Data);
		 k++;
		 j=0;
		 }
	  }
	  
	  else if(File_Data[i]!='\n' && File_Data[i]!='#')
	  {	
	//  printf("\n in Temp_Data[j]=File_Data[i];");  
	  Temp_Data[j]=File_Data[i];
	  j++;
	  }
	//  printf("\n COUNT before while 0x070000=%d  0x040000=%d",count,devcount);
	}
			 clear_lcd();
//return keyval;

}

#include "Transaction.h"
#include "popup.h"
#include "GlobalVar.h"
#include "kmy_keydrv.h"
#include "GlobalVariable.h"
#include "selectmenu.c"
void addmenu()
{	    
	     int flag=0,Lcheck = 0,Lmulspace = 0,count=-1,yfillarea=15,count1=-1,count2=1,list=0,counth=-1,countf=-1;
		 int colm=15,i2=0,Li=0;
	     int pcount=0;
	     int TextOut=0;
	   	 int FDI=0,TDJ=0;
	     char Temp_Data[50];
		 char searchname[10],con[33],con1[33];
		 struct header Lhobj;
	struct footer Lfobj;
		unsigned char phonenumber[21]={"9845182044"};
	unsigned char context[100]={"TEST"};

	    struct CustomerDetails custdet,custdet1,custdet2,custdet3;
	   
	     //kmy_FlashEraseSector(0x050000);
		 kmy_FlashWriteEnable();
	  	 R_Xcoord = 10, R_Ycoord = 25, RFill_Ycoord = 115, RFill_Xcoord =15;
	  	 memset(&custTra,'\0',sizeof(custTra));
		 custTra.slno=1;
		  while(count<256)
		{
				 	kmy_FlashReadBuffer((char *)&custTra,0x050000+custTra.slno*256,sizeof(custTra));
				    printf("\n reading custName==%s custID==%s Amount==%s\n",custTra.custName,custTra.custID,custTra.amount);
					if(custTra.slno==0||custTra.slno==-1)
					break;
					count=custTra.slno;
					custTra.slno++;
					
		
		}
	      printf("\n count==%d itms==%d\n",count,custTra.slno);
		  clear_area (5, 12, 121, 48);
		  Lmulspace = 0;
		  Lcheck = 0;
		   memset(searchname,'\0',sizeof(searchname));
		  memset(custTra.custName,'\0',sizeof(custTra.custName));
		  memset(temp,'\0',sizeof(temp));
		  reentername:
		  chklen = strlen (searchname);
		  A_len = chklen - 1;
		  memset (temp, '\0', sizeof (temp));
		  strcpy (temp, searchname);
		  showdetails (searchname, "ENTER NAME", sizeof (searchname));
		  memset (searchname, '\0', sizeof (searchname));
		  strcpy (searchname, temp);
		 searchname[sizeof(searchname)-1]='\0';
		  Lcheck = 0;
	  while (searchname[Lcheck] != '\0')
	    {
	      if ((searchname[Lcheck] >= 'a' && searchname[Lcheck] <= 'z') || (searchname[Lcheck] >= 'A' && searchname[Lcheck] <= 'Z')  || searchname[Lcheck] == ' ')
		{
		  if (searchname[Lcheck] != ' ')
		    Lmulspace = 1;
		}
	      else
		{
		  
		 popup ("ENTER CORRECT NAME");
		 
		  goto reentername;
		}
	      Lcheck = Lcheck + 1;
	    }
	   // -------------------------listname----------------------
	 
    

 	custdet.slno=1;
  
  while(count1<256)
	{
				kmy_FlashReadBuffer((char *)&custdet,0x070000+custdet.slno*256,sizeof(custdet));
				printf("\n reading slno==%d ID==%s Name==%s Mobile==%s Status==%s\n",custdet.slno,custdet.custIDdet ,custdet.custNamedet, custdet.custMobdet, custdet.custStatusdet);
				if(custdet.slno==0||custdet.slno==-1)
				break;
				count1=custdet.slno;
				custdet.slno++;
					
		
	}
	if(count1==-1)
	{
	 	popup("   LIST IS EMPTY");
	}else
	{	   
	
        	kmy_FlashReadBuffer((char *)&custdet1,0x070000+1*256,sizeof(custdet1));
		   kmy_FlashReadBuffer((char *)&custdet2,0x070000+2*256,sizeof(custdet2));
		   kmy_FlashReadBuffer((char *)&custdet3,0x070000+3*256,sizeof(custdet3));	
			
		  clear_area(xstart,ystart,xend,yend);
		  printf("list=%d",list);
		  if(custdet1.slno>0)// && (searchname[0]==custdet1.custNamedet[0]))
		   kmy_LcdTextOut(5,15,custdet1.custNamedet);
		  //kmy_LcdTextOut(5,list+15,custdet1.custNamedet);
		   if(custdet2.slno>0)// && (searchname[0]==custdet2.custNamedet[0]))
		   kmy_LcdTextOut(5,30,custdet2.custNamedet);
		  //kmy_LcdTextOut(5,list+15,custdet2.custNamedet);
		   if(custdet3.slno>0)// && (searchname[0]==custdet3.custNamedet[0]))
		   kmy_LcdTextOut(5,45,custdet3.custNamedet);	
		  //kmy_LcdTextOut(5,list+15,custdet3.custNamedet);
		  //if(list>=45)
		  //list=0;
		   Keydata=0;
		   reverse_area(xstart,yfillarea,xend,15,2);
		  // while(Keydata!=KEY_L1 && Keydata!=KEY_R1)
		  while(Keydata!=KEY_ReturnPrevious && Keydata!=KEY_Enter)
			{	
					//ReadDataFSPort();
	       			//Process_Modem_data();
			 		Keydata=0;
					Keydata=kmy_GetKeyValue();
					//if(Keydata==KEY_L1) 
					if(Keydata==KEY_ReturnPrevious)
					{
						flag=1;
						break;
					}
					//else if(Keydata==KEY_R1) 
					else if(Keydata==KEY_Enter)
					{
						flag=0;
						break;
					}
					if(Keydata==KEY_DOWN && count2<count1)
					{	
						count2++;
						clear_area(xstart,yfillarea,xend,15);
						yfillarea+=15;
						if(yfillarea>45)
						{
						 	kmy_FlashReadBuffer((char *)&custdet1,0x070000+count2*256,sizeof(custdet1));
		   					kmy_FlashReadBuffer((char *)&custdet2,0x070000+(count2+1)*256,sizeof(custdet2));
		   					kmy_FlashReadBuffer((char *)&custdet3,0x070000+(count2+2)*256,sizeof(custdet3));	
							yfillarea=15;
							clear_area(xstart,ystart,xend,yend);
						}
						   
						   if(custdet1.slno>0)// && (searchname[0]==custdet1.custNamedet[0]))
						   kmy_LcdTextOut(5,15,custdet1.custNamedet);
						   if(custdet2.slno>0)// && (searchname[0]==custdet2.custNamedet[0]))
						   kmy_LcdTextOut(5,30,custdet2.custNamedet);
						   if(custdet3.slno>0)// && (searchname[0]==custdet3.custNamedet[0]))
						   kmy_LcdTextOut(5,45,custdet3.custNamedet);
						/* shobha   if((custdet1.slno>0) && (searchname[0]==custdet1.custNamedet[0]))
		  // kmy_LcdTextOut(5,15,custdet1.custNamedet);
		  kmy_LcdTextOut(5,list+15,custdet1.custNamedet);
		   if((custdet2.slno>0) && (searchname[0]==custdet2.custNamedet[0]))
		  // kmy_LcdTextOut(5,30,custdet2.custNamedet);
		  kmy_LcdTextOut(5,list+15,custdet2.custNamedet);
		   if((custdet3.slno>0) && (searchname[0]==custdet3.custNamedet[0]))
		  // kmy_LcdTextOut(5,45,custdet3.custNamedet);	
		  kmy_LcdTextOut(5,list+15,custdet3.custNamedet); shobha */
		  //if(list>=45)
		  //list=0;

						   reverse_area(xstart,yfillarea,xend,15,2);
						   printf("\n DOWN count2==%d\n",count2);
					  
					}else if(Keydata==KEY_UP &&  count2>1)
					{
						count2--;
						clear_area(xstart,yfillarea,xend,15);
						yfillarea-=15;
						if(yfillarea<15)
						{
						 	yfillarea=45;
							kmy_FlashReadBuffer((char *)&custdet3,0x070000+count2*256,sizeof(custdet3));
		   					kmy_FlashReadBuffer((char *)&custdet2,0x070000+(count2-1)*256,sizeof(custdet2));
		   					kmy_FlashReadBuffer((char *)&custdet1,0x070000+(count2-2)*256,sizeof(custdet1));
							clear_area(xstart,ystart,xend,yend);
						}
						   if(custdet1.slno>0)// && (searchname[0]==custdet1.custNamedet[0]))
						   kmy_LcdTextOut(5,15,custdet1.custNamedet);
						   if(custdet2.slno>0)//&& (searchname[0]==custdet2.custNamedet[0]))
						   kmy_LcdTextOut(5,30,custdet2.custNamedet);
						   if(custdet3.slno>0)// && (searchname[0]==custdet3.custNamedet[0]))
						   kmy_LcdTextOut(5,45,custdet3.custNamedet);
						 /* shobha  if((custdet1.slno>0) && (searchname[0]==custdet1.custNamedet[0]))
		  // kmy_LcdTextOut(5,15,custdet1.custNamedet);
		  kmy_LcdTextOut(5,list+15,custdet1.custNamedet);
		   if((custdet2.slno>0) && (searchname[0]==custdet2.custNamedet[0]))
		  // kmy_LcdTextOut(5,30,custdet2.custNamedet);
		  kmy_LcdTextOut(5,list+15,custdet2.custNamedet);
		   if((custdet3.slno>0) && (searchname[0]==custdet3.custNamedet[0]))
		  // kmy_LcdTextOut(5,45,custdet3.custNamedet);	
		  kmy_LcdTextOut(5,list+15,custdet3.custNamedet);	 shobha*/
		 // if(list>=45)
		 // list=0;
						   reverse_area(xstart,yfillarea,xend,15,2);
						   printf("\n Up count2==%d\n",count2);
					}
			   printf("\n in while count2==%d\n",count2);
			}// while loop
			 printf("\n Before flag count2==%d\n",count2);
			if(Keydata==KEY_Enter)//confirm
			{
				printf("\n Count2==%d\n",count2);
				kmy_FlashReadBuffer((char *)&custdet2,0x070000+count2*256,sizeof(custdet2));
				strcpy(custTra.custName,custdet2.custNamedet);
				strcpy(custTra.custID,custdet2.custIDdet);
			//	goto reentername;
								//return;
			 }
			 	
		   }


		   
	 // -------------------------listname----------------------

	 memset(temp,'\0',sizeof(temp));
	  //memset(custTra.custID,'\0',sizeof(custTra.custID));
	 
	  chklen = strlen (custTra.custName);
	  A_len = chklen - 1;
	  memset (temp, '\0', sizeof (temp));
	  strcpy (temp,custTra.custName);
	  showdetails (custTra.custName, "ENTER NAME", 10);
	  //memset (custTra.amount, '\0', sizeof(custTra.amount));
	  //strcpy (custTra.amount, temp);
	  custTra.custName[sizeof(custTra.custName)-1]='\0';
	  Lcheck = 0;
	  while (custTra.custName[Lcheck] != '\0')
	    {
	
	      Lcheck = Lcheck + 1;
	   }
	  	   kmy_LcdTextOut(5,45,custTra.custID);

		  memset(temp,'\0',sizeof(temp));
	  //memset(custTra.custID,'\0',sizeof(custTra.custID));
	 
	  chklen = strlen (custTra.custID);
	  A_len = chklen - 1;
	  memset (temp, '\0', sizeof (temp));
	  strcpy (temp,custTra.custID);
	  showdetails (custTra.custID, "ENTER ID", 10);
	  //memset (custTra.amount, '\0', sizeof(custTra.amount));
	  //strcpy (custTra.amount, temp);
	  custTra.custID[sizeof(custTra.custID)-1]='\0';
	  Lcheck = 0;
	  while (custTra.custID[Lcheck] != '\0')
	    {
	
	      Lcheck = Lcheck + 1;
	   }
	   


	  memset(temp,'\0',sizeof(temp));
	  memset(custTra.amount,'\0',sizeof(custTra.amount));
	  reenterprice:
	  chklen = strlen (custTra.amount);
	  A_len = chklen - 1;
	  memset (temp, '\0', sizeof (temp));
	  strcpy (temp, custTra.amount);
	  showdetForDigit (custTra.amount, "ENTER AMOUNT", 10);
	  memset (custTra.amount, '\0', sizeof(custTra.amount));
	  strcpy (custTra.amount, temp);
	  custTra.amount[sizeof(custTra.amount)-1]='\0';
	  Lcheck = 0;
	  while (custTra.amount[Lcheck] != '\0')
	    {
	      if (!((custTra.amount[Lcheck] >= '0' && custTra.amount[Lcheck] <= '9')
		   && (custTra.amount[0] != '0')))
		{
		  popup (" INVALID AMOUNT");
		   goto reenterprice;
		}
	      Lcheck = Lcheck + 1;
	    }
			if(flag==0)//confirm
			{
				flag=selectmenu("PRINT","CANCLE",2);
				printf("\nflag in addmenu =%d\n",flag);
				if(flag==1)
				{
			  
      			 kmy_PrinterString("********************************\n");
				 kmy_PrinterString("       WEL COME \n");
				 counth=-1;
						Lhobj.slno=1;
		  				while(counth<256)
						{
						 	kmy_FlashReadBuffer((char *)&Lhobj,0x010000+Lhobj.slno*256,sizeof(Lhobj));
						    myprintf("\n reading itemid==%d \n",Lhobj.slno);
							if(Lhobj.slno==0||Lhobj.slno==-1)
							break;
							counth=Lhobj.slno;
							hobj=Lhobj;
							Lhobj.slno++;
						}
						printf("\n h1==%s h2==%s h3==%s\n",hobj.h1,hobj.h2,hobj.h3);
					
						//kmy_PrinterString("\n");
						i2=0;
						Li=strlen(hobj.h1);
						i2=32-Li;
						i2=i2/2;
						memset(con,'\0',sizeof(con));
						memset(con1,'\0',sizeof(con1));
						Li=0;
						while(Li<i2)
						{
							con[Li]=' ';
							Li++;
									
						}
						con[Li]='\0';
					if(strlen(hobj.h1)>0)
					{	
						strcpy(con1,con);
						strcat(con ,hobj.h1);
						strcat(con ,con1);
						strcat(con,"\n");
						kmy_PrinterString(con);
					}
						i2=0;
						Li=strlen(hobj.h2);
						i2=32-Li;
						i2=i2/2;
					memset(con,'\0',sizeof(con));
					memset(con1,'\0',sizeof(con1));
						Li=0;
						while(Li<i2)
						{
							con[Li]=' ';
							Li++;
							
						}
						con[Li]='\0';	
					if(strlen(hobj.h2)>0)
					{
						strcpy(con1,con);
						strcat(con ,hobj.h2);
						strcat(con ,con1);
						strcat(con,"\n");
						kmy_PrinterString(con);
					}		
						i2=0;
						Li=strlen(hobj.h3);
						i2=32-Li;
						i2=i2/2;
					memset(con,'\0',sizeof(con));
					memset(con1,'\0',sizeof(con1));
						Li=0;
						while(Li<i2)
						{
							con[Li]=' ';
							Li++;
								
						}
						con[Li]='\0';	
						if(strlen(hobj.h3)>0)
						{
						strcpy(con1,con);
						strcat(con ,hobj.h3);
						strcat(con ,con1);
						strcat(con,"\n");
						kmy_PrinterString(con);
							//if((strlen(hobj.h3)+i2*2)<32)
							{
								i2=1;
							//kmy_PrinterString("\n");

							}
						}			
				 kmy_PrinterString("********************************\n");
				 memset(con,'\0',sizeof(con));
			     strcpy(con,"NAME   :");
				 strcat(con,custTra.custName);
				 strcat(con,"\n");	
				 kmy_PrinterString(con); 
				 memset(con,'\0',sizeof(con));
			     strcpy(con,"ID     :");
				 strcat(con,custTra.custID);
				 strcat(con,"\n");	
				 kmy_PrinterString(con); 
				 memset(con,'\0',sizeof(con));
			     strcpy(con,"AMOUNT :");
				 strcat(con,custTra.amount);
				 strcat(con,"\n");	
				 kmy_PrinterString(con); 
				 kmy_PrinterString("********************************\n");
				 countf=-1;
						Lfobj.slno=1;
		  				while(countf<256)
						{
						 	kmy_FlashReadBuffer((char *)&Lfobj,0x020000+Lfobj.slno*256,sizeof(Lfobj));
						    myprintf("\n reading itemid==%d \n",Lfobj.slno);
							if(Lfobj.slno==0||Lfobj.slno==-1)
							break;
							count=Lfobj.slno;
							fobj=Lfobj;
							Lfobj.slno++;
						}
						printf("\n f1==%s f2==%s f3==%s\n",fobj.f1,fobj.f2,fobj.f3);
					
						i2=0;
						Li=strlen(fobj.f1);
						i2=32-Li;
						i2=i2/2;
						memset(con,'\0',sizeof(con));
						memset(con1,'\0',sizeof(con1));
						Li=0;
						while(Li<i2)
						{
							con[Li]=' ';
							Li++;
									
						}
						con[Li]='\0';
					if(strlen(fobj.f1)>0)
					{	
						strcpy(con1,con);
						strcat(con ,fobj.f1);
						strcat(con ,con1);
						strcat(con,"\n");
						kmy_PrinterString(con);
					}
						i2=0;
						Li=strlen(fobj.f2);
						i2=32-Li;
						i2=i2/2;
					memset(con,'\0',sizeof(con));
					memset(con1,'\0',sizeof(con1));
						Li=0;
						while(Li<i2)
						{
							con[Li]=' ';
							Li++;
							
						}
						con[Li]='\0';	
					if(strlen(fobj.f2)>0)
					{
						strcpy(con1,con);
						strcat(con ,fobj.f2);
						strcat(con ,con1);
						strcat(con,"\n");
						kmy_PrinterString(con);
					}		
						i2=0;
						Li=strlen(fobj.f3);
						i2=32-Li;
						i2=i2/2;
					memset(con,'\0',sizeof(con));
					memset(con1,'\0',sizeof(con1));
						Li=0;
						while(Li<i2)
						{
							con[Li]=' ';
							Li++;
								
						}
						con[Li]='\0';	
						if(strlen(fobj.f3)>0)
						{
						
						strcpy(con1,con);
						strcat(con ,fobj.f3);
						strcat(con ,con1);
						strcat(con,"\n");
						kmy_PrinterString(con);
						}
						 kmy_PrinterString("********************************\n");
						kmy_PrinterString("\n\n");





				if(count==0||count==-1)
				{
					custTra.slno=1;
				}else
				{
					custTra.slno=count+1;
				}
				if(strlen(custTra.custName)>0 && strlen(custTra.custID)>0 && strlen(custTra.amount)>0)
				{
				 	if(custTra.slno>0)
					 {
					  custTra.slno=1;
					  kmy_FlashEraseSector(0x050000);
					  }
					  printf("\n Befor writing sl==%d Name==%s ID==%s Ammount==%s\n",custTra.slno,custTra.custName,custTra.custID,custTra.amount);
		    		kmy_FlashWriteBuffer((char *)&custTra,0x050000+custTra.slno*256,sizeof(custTra));
					kmy_FlashWaitForWriteEnd();
					}

					}
				   else if(flag==2)//return exit
		         	{
				        return;
			        }	
								 
				/*	 if(SendMessage_TextMode(context,phonenumber)==0){	   	
		              printf("\n Send success!");
	                   }
					   else
					   {
	                       printf("\n Send failure!");
	
	                    }*/
					//	SendMessage("Test Message",	"Test Message");  
					MsgSend("TEST","Test Message","9980071121");
				//	popup (" ADDED");
		}
		
}	


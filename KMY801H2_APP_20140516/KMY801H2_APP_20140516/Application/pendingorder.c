#include "GlobalVar.h"
#include "kmy_keydrv.h"
//#include "GlobalVariable.h" 
  
void pendingorder()
{
  int flag=0,count=-1,count1=1,yfillarea=15;
  struct Msgstore obj,obj1,obj2,obj3;
  memset(&obj,'\0',sizeof(obj));
  callpending:
 // drawline1();
  obj.slno=1;
  count=-1;count1=1;yfillarea=15;
  while(count<256)
	{
				kmy_FlashReadBuffer((char *)&obj,0x070000+obj.slno*256,sizeof(obj));
				myprintf("\n reading smsno==%d sms==%s\n",obj.slno,obj.sms);
				if(obj.slno==0||obj.slno==-1)
				break;
				count=obj.slno;
				obj.slno++;
					
		
	}
	if(count==-1)
	{
	 	popup("   LIST IS EMPTY");
	}else
	{	   kmy_FlashReadBuffer((char *)&obj1,0x070000+1*256,sizeof(obj1));
		   kmy_FlashReadBuffer((char *)&obj2,0x070000+2*256,sizeof(obj2));
		   kmy_FlashReadBuffer((char *)&obj3,0x070000+3*256,sizeof(obj3));
		   clear_area(xstart,ystart,xend,yend);
		   if(obj1.slno>0)
		   kmy_LcdTextOut(5,15,obj1.mobileno);
		   if(obj2.slno>0)
		   kmy_LcdTextOut(5,30,obj2.mobileno);
		   if(obj3.slno>0)
		   kmy_LcdTextOut(5,45,obj3.mobileno);
		   Keydata=0;
		   reverse_area(xstart,yfillarea,xend,15,2);
		   //while(Keydata!=KEY_L1 && Keydata!=KEY_R1)
		    while(Keydata!=KEY_LEFT && Keydata!=KEY_RIGHT)
			{	
				//	ReadDataFSPort();
	       	//		Process_Modem_data();
			 		Keydata=0;
					Keydata=kmy_GetKeyValue();
				//	if(Keydata==KEY_L1)
					if(Keydata==KEY_LEFT)
					{
						flag=1;
						break;
					}else
					// if(Keydata==KEY_R1)
					 if(Keydata==KEY_RIGHT)
					{
						flag=0;
						break;
					}
					if(Keydata==KEY_DOWN && count1<count)
					{	
						count1++;
						clear_area(xstart,yfillarea,xend,15);
						yfillarea+=15;
						if(yfillarea>45)
						{
						 	kmy_FlashReadBuffer((char *)&obj1,0x070000+count1*256,sizeof(obj1));
		   					kmy_FlashReadBuffer((char *)&obj2,0x070000+(count1+1)*256,sizeof(obj2));
		   					kmy_FlashReadBuffer((char *)&obj3,0x070000+(count1+2)*256,sizeof(obj3));	
							yfillarea=15;
							clear_area(xstart,ystart,xend,yend);
						}
						   
						   if(obj1.slno>0)
						   kmy_LcdTextOut(5,15,obj1.mobileno);
						   if(obj2.slno>0)
						   kmy_LcdTextOut(5,30,obj2.mobileno);
						   if(obj3.slno>0)
						   kmy_LcdTextOut(5,45,obj3.mobileno);
						   reverse_area(xstart,yfillarea,xend,15,2);
						   printf("\n DOWN count1==%d\n",count1);
					  
					}else if(Keydata==KEY_UP &&  count1>1)
					{
						count1--;
						clear_area(xstart,yfillarea,xend,15);
						yfillarea-=15;
						if(yfillarea<15)
						{
						 	yfillarea=45;
							kmy_FlashReadBuffer((char *)&obj3,0x070000+count1*256,sizeof(obj3));
		   					kmy_FlashReadBuffer((char *)&obj2,0x070000+(count1-1)*256,sizeof(obj2));
		   					kmy_FlashReadBuffer((char *)&obj1,0x070000+(count1-2)*256,sizeof(obj1));
							clear_area(xstart,ystart,xend,yend);
						}
						   if(obj1.slno>0)
						   kmy_LcdTextOut(5,15,obj1.mobileno);
						   if(obj2.slno>0)
						   kmy_LcdTextOut(5,30,obj2.mobileno);
						   if(obj3.slno>0)
						   kmy_LcdTextOut(5,45,obj3.mobileno);
						   reverse_area(xstart,yfillarea,xend,15,2);
						   printf("\n Up count1==%d\n",count1);
					}

			}// while loop
			if(flag==0)//confirm
			{
				myprintf("\n Count1==%d\n",count1);
				kmy_FlashReadBuffer((char *)&obj2,0x070000+count1*256,sizeof(obj2));
			//	readinbx(obj2);
			//	flag=selectmenu("ACCEPT ORDER","REJECT ORDER","EXIT",3);
				if(flag==1)
				{
				  kmy_FlashEraseSector(0x080000); 
				  count=-1;
				  obj.slno=1;
				  flag=0;
				//  acceptorder1(obj2);
				  while(count<256)
					{
								kmy_FlashReadBuffer((char *)&obj,0x070000+obj.slno*256,sizeof(obj));
								myprintf("\n reading smsno==%d sms==%s\n",obj.slno,obj.sms);
								if(obj.slno==0||obj.slno==-1)
								break;
								if(count1==obj.slno)
								{
									flag++;
								}else
								{	obj1=obj;
									if(flag==0)
									{
										kmy_FlashWriteBuffer((char *)&obj1,0x080000+obj1.slno*256,sizeof(obj1));
									}else
									{	obj1.slno=obj1.slno-flag;
										myprintf("\n Sector 8 Writting slno==%d\n",obj1.slno);
										kmy_FlashWriteBuffer((char *)&obj1,0x080000+obj1.slno*256,sizeof(obj1));	
									}
								}
								count=obj.slno;
								obj.slno++;
									
						
					}
					if(count>0)
					{	  
						  kmy_FlashEraseSector(0x070000);
						  count=-1;
						  obj.slno=1;
						  while(count<256)
							{
										kmy_FlashReadBuffer((char *)&obj,0x080000+obj.slno*256,sizeof(obj));
										myprintf("\n reading smsno==%d sms==%s\n",obj.slno,obj.sms);
										if(obj.slno==0||obj.slno==-1)
										break;
										kmy_FlashWriteBuffer((char *)&obj,0x070000+obj.slno*256,sizeof(obj));
										count=obj.slno;
										obj.slno++;
							}

					}
				   	
				}else if(flag==2)
				{
					
				  kmy_FlashEraseSector(0x080000); 
				  count=-1;
				  obj.slno=1;
				  flag=0;
				  popup2("   PLEASE WAIT","");
				 // rejectorder(obj2);
				  popup2("   PLEASE WAIT","");
				  while(count<256)
					{
								kmy_FlashReadBuffer((char *)&obj,0x070000+obj.slno*256,sizeof(obj));
								myprintf("\n reading smsno==%d sms==%s\n",obj.slno,obj.sms);
								if(obj.slno==0||obj.slno==-1)
								break;
								if(count1==obj.slno)
								{
									flag++;
								}else
								{	obj1=obj;
									if(flag==0)
									{
										kmy_FlashWriteBuffer((char *)&obj1,0x080000+obj1.slno*256,sizeof(obj1));
									}else
									{	obj1.slno=obj1.slno-flag;
										myprintf("\n Sector 8 Writting slno==%d\n",obj1.slno);
										kmy_FlashWriteBuffer((char *)&obj1,0x080000+obj1.slno*256,sizeof(obj1));	
									}
								}
								count=obj.slno;
								obj.slno++;
									
						
					}
					if(count>0)
					{	  
						  kmy_FlashEraseSector(0x070000);
						  count=-1;
						  obj.slno=1;
						  while(count<256)
							{
										kmy_FlashReadBuffer((char *)&obj,0x080000+obj.slno*256,sizeof(obj));
										myprintf("\n reading smsno==%d sms==%s\n",obj.slno,obj.sms);
										if(obj.slno==0||obj.slno==-1)
										break;
										kmy_FlashWriteBuffer((char *)&obj,0x070000+obj.slno*256,sizeof(obj));
										count=obj.slno;
										obj.slno++;
							}

					}
				}else
				{
				 	
				}
				goto callpending;
			}else if(flag==3)//return exit
			{
				return;
			}	
	}



}
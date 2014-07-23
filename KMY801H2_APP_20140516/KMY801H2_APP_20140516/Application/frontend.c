#include "frontend.h"
#include "key.h"
#define VAR_DECLS
#include "GlobalVar.h"
//#include "parseMsg1.c"
#include "InitializeModem.h"
#include "GlobalVariable.h"
#include "ReadFromModem.h"
#include "EepromFileSystem.h"
#include "kmy_EEPROMDrv.h"

void casesensitive(char *ptr)
{
int y=0;
while(ptr[y]!='\0')
{
	ptr[y]=tolower(ptr[y]);
	y=y+1;

}
}
void frontend()//menu
{		int count=-1,act=0,flag5=0;
		struct header obj;
		struct Auro_time Lstat11,timeobj; 	
//		drawline1();
//		popup2("  Booting Device","   Please Wait");
//        flag5=Initialize_Modem();
//		popup2("  Booting Device","   Please Wait");
//		AuroGetTime(&CSQSTime);
	    loadmainmenu();
	    kmy_BatInit();
	   	kmy_TimeInit();
	    kmy_RCCInit();
		kmy_PrinterInit();
		Initialize_Modem();
		Change_Status();	
	   	
	while(1)
	{
		
	    setdata(menus,numitem);
	}
        	
	
}//END FRONTEND
void setdata(char men[10][21],int num)

{		

	int flag=1,act=0,count=-1;
    int setpos=0;	
	char string[100]={'\0'};
	char OPerID1[10];

	if(ok==1)

		{

			yfill=15;

			mainmenupos=1;

			j=0;

			set=0;

			flag1=1;	
		}
			else if(mmenu>0 && smenu1>0 && smenu2>0 && smenu3>0 && rbotton==1)

		{

			

			if(smenu3>3)

			{       

					if(smenu3%3==0) 

					{

							

							set=smenu3-3;

							flag=3;flag1=1;	


					}else

					{

							setpos=smenu3%3;

							set=smenu3-setpos;

							flag1=1;

							flag=setpos;

					}

							mainmenupos=smenu3;

							yfill=fillpos2;

							rbotton=0;

							smenu3=0;

							

			}else

			{				set=0;

							yfill=fillpos2;

							flag=smenu3;

							mainmenupos=smenu3;

							rbotton=0;

							smenu3=0;

							

			}// END NESTED IF

		}else if(mmenu>0 && smenu1>0 && smenu2>0 && rbotton==1)

		{

			

			if(smenu2>3)

			{       

					if(smenu2%3==0) 

					{
						  
						  	set=smenu2-3;

							flag=3;flag1=1;	


					}else

					{

							setpos=smenu2%3;

							set=smenu2-setpos;

							flag1=1;

							flag=setpos;

					}

							mainmenupos=smenu2;

							yfill=fillpos1;

							rbotton=0;

							smenu2=0;

							

			}else

			{				set=0;

							yfill=fillpos1;

							flag=smenu2;

							mainmenupos=smenu2;

							rbotton=0;

							smenu2=0;

							

			}// END NESTED IF
			

		}// END IF ELSE MAIN

		else if(mmenu>0 && smenu1>0 && rbotton==1)

		{	

		

			if(smenu1>3)

			{		if(smenu1%3==0) 

					{

							set=smenu1-3;

							flag=3;flag1=1;	


					}else

					{

							setpos=smenu1%3;

							set=smenu1-setpos;

							flag1=1;

							flag=setpos;

							

					}

							mainmenupos=smenu1;

							yfill=fillpos;

							rbotton=0;

							smenu1=0;

				

			}else

			{				

							

							set=0;

							yfill=fillpos;

							mainmenupos=smenu1;

							flag=smenu1;

							smenu1=0;

							rbotton=0;

			}//END NESTED IF

               

		}//END IF ELSE MAIN

		else if(mmenu>0 && smenu1==0 && rbotton==1)

		{	

			if(mmenu>3)

			{		if(mmenu%3==0)

					{

							

							set=mmenu-3;

							flag=3;flag1=1;	


					}else

					{

							setpos=mmenu%3;

							set=mmenu-setpos;

							flag1=1;

							flag=setpos;

							


					}

							mainmenupos=mmenu;

							yfill=mfillpos;

							rbotton=0;

							mmenu=0;

				

			}else

			{				

							set=0;

							yfill=mfillpos;

							mainmenupos=mmenu;

							flag=mmenu;

							rbotton=0;

							mmenu=0;

			}//END NESTED IF

			

		}//END IF ELSE MAIN

		

			

		



		//myprintf("before print\n");
		clear_area(xstart,ystart,xend,yend);

		if(strlen(men[set+1])>0)

		{

        		kmy_LcdTextOut(xstart,15,men[set+1]);

		}

		

		if(strlen(men[set+2])>0)

		{

			kmy_LcdTextOut(xstart,30,men[set+2]);

		}

		if(strlen(men[set+3])>0)

		{

        		kmy_LcdTextOut(xstart,45,men[set+3]);

		}
		

		reverse_area(xstart,yfill,xend,15,2);


 	            	

		

while(1)

{	
		   Keydata=0;
		//	while(Keydata==0)
	     	Keydata=kmy_GetKeyValue();
		//	printf("sh=%d,jj=%d",Keydata,mmenu);


              
		/*	Keydata=0;
	     	Keydata=kmy_GetKeyValue();
			printf("\n KeyValue=%d",Keydata);	  */
//			drawline1();
			GRFMFlag=1;
			ReadDataFSPort();
	        Process_Modem_data();
		/*	if(counttime>1000)
			{
		     chbattery();
			 counttime=0;
			 }*/
				//drawline1();

//	if(Keydata==KEY_R1)
	if(Keydata==KEY_Enter)

	{	       

			//myprintf("\n confrom is called");

			ok=1;
			
			
			printf("\n mmenu==%d,smenu1==%d,smenu2==%d,smenu3==%d",mmenu,smenu1,smenu2,smenu3);
			if(mmenu>0 && smenu1>0 && smenu2>0 && smenu3==0)

			{			smenu3=mainmenupos;
						if(mmenu==1 && smenu1==1 && smenu2==1 && smenu3==1)
						{
							fillpos2=yfill;
							if(recnum==0)
							{
								smenu3=0;
								//viewload();
								numitem=2;
								rbotton=1;

								ok=0;
								return;
							}	
							//acceptorder(recnum);
							smenu3=0;
							//viewload();
							numitem=2;
							rbotton=1;

							ok=0;
							return;
						}else if(mmenu==1 && smenu1==1 && smenu2==1 && smenu3==2)
						{
							fillpos2=yfill;
							if(recnum==0)
							{
								smenu3=0;
								//viewload();
								numitem=2;
								rbotton=1;

								ok=0;
								return;
							}	
							//rejectorder(recnum);
							smenu3=0;
							//viewload();
							numitem=2;
							rbotton=1;

							ok=0;
							return;
						}else if(mmenu==1 && smenu1==1 && smenu2==1 && smenu3==3)
						{
							fillpos2=yfill;
							smenu3=0;
							//viewload();
							numitem=2;
							rbotton=1;

							ok=0;
							return;
						}

                        }

			else if(mmenu>0 && smenu1>0 && smenu2==0)

			{

                               

						

						smenu2=mainmenupos;
						if(mmenu==1 && smenu1==1 && smenu2==1)

						{	

		                               		 

		                                        

						}//SMENU2=1

						else if(mmenu==1 && smenu1==1 && smenu2==2)

						{

			                //loadimts();
							smenu2=0;

							numitem=menucounter;

							rbotton=1;

							ok=0;

							return;

						

						}

					if(mmenu>1)

						mmenu=0;

						 	

		}//END SUBMENU2	

		//SUBMENU1					

		 else if(mmenu>0 && smenu1==0)

		{		
					   smenu1=mainmenupos;
						if(mmenu==1 && smenu1==1)
					   	{	
							fillpos=yfill;
							if(recnum==0)
							{
								smenu1=0;
								rbotton=1;

								ok=0;
								return;
							}	
							//acceptorder(recnum);
							rbotton=1;
						   	ok=0;
							return;
					   }else if(mmenu==1 && smenu1==2)
					   {	
						   	fillpos=yfill;
							if(recnum==0)
							{
								smenu1=0;
								rbotton=1;
								ok=0;
								return;
							}	
							//rejectorder(recnum);
							smenu1=0;
							rbotton=1;
							ok=0;
							return;
					  }else if(mmenu==1 && smenu1==3)
					  {	
							fillpos=yfill;
							smenu1=0;
							rbotton=1;
							ok=0;
							return;
						}else if(mmenu==2 && smenu1==1)
						{	

							buf[0]='\0';
							fillpos=yfill;
						//	viewmenu();
							rbotton=1;
							ok=0;
							return;
					   }else if(mmenu==2 && smenu1==2)
						{	

							buf[0]='\0';
							fillpos=yfill;
						//	addmenu();
							rbotton=1;
							ok=0;
							return;
					   }
					   else if(mmenu==2 && smenu1==3)
						{	

			                buf[0]='\0';
							fillpos=yfill;
							//viewrejectsms();
							rbotton=1;
							ok=0;
							return;
					   	}else if(mmenu==2 && smenu1==4)

						{	

			                buf[0]='\0';
							fillpos=yfill;
							rbotton=1;
							ok=0;							
							return;
					   }else if(mmenu==3 && smenu1==1)
					   {	
							return;
							
					   

						}else if(mmenu==3 && smenu1==2)

						{	

							

							return;

							

						}

			            else if(mmenu==3 && smenu1==3)

						{
							

							 return;

	

							

						} else if(mmenu==4 && smenu1==1)

						{	
							buf[0]='\0';
							fillpos=yfill;
							//dailyorder();	
						
							rbotton=1;
							ok=0;
							return;
										
								

						}else if(mmenu==4 && smenu1==2)

						{	
							buf[0]='\0';
							fillpos=yfill;
							//datewiseorder();

							rbotton=1;
							ok=0;
							return;

						}

						else if(mmenu==4 && smenu1==3)

						{	

							return;

						}else if(mmenu==4 && smenu1==4)

						{	

							return;

						}else if(mmenu==4 && smenu1==5)

						{	
							return;


						}else if(mmenu==4 && smenu1==6)

						{	
							return;


						}else if(mmenu==4 && smenu1==7)

						{	
							return;


						}else if(mmenu==5 && smenu1==1)
						{	
							buf[0]='\0';
							fillpos=yfill;
//							myprintf("\n header is called\n");
							header();
//							myprintf("\n Return to Frontend\n");
							rbotton=1;
							ok=0;
							return;

						}else if(mmenu==5 && smenu1==2)
						{	
						  	buf[0]='\0';
							fillpos=yfill;
							footer();
							rbotton=1;
							ok=0;
						    return;

						}
					/*	else if(mmenu==5 && smenu1==3)
						{	
							buf[0]='\0';
							fillpos=yfill;	
						//	ordermassage();  
							rbotton=1;
							ok=0;                                                      

							return;

						}else if(mmenu==5 && smenu1==4)
						{	
							buf[0]='\0';
							fillpos=yfill;	
						//	rejectmassage();
							rbotton=1;
							ok=0;   

							return;

						}	*/
						else if(mmenu==5 && smenu1==3)
						//else if(mmenu==5 && smenu1==5)
						{	
							buf[0]='\0';
							fillpos=yfill;
						//	setdatetime();
						//	loadprintersetting();
							//numitem=8;
							numitem=4;
							rbotton=1;
							ok=0;   
							return;
							

						}
						else if(mmenu==5 && smenu1==4)
					//	else if(mmenu==5 && smenu1==6)
						{
						 	buf[0]='\0';
							fillpos=yfill;
							count=-1;
							actobj.slno=1;
							while(count<256)
							{
							 	kmy_FlashReadBuffer((char *)&actobj,0x0310000+actobj.slno*256,sizeof(actobj));
								if(actobj.slno==-1 || actobj.slno==0)
								break;
								count=actobj.slno;
								act=actobj.deactive;
								actobj.slno++;
							}
							if(count==0||count==-1)
							{
							 	actobj.slno=1;
								actobj.deactive=0;
							}else
							{
							  	actobj.slno=count+1;
								actobj.deactive=act;
							}
//							myprintf("\n active slno==%d act==%d\n",actobj.slno,actobj.deactive);
							if(actobj.deactive>255||actobj.slno==1)
							{
							 	actobj.slno=1;
								kmy_FlashEraseSector(0x0310000);
								
							}
							if(actobj.deactive==0)
							{
//								popup(" DEACTIVE SUCCESS");
								actobj.deactive=1;
								kmy_FlashWriteBuffer((char *)&actobj,0x0310000+actobj.slno*256,sizeof(actobj));
								kmy_FlashWaitForWriteEnd();
								
								
							}else if(actobj.deactive==1)
							{
							//	popup("  ACTIVE SUCCESS");
								actobj.deactive=0;
								kmy_FlashWriteBuffer((char *)&actobj,0x0310000+actobj.slno*256,sizeof(actobj));
								kmy_FlashWaitForWriteEnd();
							
								
							}
							//loadprintersetting();
							numitem=4;
							rbotton=1;
							ok=0;   
							return;
						}
					/*	else if(mmenu==5 && smenu1==7)
						{
							buf[0]='\0';
							fillpos=yfill;
						//	clearmemory();
						//	loadprintersetting();
							numitem=8;
							rbotton=1;
							ok=0;   
							return;				
						}
						else if(mmenu==5 && smenu1==8)
						{
							buf[0]='\0';
							fillpos=yfill;
						//	percentage();
						//	loadprintersetting();
							numitem=8;
							rbotton=1;
							ok=0;   
							return;				
						} */
						


		}//END SUBMENU1


		//MAIN MENU

		else if(mmenu==0 && smenu1==0)

		{		 

				 mmenu=mainmenupos;
			

                        

			

			if(mmenu==1)

			{				
							
							//while(keydatalen==0)
							{
							printf("\n in mmenu 1");
							mfillpos=yfill;
								//recnum=inboxcall(1);
							//	pendingorder();
							    addmenu();
								mmenu=0;
							//collection();
								
							}
							rbotton=1;

							ok=0;

							return;

				

			}

             else if(mmenu==2)
			{

				mfillpos=yfill;
			//	loadmenures();
				Download_File();
				//mmenu=0;
				//numitem=2;
				return;

			}else if(mmenu==3)
		   	{
				mfillpos=yfill;
				changepass();
		
				rbotton=1;
				ok=0;
				return;

			}else if(mmenu==4)
			{	
				mfillpos=yfill;		
				loadreport();
		    //	mmenu=0;
				numitem=2;	
				printf("\n in MENU 4");
				return;

			}else if(mmenu==5)
		   	{
				mfillpos=yfill;
				loadprintersetting();
			//	mmenu=0;
				numitem=4;
				return;

			}

		}

				//myprintf("\n main menu=%d",mmenu);			

				//myprintf("\n submenu1 menu=%d",smenu1);

				//myprintf("\n submenu2  menu=%d",mainmenupos);

           			

	}// END MAIN MENU

	//else if(Keydata==KEY_L1	)
		else if(Keydata==KEY_ReturnPrevious)

	{	
	rbotton=1;

		ok=0;

		
		//myprintf("\n mmenu==%d,smenu1==%d,smenu2==%d,smenu3==%d",mmenu,smenu1,smenu2,smenu3);

		//myprintf("\n return is called");

		//myprintf("\nmain===%d",mmenu);

		//myprintf("\nsmain==%d",smenu1);

		//myprintf("\nsmain2=%d",smenu2);
		if(mmenu==1 && smenu1==1 && smenu2==1 && smenu3==0)

		{			
						//viewload();
						numitem=2; 
						return;

                }
		if(mmenu>0 && smenu1>0 && smenu2>0)

		{
			

				//loadutility();

				numitem=menucounter;

				return;

		}

		else if(mmenu>0 && smenu1>0)

		{	

			if(mmenu==1 && smenu1>0)

			{	
			

				//loadimts();

				numitem=menucounter;

				return;

				

			}if(mmenu==2 && smenu1>0)

			{	

			

		//		loadmenures();

			//	numitem=2;

				return;

				

			}if(mmenu==3 && smenu1>0)

			{	

			

		//		loadsms();

				numitem=3;

				return;

				

			}if(mmenu==4 && smenu1>0)

			{	

				

		//		loadmsetting();//loadsetting(); commit by jyotiba 08-08-11

				numitem=2;

				return;

				

			}if(mmenu==5 && smenu1>0)

			{	

			

		//		loadapplication();

				numitem=8;

				return;

				

			}

			

		}else if(mmenu>0 && smenu1==0)

		{	

	    	loadmainmenu();

			//ExtKeyboard();

			numitem=5;

			return;

			

		} 

	}//END OF RETURN

	else if(Keydata==KEY_DOWN)

	{	

		//myprintf("\n down is called");

		if(mainmenupos==num)

		{	

				continue;		

		}

		else

		{	

						flag++;

						mainmenupos++;	

				

			if(flag==4)

			{			set=set+3;

						flag=1;

						yfill=0;

						clear_area(xstart,ystart,xend,yend);

						flag1=0;

							

			}

			if(mainmenupos>5)

			{		

					

					

					if(flag1!=0)

					{

						clear_area(xstart,yfill,xend,15);

					}

					flag1=1;	

					yfill=yfill+15;	

					if(strlen(men[set+1])>0)

					{

						kmy_LcdTextOut(xstart,15,men[set+1]);

					}

					if(strlen(men[set+2])>0)

					{

		                        	kmy_LcdTextOut(xstart,30,men[set+2]);

					}

					if(strlen(men[set+3])>0)

					{			

						kmy_LcdTextOut(xstart,45,men[set+3]);

					}
					

					reverse_area(xstart,yfill,xend,15,2);

					

				

			}

			else 

			{		

					

					clear_area(xstart,yfill,xend,15);

					yfill=yfill+15;	

					if(strlen(men[set+1])>0)

					{

						kmy_LcdTextOut(xstart,15,men[set+1]);

					}

					if(strlen(men[set+2])>0)

					{

		                        	kmy_LcdTextOut(xstart,30,men[set+2]);

					}

					if(strlen(men[set+3])>0)

					{			

						kmy_LcdTextOut(xstart,45,men[set+3]);

					}

						
		                        	reverse_area(xstart,yfill,xend,15,2);

			}		

		}

				

	}//END OF DOWN

	else if(Keydata==KEY_UP)

	{		

			//myprintf("\n up is called");

			if(mainmenupos==1 && yfill==15)

			{

				

				continue;


			}

			else

			{

				

				

				if(mainmenupos>3)

				{	        

						if(yfill==15)

						{

							

							clear_area(xstart,ystart,xend,yend);

							yfill=60;

							set=set-3;

							flag=4;			

							

						}else

						{	

							clear_area(xstart,yfill,xend,15);

							

						}

							

							yfill=yfill-15;

							

					if(strlen(men[set+1])>0)

					{

						kmy_LcdTextOut(xstart,15,men[set+1]);

					}

					if(strlen(men[set+2])>0)

					{

		                        	kmy_LcdTextOut(xstart,30,men[set+2]);

					}

					if(strlen(men[set+3])>0)

					{			

						kmy_LcdTextOut(xstart,45,men[set+3]);

					}

						  

						reverse_area(xstart,yfill,xend,15,2);

						

				}

				else

				{		

							clear_area(xstart,yfill,xend,15);	

							yfill=yfill-15;	

					if(strlen(men[set+1])>0)

					{

						kmy_LcdTextOut(xstart,15,men[set+1]);

					}

					if(strlen(men[set+2])>0)

					{

		                        	kmy_LcdTextOut(xstart,30,men[set+2]);

					}

					if(strlen(men[set+3])>0)

					{			

						kmy_LcdTextOut(xstart,45,men[set+3]);

					}
					
		                        		reverse_area(xstart,yfill,xend,15,2);

				}//END SUB ELSE

					

			}//END OF ELSE

		mainmenupos=mainmenupos-1;

		flag--;

		}//END OF UP BOTTON

	  

	


       

	}// END WHILE*/


 	printf("\n menu not selected");

}//END OF SETDATA


/*void drawline1()
{
	kmy_LcdDrawLine(0,10,126,10,1);
	kmy_LcdDrawLine(0,10,0,62,1);
	kmy_LcdDrawLine(0,62,126,62,1);
    kmy_LcdDrawLine(126,10,126,62,1);

}			 */
/*void selectmenu(char *menu1,char *menu2,char *menu3,int no)
{
	
	int fd,yfill1=15,n1=1;
	clear_area (5, 12, 121, 48);
	clear_area (xstart,yfill1,xend,15);	
	if(strlen(menu1)>0)
	kmy_LcdTextOut(xstart,15,menu1);
	if(strlen(menu2)>0)
	kmy_LcdTextOut(xstart,30,menu2);
	if(strlen(menu3)>0)
	kmy_LcdTextOut(xstart,45,menu3);
	reverse_area(xstart,yfill1,xend,15,2);
	fd=0;
	while(fd!=KEY_R1 && fd!=KEY_L1)
	{
	
	fd=kmy_GetKeyValue();
		if(fd==KEY_L1)
		{
			cn.type=0;
			return ;
		}
		
		if(fd==KEY_DOWN && n1<no)
		{
			n1++;
			clear_area (xstart,yfill1,xend,15);	
			if(strlen(menu1)>0)
			kmy_LcdTextOut(xstart,15,menu1);
			if(strlen(menu2)>0)
		kmy_LcdTextOut(xstart,30,menu2);
			if(strlen(menu3)>0)
			kmy_LcdTextOut(xstart,45,menu3);
			yfill1=yfill1+15;
			reverse_area(xstart,yfill1,xend,15,2);
		}else if(fd==KEY_UP && n1>1)
		{	n1--;
			clear_area (xstart,yfill1,xend,15);	
			yfill1=yfill1-15;	
			if(strlen(menu1)>0)
			kmy_LcdTextOut(xstart,15,menu1);
			if(strlen(menu2)>0)
			kmy_LcdTextOut(xstart,30,menu2);
			if(strlen(menu3)>0)
		kmy_LcdTextOut(xstart,45,menu3);
			reverse_area(xstart,yfill1,xend,15,2);
		}
		
		
		
	}
	cn.type=yfill1/15;
}*/

int countspace (char str[200])
{
  int count = 0, k1 = 0;
  while (str[k1] != '\0')
    {
      if (str[k1] == ' ')
	count++;
      k1++;
    }
  return count;
}
 

void loadmainmenu()

{

	memset(menus,'\0',sizeof(menus));

	strcpy(menus[1],"1.TRANSACTION");

	strcpy(menus[2],"2.DOWNLOAD");

	strcpy(menus[3],"3.CHANGE PASSWORD");

	strcpy(menus[4],"4.REPORT");

	strcpy(menus[5],"5.SETTING");

	numitem=5;

	

	

}



/*void loadmenures()

{

	memset(menus,'\0',sizeof(menus)); 

	strcpy(menus[1],"1.VIEW ITEMS");

	strcpy(menus[2],"2.ADD ITEM");
	//strcpy(menus[3],"3.VIEW REJECT SMS");
}
*/

void loadreport()
{
	memset(menus,'\0',sizeof(menus)); 

	strcpy(menus[1],"1.DAILY REPORT");

	strcpy(menus[2],"2.DATEWISE REPORT");
	
}  

void loadprintersetting()
{
	int count=-1,act=0;
	memset(menus,'\0',sizeof(menus)); 
   	strcpy(menus[1],"1.SET HEADER");
	strcpy(menus[2],"2.SET FOOTER");
	//strcpy(menus[3],"3.ORDER MESSAGE");
	//strcpy(menus[4],"4.REJECT MESSAGE");
//	strcpy(menus[5],"5.DATE TIME SET");
strcpy(menus[3],"3.DATE TIME SET");
	count=-1;
	actobj.slno=1;
	while(count<256)
	{
			kmy_FlashReadBuffer((char *)&actobj,0x0310000+actobj.slno*256,sizeof(actobj));
			if(actobj.slno==-1 || actobj.slno==0)
			break;
			count=actobj.slno;
			act=actobj.deactive;
			actobj.slno++;
	}
	
	if(count==0||count==-1)
	{
	 	actobj.slno=1;
		actobj.deactive=0;
	}else
	{
	  	actobj.slno=count;
		actobj.deactive=act;
	}
//	myprintf("\n Load menu slno==%d act==%d\n",actobj.slno,actobj.deactive);
	if(actobj.deactive==0)
	{
	//	strcpy(menus[6],"6.DEACTIVE");
		strcpy(menus[4],"4.DEACTIVE");
	}else if(actobj.deactive==1)
	{
		//strcpy(menus[6],"6.ACTIVE");
		strcpy(menus[4],"4.ACTIVE");
	}
	//	strcpy(menus[5],"5.CLEAR MEMORY");
	//strcpy(menus[8],"8.SERVICE CHARGE");
//	strcpy(menus[7],"7.CLEAR MEMORY");
//	strcpy(menus[8],"8.SERVICE CHARGE");	
	
}

/* void loadsms()

{

	memset(menus,'\0',sizeof(menus));

	strcpy(menus[1],"1.TEST MESSAGE");

	strcpy(menus[2],"2.INBOX");

	strcpy(menus[3],"3.DELETE");

	//strcpy(menus[3],"3.DELETE ");

}




void loadapplication()

{

	memset(menus,'\0',sizeof(menus));

	strcpy(menus[1],"1.CALCULATOR");

	strcpy(menus[2],"2.MAGNETIC CARD TEST");

        strcpy(menus[3],"3.SMART CARD TEST");

} */
void loadmsetting()

{

		memset(menus,'\0',sizeof(menus));

		//strcpy(menus[1],"1.SET DATE TIME");

		strcpy(menus[1],"1.VOLUME");

		strcpy(menus[2],"2.CHANGE USER/PASS");

		strcpy(menus[3],"3.MESSAGE CENTRE NO");

		strcpy(menus[4],"4.MEMORY STATUS");
		strcpy(menus[5],"5.IP SETTING");

		strcpy(menus[6],"6.READ FILE");
		strcpy(menus[7],"7.ABOUT");


}	 
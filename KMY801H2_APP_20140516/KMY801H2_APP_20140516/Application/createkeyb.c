#include "GlobalVar.h"
#include "kmy_keydrv.h"
void PrntCharcreatesms(int num,char txt1[],char txt2[],char txt3[],char txt4[],char txt5[],int size)
{
	
    if(printvalid!=1)
{
	  
      if(A_len<=size-3)
       {
			    if(D_Xcoord>108&&D_Ycoord<44)
			    {
				clear_area(D_Xcoord+6,D_Ycoord+15,6+1,1);
				drawline1();
			        D_Xcoord=0;
			        D_Ycoord=D_Ycoord+12;
					//D_Xcoord+=6;
			    }
    else if(D_Xcoord>108&&D_Ycoord>32)
    {
	
			 clear_area(5,24,121,36);
			 page++;
			 charlen++;
			 if(page>2)
			{
				page=2;
				charlen=2;
			}
				
		     		 //A_len=-1;
				 D_Ycoord=20;
				 D_Xcoord=0;
			

    }
			if(yfillcurser==1)
			{
				//D_Xcoord=D_Xcoord-6;
				yfillcurser=0;
			}
		 	clear_area(D_Xcoord+6,D_Ycoord+15,6+1,1);
			drawline1();
			if(D_Xcoord==108)
			kmy_LcdDrawLine(6,D_Ycoord+27,12,D_Ycoord+27,2);
			else if(yfillcurser==2||yfillcurser==3)
			{	if(yfillcurser==3)
			 	yfillcurser=0;
				else
				yfillcurser++;
			 }
			 else
			kmy_LcdDrawLine(D_Xcoord+12,D_Ycoord+15,D_Xcoord+18,D_Ycoord+15,2);
		
     if(Keydata==num  && keyno==0)
     {
               
	   	    keyno=1;
           	temp[A_len]=txt1[0]; 
           	kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,txt1);
            start=kmy_TimeGetRtcSeconds();
           	            
     }else if(Keydata==num  && keyno==1)
     {
	           end=kmy_TimeGetRtcSeconds();
	            Gti=end-start;
	          if(Gti>1.0)
	           {                  
	                  temp[A_len]=txt1[0];
	                  keyno=0;
	                  D_Xcoord=D_Xcoord+6;
	                  A_len=A_len+1; 
	                  PrntChar(Keydata,txt1,txt2,txt3,txt4,txt5,size); 
	           }
	
	           else
	           {
	                  keyno=2;
			          temp[A_len]=txt2[0]; 
	                  kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,txt2);
	                  start=kmy_TimeGetRtcSeconds();   
		         
	           }
              
      }else if(Keydata==num && keyno==2)
      {  
	           end=kmy_TimeGetRtcSeconds();
	            Gti=end-start;
	          if(Gti>1.0)
	           {    
	                  temp[A_len]=txt2[0]; 
	                  keyno=0;
	                  D_Xcoord=D_Xcoord+6;
	                  A_len=A_len+1;              
	                  PrntChar(Keydata,txt1,txt2,txt3,txt4,txt5,size); 
	           }
	 
	           else
	           {
	                  keyno=3;
	                  temp[A_len]=txt3[0];  
	                  kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,txt3);
	                  start=kmy_TimeGetRtcSeconds();
	                  buf[0]='\0';
	           }   
      }else  if(Keydata==num && keyno==3)
      {     
	           end=kmy_TimeGetRtcSeconds();
	           Gti=end-start;
	           if(Gti>1.0)
	           {  
	                   temp[A_len]=txt3[0];
	                   keyno=0;
	                   D_Xcoord=D_Xcoord+6;
	                   A_len=A_len+1;
	                   PrntChar(Keydata,txt1,txt2,txt3,txt4,txt5,size); 
	           }
	 
	           else
	           {
	                    keyno=4;
	                    temp[A_len]=txt4[0];
	                    kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,txt4);
	                    start=kmy_TimeGetRtcSeconds();
	                    
	           }  
      }else if(Keydata==num && keyno==4)
      {   
	           end=kmy_TimeGetRtcSeconds();
	            Gti=end-start;
	           if(Gti>1.0)
	           {
	                    temp[A_len]=txt4[0];
	                    keyno=0;
	                    D_Xcoord=D_Xcoord+6;
	                    A_len=A_len+1; 
	                    PrntChar(Keydata,txt1,txt2,txt3,txt4,txt5,size); 
	           }
	           else
	           {
	                    temp[A_len]=txt5[0];  
	                    kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,txt5);
	                    start=kmy_TimeGetRtcSeconds();
	                    keyno=5;
	           }
      }else if(Keydata==num && keyno==5)
      {   
	           end=kmy_TimeGetRtcSeconds();
	            Gti=end-start;
	           if(Gti>1.0)
	           {
	                    temp[A_len]=txt5[0];
	                    keyno=0;
	                    D_Xcoord=D_Xcoord+6;
	                    A_len=A_len+1; 
	                    PrntChar(Keydata,txt1,txt2,txt3,txt4,txt5,size); 
	           }
	           else
	           {
	                    temp[A_len]=txt5[0];  
	                    kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,txt5);
	                    start=kmy_TimeGetRtcSeconds();
	                    keyno=0;
	           }
      }// ladder   
	}
	
}
}
void ChkForAlphacreatesms(char txt[10],int size)
{
	  int delpos=1,Lflag=0;
	  int i=0;
	  char ss[2];
	  ss[1]='\0';
	  kmy_LcdTextOut(5,12,txt);
	  Keydata=0;
	  Keydata=kmy_GetKeyValue();
	  key_defno=0;
	  keyno=0;
      //page=0;charlen=0;
	  printvalid=0;
 while(Keydata!=KEY_Enter)
 { 
	//draw_line(D_Xcoord+6,D_Ycoovrd+14,D_Xcoord+12,D_Ycoord+14,2);
	//draw_line(D_Xcoord+6,D_Ycoord+14,D_Xcoord+12,D_Ycoord+14,2);
      /*if(Keydata==KEY_R2)
      {	
	   
	   charlen=10;
	   loadsms();
		rbotton=1;
		ok=0;
		numitem=4;
		return;
	   //myprintf("\n exit is called\n\n");
     if(mmenu==1&&smenu1==1&&smenu2>0)
	{
		return;
	}else if(mmenu==1&&smenu1==2&&smenu2>0)
	{
		return;

	}else if(mmenu==3&&smenu1==1)
	{
		loadsms();
		rbotton=1;
		ok=0;
		numitem=4;
		return;
		

	}
      }*/
	  
        if(Keydata==KEY1)
      {
	  
          if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
              {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
              }
           }
           PrntCharcreatesms(Keydata,"1",".",",","-","!",size);  
           key_defno=Keydata;
       }

      if(Keydata==KEY2)
      {  
           if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
               {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
               }
           }
           key_defno=Keydata;
           PrntCharcreatesms(Keydata,"A","B","C","2","2",size);  
          
      }
       
      if(Keydata==KEY3)
      {
          if(Keydata==key_defno){}
          else
          {
                if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
          }
          PrntCharcreatesms(Keydata,"D","E","F","3","3",size);  
          key_defno=Keydata;
      }

      if(Keydata==KEY4)
      {
           if(Keydata==key_defno){}
           else
           {
                if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
           }
           PrntCharcreatesms(Keydata,"G","H","I","4","4",size);  
           key_defno=Keydata;
       }

       if(Keydata==KEY5)
       {
           if(Keydata==key_defno){}
           else
           {
                 if(A_len<=size-3)
                 {
                 keyno=0;A_len=A_len+1;
                 D_Xcoord=D_Xcoord+6;
                 }
           }
           PrntCharcreatesms(Keydata,"J","K","L","5","5",size);  
           key_defno=Keydata;
        }

        if(Keydata==KEY6)
        {
           if(Keydata==key_defno){}
           else
           {
                if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
           }
           PrntCharcreatesms(Keydata,"M","N","O","6","6",size);  
           key_defno=Keydata;
        }

       if(Keydata==KEY7)
        {
           if(Keydata==key_defno){}
           else
           {
                if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
           }
           PrntCharcreatesms(Keydata,"P","Q","R","S","7",size);  
          key_defno=Keydata;
        }

       if(Keydata==KEY8)
        {
           if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
               {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
               }
           }
           PrntCharcreatesms(Keydata,"T","U","V","8","8",size);  
           key_defno=Keydata;
        }

      if(Keydata==KEY9)
        {
           if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
               {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
               }
           }
           PrntCharcreatesms(Keydata,"W","X","Y","Z","9",size);  
           buf[0]='\0';
           key_defno=Keydata;
        }

    
     if(Keydata==KEY0)
      {
          if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
                {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
                }
           }
           PrntCharcreatesms(Keydata,"0"," ","0"," ","0",size);  
           key_defno=Keydata;
       }
//if(Keydata!=0)
//printf("\nup charlen=%d page=%d Alen==%d D_Xcoord=%d D_Ycoord=%d",charlen,page,A_len,D_Xcoord,D_Ycoord);
 if(Keydata==KEY_DEL)
   {
myprintf("\nup charlen======%d page==%d==Alen==%d",charlen,page,A_len);
	if(page==charlen)
	{
	
	if(A_len>=56 && A_len<=114|| A_len>=0 && A_len <57)
	{
		int i=0;
		char bs[2];
	        bs[1]='\0';
		if(A_len==114)
		{	if(charlen>=1)
			{
				page--;
				charlen--;
			}
			delpos=1;
			
			D_Xcoord=-6;
			D_Ycoord=20;
			for(i=57;i<=114;i++)
			{

				bs[0]=temp[i];
				D_Xcoord=D_Xcoord+6;
				kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,bs);
				
				if(D_Xcoord>102)
				{
					D_Xcoord=-6;
					D_Ycoord=D_Ycoord+12;
				}
			}
		}else if(A_len==56)
		{	if(charlen>=1)
			{
				page--;
				charlen--;
			}
			delpos=1;
		    clear_area(5,24,121,36);
			D_Xcoord=-6;
			D_Ycoord=20;
			for(i=0;i<57;i++)
			{

				bs[0]=temp[i];
				D_Xcoord=D_Xcoord+6;
				kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,bs);
				if(D_Xcoord>102)
				{
					D_Xcoord=-6;
					D_Ycoord=D_Ycoord+12;
				}

			}
		}
			
		}
		
	if(A_len<delpos)
	{
		 
		 Keydata=0;
		 memset(temp,'\0',sizeof(temp));
		 A_len=-1;
		 D_Xcoord=-6;
		 kmy_LcdTextOut(6,5+D_Ycoord," ");
		 
		 
	}else
     { 		
			if(D_Xcoord<0&&D_Ycoord>20)
              {
		 		 D_Xcoord=108;
                 D_Ycoord=D_Ycoord-12;
     			}
				kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord," ");
            if(D_Xcoord==108)
		    clear_area(6,D_Ycoord+26,12,1);
		    else
		    clear_area(D_Xcoord+12,D_Ycoord+14,D_Xcoord+18,2);
		    drawline1();
		    if(D_Xcoord>=6)
			{
		      kmy_LcdDrawLine(D_Xcoord+6,D_Ycoord+14,D_Xcoord+12,D_Ycoord+14,2);
			  
			}
			
			
			D_Xcoord=D_Xcoord-6;
		  	D_Ycoord=D_Ycoord;
            A_len=A_len-1;
            key_defno=0;
            keyno=0;
			printf("\n DElpos==%d A_len==%d\n",delpos,A_len);
		    }
           
	   }
    Keydata=0;
         }
	if(Keydata==KEY_UP)
	{
		myprintf("\nup charlen======%d page==%d==Alen==%d",charlen,page,A_len);
		
		if(A_len<56)
		{

			//myprintf("\n if true uppage==%d",page);
		}else
		{
			//myprintf("\n else true up page==%d",page);
			//myprintf("\n chlen=============%d",A_len);
			if(A_len>=56)
			{	
				if(page==1)
				{
				page--;
				printvalid=1;
				yfillcurser=1;
                    clear_area(5,24,121,36);
					D_Xcoord=-6;
					D_Ycoord=20;
					for(i=0;i<57;i++)
					{
						
						if(i>A_len)
						{	break;
						}
						ss[0]=temp[i];
						D_Xcoord=D_Xcoord+6;
						kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,ss);
					    if(D_Xcoord>=108)
							{
								D_Xcoord=-6;
								D_Ycoord=D_Ycoord+12;
							}    
						
					}


				}
				else if(page==2)
					{
					yfillcurser=1;
					printvalid=1;
					clear_area(5,24,121,36);
					D_Xcoord=-6;
					D_Ycoord=20;
					page--;
					for(i=57;i<=114;i++)
					{
							
						if(i>A_len)
						{		
							break;
						}
							ss[0]=temp[i];
							D_Xcoord=D_Xcoord+6;
							
							kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,ss);
							 if(D_Xcoord>=108)
							{
								D_Xcoord=-6;
								D_Ycoord=D_Ycoord+12;
							}  

					}
				}
				
				}
				
					


			//}		

		}
         Keydata=0;

	}
	if(Keydata==KEY_DOWN)
	{
		myprintf("\nup charlen======%d page==%d==Alen==%d",charlen,page,A_len);
		
		if(page==charlen)
		{
		
			printvalid=0;
			//myprintf("\nzzzzzzzzzzzzzzzzzzzzzzz=%d",page);
			//myprintf("\nzzzzzzzzzzzzzzzzzzzzz11=%d",charlen);	
		}else
		{
			//myprintf("\nelse zzzzzzzzzzzzzzzzzzzzzzz=%d",page);
			//myprintf("\nelse zzzzzzzzzzzzzzzzzzzzz11=%d",charlen);	
			printvalid=0;	
			if(A_len>=56)
			{	
				
			if(page==0)
			{		page++;
					clear_area(5,24,121,36);
					D_Xcoord=-6;
					D_Ycoord=20;
					yfillcurser=1;
			
					for(i=57;i<=114;i++)
					{
							
							if(i>A_len)
						{		
							break;
						}
							ss[0]=temp[i];
							D_Xcoord=D_Xcoord+6;
						
							kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,ss);
							if(D_Xcoord>=108)
							{
								D_Xcoord=-6;
								D_Ycoord=D_Ycoord+12;
							}  

					}
				}
			else if(page==1)
			{
				page++;
				yfillcurser=1;
				clear_area(5,24,121,36);
					D_Xcoord=-6;
					D_Ycoord=20;
					for(i=114;i<=size;i++)
				{
					if(i>A_len)
						{		
							break;
						}
					ss[0]=temp[i];D_Xcoord=D_Xcoord+6;
					
					kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,ss);
					if(D_Xcoord>=108)
							{
								D_Xcoord=-6;
								D_Ycoord=D_Ycoord+12;
							}  

			}	}
			}
                       
			}
			
			Keydata=0;
	}
 		Keydata=0;
	    Keydata=kmy_GetKeyValue();
 }     
   
   temp[A_len+1]='\0';
   myprintf("\ntemp=%s %d\n",temp,A_len);
   buf[0]='\0';   
}



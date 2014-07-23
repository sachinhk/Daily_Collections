/***********************************
Author:Jyotiba Vaijinath kadam
Date:2011-12-26 20:09
Remarks:terminal kayboard
LastModified:Date:2011-12-26 20:09
Remarks:terminal kayboard
************************************/
//#include "viewdetails.c"
#include "kmy_KeyDrv.h"
#include "GlobalVar.h"
void prntforclear(char text[40])
{
  int i=1,x=0,y=20;
   while(text[i]!='\0')
  {
      kmy_LcdTextOut(5+x,5+y," ");
      x=x+6;
      i=i+1;
      if(x>108)
      {
        x=0;
        y=y+12;
      }
  }
   kmy_LcdTextOut(5+x,5+y," ");
}
void PrntNo(int num,char txt1[],int size)
{
    if(A_len<=size-3)
    {
    if(D_Xcoord>108)
    {
        D_Xcoord=6;
        D_Ycoord=D_Ycoord+12;
    }
   if(Keydata==num  && keyno==0)
     {
	       keyno=1;
           temp[A_len]=txt1[0]; 
           kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,txt1);
           start=kmy_TimeGetRtcSeconds();
           buf[0]='\0';      
          
     }
   }
}
void PrntChar(int num,char txt1[],char txt2[],char txt3[],char txt4[],char txt5[],int size)
{
  
  if(A_len<=size-3)
  {
     if(D_Xcoord>108)
    {
        D_Xcoord=0;
        D_Ycoord=D_Ycoord+12;
    }
 
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
      }   
 
  }
}


void ChkForAlpha(char txt[10],int size)
{
   kmy_LcdTextOut(5,12,txt);
   Keydata=0;
   Keydata=kmy_GetKeyValue();
   key_defno=0;
   keyno=0;
    while(Keydata!=KEY_Enter)
    { 

     
	 //printf("\n data====%d\n",Keydata); 
    /* if(back==2&&strlen(temp)==57||back==2&&strlen(temp)==114)
    {
	
	 	 clear_area(5,24,121,36);
     		 
		 D_Ycoord=20;
		 D_Xcoord=-6;

    }*/ // jyotiba 04/28/12
   

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
           PrntChar(Keydata,"1",".",",","-","!",size);  
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
          
		   //printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$\n");
           PrntChar(Keydata,"A","B","C","2","2",size);
		    key_defno=Keydata;  
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
          PrntChar(Keydata,"D","E","F","3","3",size);  
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
           PrntChar(Keydata,"G","H","I","4","4",size);  
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
           PrntChar(Keydata,"J","K","L","5","5",size);  
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
           PrntChar(Keydata,"M","N","O","6","6",size);  
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
           PrntChar(Keydata,"P","Q","R","S","7",size);  
           buf[0]='\0';
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
           PrntChar(Keydata,"T","U","V","8","8",size);  
         
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
           PrntChar(Keydata,"W","X","Y","Z","9",size);  
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
           PrntChar(Keydata,"0"," ","0"," ","0",size);  
           buf[0]='\0';
           key_defno=Keydata;
       }



       
       if(Keydata==KEY_DEL)
       {
            if(A_len<0)
            {
                buf[0]='\0';
            }
            else
            { 
            buf[0]='\0';
            if(D_Xcoord<0&&D_Ycoord>20)
              {
                 D_Xcoord=108;
                 D_Ycoord=D_Ycoord-12;
              }
            kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord," ");
            D_Xcoord=D_Xcoord-6;
            D_Ycoord=D_Ycoord;
            A_len=A_len-1;
             key_defno=0;
            keyno=0;
            }
       }
     
		 Keydata=0;
         Keydata=kmy_GetKeyValue();
	
 }     
    
   
   temp[A_len+1]='\0';
   myprintf("\ntemp=%s %d\n",temp,A_len);
    buf[0]='\0';   
}

 /*
selection(char txt1[10],char txt2[10],char txt3[10],char txt4[10],int no)
{
        if(strcmp(CALL_Status,"RINGING")==0)
           			{    strcpy(CALL_Status,"DISCONNECT");
                                  }
	if(buf[0]==KEY_UP)
        {
		if(pos==0)
    	        {
			buf[0]='\0';
    	        }
     	        else
     	        {
     	       	        pos=pos-1;
                  	clear_area(R_Xcoord, R_Ycoord, RFill_Ycoord, RFill_Xcoord);
                        kmy_LcdTextOut(10,fxcor,txt1);
                        kmy_LcdTextOut(10,sxcor,txt2);
			kmy_LcdTextOut(10,txcor,txt3);
                       // kmy_LcdTextOut(10,70,txt4);
                        R_Ycoord=R_Ycoord-15;
                        fill_rect( R_Xcoord, R_Ycoord, RFill_Ycoord, RFill_Xcoord,2);
                      //  RFill_Ycoord=115,RFill_Xcoord=15;
     	        }
        }
  		 
        if(buf[0]==KEY_DOWN)
        {
     
               if(pos==no-1)
               {
                        buf[0]='\0';
               }
               else
               {
                        pos=pos+1;
                        clear_area(R_Xcoord, R_Ycoord, RFill_Ycoord, RFill_Xcoord);
                        kmy_LcdTextOut(10,fxcor,txt1);
                        kmy_LcdTextOut(10,sxcor,txt2);
			kmy_LcdTextOut(10,txcor,txt3);
                        //kmy_LcdTextOut(10,70,txt4);
                        R_Ycoord=R_Ycoord+15;
                        fill_rect( R_Xcoord, R_Ycoord, RFill_Ycoord, RFill_Xcoord,2);
                      //  RFill_Ycoord=115,RFill_Xcoord=15;
               }
        }
 cn.type=pos;

}  */



void ChkForDig(char txt[10],int size)
{
  
int page=0;
  buf[0]='\0';

  kmy_LcdTextOut(5,12,txt);
  Keydata=0;
         Keydata=kmy_GetKeyValue();
 while(KEY_Enter!=Keydata)
 { 
        
      if(Keydata==KEY1)
      {
           end=kmy_TimeGetRtcSeconds();
            Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }

          if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
               {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
               }
           }
           PrntNo(Keydata,"1",size);  
           key_defno=Keydata;
       }

      if(Keydata==KEY2)
      {  
            end=kmy_TimeGetRtcSeconds();
            Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
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
           PrntNo(Keydata,"2",size);  
          
      }
       
      if(Keydata==KEY3)
      {
           end=kmy_TimeGetRtcSeconds();
            Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
          if(Keydata==key_defno){}
          else
          {
                if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
          }
          PrntNo(Keydata,"3",size);  
         
          key_defno=Keydata;
      }

      if(Keydata==KEY4)
      {

            end=kmy_TimeGetRtcSeconds();
            Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
           if(Keydata==key_defno){}
           else
           {
                if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
           }
           PrntNo(Keydata,"4",size);  
           buf[0]='\0';
           key_defno=Keydata;
       }

       if(Keydata==KEY5)
       {
            end=kmy_TimeGetRtcSeconds();
            Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
           if(Keydata==key_defno){}
           else
           {
                 if(A_len<=size-3)
                 {
                 keyno=0;A_len=A_len+1;
                 D_Xcoord=D_Xcoord+6;
                 }
           }
           PrntNo(Keydata,"5",size);  
          
           key_defno=Keydata;
        }

        if(Keydata==KEY6)
        {
            end=kmy_TimeGetRtcSeconds();
            Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
           if(Keydata==key_defno){}
           else
           {
		if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
           }
           PrntNo(Keydata,"6",size);  
          
           key_defno=Keydata;
        }

       if(Keydata==KEY7)
        {
            end=kmy_TimeGetRtcSeconds();
           Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
           if(Keydata==key_defno){}
           else
           {
		if(A_len<=size-3)
                {
                keyno=0;A_len=A_len+1;
                D_Xcoord=D_Xcoord+6;
                }
           }
           PrntNo(Keydata,"7",size);  
          
           key_defno=Keydata;
        }

       if(Keydata==KEY8)
        {
            end=kmy_TimeGetRtcSeconds();
           Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
           if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
                {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
                }
           }
           PrntNo(Keydata,"8",size);  
           
           key_defno=Keydata;
        }

      if(Keydata==KEY9)
        {
            end=kmy_TimeGetRtcSeconds();
           Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
           if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
               {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
               }
           }
           PrntNo(Keydata,"9",size);  
           
           key_defno=Keydata;
        }

    
     if(Keydata==KEY0)
      {
           end=kmy_TimeGetRtcSeconds();
           Gti=end-start;
           if(Gti>=0.0)
           {
              key_defno=0;
           }
          if(Keydata==key_defno){}
           else
           {
               if(A_len<=size-3)
                {
               keyno=0;A_len=A_len+1;
               D_Xcoord=D_Xcoord+6;
                }
           }
           PrntNo(Keydata,"0",size);  
          
           key_defno=Keydata;
       }
      
       if(Keydata==KEYXING)
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
           PrntChar(Keydata,"+","-",".","/","w",size);  
           
           key_defno=Keydata;
       }
       
	
        if(Keydata==KEY_DEL)
       {
	
	{
            if(A_len<0)
            {
                buf[0]='\0';
            }
            else
            { 
            buf[0]='\0';
            if(D_Xcoord<6&&D_Ycoord>20)
              {
                 D_Xcoord=108;
                 D_Ycoord=D_Ycoord-12;
              }
            kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord," ");
            D_Xcoord=D_Xcoord-6;
            D_Ycoord=D_Ycoord;
            A_len=A_len-1;
            key_defno=0;
            keyno=0;
            }
       }
    }
 	 Keydata=0;
         Keydata=kmy_GetKeyValue();
 

     
}
 temp[A_len+1]='\0';
 temp[155]='\0';
  myprintf("\ntemp=%s %d\n",temp,A_len);
    buf[0]='\0';   

}


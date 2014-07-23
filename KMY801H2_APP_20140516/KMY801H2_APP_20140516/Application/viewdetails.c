/***********************************
Author:Jyotiba Vaijinath kadam
Date:2012-01-01 17:54
Remarks:view all transactions details  
LastModified:Date:2012-02-03 14:14
Remarks:
************************************/
#include "GlobalVar.h"
#include "keyboard.h"
#include "viewdetails.h"
int show(char a[160],char txt[40],int size)
{
    int i;
    char s[2];
  	D_Ycoord=20;
	D_Xcoord=-6;
	clear_area(5,12,121,48);
	kmy_LcdTextOut(5,12,txt);
    myprintf("********entered show detail*******");
    s[1]='\0';
	page=0;
    charlen=0;
	chklen=strlen(a);
	A_len=chklen-1;
   for(i=0;i<chklen;i++)
   {

        s[0]=a[i];
		D_Xcoord=D_Xcoord+6;
		kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,s);
		//if(i==chklen-1)
		//kmy_LcdTextOut(D_Xcoord+6,5+D_Ycoord," ");
		if(i==56 || i==113)
		{ 
		 page++;
		 charlen++;
		D_Xcoord=-6;
		D_Ycoord=20;
		clear_area(5,22,121,42);
		//drawline1();	
		if(page>2)
		{
		page=2;
		charlen=2;
		}
		}
	    if(D_Xcoord>=108)
		 {	//text_out(5+D_Xcoord+6,5+D_Ycoord," ");
			D_Xcoord=-6;
			D_Ycoord=D_Ycoord+12;
		}
		if(A_len==56)
		D_Xcoord=-6;
	}
  kmy_LcdTextOut(11+D_Xcoord,5+D_Ycoord," ");
  printf("\n#####a[]=%s xco==%d len==%d",a,D_Xcoord,strlen(a));
  //ChkForAlphacreatesms(txt,size);
  myprintf("\nreturned from print\n");
  return 1;
}
int showdetails(char a[40],char txt[40],int size)
{
   int i,fd;
   char s[1];
  D_Ycoord=20;
   	D_Xcoord=-6;


  
  
	clear_area(5,12,121,48);
   myprintf("********entered show detail*******");
    s[1]='\0';
   for(i=0;i<chklen;i++)
   {
       s[0]=a[i];
       
       myprintf("s[0]=%c",s[0]);
        
		D_Xcoord=D_Xcoord+6;
		kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,s);
		  if(D_Xcoord>102)
    		{	kmy_LcdTextOut(5+D_Xcoord+6,5+D_Ycoord," ");
			D_Xcoord=-6;
			D_Ycoord=D_Ycoord+12;
		}
		
		if(i==chklen-1)
		kmy_LcdTextOut(5+D_Xcoord+6,5+D_Ycoord," ");
	
	
  }
   myprintf("\na[]=%s",a);
   ChkForAlpha(txt,size);
   myprintf("\nreturned from print\n");
   return 1;
}
 
int showdetForDigit(char a[40],char txt[40],int size)
{
   int i,fd;
   char s[1];
   
 

	D_Ycoord=20;
   	D_Xcoord=-6;
	clear_area(5,12,121,48);
   myprintf("********entered show detail*******");
    s[1]='\0';
   for(i=0;i<chklen;i++)
   {
       s[0]=a[i];
       myprintf("s[0]=%c",s[0]);
       D_Xcoord=D_Xcoord+6;
	   kmy_LcdTextOut(5+D_Xcoord,5+D_Ycoord,s);
	   if(D_Xcoord>102)
    		{	 kmy_LcdTextOut(5+D_Xcoord+6,5+D_Ycoord," ");
			D_Xcoord=-6;
			D_Ycoord=D_Ycoord+12;
		}
		
	 if(i==chklen-1)
	 kmy_LcdTextOut(5+D_Xcoord+6,5+D_Ycoord," ");
	
	 
       
   }

 myprintf("\na[]=%s",a);
 
   
  ChkForDig(txt,size);
  
 
  
  return 0;
}


int checklength(char b[40])
{  
   myprintf("********entered checklen*******");
   
   myprintf("\n enterd here %s\n",b);
   while(b[chklen]!='\0')
   {
        chklen=chklen+1;
   }
   A_len=chklen-1;
  myprintf("\nchklen=%d",chklen);
  return 0;
}   

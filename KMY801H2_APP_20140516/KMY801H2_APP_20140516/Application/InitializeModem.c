/***********************************
Author:Satish
Date:2012-02-02 15:05
Remarks:initialisation modem
LastModified:Date:2012-02-02 15:05
Remarks:initialisation modem
************************************/
#include <stdlib.h>
#include <string.h>
#define GPB0_H 51
#define GPB0_L 52
#define GPB1_H 61
#define GPB1_L 62
#include "InitializeModem.h"
 
//#include "calMaxamt.c"
//#include "clearfiles.c"
//#include "datediff1.c"
 
int Initialize_Modem()
{
	char string[1024],imeino[20];//=NULL;
	int Vol = 3,i,LModemStatus=0,fdinkey,Lmc=0,Lnc=0,gpiofd,dbclr=0,speaker,j=0;
	int winc = 0,inc = 0;
	long LstartTime,LendTime;
    
	char SPKFlag = 1,x,chk='\0';
	char buf[10]={'\0'};
        char *vol;
        char todaydate[20],Lexetest[20],Lchklatestdate[10];
        char dateclr[10],Ldelmsgcommand[10],storedate[20];			 
		 int Lfd_ttyS1,ring_fd1;
      
     long ts;

		 long     now;
		
	     /*if(Lexe!=NULL)
        {
         fgets(Lexetest,20,Lexe);
        fclose(Lexe);
        myprintf("\n\nLexetest=%s\n",Lexetest);
        memset (Decryptvalue, '\0', sizeof (Decryptvalue));
	  Decrypt (Lexetest,'s');
        if(strcmp(Decryptvalue,"aurodisplay")==0)
        {*/
      myprintf("GAN\n\n");
     
        // now = kmy_TimeGetRtcSeconds();
	ts = kmy_TimeGetRtcSeconds();
        
	//string = (char *)malloc(1024*sizeof(char)); //prev 1024
       
       
       
       
        
      memset(string,'\0',1024);
 
       LstartTime=kmy_TimeGetRtcSeconds();
	write(Lfd_ttyS1,"AT+CPIN?\r\n",strlen("AT+CPIN?\r\n"));
	//send_at_cmd	("AT+CPIN?\r\n");
       LModemStatus=0;
       
       while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,400000);
            
	     if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			if(strstr(string,"ERROR") != NULL)
                        {
			LModemStatus=1;
                       // clear_area(5,12,121,48);
                       // popup(" INSERT SIM CARD");
			return; 
                        }
                   break;     
                   
            }
           LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               LModemStatus=1;
               //clear_area(5,12,121,48);
               //popup(" INSERT SIM CARD");
               return;
            }
          }

         myprintf("Data From Modem =%s\n",string);

        myprintf("\n ATE called");
        //myprintf("\n\nLexetest=%s\n",Lexetest);
           LstartTime=kmy_TimeGetRtcSeconds();
	write(Lfd_ttyS1,"ATE1\r\n",strlen("ATE1\r\n"));
      
       
       while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,400000);
            
	     if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			if(strstr(string,"ERROR") != NULL)
                        {
			LModemStatus=1;
                     //   clear_area(5,12,121,48);
                       // popup(" Echo Not Enabled");
			 return;
                        }
                        break;
                  
            }
           LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               LModemStatus=1;
             //  clear_area(5,12,121,48);
              // popup(" Echo Not Enabled");
               return;
            }
          }
	   myprintf("Data From Modem =%s\n",string);


       //imei number
           myprintf("\n imei called");
       LstartTime=kmy_TimeGetRtcSeconds();
        write(Lfd_ttyS1,"AT+CGSN\r\n",strlen("AT+CGSN\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();
		 while(1)
         {
             memset(string,'\0',sizeof(string));
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
 

           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
			LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          }
  inc = 0;
  winc = 0;
  
  while (string[inc] != '\r')
    {
      inc = inc + 1;
    }
  inc = inc + 1;
  while (string[inc] != '\r')
    {
      inc = inc + 1;
    }
  inc = inc + 1;
  while (string[inc] != '\n')
    {
      inc = inc + 1;
    }
	inc = inc + 1;
  while (string[inc] != '\r')
    {
      imeino[winc] = string[inc];
      winc = winc + 1;
      inc = inc + 1;
    }
  imeino[winc] = '\0';
  strncpy(imeino,&string[10],15);
  myprintf ("\nimeino=%s inc=%d winc=%d\n", imeino,inc,winc);
  myprintf("\n imei number=%s\n",string);
  if(strcmp("862170010587978",imeino)!=0)
  {
   			//popup("CONSULT OPERATOR");
			LModemStatus=1;
			
  }
 


       
     /*  while(1)
         {
           memset(GIMEI,'\0',sizeof(GIMEI));
	     ReadDatasFromtty1(Lfd_ttyS1,GIMEI,0,400000);
            
	     if(strstr(GIMEI,"OK") != NULL || strstr(GIMEI,"ERROR") != NULL)
		{
			if(strstr(GIMEI,"ERROR") != NULL)
                        {
			LModemStatus=1;
                      //  clear_area(5,12,121,48);
                       // popup(" IMEI Not Enabled");
			 return;
                        }
                        break;
                  
            }
           LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               LModemStatus=1;
              // clear_area(5,12,121,48);
              // popup(" IMEI Not Enabled");
               return;
            }
          }
	   myprintf("Data From Modem =%s\n",GIMEI); */



           myprintf("\nCMGF called %i\n\n",Lfd_ttyS1);
         write(Lfd_ttyS1,"AT+CMGF=1\r\n",strlen("AT+CMGF=1\r\n"));
       
         
	   LstartTime=kmy_TimeGetRtcSeconds();
	
	   while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,400000);
           
	     if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{ 
                  if(strstr(string,"ERROR") != NULL)
                      {
			LModemStatus=1;
                       //  clear_area(5,12,121,48);
                        //popup("CAN'T SUPPORT SMS");
			return;
                      }
                   break;

            }
           LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               LModemStatus=1;
              // clear_area(5,12,121,48);
               //popup("CAN'T SUPPORT SMS");
               return;
            }
          }
      
         myprintf("\nCNMI  called\n\n");
	   LstartTime=kmy_TimeGetRtcSeconds();
	   myprintf("Data From Modem =%s\n",string);
         write(Lfd_ttyS1,"AT+CNMI = 1,1,0,0,0\r\n",strlen("AT+CNMI = 1,1,0,0,0\r\n")); 
         while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
            

	   if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			if(strstr(string,"ERROR") != NULL)
                        {
			LModemStatus=1;
                     //    clear_area(5,12,121,48);
                       // popup("CAN'T SHOW SMS");
			return;
                        }
                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               LModemStatus=1; 
               //clear_area(5,12,121,48);
                        //popup("CAN'T SHOW SMS");
               return;
            }

          }

         myprintf("Data From Modem =%s\n\n\n\n",string);
         myprintf("\nCreg  called\n\n");
	   LstartTime=kmy_TimeGetRtcSeconds();

         write(Lfd_ttyS1,"AT+CREG?\r\n",strlen("AT+CREG?\r\n")); 
         while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
            
           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			if(strstr(string,"ERROR") != NULL)
                        {
			LModemStatus=1;
                      //  clear_area(5,12,121,48);
                      //  popup("  SIM NOTREGISTERED");
			return;
                        }
                     break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
                    LModemStatus=1;
                     // clear_area(5,12,121,48);
                       // popup("  SIM NOTREGISTERED");
               return;
            }

 
	     //break;
          }

         myprintf("Data From Modem =%s\n\n\n\n",string); 
         myprintf("\nCSQ  called\n\n");
	   LstartTime=kmy_TimeGetRtcSeconds();

         write(Lfd_ttyS1,"AT+CSQ\r\n",strlen("AT+CSQ\r\n")); 
         while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
            
           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

	     //break;
          }
         //  if(LModemStatus!=1)
          // strcpy(Sig_status,string); 
           myprintf("Data From Modem =%s,l=%d\n\n\n\n",string,strlen(string)); 
         myprintf("\nCBC  called\n\n");
	   LstartTime=kmy_TimeGetRtcSeconds();

         write(Lfd_ttyS1,"AT+CADC?\r\n",strlen("AT+CADC?\r\n")); 
         while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
            
           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

	     //break;
          }
          //if(LModemStatus!=1 &&strlen(Sig_status)>0)
          //ExtKeyboard();                                 //IMPORTANT FOR EXTERNAL KEYBOARD (JYOTIBA)
	//keybordactive=keyboardflag;                      //IMPORTANT FOR EXTERNAL KEYBOARD (JYOTIBA)
         // strcpy(Bat_status,string);
         // sigstrength(Sig_status,Bat_status);
          myprintf("\nCSQ  called\n\n");
//          myprintf("Data From Modem =%s,l=%d\n\n\n\n",Sig_status,strlen(Sig_status));
         myprintf("Data From Modem =%s\n\n\n\n",string);
        
	   myprintf("\nCALM  called\n\n"); //to disanble default ring tone
        
         //  AT+STTONE  = 19 sets indian Dial Tone
	LstartTime=kmy_TimeGetRtcSeconds();
	
         write(Lfd_ttyS1,"AT+STTONE=?\r\n",strlen("AT+STTONE=?\r\n")); 
        while(1)
         {
           memset(string,'\0',1024);
	     ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
           myprintf("Data From Modem =%s\n\n\n",string);

	     break;
          }
        myprintf("Data From Modem =%s\n\n\n\n",string);
        write(Lfd_ttyS1,"AT+CALM=0\r\n",strlen("AT+CALM=0\r\n"));
        while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
            
             if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }
  
             //break;
          }
         LstartTime=kmy_TimeGetRtcSeconds();
         myprintf("Data From Modem =%s\n\n\n\n",string);
	/*  write(Lfd_ttyS1,"AT+CLVL=20\r\n",strlen("AT+CLVL=20\r\n"));
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
 
            if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			if(strstr(string,"ERROR") != NULL)
			LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               LModemStatus=1;
               break;
            }

             //break;
          }

        myprintf("Data From Modem =%s\n\n\n\n",string);
	  LstartTime=kmy_TimeGetRtcSeconds();*/

	 write(Lfd_ttyS1,"AT+CHFA=1\r\n",strlen("AT+CHFA=0\r\n"));
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
            
		if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
              // LModemStatus=1;
               break;
            }

              
          }
          myprintf("Set Call Busy");
	  LstartTime=kmy_TimeGetRtcSeconds();
	  myprintf("Data From Modem =%s\n\n\n\n",string); 
          write(Lfd_ttyS1,"ATX4\r\n",strlen("ATX4\r\n"));
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
           myprintf("Data From Modem =%s\n\n\n\n",string);
              
             break;
          } 
     /*write(Lfd_ttyS1,"ATO\r\n",strlen("+++\r\n"));
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
           myprintf("Data From Modem =%s\n\n\n\n",string);

             break;
          } 


        write(Lfd_ttyS1,"ATX\x4\r\n",strlen("ATX0x4\r\n"));
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
           myprintf("Data From Modem =%s\n\n\n\n",string);

             break;
		
          } 
         write(Lfd_ttyS1,"ATS6?\r\n",strlen("ATS1,9,20\r\n"));
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
           myprintf("Data From Modem =%s\n\n\n",string);

             break;
          }*/ 
 
        myprintf("\n SAt ATH issued");
        write(Lfd_ttyS1,"ATH\r\n",strlen("ATH\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();
        memset(string,'\0',1024);
         while(1)
         {
           
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
         
             if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
		   myprintf("\n Time out ATH");
               break;
            }

             //break;
          } 
        
        myprintf("Data From Modem =%s\n\n\n\n",string);
        /*write(Lfd_ttyS1,"AT+CLDTMF= 2,\"#,#,*,#,#,*,#,#,*,#,#,*,#,#,*\"\r\n",strlen("AT+CLDTMF= 2,\"A,B,C,D,0,1,2,3,4,5,6,7,8,9,#,*\"\r\n"));
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
           myprintf("Data From Modem =%s\n\n\n",string);

             break;
          }*/
      /*write(Lfd_ttyS1,"AT+CCWA=?\r\n",strlen("AT+CCWA=?\r\n"));
         LstartTime=kmy_TimeGetRtcSeconds();

         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
 

           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          } 
         myprintf(" Exit from ccwa?=Data From Modem =%s\n\n\n\n",string);
        write(Lfd_ttyS1,"AT+CCWA=1,0\r\n",strlen("AT+CCWA=1,0\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();

         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
 
		if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }
	
              
          } 
         LstartTime=kmy_TimeGetRtcSeconds();
        myprintf("Exit from ccwa=1 = Data From Modem =%s\n\n\n\n",string);
         write(Lfd_ttyS1,"AT+COPS?\r\n",strlen("AT+COPS?\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();

         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
 

           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          }*/ // jyotiba 05/10/12 ccwa cal wating enable  
         LstartTime=kmy_TimeGetRtcSeconds();
         myprintf("Data From Modem =%s\n\n\n\n",string);
        write(Lfd_ttyS1,"AT+COLP=1\r\n",strlen("AT+COLP=1\r\n"));
 
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
 

             if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
                 //if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          } 

          LstartTime=kmy_TimeGetRtcSeconds();    
         myprintf("Data From Modem =%s\n\n\n\n",string); 
        write(Lfd_ttyS1,"AT+CRC=1\r\n",strlen("AT+CRC=1\r\n"));
            while(1)         
           {          
                memset(string,'\0',1024);
              ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
                 if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
 		{                  
//if(strstr(string,"ERROR") != NULL) 			
//LModemStatus=1;                    
 break;             
} 		
LendTime=kmy_TimeGetRtcSeconds();           
 if(difftime1(LendTime,LstartTime)>60)           
  {               
 //LModemStatus=1;             
   break;             
}           
 }          
 myprintf("Data From Modem =%s\n\n\n\n",string);
         myprintf("Data From Modem =%s\n\n\n\n",string);
        write(Lfd_ttyS1,"AT+CLIP=1\r\n",strlen("AT+COLP=1\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();

         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
            

             if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
                 //if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          } 
          myprintf("Data From Modem =%s\n\n\n\n",string);
          write(Lfd_ttyS1,"AT+GSV\r\n",strlen("AT+GSV\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();

         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
            

             if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
                  //if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          } 
           myprintf("Data From Modem =%s\n\n\n\n",string);
	     
        LstartTime=kmy_TimeGetRtcSeconds();
         write(Lfd_ttyS1,"AT+CLVL?\r\n",strlen("AT+CLVL?\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();

         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
 

           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          } 

          myprintf("Data From Modem =%s\n\n\n\n",string);  
          LstartTime=kmy_TimeGetRtcSeconds();
       /*  vol=strstr(string," ");
          if(vol!=NULL)
         GSpeakerVolume=0;
          else
           GSpeakerVolume=0;*/



          /* AT CSCA*/

          
        LstartTime=kmy_TimeGetRtcSeconds();
         write(Lfd_ttyS1,"AT+CSCA?\r\n",strlen("AT+CSCA?\r\n"));
        LstartTime=kmy_TimeGetRtcSeconds();

         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
 

           if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
			//if(strstr(string,"ERROR") != NULL)
			//LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               //LModemStatus=1;
               break;
            }

          } 
         myprintf("Data From Modem =%s\n\n\n\n",string);
          myprintf("string length =%d\n\n\n\n",strlen(string));
       //  strncpy(GMCnumber, string+10,23);
        while(string[Lmc]!='\"' && string[Lmc]!='\0')
          {
              Lmc=Lmc+1;
          }
           Lmc=Lmc+1;
       /*    while(string[Lmc]!='\"' && string[Lmc]!='\0')
          {
              GMessageCNo[Lnc]=string[Lmc];
              Lmc=Lmc+1;
              Lnc=Lnc+1;
          }*/
//         myprintf("\n Message centre no=%s",GMessageCNo);
   /* end CSA*/
         
         

       
        /*write(Lfd_ttyS1,"AT+SMEXTRAUNSOL=0\r\n",strlen("AT+SMEXTRAUNSOL=0\r\n"));
         
         while(1)
         {
           memset(string,'\0',1024);
             ReadDatasFromtty1(Lfd_ttyS1,string,0,80000);
            
		if(strstr(string,"OK") != NULL || strstr(string,"ERROR") != NULL)
		{
                   if(strstr(string,"ERROR") != NULL)
			LModemStatus=1;

                   break;
            }
		LendTime=kmy_TimeGetRtcSeconds();
           if(difftime1(LendTime,LstartTime)>60)
            {
               LModemStatus=1;
               break;
            }

              
          } 
           
         myprintf("Data From Modem =%s\n\n\n\n",string);*/
          //Ring();   
         //close_lcd();     
	
       
	
        
        //this is to read teminal details
         

         //this function is to calculate amount of transactions done by the day
          
       

        myprintf("\n\n");
       //this section is to check weather the report sent by the terminal to block the terminal for todays transaction
       
       
        
       //check datacall management numbers exists or not
      
    //this section is to check weather the copy of files exists during the cear database process and rename that to orginal name

     

    //this section is to delete data-base 
             
        
		  
		 //popup("  INITIALIZING....");
           
     //end of delete data-base section
		
	
     //to delete messages of sim
   for(Lnc=1;Lnc<=30;Lnc++)
       {
        memset(string,'\0',sizeof(string));
        sprintf(Ldelmsgcommand,"AT+CMGD=%d\r\n",Lnc);
        write(Lfd_ttyS1,Ldelmsgcommand,strlen(Ldelmsgcommand));
        ReadDatasFromtty1(Lfd_ttyS1,string,0,40000);
        usleep(10000);
        myprintf("string==%s",string);
       }

                   
 
  printf("lmodsta=%d",LModemStatus);
        // LModemStatus=0;
      return LModemStatus;
   }
  /*else
   { LModemStatus=0;
     popup(" COPIED VERSION");
   }
 }
  else
   { LModemStatus=0;
     popup(" COPIED VERSION");
   }
  }*/

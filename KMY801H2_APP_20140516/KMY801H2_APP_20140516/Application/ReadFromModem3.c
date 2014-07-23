#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ReadFromModem.h"
#define VAR_DECLSS
#include "GlobalVariable.h"
#include "kmy_USART3Drv.h"
 
void AuroGetTime_start()
{
 	unsigned int  year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min; 
	unsigned char sec;
	unsigned char weekday;
 kmy_TimeGetTime(&year,&mon,&day,&hour,&min,&sec,&weekday);
 Ghs=hour;
 Gms=min;
 Gss=sec;

 }

void AuroGetTime_end()
{
 	unsigned int  year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min; 
	unsigned char sec;
	unsigned char weekday;
 kmy_TimeGetTime(&year,&mon,&day,&hour,&min,&sec,&weekday);
 
 Ghe=hour;
 Gme=min;
 Gse=sec;

 }


long AuroGetTime()
{
 unsigned int  year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min; 
	unsigned char sec;
	unsigned char weekday;
 kmy_TimeGetTime(&year,&mon,&day,&hour,&min,&sec,&weekday);
 return ( ((hour*60*60)+(min*60)+sec));
 }

void AuroGetDateTime(struct Auro_time *t)
{
 kmy_TimeGetTime(&(t->year),&(t->mon),&(t->day),&(t->hour),&(t->min),
 &(t->sec),&(t->weekday));
}
 char* Itoa(int value, char* str, int radix) 
{
 static char dig[] =
 "0123456789"
 "abcdefghijklmnopqrstuvwxyz";
 int n = 0, neg = 0;
 unsigned int v;
 char* p, *q;
 char c;
 if (radix == 10 && value < 0) {
 value = -value;
 neg = 1;
 }
 v = value;
 do {
 str[n++] = dig[v%radix];
 v /= radix;
 } while (v);
 if (neg)
 str[n++] = '-';
 str[n] = '\0';

 for (p = str, q = p + (n-1); p < q; ++p, --q)
 c = *p, *p = *q, *q = c;
 return str;
 }

 


 
void write(int fd,char *str,int leng)
 {
 	send_at_cmd(str);
 }

long difftime1(long endt,long stt)
{
  	return (endt-stt);
} 

int difftime_sten()
{
  int temp=0,tdf;
  int h1,m1,s1,h2,m2,s2;
 // printf("\n @@@Time %d,%d %d %d %d %d",Ghs,Gms,Gss,Ghe,Gme,Gse);
   
   h1=Ghe;
   m1=Gme;
   s1=Gse;
   h2=Ghs;
   m2=Gms;
   s2=Gss;


    if(s1<s2)
	{
		s1=s1+60;
		m1--;
	}
	
	if(m1<m2)
	{
		m1+=60;
		h1--;	    
	}
	
	temp=s1-s2;

	if(m1>m2 && h1>=h2)
	{
		temp+=(m1-m2)*60;
	}

	return temp;

}

int ReadDatasFromtty1(int fd,unsigned char *rcv_buf,int sec,int usec)
{
	int reclen;
	int size=1023;
	reclen=kmy_USART3ReceiveData(rcv_buf,size,sec*1000+usec/1000);
	*(rcv_buf+reclen)=0;

	return reclen;
}
int ReadDataFSPort()
    {
           
           unsigned char string[1024];  
		   int i;
	     //SetRawMode(fd_ttySS1);	
           //while(1)  sat apr 9 12 
	      {
           		memset(string,'\0',1024);
                   
			i=ReadDatasFromtty1(fd_ttySS1,string,0,20000);
			 
			 
                      if(strlen(string)>0)
	                 {
                       
                        printf("Retunr from UP %s,%d,%d\n",string,fd_ttySS1,GRFMFlag);                        
                        Update_To_Que(string);

                       }

            }
           //close(fd_ttySS1);
             
	    
	       
	 return 1;        
                
    }           

             
   
    
int Update_To_Que(unsigned char LSRdata[1024])  //String LSRdata)
    {
        int i=0;
          char c;
//           printf("Update to Que Called");
            while(i< strlen(LSRdata)) //LSRdata.length())
            {

                c=LSRdata[i];
            
                if(Rear_End==1024)
                {
                    if(Front_End==0)
                    {
                     //printf("QueFull");       // System.out.println("Que Full");
                     continue;
                    }
                    else
                    {
                       Rear_End=0;
                    }
                }
               /* if((Rear_End+1) == Front_End)
                {
                     //printf("Que Full"); //System.out.println("Que Full");
                     continue;
                }*/
                  //System.out.print("c=4"+c);
                   //printf("Update Que Called Value");
                   Modem_data[Rear_End]=c;
               /*    if(c=='\r')
                   {
		       printf("I got slash r in Que\n\n");
                   }*/
                   Rear_End++;
                   i++;



               }
        

		GRFMFlag=1;
        }
    int Process_Modem_data()
    {
        
	  char temp[1000];
	  char LvarStoTerm[1024],*strptr;
	  char t=26;
        char LcmglIndex[40];   
		char Lc;
		int LtimeDiff=0;
         
          //printf("CSQ DATA =%s,l=%d\n\n\n\n",Sig_status,strlen(Sig_status)); 
         //try
       //  {
           //  System.out.println("Process Called");
            // System.out.println("Front Pos"+Front_End+";Rear="+Rear_End);
          
	    //write(fd_ttySS1,"AT+CMGL=\"ALL\"\r\n",strlen("AT+CMGL=\"ALL\"\r\n"));   
	    //strcpy(CMGL_Status,"Busy");
	    //CMGLSTime=AuroGetTime();
	    
          //LsTime=1;
	    
	    //AuroGetTime(&CBCSTime);
           //strcpy(CALL_Status,"CALL_DATA");
		   CSQETime=AuroGetTime();
			CMGLETime==AuroGetTime();
		    if(strcmp(MSGSend_Status,"OK")!=0  || strcmp(MSGSend_Status,"OK")!=0 || 
					    strcmp(MSGRead_Status,"OK")!=0 || strcmp(MSGDelete_Status,"OK")!=0 || strcmp(CMGL_Status,"OK")!=0 ||   strcmp(CSQ_Status,"OK")!=0 || strcmp(CBC_Status,"OK")!=0 || strcmp(CLV_Status,"OK")!=0 || strcmp(MSC_Status,"OK")!=0 ||  strcmp(CALL_Status,"IDLE")!=0 || RMIDFend != RMIDRend || DMIDFend != DMIDRend || SMIDFend != SMIDRend)	
	{
		GRFMFlag=1;
		GStartTime=AuroGetTime();
		//printf("\n Time %uc,%uc,%uc",GStartTime.hour,GStartTime.min,GStartTime.sec);
//		printf("\nIn Sendin Messga: Gdat=%d ;Loc%d, C=%s,R=%s,D=%s,S=%s,Re=%s,L=%s,V=%s,Ba=%s,Sig=%s,MSC=%s,GRF=%d,%d,%d\n\n\n",SMIDFend,SMIDRend,CALL_Status,MSGRead_Status,MSGDelete_Status,MSGSend_Status,REC_Status,CMGL_Status,CLV_Status,CBC_Status,CSQ_Status,MSC_Status,GRFMFlag,Front_End , Rear_End);

	}
	else if( (difftime1(CSQETime,CSQSTime)>=30.0) || difftime1(CMGLETime,CMGLSTime)>=100.0)
	{
	  GRFMFlag=1;
		GStartTime=AuroGetTime();
	}
	

         while(GRFMFlag==1)
          {
//	            printf("Process Called");           
		     
		                if(LsTime>0)
                 	{
			       AuroGetTime_end();
				   LtimeDiff=difftime_sten();
                      if(LtimeDiff>=90.0)
                        {
                               
                               //printf("\n Entered in Timer %s,%s,%s",MSGSend_Status,MSGRead_Status,MSGDelete_Status);
							   //printf("\n Entered in Timer %d,%d,%d,%d,%d,%d",Lend.hour,Lend.min,Lend.sec,Lstat1.hour,Lstat1.min,Lstat1.sec);
								 myprintf("\n @@@Time %d,%d %d %d %d %d",Ghs,Gms,Gss,Ghe,Gme,Gse);
			      
                               if(strcmp(MSGSend_Status,"Busy")==0  || strcmp(MSGSend_Status,"RBusy")==0 || 
					    strcmp(MSGRead_Status,"Busy")==0 || strcmp(MSGDelete_Status,"Busy")==0 || strcmp(CMGL_Status,"Busy")==0|| 
					    strcmp(CSQ_Status,"Busy")==0 || strcmp(CBC_Status,"Busy")==0 || strcmp(CLV_Status,"Busy")==0 || strcmp(MSC_Status,"Busy")==0)	
					   {  
                                    if(strcmp(MSGRead_Status,"Busy")==0)
						LMSGReadTC++;
                                    else if(strcmp(MSGRead_Status,"Busy")==0)
						LMSGDeleteTC++;
						else if(strcmp(MSGRead_Status,"Busy")==0)
						LMSGSendTC++;
						
						if(LMSGReadTC>2)
						{
							RMIDFend++;
                                		LsTime=0;
			       	  		LError_Count=0;
							LMSGReadTC=0;
		                              if(DMIDRend==50)
            		                    {
                                    	     DMIDRend=0;
                                    	  }
                                		DMessage_ID[DMIDRend]=RMessage_ID[RMIDFend-1];
                                		DMIDRend++;

						}
						else if(LMSGDeleteTC>2)
						{
							DMIDFend++;
                                		LsTime=0;
			       	  		LError_Count=0;
							LMSGDeleteTC=0;
		                              
						}
						else if(LMSGSendTC>2)
						{
							SMIDFend++;
                                		LsTime=0;
			       	  		LError_Count=0;
							LMSGSendTC=0;
		                              
						}

                        strcpy(MSGSend_Status,"OK");
				    	strcpy(MSGRead_Status,"OK");
                        strcpy(MSGDelete_Status,"OK");
						strcpy(CMGL_Status,"OK");
						strcpy(CSQ_Status,"OK");
						strcpy(CBC_Status,"OK");
						strcpy(CLV_Status,"OK");
						strcpy(MSC_Status,"OK");

                                 }
                     
					  else if(strcmp(CALL_Status,"RINGING")==0)
				 	   {
                                   strcpy(CALL_Status,"IDLE");//check Reciever Status then assign required message
                                 }
  					  else if(strcmp(CALL_Status,"CONNECTING")==0)
				 	   {
                                   strcpy(CALL_Status,"IDLE");//check Reciever Status then assign required message
                                 }
					  else if(strcmp(CALL_Status,"DISCONNECTING")==0)
				 	   {
						sprintf(LvarStoTerm,"ATH\r\n");
                 				write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 				strcpy(Last_Command,"ATH");
                        			LCommandLineCount=0;
						LsTime=0;
						LError_Count=0;
						
 				 		//memset(PhoneNumber,'\0',sizeof(PhoneNumber));
			     			//memset(DispPhoneNo,'\0',sizeof(DispPhoneNo));
	                 			strcpy(CALL_Status,"IDLE1_COM");
                                   		
                                 	}
					  else if(strcmp(CALL_Status,"DISCONNECTINGD")==0)
				 	   {
                                   strcpy(CALL_Status,"IDLE");// check Reciever Status then assign required message and
										  // call disconnected status
                                 }
                               else if(strcmp(CALL_Status,"DIALED")==0)
				 	   {
                                    strcpy(CALL_Status,"IDLE");// check Reciever Status then assign required message and
						strcpy(Dial_Status,"IDLE");		  // call disconnected status
                                 }
					 else if(strcmp(CALL_Status,"IDLE2")==0)
				 	   {
                                   strcpy(CALL_Status,"IDLE");// check Reciever Status then assign required message and
						strcpy(Dial_Status,"IDLE");		  // call disconnected status
                                 }
					 else if(strcmp(CALL_Status,"RECDISCONNECT")==0)
				 	   {
                                   strcpy(CALL_Status,"IDLE");// check Reciever Status then assign required message and
						strcpy(Dial_Status,"IDLE");		  // call disconnected status
                                 }

					
                                AuroGetTime_start(); 
                                 LsTime=0;
                                // //printf("Time=%f",difftime1(Lend,Lstat1));
                                 if(strcmp(CALL_Status,"DISCONNECT_DATA")==0 || strcmp(CALL_Status,"DISCONNECTING_DATA")==0 
				    || strcmp(CALL_Status,"CALL_DATA")==0 || strcmp(CALL_Status,"DATADIALED_COM")==0 
				  || strcmp(CALL_Status,"DATADIALED")==0 || strcmp(CALL_Status,"DATARECOK")==0 )
				{
					write(fd_ttySS1,"+++",strlen("+++"));                        
			  		usleep(1000000);
                          		write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));   
			  		ERROR_Count=0;
                          		strcpy(Last_Command,"ATH");
			  		LCommandLineCount=1;
	              			//strcpy(Dial_Status,"DIALED_COM");
                    			strcpy(CALL_Status,"DISCONNECTING_DATA");
                                        AuroGetTime_start(); 
                                        LsTime=1;
					LMDstat=AuroGetTime();
				}

                        }
                            
                              
                             
                  }
             

             if(strcmp(MSGRead_Status,"OK")==0 && strcmp(MSGDelete_Status,"OK")==0 && strcmp(MSGSend_Status,"OK")==0 && strcmp(CMGL_Status,"OK")==0 && strcmp(CSQ_Status,"OK")==0 && (strcmp(CLV_Status,"OK")==0 || strcmp(CLV_Status,"SET")==0 ) )
             {
               // 14/10/11 process_call();
                  //   if(strcmp(Dial_Status,"DIAL")==0)
		   //printf("ReadMODEMDialed Number %s\n\n\n",Dial_No);
		if(strcmp(CALL_Status,"IDLE")==0 && strcmp(Dial_Status,"DIAL")==0 && strcmp(REC_Status,"ONH")==0)
                {
		        //printf("Dialed Number %s\n\n\n",Dial_No);
                    write(fd_ttySS1,Dial_No,strlen(Dial_No));   
			  ERROR_Count=0;
                    //strcpy(PhoneNumber,Dial_No);
			  strcpy(Last_Command,Dial_No);
			  LCommandLineCount=1;
	              strcpy(Dial_Status,"DIALED_COM");
                    strcpy(CALL_Status,"DIALED_COM");
                    CALLTime=AuroGetTime();
					LsTime=1;
                    AuroGetTime_start(); 
			 LCommandLineCount=1;
			LMDstat=AuroGetTime();
			  continue;
	
		 }

                
               else if(strcmp(CALL_Status,"DISCONNECT_DATA")==0)// && strcmp(Dial_Status,"DIAL")==0 && strcmp(REC_Status,"ONH")==0)
                {
			  write(fd_ttySS1,"+++",strlen("+++"));                        
			  usleep(1000000);
                          write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));   
			  ERROR_Count=0;
                          strcpy(Last_Command,"ATH");
			  LCommandLineCount=1;
	              //strcpy(Dial_Status,"DIALED_COM");
                    strcpy(CALL_Status,"DISCONNECTING_DATA");
		    GRecDCallData[0]='\0';
		    GRecDCallData[1]='\0';	
                    CALLTime=AuroGetTime();
			LsTime=1;
                        AuroGetTime_start(); 
			      LCommandLineCount=1;
			   LMDstat=AuroGetTime();
		    //strcpy(GDataTOSend,"GAN;\n");
                     
			  continue;
	
		 }


                else if(strcmp(CALL_Status,"REC_DATACALL")==0 && strlen(Dial_No)>0)// && strcmp(Dial_Status,"DIAL")==0 && strcmp(REC_Status,"ONH")==0)
                {
                          usleep(1000);
                         
                    write(fd_ttySS1,"ATA\r\n",strlen("ATA\r\n"));   
			  ERROR_Count=0;
                      strcpy(Last_Command,"ATA");
			  LCommandLineCount=1;
	              //strcpy(Dial_Status,"DIALED_COM");
                    strcpy(CALL_Status,"ACC_DATACALLCOM");
		    GRecDCallData[0]='\0';
		    GRecDCallData[1]='\0';	
                    CALLTime=AuroGetTime();
			LsTime=1;
                        AuroGetTime_start(); 
			      LCommandLineCount=1;
			   LMDstat=AuroGetTime();
		    //strcpy(GDataTOSend,"GAN;\n");
                     
			  continue;
	
		 }
                else if(strcmp(CALL_Status,"CALL_DATA")==0)// && strcmp(Dial_Status,"DIAL")==0 && strcmp(REC_Status,"ONH")==0)
                {
                          usleep(1000);
                               //printf("\nDATA CALL: Gdat=%d ;Loc%d, C=%s,R=%s,D=%s,S=%s,Re=%s,L=%s,V=%s,Ba=%s,Sig=%s,MSC=%s\n\n\n",DataCTranStatus,LCommandLineCount,CALL_Status,MSGRead_Status,MSGDelete_Status,MSGSend_Status,REC_Status,CMGL_Status,CLV_Status,CBC_Status,CSQ_Status,MSC_Status);

   
                        //strcpy(Dial_No,"ATD9740217699\r\n");
		        //printf("Dialed Number %s\n\n\n",Dial_No);
                    write(fd_ttySS1,Dial_No,strlen(Dial_No));   
			  ERROR_Count=0;
                    //strcpy(PhoneNumber,Dial_No);
			  strcpy(Last_Command,Dial_No);
			  LCommandLineCount=1;
	              //strcpy(Dial_Status,"DIALED_COM");
                    strcpy(CALL_Status,"DATADIALED_COM");
			  GRecDCallData[0]='\0';
		    	  GRecDCallData[1]='\0';	
                    CALLTime=AuroGetTime();
		    	  //strcpy(GDataTOSend,"GAN;\n");
                    LsTime=1;
                    AuroGetTime_start(); 
			  LCommandLineCount=1;
			  LMDstat=AuroGetTime();
			  continue;
	
		 }
               else if(strcmp(CALL_Status,"CONNECT")==0)
               {
                 write(fd_ttySS1,"AT+CHFA=0\r\n",strlen("AT+CHFA=0\r\n"));   
			
                 
                 /*sleep(1); 
                 sprintf(LvarStoTerm,"ATA\r\n");
                 write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 strcpy(CALL_Status,"BUSY");*/
			strcpy(Last_Command,"AT+CHFA=0");
			LCommandLineCount=1;
                 strcpy(CALL_Status,"CONNECTING_COM");
			LsTime=1;
                       AuroGetTime_start(); 
			      LCommandLineCount=1;
			  LMDstat=AuroGetTime();
		     continue;
               }
               else if(Front_End == Rear_End && strcmp(CALL_Status,"DISCONNECT")==0)
               {
                 ////printf("Diconnet issued");
                  write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));
			ERROR_Count=0;

                 /*sleep(1);
                 sprintf(LvarStoTerm,"ATH\r\n");
                 write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 strcpy(CALL_Status,"IDLE");*/
		     LCommandLineCount=1;
		     LsTime=1;
                        AuroGetTime_start(); 
			      LCommandLineCount=1;	
		     LMDstat=AuroGetTime();
	
		     strcpy(Last_Command,"ATH");

                 strcpy(CALL_Status,"DISCONNECTING_COM");
			continue;

               }
              else if(strcmp(CALL_Status,"DISCONNECTD")==0)
                {
                   //printf("Diconnet issued");
		      sprintf(LvarStoTerm,"ATH\r\n"); //6/5/11
                  write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 
		      strcpy(Last_Command,"ATH");
                  LCommandLineCount=1;
                      //memset(PhoneNumber,'\0',sizeof(PhoneNumber));
			    // memset(DispPhoneNo,'\0',sizeof(DispPhoneNo));
                 strcpy(CALL_Status,"DISCONNECTINGD_COM");	
			LsTime=1;
                        AuroGetTime_start(); 
			      LCommandLineCount=1;
			LMDstat=AuroGetTime();
			continue;

                }
		  else if(Front_End == Rear_End && strcmp(CALL_Status,"IDLE")==0 && strcmp(REC_Status,"ONHE")==0 )
                {  	
                         
 	     			write(fd_ttySS1,"AT+CLDTMF= 2,\"#,#,*,#,#,*,#,#,*,#,#,*,#,#,*\"\r\n",strlen("AT+CLDTMF= 2,\"A,B,C,D,0,1,2,3,4,5,6,7,8,9,#,*\"\r\n"));
           			strcpy(Last_Command,"AT+CLDTMF= 2,\"#,#,*,#,#,*,#,#,*,#,#,*,#,#,*\"");
                        //printf("\n InlasComm=%s",Last_Command);
           			strcpy(REC_Status,"ONH_COM");
				LCommandLineCount=1;
				LsTime=1;
                        	AuroGetTime_start(); 
			        LCommandLineCount=1;
				LMDstat=AuroGetTime();
				continue;


                 }
             else if(Front_End == Rear_End && strcmp(CALL_Status,"IDLE")==0 && strcmp(REC_Status,"OFHE")==0 ) 
                  {
                  	write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));    
				LCommandLineCount=1;
                      	strcpy(Last_Command,"ATH");
                      	strcpy(REC_Status,"OFHM_COM");
			      LsTime=1;
                        AuroGetTime_start(); 
			      LCommandLineCount=1;
				LMDstat=AuroGetTime();
				continue;


			}
                  
              }
               if( (strcmp(CALL_Status,"IDLE")==0 || strcmp(CALL_Status,"DIALEDB")==0 || strcmp(CALL_Status,"BUSY")==0) && (strcmp(REC_Status,"OFH")==0 || strcmp(REC_Status,"ONH")==0) && strcmp(MSGRead_Status,"OK")==0 && strcmp(MSGDelete_Status,"OK")==0 
			&& strcmp(MSGSend_Status,"OK")==0 && strcmp(CMGL_Status,"OK")==0 && strcmp(CSQ_Status,"OK")==0 && strcmp(CBC_Status,"OK")==0 && 
                  (strcmp(CLV_Status,"OK")==0 || strcmp(CLV_Status,"SET")==0) && (strcmp(MSC_Status,"OK")==0 || strcmp(MSC_Status,"SET")==0))
               {
		          CMGLETime=AuroGetTime();
                  CSQETime=AuroGetTime();
			       CBCETime=AuroGetTime();
               
                  if(strcmp(MSC_Status,"SET")==0)
			{	
				sprintf(LvarStoTerm,"AT+CSCA=\"%s\"\r\n",GMessageCNo);
		     		write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));	
				ERROR_Count=0;
			 	strcpy(Last_Command,LvarStoTerm);
 	                  strcpy(MSC_Status,"Busy_COM");
                        LsTime=1;
                        AuroGetTime_start();
			      LCommandLineCount=1;
			 LMDstat=AuroGetTime();

                   continue;

			}
       
		     else if(strcmp(CLV_Status,"SET")==0)
			{	
				sprintf(LvarStoTerm,"AT+CLVL=%d\r\n",GSpeakerVolume);
		     		write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));	
				ERROR_Count=0;
			 	strcpy(Last_Command,LvarStoTerm);
 	                  strcpy(CLV_Status,"Busy_COM");
                        LsTime=1;
                        AuroGetTime_start();
			      LCommandLineCount=1;
			 LMDstat=AuroGetTime();

                   continue;

			}
                 else if(Front_End == Rear_End && RMIDFend != RMIDRend) 
                 {
                    
                   //System.out.println("Message"+RMIDFend);
                    if(RMIDFend==50)
               	   {
                      RMIDFend=0;
                     }

                    //printf("Message=%d\n\n\n",RMIDFend);
                   //printf("Message=%d\n\n\n",RMIDFend);
                    sprintf(LvarStoTerm,"AT+CMGR=%d\r\n",RMessage_ID[RMIDFend]);
                    write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                   //os.write(("AT+CMGR="+RMessage_ID[RMIDFend]+"\r\n").getBytes());
                    
			 ERROR_Count=0;
			 strcpy(Last_Command,LvarStoTerm);
 	
                   strcpy(MSGRead_Status,"Busy_COM");
                   //strcpy(MSGReadData,"");
                   memset(MSGReadData,'\0',1000);
                   //Thread.sleep(1000);
			 LsTime=1;
                  AuroGetTime_start();
			 LCommandLineCount=1;
			  LMDstat=AuroGetTime();
                   continue;
                 }
                 else if(DMIDFend != DMIDRend) // && strcmp(MSGDelete_Status,"OK")==0 && strcmp(MSGRead_Status,"OK")==0 && strcmp(MSGSend_Status,"OK")==0 && strcmp(CMGL_Status,"OK")==0))
                  {
                     //System.out.println("Delete Message"+DMIDFend);
			   if(DMIDFend==50)
               	   {
                        DMIDFend=0;
                     }
	
                     //printf("Delete Message %d",DMIDFend);
                     sprintf(LvarStoTerm,"AT+CMGD=%d\r\n",DMessage_ID[DMIDFend]);
                     write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
			   ERROR_Count=0;
                     strcpy(Last_Command,LvarStoTerm);

    
                     //os.write(("AT+CMGD="+DMessage_ID[DMIDFend]+"\r\n").getBytes());
                      strcpy(MSGDelete_Status,"Busy_COM");
                     //Thread.sleep(1000);
			    LsTime=1;
			    LCommandLineCount=1;
			    LMDstat=AuroGetTime();
                      AuroGetTime_start();
	
                      continue;
                   }
                  else if(SMIDFend != SMIDRend)// && strcmp(MSGRead_Status,"OK")==0 && strcmp(MSGDelete_Status,"OK")==0 && strcmp(MSGSend_Status,"OK")==0 && strcmp(CMGL_Status,"OK")==0))
                   {
                       //printf("\nIn Sendin Messga: Gdat=%d ;Loc%d, C=%s,R=%s,D=%s,S=%s,Re=%s,L=%s,V=%s,Ba=%s,Sig=%s,MSC=%s\n\n\n",SMIDFend,SMIDRend,CALL_Status,MSGRead_Status,MSGDelete_Status,MSGSend_Status,REC_Status,CMGL_Status,CLV_Status,CBC_Status,CSQ_Status,MSC_Status);

   
                      //printf("Sen Message%d\n\n",SMIDFend);
			    if(SMIDFend==50)
               	    {
                        SMIDFend=0;
                      }

                      //sprintf(LvarStoTerm,"AT+CMGS=\"8553710926\"\r\n");
                      sprintf(LvarStoTerm,"%s%s%s%s%s","AT+CMGS=","\"",SMessage_MobNo[SMIDFend],"\"","\r\n");
					  printf("\n%s%s%s%s%s","AT+CMGS=","\"",SMessage_MobNo[SMIDFend],"\"","\r\n");
		      //sprintf(LvarStoTerm,"%s%s%s%s%s","AT+CMGS=","\"","0208903805","\"","\r\n");
                      write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
			    ERROR_Count=0;
	
                      //os.write(("AT+CMGS=\"9980071121\"\n").getBytes());
			    strcpy(Last_Command,LvarStoTerm);

                      strcpy(MSGSend_Status,"RBusy_COM");
			    LsTime=1;
			    LMDstat=AuroGetTime();
                      AuroGetTime_start();
			    LCommandLineCount=1;

                      continue;
                  }
			else if(difftime1(CMGLETime,CMGLSTime)>=100.0 && Front_End == Rear_End)
                   {
	                       write(fd_ttySS1,"AT+CMGL=\"ALL\"\r\n",strlen("AT+CMGL=\"ALL\"\r\n"));
				         strcpy(Last_Command,"AT+CMGL=\"ALL\"");
	                       strcpy(CMGL_Status,"Busy_COM");
				     AuroGetTime_start(); 
				     LMDstat=AuroGetTime();
	                       LsTime=1;
				     
		                 LCommandLineCount=1;
	                       continue;

 	             }
			else if(difftime1(CSQETime,CSQSTime)>=30.0 && Front_End == Rear_End)
                   {
                       write(fd_ttySS1,"AT+CSQ\r\n",strlen("AT+CSQ\r\n"));
			     strcpy(Last_Command,"AT+CSQ");
                       strcpy(CSQ_Status,"Busy_COM");
			    AuroGetTime_start(); 
			     LMDstat=AuroGetTime();
                       LsTime=1;
			     //printf("\n CSQ Called\n");
	                 LCommandLineCount=1;
                       continue;

 	             }
			/*else if(difftime1(CBCETime,CBCSTime)>=30.0)
                   {
                       write(fd_ttySS1,"AT+CADC?\r\n",strlen("AT+CADC?\r\n"));
			     strcpy(Last_Command,"AT+CADC");
                       strcpy(CBC_Status,"Busy_COM");
			     Lstat1=AuroGetTime(); 
			     LMDstat=AuroGetTime();	
                       LsTime=1;
			     //printf("\n CBC Called\n");
	                 LCommandLineCount=1;
                       continue;

	
 	             }*/

               } // end of if check IDLE
            
              
                     if(strcmp(CALL_Status,"DATARECOK")==0)
		 {
			if(strlen(GDataTOSend)>1)
			 {
			   write(fd_ttySS1,GDataTOSend,strlen(GDataTOSend));
                           //write(fd_ttySS1,"\r\n",strlen("\r\n"));
                           //printf("\n Msg Sent %s",GDataTOSend);

			   GDataTOSend[0]='\0';
                           usleep(2000000);
                            
			   
			 }
                        if(DataCTranStatus==3)
                            strcpy(CALL_Status,"DISCONNECT_DATA");
                 }
               if(Front_End == Rear_End)
               {
                   //System.out.println("Nil"+Rear_End);
                   //Thread.sleep(1000);
                    //usleep(1);
                  // printf("\n Retunr from Pre");
			

                if(LCommandLineCount>0)
			{
                  
                    LMDend=AuroGetTime();
                    if(difftime1(LMDend,LMDstat)>=20.0)// || LCommandLineCount>1500)
                     {//printf("\n Before Timer %d",LsTime);
                            LCommandLineCount=1500;
							printf("\n Update_MODEMCommand 1 Loctemp=%s Last_Command=%s",temp,Last_Command);
				    Update_MODEMCommand(temp,Last_Command,0);
			  	    LCommandLineCount=1;
				    LUMNTimeC++;
				    LMDstat=AuroGetTime();
                     }
                  } 	
                
			GRFMFlag=0;
			      //data[pos+1]='\0';
				//printf("\n Update_MODEMCommand 1 Loctemp=%s Last_Command=%s",data,Last_Command);
                  break;
                   continue;
               }
		   if(Front_End==10000)
               {
                   Front_End=0;
               }
               data[pos]=Modem_data[Front_End];
               printf(":%c,%d,%d;",data[pos],data[pos],pos);
               if(strcmp(MSGSend_Status,"RBusy")==0 && data[pos]=='>')
                       {
                        //printf("Sending Message");
                        //os.write((SMessage_ID[SMIDFend]+"\r\n").getBytes());
                        
                        //RMIDFend++;
                         
                        sprintf(LvarStoTerm,SMessage_ID[SMIDFend],"\r\n");
                        write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                        write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
				ERROR_Count=0;

                        
                        //os.write((int)t);
                        strcpy(MSGSend_Status,"Busy");
				LsTime=1;
                       AuroGetTime_start();

                       }
                  if(strcmp(CALL_Status,"DATARECOK")==0 && data[pos]==';')
                       {
                            data[pos+1]='\0';
                            strcpy(GRecDCallData,data);
                            //strcpy(PhoneNumber,"\"+919740217699\"");
			  //  cutMsg1(GRecDCallData,'c');

			    	
			}
               if(strcmp(CALL_Status,"DATARECOK")==0 && data[pos]=='}')
                  {
                        //printf("%c",data[pos]);
			
			strcpy(CALL_Status,"DISCONNECT_DATA");
		   }
                 
                 
               if(data[pos]=='\n' && pos>=2)
               {
                 
                   //strcpy(temp,"");
                   memset(temp,'\0',1000);
                      
                   if(pos==3 && data[0]=='\r' && data[1]=='\n' && data[2]=='\r' && data[3]=='\n')
                    {
                         pos=1;
                    }
                   else if(data[pos-1]=='\r' && data[pos-2]=='\n' && data[pos-3]=='\r')
                   {
                       data[pos+1]='\0';
                       strcpy(temp,data); //     temp=String.copyValueOf(data,0,pos+1);

                       //System.out.println("DAta From Modem="+temp+";pos="+Front_End +";Lapos="+Rear_End);
                      printf("\nDAta From Modem1 DAta=%s temp=%s pos=%d Lapos=%d",data,temp,Front_End,Rear_End );
                       pos=-1;
                   }
                   else if(data[pos-1]==13)   //'\r') && data[pos-2]=='\n' && data[pos-3]=='\r')
                   {
                     // //printf("In Slahs \n\n");
                       data[pos+1]='\0';
                       strcpy(temp,data);//temp=String.copyValueOf(data,0,pos+1);
                       //System.out.println("DAta From Modem="+temp+(int)temp.charAt(0)+";pos="+Front_End +";Lapos="+Rear_End);
			           printf("\nData From Modem2 Data=%s temp=%s pos=%d Lapos=%d",data,temp,Front_End,Rear_End);
						//text_out(temp,0,0);
					    //text_out_to_row(0,temp); 
                       pos=-1;
                   }
                     

                 //  if(strcmp(CALL_Status,"RINGING")==0)
                      // Ring();
			
                  LMDstat=AuroGetTime();
			   	printf("\n Update_MODEMCommand 2 Loctemp=%s Last_Command=%s",temp,Last_Command);
			Update_MODEMCommand(temp,Last_Command,1);
                  
			if(strcmp(REC_Status,"OFHC")==0 && strstr(temp,"OK") != NULL)
			 {
				strcpy(REC_Status,"OFHE");
				LsTime=0;
				LError_Count=0;

                   }
			else if(strcmp(REC_Status,"OFHC")==0 && strstr(temp,"ERROR") != NULL)
			 {
				strcpy(REC_Status,"OFHC_COM");
           		//	write(fd_ttySS1,"AT+CLDTMF\r\n",strlen("AT+CLDTMF\r\n"));
				LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 usleep(1);
					   LError_Count=0;
	 				   
					}


                   }
                  else if(strcmp(REC_Status,"OFHM")==0 && strstr(temp,"OK") != NULL)
			 {
				strcpy(REC_Status,"OFH");
				LsTime=0;
				LError_Count=0;

                   }
			else if(strcmp(REC_Status,"OFHM")==0 && strstr(temp,"ERROR") != NULL)
			 {
				strcpy(REC_Status,"OFHE");
				LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 usleep(1);
					   LError_Count=0;
	 				   
					}

                   }

			/*else if(strcmp(REC_Status,"ONHM")==0 && strstr(temp,"OK") != NULL)
			 {
				strcpy(REC_Status,"ONH");

                   }
                  else if(strcmp(REC_Status,"ONHM")==0 && strstr(temp,"ERROR") != NULL)
			 {
				strcpy(REC_Status,"ONHE");

                   }*/

                   	else if( (strcmp(CALL_Status,"DATADIALED")==0 || strcmp(CALL_Status,"ACC_DATACALL")==0) && strstr(temp,"CONNECT 9600") != NULL)
			 {
				strcpy(CALL_Status,"DATARECOK");
				LsTime=1;
			AuroGetTime_start();
				LError_Count=0;

                   	 
                   }
                   else if( (strcmp(CALL_Status,"DATADIALED")==0 || strcmp(CALL_Status,"ACC_DATACALL")==0 || strcmp(CALL_Status,"DATARECOK")==0 || strcmp(CALL_Status,"DISCONNECTING_DATA")==0) &&  (strstr(temp,"ERROR") != NULL || strstr(temp,"NO CARRIER") != NULL || strstr(temp,"NO DIALTONE") != NULL || strstr(temp,"BUSY") != NULL) || strstr(temp,"NO ANSWER") )
			 {
                                //if(strstr(GRecDCallData,"AURO END;")!= NULL)
                                if(strstr(temp,"ERROR")!=NULL)
				 {
			            ptrtemp=strstr(temp,"ERROR");
				    SlnSlrPos=strlen(ptrtemp);
				    if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || (SlnSlrPos==7 && ptrtemp[5]=='\r' && ptrtemp[6]=='\n'))
                        
				strcpy(CALL_Status,"IDLE");
				
				//else 
				//strcpy(CALL_Status,"CALL_DATA");
				LsTime=0;
				LError_Count=0;
				Dial_No[0]='\0';
				}
				else if(strstr(temp,"NO CARRIER")!=NULL)
				 {
			            ptrtemp=strstr(temp,"NO CARRIER");
				    SlnSlrPos=strlen(ptrtemp);
				    if( ( temp[0]=='\r' && temp[1]=='\n' && temp[12]=='\r'&& temp[13]=='\n') || (SlnSlrPos==12 && ptrtemp[10]=='\r' && ptrtemp[11]=='\n'))
                        
				strcpy(CALL_Status,"IDLE");
				
				//else 
				//strcpy(CALL_Status,"CALL_DATA");
				LsTime=0;
				LError_Count=0;
				Dial_No[0]='\0';
				}
				else if(strstr(temp,"NO DIALTONE")!=NULL)
				 {
			            ptrtemp=strstr(temp,"NO DIALTONE");
				    SlnSlrPos=strlen(ptrtemp);
				    if( ( temp[0]=='\r' && temp[1]=='\n' && temp[13]=='\r'&& temp[14]=='\n') || (SlnSlrPos==13 && ptrtemp[11]=='\r' && ptrtemp[12]=='\n'))
                        
				strcpy(CALL_Status,"IDLE");
				
				//else 
				//strcpy(CALL_Status,"CALL_DATA");
				LsTime=0;
				LError_Count=0;
				Dial_No[0]='\0';
				}
				else if(strstr(temp,"BUSY")!=NULL)
				 {
			            ptrtemp=strstr(temp,"BUSY");
				    SlnSlrPos=strlen(ptrtemp);
				    if( ( temp[0]=='\r' && temp[1]=='\n' && temp[6]=='\r'&& temp[7]=='\n') || (SlnSlrPos==6 && ptrtemp[4]=='\r' && ptrtemp[5]=='\n'))
                        
				strcpy(CALL_Status,"IDLE");
				
				//else 
				//strcpy(CALL_Status,"CALL_DATA");
				LsTime=0;
				LError_Count=0;
				Dial_No[0]='\0';
				}
			     else if(strstr(temp,"NO ANSWER")!=NULL)
				 {
			            ptrtemp=strstr(temp,"NO ANSWER");
				    SlnSlrPos=strlen(ptrtemp);
				    if( ( temp[0]=='\r' && temp[1]=='\n' && temp[11]=='\r'&& temp[12]=='\n') || (SlnSlrPos==11 && ptrtemp[9]=='\r' && ptrtemp[10]=='\n'))
                        
				strcpy(CALL_Status,"IDLE");
				
				//else 
				//strcpy(CALL_Status,"CALL_DATA");
				LsTime=0;
				LError_Count=0;
				Dial_No[0]='\0';
				}

                   	 
                   }
                   else if(strcmp(CALL_Status,"DISCONNECTING_DATA")==0 && strstr(temp,"OK"))
			{
				if(strstr(temp,"OK")!=NULL)
				 {
			            ptrtemp=strstr(temp,"OK");
				    SlnSlrPos=strlen(ptrtemp);
				    if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || (SlnSlrPos==4 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))
                        
				strcpy(CALL_Status,"IDLE");
				
				//else 
				//strcpy(CALL_Status,"CALL_DATA");
				LsTime=0;
				LError_Count=0;
				Dial_No[0]='\0';
				}				
				 
			} 

			else if(strcmp(CALL_Status,"IDLE")==0 && strcmp(REC_Status,"ONHC")==0 && strstr(temp,"OK") != NULL)
			 {
				strcpy(REC_Status,"ONHE");
				LsTime=0;
				LError_Count=0;

                   	 
                   }
                 else if(strcmp(CALL_Status,"IDLE")==0 && strcmp(REC_Status,"ONHC")==0 && strstr(temp,"ERROR") != NULL)
			 {
				strcpy(REC_Status,"ONHC_COM");
		     		write(fd_ttySS1,"AT+CHFA=0\r\n",strlen("AT+CHFA=0\r\n"));
			          LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 usleep(1);
					   LError_Count=0;
	 				   
					}
                                  
	
                   }
                  
                 else if(strcmp(CALL_Status,"CONNECTING")==0 && strstr(temp,"OK") != NULL)
			 {
                    	sprintf(LvarStoTerm,"ATA\r\n");
                 		write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 		strcpy(Last_Command,"ATA");
                        LCommandLineCount=0;
                        LsTime=0;
				LError_Count=0;
 				CALLTime=AuroGetTime();
                 		strcpy(CALL_Status,"BUSY_COM");

                   }
			else if(strcmp(CALL_Status,"CONNECTING")==0 && strstr(temp,"ERROR") != NULL)
			 {
                    	 
                 		strcpy(CALL_Status,"CONNECT");
				    LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 usleep(1);
					   LError_Count=0;
	 				   
					}


                   }

                  else if(strcmp(CALL_Status,"DISCONNECTING")==0  && strstr(temp,"OK") != NULL)
			 {
                 		sprintf(LvarStoTerm,"ATH\r\n");
                 		write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 		strcpy(Last_Command,"ATH");
                        LCommandLineCount=0;
				LsTime=0;
				LError_Count=0;
 				// memset(PhoneNumber,'\0',sizeof(PhoneNumber));
			     //memset(DispPhoneNo,'\0',sizeof(DispPhoneNo));
	                 	strcpy(CALL_Status,"IDLE1_COM");

                    }
                  else if(strcmp(CALL_Status,"DISCONNECTING")==0  && strstr(temp,"ERROR") != NULL)
			 {
	                 	strcpy(CALL_Status,"DISCONNECT");
				LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 usleep(1);
					   LError_Count=0;
	 				   
					}

                    }

                  else if(strcmp(CALL_Status,"DISCONNECTINGD")==0  && (strstr(temp,"OK") != NULL  ))
			 {
                 		sprintf(LvarStoTerm,"ATH\r\n"); //6/5/11
				//sprintf(LvarStoTerm,"ATH\r\n"); //6/5/11

                 		write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 		strcpy(Last_Command,"ATH\r\n");
				LCommandLineCount=0;
                        LsTime=0;
				LError_Count=0;

                 		strcpy(CALL_Status,"IDLE2_COM");

                    }
			else if(strcmp(CALL_Status,"DISCONNECTINGD")==0  && (strstr(temp,"ERROR") != NULL  ))
			 {
                 		strcpy(CALL_Status,"DISCONNECTD");
				LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 usleep(1);
					   LError_Count=0;
	 				   
					}


                    }
			else if(strcmp(CALL_Status,"DIALED")==0  && (strstr(temp,"OK") != NULL  ))
			 {
                 		
				LCommandLineCount=0;
                        LsTime=0;
				LError_Count=0;

                 		strcpy(CALL_Status,"DIALEDB");

                    }

			else if(strcmp(CALL_Status,"IDLE2")==0  && (strstr(temp,"OK") != NULL  ))
			 {
                 		
				LCommandLineCount=0;
                        LsTime=0;
				LError_Count=0;

                 		strcpy(CALL_Status,"IDLE");

                    }
			else if(strcmp(CALL_Status,"IDLE2")==0  && (strstr(temp,"ERROR") != NULL  ))
			 {
                 		 
				sprintf(LvarStoTerm,"ATH\r\n"); //6/5/11
				//sprintf(LvarStoTerm,"ATH\r\n"); //6/5/11

                 		write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
                 		strcpy(Last_Command,"ATH\r\n");
				LCommandLineCount=0;
                        LsTime=0;
				LError_Count=0;

                 		strcpy(CALL_Status,"IDLE2_COM");

				LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 usleep(1);
					   LError_Count=0;
	 				   
					}


                    }


                  else if(strcmp(CALL_Status,"IDLE")==0 && strcmp(REC_Status,"ONH")==0 && strstr(temp,"+CRING") != NULL)
			 {
                        //strcpy(CALL_Status,"RINGING");
                        //RingCallStatus=1;
                        //Ring(); 
		       	sprintf(LvarStoTerm,"ATH\r\n");
                        write(fd_ttySS1,LvarStoTerm,strlen(LvarStoTerm));
				ERROR_Count=0;

                       //printf("Rinign Assigned");
                   }
                  else if(strcmp(CALL_Status,"IDLE")==0 && strstr(temp,"+CRING") != NULL && strcmp(REC_Status,"OFH")==0)
			 {

                                if(strstr(temp,"+CRING: VOICE")!=NULL)
                      	        ptrtemp=strstr(temp,"+CRING: VOICE");
				else if(strstr(temp,"+CRING: REL ASYNC")!=NULL)
                      	        ptrtemp=strstr(temp,"+CRING: REL ASYNC");
			          
				SlnSlrPos=strlen(ptrtemp)-2;
                                //printf("\n SAtish CRing =%d\n",SlnSlrPos);
                                
                        if(SlnSlrPos<=0)
                        SlnSlrPos=1;
                        if(ptrtemp[SlnSlrPos]=='\r'&& ptrtemp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			      {
                        
				if( (strcmp(MSGRead_Status,"OK")==0 &&  strcmp(MSGDelete_Status,"OK")==0 && strcmp(MSGSend_Status,"OK")==0 && strcmp(CMGL_Status,"OK")==0 && strcmp(CSQ_Status,"OK")==0)
                            || (strcmp(MSGRead_Status,"BUSY_COM")==0 ||  strcmp(MSGDelete_Status,"BUSY_COM")==0 || strcmp(CMGL_Status,"BUSY_COM")==0 || strcmp(CSQ_Status,"BUSY_COM")==0 || strcmp(CBC_Status,"BUSY_COM")==0 ))
				{
                             if(strstr(temp,"VOICE")!=NULL)
                          	 strcpy(CALL_Status,"DISCONNECTD"); // satish 14/10/11strcpy(CALL_Status,"RINGING");
				     else if (strstr(temp,"REL ASYNC")!=NULL)
					 strcpy(CALL_Status,"REC_DATACALL");
				  LsTime=1;
		              AuroGetTime_start();	
                          RingCallStatus=1;
				  Change_Status();  

                        }

                       //Ring(); 
                       
                       	//printf("Rinign Assigned");
				}
                   }
                   /*else if(strcmp(CALL_Status,"DISCONNECTING")==0  && strstr(temp,"ERROR") != NULL)
			 {
 
                 		write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));
				ERROR_Count++;
				if(ERROR_Count>5)
				  {
                         	//printf("DICONNECTING Error Exceeds 5");
                              ERROR_Count=0;
				  }

                    }
 
                  else if(strcmp(CALL_Status,"IDLE")==0 && strcmp(REC_Status,"ONHE")==0 && strstr(temp,"ERROR") != NULL)
                   {
                   	////printf("Hit->>%s",CALL_Status);
           			//ioctl(fd_Receiver,72,0);          
           			//Change_Speaker(3); 
		           	//printf("ON");
           			//strcpy(REC_Status,"ONHE");
           			write(fd_ttySS1,"AT+CHFA=0\r\n",strlen("AT+CHFA=0\r\n"));   
                        ERROR_Count++;
                        if(ERROR_Count>5)
				  {
                         	printf("ONHE Error Exceeds 5");
                              ERROR_Count=0;
				  }
			 }
                   else if(strcmp(CALL_Status,"IDLE")==0 && strcmp(REC_Status,"OFHE")==0 && strstr(temp,"ERROR") != NULL)
     		        {
           			printf("Disconnect Call From Dial");
           			//strcpy(REC_Status,"OFHE");
           			write(fd_ttySS1,"AT+CLDTMF\r\n",strlen("AT+CLDTMF\r\n"));
           			//sleep(1);               
           			//ioctl(fd_Receiver,71,0);
           			//Change_Speaker(1);  
				ERROR_Count++;

				if(ERROR_Count>5)
				  {
                         	printf("OFHE Error Exceeds 5");
                              ERROR_Count=0;
				  }
           
		        }*/
                   

                  else if ( strcmp(CALL_Status,"IDLE")!=0 && (strstr(temp,"NO ANSWER") != NULL  || strstr(temp,"NO CARRIER") != NULL  || strstr(temp,"ERROR") !=NULL))
                   {
                      if (strcmp(CALL_Status,"BUSY")==0)
                       {
				 //ReceivedCalls(PhoneNumber);
				strcpy(CALL_Status,"RECDISCONNECT");
				}
                      else if(strcmp(CALL_Status,"RINGING")==0 )
				{
                        //MissedCalls(PhoneNumber);    
				strcpy(CALL_Status,"IDLE");
				}
                      else if(strcmp(CALL_Status,"DIALED")==0 || strcmp(CALL_Status,"DIALEDB")==0)
				{strcpy(Dial_Status,"IDLE");
				 strcpy(CALL_Status,"IDLE");

                        //DialedCalls(PhoneNumber);
                        }
                       Test_Variable=1;
			     //memset(PhoneNumber,'\0',sizeof(PhoneNumber));
			    // memset(DispPhoneNo,'\0',sizeof(DispPhoneNo));
	
		          
			    //printf("\n\nCall--tat-%s,%s\n\n\n",CALL_Status,REC_Status);
                   } 
                   else if(strcmp(MSGRead_Status,"Busy")==0 ||  strcmp(MSGDelete_Status,"Busy")==0 || 
				   strcmp(MSGSend_Status,"Busy")==0 || strcmp(MSGSend_Status,"RBusy")==0 || 
				   strcmp(CMGL_Status,"Busy")==0 || strcmp(CSQ_Status,"Busy")==0  || 
                           strcmp(CBC_Status,"Busy")==0 || strcmp(CLV_Status,"Busy")==0 || strcmp(MSC_Status,"Busy")==0 )
                   {
             
                       if(strstr(temp,"OK")!=NULL && strcmp(MSGRead_Status,"Busy")==0)
                        {
                                ptrtemp=strstr(temp,"OK");
			        SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                          //satish comment 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n')) //|| (SlnSlrPos==2 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))
                           {
                           
				     RMIDFend++;
                             //System.out.println("MSG Content="+MSGReadData);
                             //ReadSms.parseCMGR(MSGReadData);
                             //  num[13]=chkno(MSGReadData);
                              // StoreMSGToFile(MSGReadData);
                               strcpy(MSGRead_Status,"OK");                     
                               cutMsg1(MSGReadData,'m');
                                //RechPrint(MSGReadData);
                                LsTime=0;
			       	  LError_Count=0;
					  LMSGReadTC=0;

                             //System.out.println("Ready to read next Message");
                               //printf("Ready to read next Message");
                             if(DMIDRend==50)
                                {
                                    if(DMIDFend==0)
                                    {
                                       // System.out.println("Que Full");
                                       // continue;
                                    }
                                    else
                                    {
                                        DMIDRend=0;
                                    }
                                }
                                if((DMIDRend+1) == DMIDFend)
                                {
                                   // System.out.println("Que Full");
                                   // continue;
                                }
                                //System.out.print("c1="+c);

                                DMessage_ID[DMIDRend]=RMessage_ID[RMIDFend-1];
                                DMIDRend++;

                           } //end of /r/n if
                       } // End of OK Check if
                       else if(strstr(temp,"OK")!=NULL && strcmp(MSGDelete_Status,"Busy")==0)
                       {
                           
                           //if((temp[0]==10 && temp[1]=='\n')|| (temp[2]==10 && temp[3]=='\n'))
				 //  ptrtemp=strstr(temp,"OK");
				   SlnSlrPos=strlen(temp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                           // comment Satish 10/10/11 if(temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			 if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || (SlnSlrPos==2 && temp[2]=='\r' && temp[3]=='\n'))
                           {

                             DMIDFend++;
                             strcpy(MSGDelete_Status,"OK");
				     LsTime=0;
			           LError_Count=0;
				     LMSGDeleteTC=0;
 	

                           }
                       }
                       else if(strstr(temp,"OK")!=NULL && strcmp(MSGSend_Status,"Busy")==0)
                       {
                           //if(temp[0]==10 && temp[1]=='\n')
   				   ptrtemp=strstr(temp,"OK");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                           //Comments 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			if( (temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || ( SlnSlrPos==2 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))

                           {
                             SMIDFend++;
                             strcpy(MSGSend_Status,"OK");
				     LsTime=0;
			           LError_Count=0;
				     LMSGSendTC=0;
				     //printf("\n Messge OK %d\n",SMIDFend);

                           }
                       }
			else if(strstr(temp,"OK")!=NULL && strcmp(CMGL_Status,"Busy")==0)
                       {
                           //if(temp[0]==10 && temp[1]=='\n')
   				   ptrtemp=strstr(temp,"OK");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                          // Comment Satish 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			  if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || ( SlnSlrPos==2 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))

                           {
                              
                             strcpy(CMGL_Status,"OK");
				     LsTime=0;
			           LError_Count=0;
				     CMGLSTime=AuroGetTime();
 

                           }
                       }
			     else if(strstr(temp,"OK")!=NULL && strcmp(CSQ_Status,"Busy")==0)
                       {
                           //if(temp[0]==10 && temp[1]=='\n')
   				   ptrtemp=strstr(temp,"OK");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                          // Comment Satish 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
                          if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || ( SlnSlrPos==2 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))
                           {
                              
                             strcpy(CSQ_Status,"OK");
				     LsTime=0;
			           LError_Count=0;
				     CSQSTime=AuroGetTime();
 

                           }
                       }
			    else if(strstr(temp,"OK")!=NULL && strcmp(CBC_Status,"Busy")==0)
                       {
                           //if(temp[0]==10 && temp[1]=='\n')
   				   ptrtemp=strstr(temp,"OK");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                          //Comment SAtis 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
                            if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || ( SlnSlrPos==2 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))
                           {
                              
                             strcpy(CBC_Status,"OK");
				     LsTime=0;
			           LError_Count=0;
				     CBCSTime=AuroGetTime();
 

                           }
                       }
			    else if(strstr(temp,"OK")!=NULL && strcmp(CLV_Status,"Busy")==0)
                       {
                           //if(temp[0]==10 && temp[1]=='\n')
   				   ptrtemp=strstr(temp,"OK");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                           // Comment 10/10/11if( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			  if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || (SlnSlrPos==2 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))

                           {
                              
                             strcpy(CLV_Status,"OK");
				     LsTime=0;
			           LError_Count=0;
				      
 

                           }
                       }
	 		    else if(strstr(temp,"OK")!=NULL && strcmp(MSC_Status,"Busy")==0)
                       {
                           //if(temp[0]==10 && temp[1]=='\n')
   				   ptrtemp=strstr(temp,"OK");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                           // Comment satis 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			if( ( temp[0]=='\r' && temp[1]=='\n' && temp[4]=='\r'&& temp[5]=='\n') || ( SlnSlrPos==2 && ptrtemp[2]=='\r' && ptrtemp[3]=='\n'))

                           {
                              
                             strcpy(MSC_Status,"OK");
				     LsTime=0;
			           LError_Count=0;
				      
 

                           }
                       }

	
                       else if(strstr(temp,"ERROR")!=NULL && strcmp(MSGDelete_Status,"Busy")==0)
                       {
                           //if((temp[0]==10 && temp[1]=='\n')|| (temp[5]==10 && temp[6]=='\n'))
   				   ptrtemp=strstr(temp,"ERROR");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                           // Comment 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			   if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && ptrtemp[6]=='\n'))
                           {
                            strcpy(MSGDelete_Status,"OK");
                            //System.out.println("Unable to Delete");
				    LsTime=0;
                            LError_Count++;
				    if(LError_Count>4)
		           		{
                                 DMIDFend++;  
					   LError_Count=0;
	 				   //printf("\nEror occured while delete=%d",DMIDFend);
                              }
	
                           }
				                           
                       }
                       else if(strstr(temp,"ERROR")!=NULL && strcmp(MSGRead_Status,"Busy")==0)
                       {
                           //if((temp[0]==10 && temp[1]=='\n')|| (temp[5]==10 && temp[6]=='\n'))
				   ptrtemp=strstr(temp,"ERROR");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                           // Comment 10/10/11if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
                           if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n'))// || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && temp[6]=='\n'))

                           {
                            strcpy(MSGRead_Status,"OK");
                           // System.out.println("Unable to Read");
				    LsTime=0;
                            LError_Count++;
		   		    if(LError_Count>4)
		                {
                               RMIDFend++;  
					   if(DMIDRend==50)
                                {
                                    if(DMIDFend==0)
                                    {
                                       // System.out.println("Que Full");
                                       // continue;
                                    }
                                    else
                                    {
                                        DMIDRend=0;
                                    }
                                }
                                if((DMIDRend+1) == DMIDFend)
                                {
                                   // System.out.println("Que Full");
                                   // continue;
                                }
                                //System.out.print("c1="+c);

                                DMessage_ID[DMIDRend]=RMessage_ID[RMIDFend-1];
                                DMIDRend++;


			     		 LError_Count=0;
					 //printf("\nEror occured while Read=%d",RMIDFend);
 
                            }
	
                           }

                       }
                       else if(strstr(temp,"ERROR")!=NULL && (strcmp(MSGSend_Status,"Busy")==0 || strcmp(MSGSend_Status,"RBusy")==0))
                       {
                           //if(temp.charAt(0)=='\r' && temp.charAt(1)=='\n')
				   ptrtemp=strstr(temp,"ERROR");
				   SlnSlrPos=strlen(ptrtemp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                           // Commane 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
                            if( (strstr(temp,"+CMS ERROR")!=NULL) || ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && temp[6]=='\n'))


                           {
                            strcpy(MSGSend_Status,"OK");
                            //System.out.println("Resend ing Messgae");
                            //printf("Resening Message %d,%s\n\n",SlnSlrPos,temp);
				    LsTime=0;
                            LError_Count++;
                            if(LError_Count>4)
		                {
                              SMIDFend++;  
			            LError_Count=0;
			            //printf("\nEror occured while Send=%d",DMIDFend);
                            }

                           }

                       }
			     else if(strstr(temp,"ERROR")!=NULL && strcmp(CMGL_Status,"Busy")==0) 
				{
			   	   ptrtemp=strstr(temp,"ERROR");
				   SlnSlrPos=strlen(ptrtemp)-2;
                        	   if(SlnSlrPos<=0)
	                           SlnSlrPos=1;
                          //Comment 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
			 if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && temp[6]=='\n'))


                           {
                            strcpy(CMGL_Status,"OK");
                            //System.out.println("Resend ing Messgae");
                            //printf("Resening Message");
				    LsTime=0;
                            LError_Count++;
                            if(LError_Count>4)
		                {
                               
			            LError_Count=0;
			            CMGLSTime=AuroGetTime();
 
                            }


                           }

                         }
			     else if(strstr(temp,"ERROR")!=NULL && strcmp(CSQ_Status,"Busy")==0) 
				{
					ptrtemp=strstr(temp,"ERROR");
					SlnSlrPos=strlen(ptrtemp)-2;
                           		if(SlnSlrPos<=0)
                           		SlnSlrPos=1;
                         // Commant 10/10/11  if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
				 if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && temp[6]=='\n'))
                           {
                            strcpy(CSQ_Status,"OK");
                            //System.out.println("Resend ing Messgae");
                            //printf("Resening Message");
				    LsTime=0;
                            

                           }

                         }
				else if(strstr(temp,"ERROR")!=NULL && strcmp(CBC_Status,"Busy")==0) 
				{
					ptrtemp=strstr(temp,"ERROR");
					SlnSlrPos=strlen(ptrtemp)-2;
		                         if(SlnSlrPos<=0)
                 		          SlnSlrPos=1;
                           // Comment 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
				 if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && temp[6]=='\n'))
                           {
                            strcpy(CBC_Status,"OK");
                            //System.out.println("Resend ing Messgae");
                            //printf("Resening Message");
				    LsTime=0;
                            

                           }

                         }
				else if(strstr(temp,"ERROR")!=NULL && strcmp(CLV_Status,"Busy")==0) 
				{
					ptrtemp=strstr(temp,"ERROR");
					SlnSlrPos=strlen(temp)-2;
                           if(SlnSlrPos<=0)
                           SlnSlrPos=1;
                          // Comment 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
		 if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && temp[6]=='\n'))
                           {
                            strcpy(CLV_Status,"OK");
                            //System.out.println("Resend ing Messgae");
                            //printf("Resening Message");
				    LsTime=0;
                            

                           }

                         }
                        else if(strstr(temp,"ERROR")!=NULL && strcmp(MSC_Status,"Busy")==0) 
				{
					ptrtemp=strstr(temp,"ERROR");
					SlnSlrPos=strlen(temp)-2;
                           		if(SlnSlrPos<=0)
                           		SlnSlrPos=1;
                           // Comment 10/10/11 if( temp[0]=='\r' && temp[1]=='\n' && temp[SlnSlrPos]=='\r'&& temp[SlnSlrPos+1]=='\n') //|| (temp[2]==10 && temp[3]=='\n'))
		 		if( ( temp[0]=='\r' && temp[1]=='\n' && temp[7]=='\r'&& temp[8]=='\n') || ( SlnSlrPos==5 && ptrtemp[5]=='\r' && temp[6]=='\n'))	
                           {
                            strcpy(MSC_Status,"OK");
                            //System.out.println("Resend ing Messgae");
                            //printf("Resening Message");
				    LsTime=0;
                            

                           }

                       
                         }


                        else if(strstr(temp,"+CSQ:") !=NULL && strcmp(CSQ_Status,"Busy")==0)
				 {
 					// Sajid Call csq function
				          strcpy(Sig_status,temp);
						  signalstrenth(Sig_status);
						   myprintf("CSQ DATA =%s\n",Sig_status);
                 }
				else if(strstr(temp,"+CADC:") !=NULL && strcmp(CBC_Status,"Busy")==0)
				 {
 					// Sajid Call CBC function
				          //sigstrength(temp);
                                       strcpy(Bat_status,temp);
                                      // sigstrength(Sig_status,Bat_status);
                                       //printf("CSQ DATA =%s,l=%d\n\n\n\n",Sig_status,strlen(Sig_status)); 
                         }

                        if(strcmp(MSGRead_Status,"Busy")==0)
                        {
                           strcat(MSGReadData,temp);
                        }
				
                    
                   }//End of Busy Check
                   if(strstr(temp,"+CLIP")!=NULL)
                   {
                         
                        //strcpy(PhoneNumber,temp);
                        //strcpy(Dial_No,PhoneNumber);
					CALLTime=AuroGetTime();
	
                   }
			 

                   if(strstr(temp,"+CMTI")!=NULL)
                   {
                       //System.out.println("CMTI="+temp);
                         //printf("CMTI=%s\n",temp);
                       
                       strptr=strstr(temp,",");  //temp=temp.substring(temp.indexOf(",")+1); // "SM",15
                       strcpy(temp,strptr+1);
                       
                       for(Li=0;Li<=strlen(temp);Li++)
                       {
                           Lc=temp[Li];
                           if(Lc==10 || Lc=='\n')
                               break;
                       }
                       strncpy(temp,temp,Li);// temp=temp.substring(0,Li).trim();
                       
                       if(RMIDRend==50)
                        {
                            if(RMIDFend==0)
                            {
                                //System.out.println("Que Full");
                                //continue;
                            }
                            else
                            {
                                RMIDRend=0;
                            }
                        }
                        if((RMIDRend+1) == RMIDFend)
                        {
                          //  System.out.println("Que Full");
                            //continue;
                        }
                        //System.out.print("c2="+c);

                        RMessage_ID[RMIDRend]= atoi(temp);// Integer.parseInt(temp);
                        RMIDRend++;
			printf("\n Messages read %d",RMIDRend);
                       

                   } //End of CMTI if

                   else if(strstr(temp,"+CMGL")!=NULL)
                   {
                       
                       //temp=temp.substring(temp.indexOf(":")+1); // "SM",15
                       strptr=strstr(temp,":");
                      
                       if(strptr!=NULL)
				{     
                          strptr++;	
                       strcpy(temp,strptr);
	                   //printf("CMGL=%s\n\n",temp);
				strptr=strstr(temp,",");	
                       
				
                       for(Li=0;Li<=strlen(temp);Li++)
                       {
                           Lc=temp[Li];
                           if(Lc==',')
                               break;
				   LcmglIndex[Li]=Lc;	
                       }
			     LcmglIndex[Li]='\0';	
	
                        
                       //printf("\n CMGL aft n copuy=%s,%d",LcmglIndex,Li);
                       //temp=temp.substring(0,temp.indexOf(",")).trim();
                       
                             
                       		 
                       		if(RMIDRend==50)
                        	{
                            		if(RMIDFend==0)
                            		{
                              		//  System.out.println("Que Full");
                                	//continue;
                        	    	}
                            		else
                            		{
                             	   	RMIDRend=0;
                            		}
                        	}
                        	if((RMIDRend+1) == RMIDFend)
                        	{
                           		// System.out.println("Que Full");
                            		//continue;
                        	}
                        	

                        	RMessage_ID[RMIDRend]=atoi(LcmglIndex);
					//printf("\nCMGL Index=%d,%d\n",RMessage_ID[RMIDRend],RMIDRend);
                         	RMIDRend++;
                         }

                   } //End of CMGL if
               }
               if(strcmp(CALL_Status,"DATARECOK")==0 && data[pos]==';') 
                 pos=-1;
               pos++;
               Front_End++;
               
               //printf("In Proce %c,%d",data[pos-1],pos);
          }
         //}//End of Before While try
         /*catch(Exception e)
         {
             //System.out.println("Data="+temp);
             System.out.println("Exce in nRead From SMS"+e);
             System.out.println("Front Pos"+Front_End+";Rear="+Rear_End);
         }*/
             //printf("\n Main Thread Terminatedsat\n\n\n");
          
       
    }



//}


    

int Update_MODEMCommand(char Loctemp[50],char Loc_Command[50],int status)
{
   printf("\nIn UP: LocComm=%s",Loc_Command);
   printf("\nIn UP: Loctemp=%s",Loctemp);
   printf("\nIn UP: Gdat=%d ;Loc%d, C=%s,R=%s,D=%s,S=%s,Re=%s,L=%s,V=%s,Ba=%s,Sig=%s,MSC=%s\n\n\n",DataCTranStatus,LCommandLineCount,CALL_Status,MSGRead_Status,MSGDelete_Status,MSGSend_Status,REC_Status,CMGL_Status,CLV_Status,CBC_Status,CSQ_Status,MSC_Status);

   
   if(strcmp(CALL_Status,"DIALED_COM")==0)  
     {
            if(strstr(Loctemp,">")!=NULL)
              {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     //strcpy(MSGSend_Status,"Busy");
		     strcpy(CALL_Status,"IDLE");
   	           strcpy(Dial_Status,"DIAL");	
	

              } 	 
		else if(strstr(Loctemp,Loc_Command)!=NULL)
		  {
                strcpy(CALL_Status,"DIALED");
		    strcpy(Dial_Status,"DIALED");	
		    LUMNTimeC=0;
		    LsTime=1;
                	AuroGetTime_start();	

	
		  }
		 
             else if(LCommandLineCount>=1500) 
		  {
             	strcpy(CALL_Status,"IDLE");
		      strcpy(Dial_Status,"DIAL");	

		  }
     }
   
   else if(strcmp(CALL_Status,"CONNECTING_COM")==0)  
     {
            
		if(strstr(Loctemp,">")!=NULL)
              {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     strcpy(CALL_Status,"CONNECT");
	
		  }
		else if(strstr(Loctemp,"AT+CHFA=0")!=NULL)
		  {	
             	strcpy(CALL_Status,"CONNECTING");
			LsTime=1;
                	AuroGetTime_start();	
			LUMNTimeC=0;

		  } 
             else if(LCommandLineCount>=1500) 
		  {	
                 strcpy(CALL_Status,"CONNECT");
		     LsTime=0;
              }
     }
   else if(strcmp(CALL_Status,"DISCONNECTING_COM")==0)  
     {
		 if(strstr(Loctemp,">")!=NULL)
              {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1000);
		     strcpy(CALL_Status,"DISCONNECT");
	
		  }	
             else if(strstr(Loctemp,"ATH")!=NULL)
		 {
		      strcpy(CALL_Status,"DISCONNECTING");
			LsTime=1;
                	AuroGetTime_start();	
			LUMNTimeC=0;

		 }
		 else if(LCommandLineCount>=1500)
             strcpy(CALL_Status,"DISCONNECT");
     }
   else if(strcmp(CALL_Status,"DISCONNECTINGD_COM")==0)  
     {
		 if(strstr(Loctemp,">")!=NULL)
              {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     strcpy(CALL_Status,"DISCONNECTD");
	
              }
             else if(strstr(Loctemp,"ATH")!=NULL)
		  {
             	strcpy(CALL_Status,"DISCONNECTINGD");
			LsTime=1;
                AuroGetTime_start();	
			LUMNTimeC=0;

		 }
             else if(LCommandLineCount>=1500)
             strcpy(CALL_Status,"DISCONNECTD");
     }
   else if (strcmp(MSGRead_Status,"Busy_COM")==0)
     {
	     myprintf("In READE->",strstr(Loctemp,Loc_Command));
		 if(strstr(Loctemp,Loc_Command)==NULL)
		 myprintf ("kongggggggg");
             if(strstr(Loctemp,">")!=NULL)
              {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     strcpy(MSGRead_Status,"OK");
   		  }	
		 else if(strstr(Loctemp,"AT+CMGR")!=NULL)
		  {
             	strcpy(MSGRead_Status,"Busy");
			LUMNTimeC=0;

		  }
             else if(LCommandLineCount>=1500)
             strcpy(MSGRead_Status,"OK");
     }
   else if (strcmp(MSGDelete_Status,"Busy_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
              {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     strcpy(MSGDelete_Status,"OK");
	
		  }
             else if(strstr(Loctemp,"AT+CMGD")!=NULL)
		  {
             	strcpy(MSGDelete_Status,"Busy");
			LUMNTimeC=0;

		  }
             else if(LCommandLineCount>=1500)
             strcpy(MSGDelete_Status,"OK");
     }
   else if (strcmp(MSGSend_Status,"RBusy_COM")==0 )
     {
             if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     strcpy(MSGSend_Status,"OK");

		 }
		 else if(strstr(Loctemp,"AT+CMGS")!=NULL)
		 {
             	strcpy(MSGSend_Status,"RBusy");
			LUMNTimeC=0;
		 }
             else if(LCommandLineCount>=1500)
             strcpy(MSGSend_Status,"OK");
     }
   else if (strcmp(REC_Status,"ONH_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     strcpy(REC_Status,"ONHE");


		 }
	
             else if(strstr(Loctemp,Loc_Command)!=NULL)
		 {
             	strcpy(REC_Status,"ONH");
			LUMNTimeC=0;
			LsTime=1;
                	AuroGetTime_start();	

		 }
             else if(LCommandLineCount>=1500)
             strcpy(REC_Status,"ONHE");
     }
   else if (strcmp(REC_Status,"ONHC_COM")==0)
     {
             if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     write(fd_ttySS1,"AT+CHFA=0\r\n",strlen("AT+CHFA=0\r\n"));   
		 }

		 else if(strstr(Loctemp,"AT+CHFA=0")!=NULL)
		 {
            	strcpy(REC_Status,"ONHC");
			LsTime=1;
              	AuroGetTime_start();	
			LUMNTimeC=0;

		 }
             else if(LCommandLineCount>=1500)
		  {
               //strcpy(REC_Status,"ONHE");
                     usleep(1000000);
		     write(fd_ttySS1,"AT+CHFA=0\r\n",strlen("AT+CHFA=0\r\n"));   
	             usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
              }

     }
   else if (strcmp(REC_Status,"OFHC_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     write(fd_ttySS1,"AT+CLDTMF\r\n",strlen("AT+CLDTMF\r\n"));   

		 }
             else if(strstr(Loctemp,"AT+CLDTMF")!=NULL)
		 {
                strcpy(REC_Status,"OFHC");
		    LsTime=1;
                AuroGetTime_start();	
	  	    LUMNTimeC=0;

		 }
             else if(LCommandLineCount>=1500)
		  {
                     usleep(1000000);
		     write(fd_ttySS1,"AT+CLDTMF\r\n",strlen("AT+CLDTMF\r\n"));   
	             usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
              }

     }

   else if (strcmp(REC_Status,"OFHM_COM")==0)
     {
    	      if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     strcpy(REC_Status,"OFHE");
		 }
		else if(strstr(Loctemp,Loc_Command)!=NULL)
		 {
		    strcpy(REC_Status,"OFHM");	
		    LsTime=1;
                AuroGetTime_start();	
		    LUMNTimeC=0;
		 }	
             
             else if(LCommandLineCount>=1500)
             strcpy(REC_Status,"OFHE");
     }
   else if (strcmp(CALL_Status,"BUSY_COM")==0)
     {
             if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     //strcpy(CALL_Status,"CONNECTING"); 6/5/11
		     strcpy(CALL_Status,"BUSY_COM"); //check for receiver Status prev CONNECTING
                  
                 write(fd_ttySS1,"ATA\r\n",strlen("ATA\r\n"));
                  

		 }
		 else if(strstr(Loctemp,Loc_Command)!=NULL)
		 {
             	strcpy(CALL_Status,"BUSY");
			LUMNTimeC=0;

		 }
             else if(LCommandLineCount>=1500)
               {
                 strcpy(CALL_Status,"BUSY_COM"); //check for receiver Status prev CONNECTING
                  usleep(1000000);
                 write(fd_ttySS1,"ATA\r\n",strlen("ATA\r\n"));
                   usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
               }          
     }
   else if (strcmp(CALL_Status,"IDLE1_COM")==0)
     {
             if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     // strcpy(CALL_Status,"DISCONNECTING"); 6/5 11
		     strcpy(CALL_Status,"IDLE1_COM"); //check for receiver Status prev CONNECTING
                  
                 write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));
                  

		 }
		 else if(strstr(Loctemp,Loc_Command)!=NULL)
		 {
             	strcpy(CALL_Status,"IDLE");
			LUMNTimeC=0;

		 }
             else if(LCommandLineCount>=1500)
		 {
                 //strcpy(CALL_Status,"DISCONNECTING");  6/5/11
		     strcpy(CALL_Status,"IDLE1_COM"); //check for receiver Status prev CONNECTING
                    usleep(1000000);
                 write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));
                  usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
		 }
     }
   else if (strcmp(CALL_Status,"IDLE2_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		     //strcpy(CALL_Status,"DISCONNECTINGD"); 6/5/11
		     strcpy(CALL_Status,"IDLE2_COM"); //check for receiver Status prev CONNECTING
                  
                 write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));
                  
	
		 }
             if(strstr(Loctemp,"ATH")!=NULL)
		 {
              	strcpy(CALL_Status,"IDLE2");
			LUMNTimeC=0;
			LsTime=1;
                	AuroGetTime_start();	


		 }
             else if(LCommandLineCount>=1500)
		 {
               //strcpy(CALL_Status,"DISCONNECTINGD"); 6/5/11
		     strcpy(CALL_Status,"IDLE2_COM"); //check for receiver Status prev CONNECTING
                     usleep(1000000);
                 write(fd_ttySS1,"ATH\r\n",strlen("ATH\r\n"));
                 usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
                  
		 }

		
     }
     else if (strcmp(CMGL_Status,"Busy_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		      
		     strcpy(CMGL_Status,"Busy_COM"); //check for receiver Status prev CONNECTING
                  
                 write(fd_ttySS1,"AT+CMGL=\"ALL\"\r\n",strlen("AT+CMGL=\"ALL\"\r\n"));
                  
	
		 }
             if(strstr(Loctemp,"AT+CMGL=\"ALL\"")!=NULL)
		 {
                 strcpy(CMGL_Status,"Busy");
			LUMNTimeC=0;
			LsTime=1;
                	AuroGetTime_start();	

		 }
             else if(LCommandLineCount>=1500)
		 {
                
		     strcpy(CMGL_Status,"OK"); //strcpy(CMGL_Status,"Busy_COM"); 09/06/11 to avoid multiple read
                  
                 //write(fd_ttySS1,"AT+CMGL=\"ALL\"\r\n",strlen("AT+CMGL=\"ALL\"\r\n"));

                  
		 }

		
     }
     else if (strcmp(CSQ_Status,"Busy_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		      
		     strcpy(CSQ_Status,"Busy_COM");  
                  
                 write(fd_ttySS1,"AT+CSQ\r\n",strlen("AT+CSQ\r\n"));
                  
	
		 }
             if(strstr(Loctemp,"AT+CSQ")!=NULL)
		 {
                 strcpy(CSQ_Status,"Busy");
			LUMNTimeC=0;
			LsTime=1;
                AuroGetTime_start();	


		 }
             else if(LCommandLineCount>=1500)
		 {
                
		     strcpy(CSQ_Status,"Busy_COM");  
                    usleep(1000000);
                 write(fd_ttySS1,"AT+CSQ\r\n",strlen("AT+CSQ\r\n"));
                  usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
                  
		 }

		
     }
   else if (strcmp(CBC_Status,"Busy_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		      
		     strcpy(CBC_Status,"Busy_COM");  
                  
                 write(fd_ttySS1,"AT+CADC?\r\n",strlen("AT+CADC?\r\n"));
                  
	
		 }
             if(strstr(Loctemp,"AT+CADC")!=NULL)
		 {
                 strcpy(CBC_Status,"Busy");
			LUMNTimeC=0;
			LsTime=1;
                AuroGetTime_start();	


		 }
             else if(LCommandLineCount>=1500)
		 {
                
		     strcpy(CBC_Status,"Busy_COM");  
                     usleep(1000000);
                 write(fd_ttySS1,"AT+CADC?\r\n",strlen("AT+CADC?\r\n"));
                      usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
                  
		 }

		
     }
else if (strcmp(CLV_Status,"Busy_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		      
		     strcpy(CLV_Status,"Busy_COM");  
		     sprintf(Loc_Command,"AT+CLVL=%d\r\n",GSpeakerVolume);
	     	     write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
		
		 }
             if(strstr(Loctemp,"AT+CLV")!=NULL)
		 {
                 strcpy(CLV_Status,"Busy");
			LUMNTimeC=0;
			LsTime=1;
                	AuroGetTime_start();	


		 }
             else if(LCommandLineCount>=1500)
		 {
                
		     strcpy(CLV_Status,"Busy_COM");  
		     sprintf(Loc_Command,"AT+CLVL=%d\r\n",GSpeakerVolume);
                     usleep(1000000);
	     	     write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
                     usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
		 }

		
     }
 else if (strcmp(MSC_Status,"Busy_COM")==0)
     {
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		      
		     strcpy(MSC_Status,"Busy_COM");  
		     sprintf(Loc_Command,"AT+CSCA=\"%s\"\r\n",GMessageCNo);
	     	     write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
		
		 }
             if(strstr(Loctemp,"AT+CSCA")!=NULL)
		 {
                 strcpy(MSC_Status,"Busy");
			LUMNTimeC=0;
			LsTime=1;
                	AuroGetTime_start();	


		 }
             else if(LCommandLineCount>=1500)
		 {
                
		     strcpy(MSC_Status,"Busy_COM");  
		     sprintf(Loc_Command,"AT+CSCA=\"%s\"\r\n",GMessageCNo);
                     usleep(1000000);
	     	     write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
                     usleep(1000000);
                 write(fd_ttySS1,"ATE1\r\n",strlen("ATE1\r\n"));
		 }

		
     }

 else if (strcmp(CALL_Status,"DATADIALED_COM")==0)
     {
             //printf("Sat data call %s",CALL_Status);
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		      
		     strcpy(CALL_Status,"DATADIALED_COM");  
		     //sprintf(Loc_Command,"ATD9740217699\r\n",GMessageCNo);
	     	     write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
		
		 }
             if(strstr(Loctemp,"ATD")!=NULL)
		 {
                 strcpy(CALL_Status,"DATADIALED");
			LUMNTimeC=0;
			LsTime=1;
                	AuroGetTime_start();	


		 }
             else if(LCommandLineCount>=1500)
		 {
                
		     strcpy(CALL_Status,"CALL_DATA");  
		     //sprintf(Loc_Command,"ATD9740217699\r\n",GMessageCNo);
	     	     //write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
                  
		 }

		
     }

//REC DATA CALL Statrt

 else if (strcmp(CALL_Status,"ACC_DATACALLCOM")==0)
     {
             //printf("Sat data call %s",CALL_Status);
		 if(strstr(Loctemp,">")!=NULL)
             {
                 write(fd_ttySS1,"\x1A\r\n",strlen("\x1A\r\n"));
                 usleep(1);
		      
		     strcpy(CALL_Status,"ACC_DATACALLCOM");  
		     sprintf(Loc_Command,"ATA\r\n",GMessageCNo);
	     	     write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
		
		 }
             if(strstr(Loctemp,"ATA")!=NULL)
		 {
                        strcpy(CALL_Status,"ACC_DATACALL");
			LUMNTimeC=0;
			LsTime=1;
                	AuroGetTime_start();	


		 }
             else if(LCommandLineCount>=1500)
		 {
                
		     strcpy(CALL_Status,"ACC_DATACALLCOM");  
		     //sprintf(Loc_Command,"ATD9740217699\r\n",GMessageCNo);
	     	     //write(fd_ttySS1,Loc_Command,strlen(Loc_Command));	
                  
		 }

		
     }


//REC DATA CALL End
     
    if(LUMNTimeC>=3)
     {
	 //close(fd_ttySS1);
         LUMNTimeC=0;
           //fd_ttySS1=open("/dev/ttyS1",O_RDWR);
           //printf("\n Modem File Reopened");
     }
    //break;



    LCommandLineCount++;   
    if(LCommandLineCount>=1800)
    LCommandLineCount=1;

	return 1;
  }             

int Change_Status()
{

	strcpy(MSGSend_Status,"OK");
	strcpy(MSGRead_Status,"OK");
	strcpy(MSGDelete_Status,"OK");
	strcpy(CMGL_Status,"OK");
	strcpy(CSQ_Status,"OK");
	strcpy(CBC_Status,"OK");
	strcpy(CALL_Status,"IDLE");
	strcpy(REC_Status,"OFH");
	strcpy(CLV_Status,"OK");
	strcpy(MSC_Status,"OK");
	return 1;
   
}

/***********************************
Author:Jyotiba Vaijinath kadam
Date:2011-01-11 15:28
Remarks:identify which message got from the server 
LastModified:Date:2012-02-03 14:14
Remarks:
************************************/
#include<stdlib.h>
#include "StoreMSG.h"
#include "GlobalVar.h"
//#include "saveorder.c"
//#include "senditeminfo.c"
void cutMsg1(char Cmsg[1000],char tpe)
{
  char com[35], mobno[18], space[7], date[12], time[15], header[15],mobno1[18],
    msg[1000], MsgToSend[1000];
  int chkmanno = 0, i = 0, j = 0, k = 0, size = 0, z=0,x=0;
  myprintf ("\ncmsg=%s\nlength=%d", Cmsg, strlen (Cmsg));
   if(tpe=='m')
  {
  while ((Cmsg[i] != ',' && Cmsg[i] != '\0'))
   {
      //com[j]=Cmsg[i];
	j = j + 1;
      i = i + 1;
      size = size + 1;
   }
  
    //com[j]='\0';
    //myprintf("com=%s\n",com);
    j = 0;
  i = i + 1;
  size = 0;
  while ((Cmsg[i] != ',' && Cmsg[i] != '\0') && size < (sizeof (mobno) - 1))
    {
      mobno[j] = Cmsg[i];
      j = j + 1;
      i = i + 1;
      size = size + 1;
    }
  mobno[j] = '\0';
  myprintf ("mobno=%s\n", mobno);
  j = 0;
  i = i + 1;
  size = 0;
  while ((Cmsg[i] != ',' && Cmsg[i] != '\0') && size < (sizeof (space) - 1))
    
    {
       space[j] = Cmsg[i];
        j = j + 1;
       i = i + 1;
      size = size + 1;
    }
  space[j] = '\0';
  myprintf ("space=%s\n", space);
  j = 0;
  i = i + 1;
  size = 0;
  while ((Cmsg[i] != ',' && Cmsg[i] != '\0') && size < (sizeof (date) - 1))
    
    {
      date[j] = Cmsg[i];
      j = j + 1;
      i = i + 1;
      size = size + 1;
    
}
  
 date[j] = '\0';
 myprintf ("date=%s\n", date);
 j = 0;
 i = i + 1;
 size = 0;
  while ((Cmsg[i] != '\"' && Cmsg[i] != '\0') && size < (sizeof (time) - 1))
    {
      time[j] = Cmsg[i];
      j = j + 1;
      i = i + 1;
      size = size + 1;
    }
  time[j] = '\0';
  myprintf ("\nparse msg called \n");
  myprintf ("time=%s\n", time);

   j = 0;
  i = i + 1;
  size = 0;
 }
 
 
while ((Cmsg[i] != ';' && Cmsg[i] != '\0') && size < (sizeof (msg) - 1))
    {
      
	//myprintf("Cmsg[i]=%s ",Cmsg[i]);
	msg[j] = Cmsg[i];
        j = j + 1;
        i = i + 1;
        size = size + 1;
    }
  msg[j] = Cmsg[i];
  j = j + 1;
  msg[j] = '\0';
  myprintf ("msg=%s\n", msg);
  myprintf ("msglen=%d\n", strlen (msg));
  myprintf ("\nmsg[0]=%c", msg[0]);
  
while (msg[0] == '\n' || msg[0] == '\r')
    {
     k = 0;
     while (msg[k] != '\0')
	{
	  msg[k] = msg[k + 1];
	  k = k + 1;
	}
    }
strcpy (MsgToSend, msg); 
  
j = 0;
i = 0;
size = 0;
  myprintf ("\nImportant modem hag\n");
  while ((msg[i] != ' ' && msg[i] != '\0') && size < (sizeof (header) - 1))
    {
      header[j] = msg[i];
      j = j + 1;
      i = i + 1;
      size = size + 1;
    }
  header[4] = '\0';
	j = 0;
	i = 0;
	size = 0;
    
	if(mobno[0]=='\"')
	{
		while(mobno[i]!='\0'&& i<sizeof(mobno))
		{
			mobno[i]=mobno[i+1];
			i++;	
		}
		if(mobno[strlen(mobno)-1]=='\"')
		mobno[strlen(mobno)-1]='\0';
	}
	casesensitive(header);
 if(strcmp(header,"INFO")==0||strcmp(header,"info")==0)
	{	z=0;x=0;
		while(mobno[z]!='\0' && z<sizeof(mobno))
    	{
	     if(mobno[z]!='\"')
	     {
		    mobno1[x]=mobno[z];
		    x=x+1;
	     }
    	z=z+1;
    	}
       mobno1[x]='\0';
       mobno1[15]='\0';
//	   senditeminfo(mobno1);
	}else
	{
		 StoreMSGToFile (mobno, MsgToSend);	//MsgstoreData);
	}
  
  myprintf ("headr=%s\n",header);
  myprintf ("mobno=%s\n", mobno);
  myprintf ("space=%s\n", space);
  myprintf ("date=%s\n", date);
  myprintf ("time=%s\n", time);
  //myprintf ("\nmobile no for inbox=%s\n\n\n", mobno);
  //myprintf ("\nmsg no for inbox=%s", MsgToSend);
  //myprintf ("\nA msg from Invalid number\n");
 }				//main end

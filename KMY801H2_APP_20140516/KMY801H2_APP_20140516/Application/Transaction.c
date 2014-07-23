#include "Transaction.h"
#include "lcd.h"
#include "gprs.h"
#include <string.h>
#include "setting.h"
#include <stdio.h>
#include "key.h"
#include "kmy_wifi.h"
#include "GlobalVar.h"


static char tcp_connect_send_recieve(unsigned char *SendBuff,unsigned int Sendlen,unsigned char *recBuff,unsigned int recBuffSize)
{
	char retVal;

	clear_lcd();
	DrawTitle_to_row_Language2(1,"GPRS","GPRS");
	DrawTitle_to_row_Language2(2,"正在通讯……","Communicating");

	clear_area_to_row(3,3);
	DrawTitle_to_row_Language2(3,"GPRS网络连接中…","Connecting");
//	retVal=tcp_Connect("31.170.167.97,80"); 	//IP,Port
    retVal=tcp_Connect("192.168.1.74,50000"); 	//IP,Port
//	retVal=tcp_Connect("weslen.yupage.com");	//Domain
	if(retVal!=TCP_OK){goto failreturn;}

	clear_area_to_row(3,3);
	DrawTitle_to_row_Language2(3,"发送数据中…","Sending");
	retVal=tcp_send(SendBuff,Sendlen);
	if(retVal!=TCP_SendOK){goto failreturn;}

	clear_area_to_row(3,3);
	DrawTitle_to_row_Language2(3,"接受数据中…","Receiving");
	retVal=tcp_receive(recBuff,recBuffSize,"HTTP/1.1 200 OK",30);
	if(retVal!=TCP_ReceiveOK){goto failreturn;}		
	tcp_DisConnect();
	return TCP_CONNECT_AND_RIGHT_RECEIVE;

	failreturn:
	tcp_DisConnect();
	return retVal;
}

static char WiFi_TCP_Connect(unsigned char *SendBuff,unsigned int Sendlen,unsigned char *recBuff,unsigned int recBuffSize)
{
	char retVal;
	
	clear_lcd();	
	DrawTitle_to_row_Language2(1,"WiFi","WiFi");
	DrawTitle_to_row_Language2(2,"正在通讯……","Communicating");

	clear_area_to_row(3,3);
	DrawTitle_to_row_Language2(3,"WiFi网络连接中…","Connecting");
	//retVal=WiFi_Connect("31.170.167.97,80");    //    IP,Port
		retVal=WiFi_Connect("192.168.1.74,50000");    //    IP,Port
//	retVal=WiFi_Connect("weslen.yupage.com,80");//Domain
	if(retVal!=TCP_OK){goto failreturn;}

	clear_area_to_row(3,3);
	DrawTitle_to_row_Language2(3,"发送数据中…","Sending");
	retVal=WiFi_Send(SendBuff,Sendlen);
	if(retVal!=TCP_SendOK){goto failreturn;}

	clear_area_to_row(3,3);
	DrawTitle_to_row_Language2(3,"接受数据中…","Receiving");
	retVal=WiFi_Receive(recBuff,recBuffSize,"HTTP/1.1 200 OK",30000);
	if(retVal!=TCP_ReceiveOK){goto failreturn;}		

	return TCP_CONNECT_AND_RIGHT_RECEIVE;

	failreturn:
	return retVal;
}

static void DisplayTcpConnectSendRecieveError(unsigned char retval)
{
	clear_lcd();
	if(retval==TCP_ConnectFailure){
		DrawTitle_to_row_Language2(2,"连接服务器失败!","Connect error");
	}else if(retval==TCP_SendFailure){
		DrawTitle_to_row_Language2(2,"发送失败!","Send error");
	}else if(retval==TCP_UserSelfReturn){
		DrawTitle_to_row_Language2(2,"用户返回!","User return");
	}else if(retval==TCP_ReceiveBuffNotEnough){
		DrawTitle_to_row_Language2(2,"接收缓冲区不够","Buff Not enough");
	}else if(retval==TCP_ReceiveTimeOut){
		DrawTitle_to_row_Language2(2,"接收超时","Receive time out");
	}else{
		DrawTitle_to_row_Language2(2,"未知错误!","Undefined error");
	}
}
//char Download_File()
char Download_File(char *msg)
{
    unsigned char keyval;
	unsigned int pos,temp;	
	int count=-1;   
	char Temp_Data[50];
	int i=0,j=0,k=0;
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
		printf("\n COUNT==%d",count);
		//---------------- copy from File_Data[] to sector---------------
		for(i=0;File_Data[i]!='\0';i++)
	    {
			if(File_Data[i]=='\n')
	         {

	    		Temp_Data[j]='\0';
	  			printf("Temp_Data=%s\n",Temp_Data);
				strcpy(custdet.custStatusdet,Temp_Data);
				j=0;
				k=0;
				
				
		  
		       
				printf("\n count in write =%d",count);
		  			if(count==0||count==-1)
						{
							custdet.slno=1;
							count=1;
						   	printf("\n custdet.slno in if=%d",custdet.slno);
						}
					else
						{
						//	custdet.slno=count+1;
							custdet.slno=count;
				   			printf("\n custdet.slno in else=%d",custdet.slno);
						}

				if(strlen(custdet.custIDdet)>0 && strlen(custdet.custNamedet)>0 && strlen(custdet.custMobdet)>0 && strlen(custdet.custStatusdet)>0)
				{
				 	if(custdet.slno>255)
					 {
					  printf("\n custdet.slno in >255=%d",custdet.slno);
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
	}
	  
	else if(File_Data[i]=='#')
	  {
	   Temp_Data[j]='\0';
	   	printf("\n COUNT in #==%d",count);
	  	printf("Temp_Data=%s\n",Temp_Data);
		if(	k==0)
		strcpy(custdet.custIDdet,Temp_Data);
		if(	k==1)
		strcpy(custdet.custNamedet,Temp_Data);
		if(	k==2)
		strcpy(custdet.custMobdet,Temp_Data);
		 k++;
		j=0;
	  }
	  
	  else if(File_Data[i]!='\n' && File_Data[i]!='#')
	  {	
	  printf("\n in Temp_Data[j]=File_Data[i];");  
	  Temp_Data[j]=File_Data[i];
	  j++;
	  }
	  printf("\n COUNT before while==%d",count);
	}

	

}

char Transaction(char *msg)
{
	unsigned char SendBuff[1000]={"GET http://weslen.yupage.com/OrderRequest.php HTTP/1.1\r\nHost: weslen.yupage.com\r\nPragma: no-cache\r\nAccept: */*\r\nProxy-Connection: Keep-Alive\r\n\r\n"};
	unsigned char retval;
	unsigned char recBuff[3048];
	unsigned int SendLen;
	
	SendLen=strlen((char*)SendBuff);

	if(NetworkSwitch[0]==WiFi)
	{
		retval=WiFi_TCP_Connect(SendBuff,SendLen,recBuff,sizeof(recBuff));
	}
	else
	{
    	retval=tcp_connect_send_recieve(SendBuff,SendLen,recBuff,sizeof(recBuff));
	}
	printf("retval=<%2x>\r\n",retval);

	if(retval!=TCP_CONNECT_AND_RIGHT_RECEIVE){	
		DisplayTcpConnectSendRecieveError(retval);
		WaitkeyAndAutoExit(10);
	    return retval;
	}
	printf("\r\n-------------------\r\nrecBuff:\r\n%s\r\n",recBuff);
	printf("\r\n-------------------\r\n");
	clear_lcd();
	DrawTitle_to_row_Language2(2,"接收成功","Receive ok");
	WaitkeyAndAutoExit(10);
	return retval;
}




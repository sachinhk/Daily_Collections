
//#include <rtl.h>
//#include <RTL.h>

/*U8 tcp_soc;

U16 tcp_callback (U8 soc, U8 event, U8 *ptr, U16 par) {
  /* This function is called on TCP event 
  // ..
  return (0);
}

void main (void) {
  U8 rem_ip[4] = {192,168,1,110};

//  init ();
  /* Initialize the TcpNet 
  init_TcpNet ();
  tcp_soc = tcp_get_socket (TCP_TYPE_CLIENT, 0, 30, tcp_callback);
  if (tcp_soc != 0) {
    /* Start Connection 
    tcp_connect (tcp_soc, rem_ip, 80, 1000);
  }

 // while (1);
    /* Run main TcpNet 'thread' 
   // main_TcpNet ();
     //..
  }
   */

/*__task void client (void) {
  /* LED Control client task. 
  SOCKADDR_IN addr;
  int sock, res;
  char dbuf[4];
  U8 p2val,lshf;

  while (1) {
    sock = socket (AF_INET, SOCK_STREAM, 0);

    addr.sin_port      = htons(1001);
    addr.sin_family    = PF_INET;
    addr.sin_addr.s_b1 = 192;
    addr.sin_addr.s_b2 = 168;
    addr.sin_addr.s_b3 = 0;
    addr.sin_addr.s_b4 = 100;

    connect (sock, (SOCKADDR *)&addr, sizeof (addr));
}
}	  */



#include <rtl.h>
#include <rl_net.h>
#include <net_config.h>

//__task void server (void *argv) {
  /* Server task runs in 2 instances. 	 */
 int main(void)
  {
   net_initialize ();
  SOCKADDR_IN addr;
  int sock, sd, res;
  int type = (int)argv;
  char dbuf[4];

  while (1) {
    sock = socket (AF_INET, type, 0);

    addr.sin_port        = htons(1001);
    addr.sin_family      = PF_INET;
    //addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_addr.s_b1 = 192;
    addr.sin_addr.s_b2 = 168;
    addr.sin_addr.s_b3 = 1;
    addr.sin_addr.s_b4 = 74;

    bind (sock, (SOCKADDR *)&addr, sizeof(addr));

    if (type == SOCK_STREAM) {
      listen (sock, 1);
      sd = accept (sock, NULL, NULL);
      closesocket (sock);
      sock = sd;
    }

    while (1) {
      res = recv (sock, dbuf, sizeof (dbuf), 0);
      if (res <= 0) {
        break;
      }
      procrec ((U8 *)dbuf);
    }
    closesocket (sock);
  }
  return (0);	
}






//---------------------form socket----------------------
/*
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <sys/wait.h>
//#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
//#include <arpa/inet.h>
//#include <netdb.h>

#define PORT 50000
#define LENGTH 512


void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(void)
{
	/* Variable Definition 
	int sockfd; 
	int nsockfd;
	char revbuf[LENGTH]; 
	struct sockaddr_in remote_addr;
	struct hostent *host;
	 char ipaddress[16];
	 
	 //-----------------From IMT---------------------------
	// memset (ipaddress, '\0', sizeof (ipaddress));
	//      strcpy (ipaddress, "192.168.1.74");
	//      if (strlen (ipaddress) > 0)
		
	//	  host = gethostbyname (ipaddress);
		 
		  //-----------------From IMT----------------------//

	/* Get the Socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      {
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
		exit(1);
	}

	/* Fill the socket address struct 
	//fcntl (sockfd, F_SETFL | O_NONBLOCK);
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(PORT); 
	//remote_addr.sin_port = 50000; 
	//remote_addr.sin_addr = *((struct in_addr *) host->h_addr);  //From IMT
	//remote_addr.sin_addr =inet_addr("192.168.1.74");;
	
	inet_pton(AF_INET,"192.168.1.74", &remote_addr.sin_addr); 
	bzero(&(remote_addr.sin_zero), 8);
  //printf("remote addr=%ld  \t remote port =%d",remote_addr.sin_addr,remote_addr.sin_port);
 // printf("remote port =%d",remote_addr.sin_port);
//if (connect(sock, (struct sockaddr *) &server_addr,sizeof (struct sockaddr)) == -1)
		  //  {
		      //perror ("Connect");
		    // myprintf ("\n Connection Opennindfdsg Problem\n\n");
		     // popup (" CONNECT ERROR");
	/* Try to connect the remote 
	if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr) == -1))
	{
		fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
		exit(1);
	}
	else 
		printf("[Client] Connected to server at port %d...ok!\n", PORT);

	/* ------------------- Send File to Server ----------------*/
	//if(!fork())
	//{
/*		char* fs_name = "/home/aryan/Desktop/quotidiani.txt";
		char sdbuf[LENGTH]; 
		printf("[Client] Sending %s to the Server... ", fs_name);
		FILE *fs = fopen(fs_name, "r");
		if(fs == NULL)
		{
			printf("ERROR: File %s not found.\n", fs_name);
			exit(1);
		}

		bzero(sdbuf, LENGTH); 
		int fs_block_sz; 
		while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
		{
		    if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
		    {
		        fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
		        break;
		    }
		    bzero(sdbuf, LENGTH);
		}
		printf("Ok File %s from Client was Sent!\n", fs_name); 
		------------------- Send File to Server ----------------*/
	//}


	/* Receive File from Server 
	printf("[Client] Receiveing file from Server and saving it as final.txt...");
	char* fr_name = "/home/shobha/final.txt";
	FILE *fr = fopen(fr_name, "w");
	if(fr == NULL)
		printf("File %s Cannot be opened.\n", fr_name);
	else
	{
		bzero(revbuf, LENGTH); 
		int fr_block_sz = 0;
	    while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0)
	    {
			int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
	        if(write_sz < fr_block_sz)
			{
	            error("File write failed.\n");
	        }
			bzero(revbuf, LENGTH);
			if (fr_block_sz == 0 || fr_block_sz != 512) 
			{
				break;
			}
		}
		if(fr_block_sz < 0)
        {
			if (errno == EAGAIN)
			{
				printf("recv() timed out.\n");
			}
			else
			{
				fprintf(stderr, "recv() failed due to errno = %d\n", errno);
			}
		}
	    printf("Ok received from server!\n");
	    fclose(fr);
	}
	close (sockfd);
	printf("[Client] Connection lost.\n");
	return (0);		
}  */
//--------------------------from socket--------------------------
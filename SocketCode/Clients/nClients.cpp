#include <netinet/in.h>      
#include <sys/types.h>      
#include <sys/socket.h>      
#include <arpa/inet.h>  
#include <stdio.h>          
#include <stdlib.h>         
#include <string.h>  
#include <unistd.h>    
#include <pthread.h>   
#include <map> 
#include <string> 
#include <sstream>
#include <iostream> 
#define random(x) (rand()%x)
using namespace std;

#define SERVER_IP "192.168.169.129"
//#define SERVER_PORT 4000
#define LENGTH_OF_LISTEN_QUEUE 20  
#define BUFFER_SIZE 1024 

typedef struct REC_DATA
{
	int client_port;
}REC_DATA;

void recvServer()
{

}

void sendToServer(void *data)
{	while(1){

int i,j,k;
for(i = 0;i<10000;i++)
	for(j = 0;j<10000;j++)
		for(k=0;k <10;k++)
			;
	char sendline[4096];  
cin>>sendline;
//	fgets(sendline, 4096, stdin);  
//	sendline[strlen(sendline)-1] ='\0';
printf("%d\n",strlen(sendline));
	printf("in pretead\n");
	REC_DATA *dt=(REC_DATA*)data;
	int opt = 1; 
char buf[10];
sprintf(buf, "%d", dt->client_port);
string b = buf;
	string sendString = " port:"+b+":";
	char sendBuffer[255]="p1:1:";
//strcpy(sendBuffer, sendString.c_str());
string woshia  = sendline;
sendString = sendString+woshia;
//strcat(sendline, sendString.c_str());
strcpy(sendline, sendString.c_str());
	struct sockaddr_in c_addr;  
    	bzero(&c_addr,sizeof(c_addr));   
      
    	c_addr.sin_family = AF_INET;  
    	c_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  
    	c_addr.sin_port = htons(4000);  
	int c_socket = socket(PF_INET,SOCK_STREAM,0);  
    	if( c_socket < 0)  
    	{  
    	    printf("Create Socket Failed!");  
       	    exit(1); 
    	}  
   
    	if( connect( c_socket,(struct sockaddr *)&c_addr,sizeof(struct sockaddr) )==-1 )
	{
	    printf(" Failed!\n");  
       	    exit(1); 
	}
	send(c_socket,sendline,strlen(sendline),0);
	close(c_socket);
}
}

int main(int argc, char **argv)  
{  
	struct sockaddr_in server_addr;  
	int server_socket;  
	int opt = 1;  
	int local_port = 5000+random(100);
	/* init socket */
	bzero(&server_addr,sizeof(server_addr));   
	server_addr.sin_family = AF_INET;  
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
	server_addr.sin_port = htons(local_port);  
  
	/* create a socket */  
	server_socket = socket(PF_INET,SOCK_STREAM,0);  
	if( server_socket < 0)  
	{  
		printf("Create Socket Failed!");  
		exit(1);  
	}  
   
	/* bind socket to a specified address*/  
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  
	
	while( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))  
	{  
		printf("Server Bind Port Failed!\n"); 
		local_port += 1; 
		server_addr.sin_port = htons(local_port);  
	}
	printf("port is: %d\n", local_port);
/*
	if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))  
	{  
		printf("Server Bind Port :  Failed!");   
		exit(1);  
	} */ 

sockaddr_in sin;
memcpy (&sin, &server_addr, sizeof(sin));
char ips[200];
sprintf(ips, inet_ntoa(sin.sin_addr));
printf("ip is  %s\n",ips);
int pps;
pps = sin.sin_port;
printf("port is  %d\n",htons(pps));

	/* listen a socket */  
	if(listen(server_socket, LENGTH_OF_LISTEN_QUEUE))  
	{  
		printf("Server Listen Failed!\n");   
		exit(1);  
	}  

	/* !!!create a thread!!! */
	REC_DATA ra;
	ra.client_port = local_port;
	pthread_t pt; 
	int pret=pthread_create(&pt,NULL,( void* (*)(void*) )sendToServer,(void *)&ra);
	if(pret!=0)
	{
		printf ("Create pthread error!\n");
		exit (1);
	}
	
	/* run server */  
	char buffer[BUFFER_SIZE];
	while (1)   
	{  
		struct sockaddr_in client_addr;  
		socklen_t length = sizeof(client_addr);
		printf("wait for recieve results\n");
		
		int client_sockets;

		client_sockets = accept(server_socket, (struct sockaddr*)&client_addr, &length);
		
		if( client_sockets < 0)  
		{  
			printf("Server Accept Failed!\n");  
			continue;  
		}
		printf("Get a connect!\n");
		// get data 
		int ret = recv(client_sockets, buffer, BUFFER_SIZE, 0); 
		if(ret>0)
		{
			buffer[ret] = '\0';
		}
		printf("**************\n");
		printf("recieved data:%s\n\n",buffer);
		printf("**************\n");
	}
}

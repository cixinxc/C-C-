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
#include "add.c"
#include "multiply.c"
  //p1:10:p2:20

using namespace std;

#define CLIENT_IP "192.168.169.129"
#define CLIENT_PORT 30000
#define SERVER_PORT 4000
#define LENGTH_OF_LISTEN_QUEUE 20  
#define BUFFER_SIZE 1024  

typedef struct REC_DATA
{
	char recData[1024];
	int length;
	char client_ip[20];
	int client_socket;
}REC_DATA;

void sendToClient(void *data)
{
	int i,j,k;

	REC_DATA *dt=(REC_DATA*)data;

	map<string, string> params;
	int add_data;	//计算结果
	/****************数据处理******************/
	char *p = NULL;
	p = strtok(dt->recData, ":");
	int b = 0;	
	while(p)
	{
		b++;
		string p_key = p;
		p=strtok(NULL,":");
		if(b/2==0) { params.insert( map<string,string>::value_type(p_key,p) ); }
		else { b=0; }
	}

	std::string p1 = params["p1"];
	std::string p2 = params["p2"];
	std::string c_port = params[" port"];
	std::string program = params["program"];
	stringstream ss;  
	stringstream ss2;  
	stringstream ss_port;  
  
	ss<<p1;  
	int p11;
	ss>>p11;
  
	ss2<<p2;  
	int p22;
	ss2>>p22; 

	ss_port<<c_port;
	int p_port;	
	ss_port>>p_port;

	printf("params is :%d	%d   ", p11,p22); 
	if( program=="add" )
	{
		add_data = add(p11,p22);
	}
	else
	{
		add_data = multiply(p11,p22);
	}
	
	printf("result is:%d\n",add_data);
	printf("client port is:%d\n",p_port);
	/****************数据处理******************/

	stringstream ss3;
	ss3<<add_data;
	string s1 = ss3.str();
	char *sendDatas = ( char * )s1.data();
	char sendBuffer[255]="";
	strcpy(sendBuffer,s1.c_str());
	
	//send(dt->client_socket,sendBuffer,strlen(sendBuffer),0);
	printf(" succeed:%s	%d\n",dt->client_ip, p_port);  
	/***********Send Data To Server***********/

	int opt = 1; 
	struct sockaddr_in c_addr;  
    	bzero(&c_addr,sizeof(c_addr));   
      
    	c_addr.sin_family = AF_INET;  
    	c_addr.sin_addr.s_addr = inet_addr(dt->client_ip);  
    	c_addr.sin_port = htons(p_port);  
	int c_socket = socket(PF_INET,SOCK_STREAM,0);  
    	if( c_socket < 0)  
    	{  
    	    printf("Create Socket Failed!");  
       	    exit(1); 
    	}  
   
    	if( connect( c_socket,(struct sockaddr *)&c_addr,sizeof(struct sockaddr) )==-1 )
	{
	    printf("connect client Failed!\n");  
       	//    exit(1); 
	}
	printf("send end\n");
	printf("send code is: %d",send(c_socket,sendBuffer,strlen(sendBuffer),0));
	
	close(c_socket);

}

int main(int argc, char **argv)  
{  
	struct sockaddr_in server_addr;  
	int server_socket;  
	int opt = 1;  
	/* init socket */
	bzero(&server_addr,sizeof(server_addr));   
	server_addr.sin_family = AF_INET;  
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
	server_addr.sin_port = htons(SERVER_PORT);  
  
	/* create a socket */  
	server_socket = socket(PF_INET,SOCK_STREAM,0);  
	if( server_socket < 0)  
	{  
		printf("Create Socket Failed!");  
		exit(1);  
	}  
   
	/* bind socket to a specified address*/  
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  
	if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))  
	{  
		printf("Server Bind Port : %d Failed!", SERVER_PORT);   
		exit(1);  
	}  

	/* listen a socket */  
	if(listen(server_socket, LENGTH_OF_LISTEN_QUEUE))  
	{  
		printf("Server Listen Failed!");   
		exit(1);  
	}  
	
	/* run server */  
	char buffer[BUFFER_SIZE];
	while (1)   
	{  
		strcpy(buffer, "");
		struct sockaddr_in client_addr;  
		socklen_t length = sizeof(client_addr); 
		printf("wait for connection\n");	
		int client_socket;
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);


	sockaddr_in sin;
        memcpy (&sin, &client_addr, sizeof(sin));
	char ips[200];
	sprintf(ips, inet_ntoa(sin.sin_addr));
        printf("%s\n",ips);
	int pps;
	pps = sin.sin_port;
        printf("%d\n",pps);

		if( client_socket < 0)  
		{  
			printf("Server Accept Failed!\n");  
			continue;  
		}
		printf("Get a connect!\n");
		/* get data */
		int ret = recv(client_socket, buffer, BUFFER_SIZE, 0); 
		if(ret>0)
		{
			buffer[ret] = '\0';
		}
		printf("recieve data length is:%d\n",ret);
		printf("data is:%s\n",buffer);

		REC_DATA ra;
		for(int dl = 0;dl<ret;dl++)
		{
			ra.recData[dl] = buffer[dl];
		}
		ra.length = ret;	
		sprintf(ra.client_ip, inet_ntoa(sin.sin_addr));
		ra.client_socket = sin.sin_port;

		/* create a thread!!! */

		pthread_t pt; 
		int pret=pthread_create(&pt,NULL,( void* (*)(void*) )sendToClient,(void *)&ra);
		if(pret!=0){
			printf ("Create pthread error!\n");
			exit (1);
		}
          
		close(client_socket); 
		printf("*********************\n"); 
	}
	return 0;  
}  

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
  
using namespace std;

//#define SERVER_IP "192.168.0.21"
//#define SERVER_IP "192.168.169.129"
#define SERVER_IP "172.21.15.68"
#define SERVER_PORT 6666
#define HELLO_WORLD_SERVER_PORT 4000  
#define LENGTH_OF_LISTEN_QUEUE 20  
#define BUFFER_SIZE 1024  

typedef struct REC_DATA
{
    char recData[1024];
    int length;
}REC_DATA;

void sendToServer(void *data)
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
	stringstream ss;  
	stringstream ss2;  
	ss<<p1;  
	int p11;
	ss>>p11;  
	ss2<<p2;  
	int p22;
	ss2>>p22; 
//	printf("add = %d\n", add(p11,p22)); 
	add_data = add(p11,p22);
	printf("result is:%d\n",add_data);
	/****************数据处理******************/
	stringstream ss3;
	ss3<<add_data;
	string s1 = ss3.str();
	char *sendDatas = ( char * )s1.data();
	char sendBuffer[255]="";
	strcpy(sendBuffer,s1.c_str());
	/***********Send Data To Server***********/
	int opt = 1; 
	struct sockaddr_in c_addr;  
    	bzero(&c_addr,sizeof(c_addr));   
    	c_addr.sin_family = AF_INET;  
    	c_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  
    	c_addr.sin_port = htons(SERVER_PORT);  

	int c_socket = socket(PF_INET,SOCK_STREAM,0);  
	bind(c_socket,(struct sockaddr*)&c_addr,sizeof(c_addr));
	
//	connect(c_socket,(struct sockaddr*)&c_addr, sizeof(c_addr)) ;
//	send(c_socket,sendBuffer,strlen(sendBuffer),0);
	printf("send success!");
	close(c_socket);
}

int main(int argc, char **argv)  
{  
    struct sockaddr_in server_addr;  
    int server_socket;  
    int opt = 1;  
     
    bzero(&server_addr,sizeof(server_addr));   
      
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);  
  
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
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT);   
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
	struct sockaddr_in client_addr;  
	socklen_t length = sizeof(client_addr); 
	printf("wait for connection\n");	
	int sClient;
    	sClient = accept(server_socket, (struct sockaddr*)&client_addr, &length);
	if( sClient < 0)  
        {  
            printf("Server Accept Failed!\n");  
            continue;  
        }  
	printf("Get a connect!\n");
	//get data
	int ret = recv(sClient, buffer, BUFFER_SIZE, 0); 
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
	//create a thread!!!

     	//sendToServer(buffer);
	pthread_t pt; 
	int pret=pthread_create(&pt,NULL,( void* (*)(void*) )sendToServer,(void *)&ra);
    	if(pret!=0){
        	printf ("Create pthread error!\n");
        	exit (1);
    	}
          
        close(sClient);  
    }
    return 0;  
}  

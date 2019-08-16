// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<pthread.h>
#define PORT 8888
#define MAX_THREADS 10
#include<bits/stdc++.h>
#define fi first
#define se second
using namespace std;
typedef pair<int,int> ii;

int cnt = 0, thread_id[MAX_THREADS+10];

pthread_t client_threads[MAX_THREADS];
pthread_cond_t read_cond  = PTHREAD_COND_INITIALIZER;

ii thread_data[MAX_THREADS];
struct sockaddr_in address; 
int server_fd, new_socket1[MAX_THREADS];
int addrlen = sizeof(address);  

int opt = 1; 
bool working[MAX_THREADS+10];

void inittt()
{   
	for(int i =0 ;i<MAX_THREADS;i++)
	{
		working[i] = false;
		thread_id[i] = i;
	}
	return ;
} 

bool isexit()
{
	bool is_return = true;
	for(int i =0;i<MAX_THREADS;i++) is_return = is_return & (!working[i]);
	return !is_return;
}

int vacant_thread()
{
	int ptr = 0;
	while(working[ptr++]);

	if(ptr == MAX_THREADS+1) return -1;
	else return ptr-1;
}

void* handle_client(void *arg)
{
	ii *new_socket_data = (ii*)arg;
	int new_socket = (*new_socket_data).se;
	int ID = (*new_socket_data).fi;

	while(true)
	{
	int valread;
	char buffer[1024] = {0};
	char hello[1000] = "hello messege sent";
	valread = read(new_socket , buffer, 1024); 
	printf("%s\n",buffer); 
	 
	send(new_socket , hello , strlen(hello) , 0); 	
	printf("%s\n", hello);
	if(buffer[0] == 'E')break; 
	}

	 working[ID] = false;
	 pthread_cond_signal(&read_cond);
	 pthread_exit(0);
} 	

int main(int argc, char const *argv[]) 
{ 	
	do
   {
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}  
	if (setsockopt(server_fd, 
		           SOL_SOCKET,
		           SO_REUSEADDR | SO_REUSEPORT, 
		           &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 

	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, 
		(struct sockaddr *)&address, 
		sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}

	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	int thread_ki_id = vacant_thread();

	while(vacant_thread()==-1)
		pthread_cond_wait(&read_cond,NULL);
	
	int e  =vacant_thread();
	
	working[e] = true;
	
	if ((new_socket1[e] = accept(server_fd, 
		                         (struct sockaddr *)&address, 
					             (socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	thread_data[e] = {thread_id[e],new_socket1[e]};
	pthread_create(&client_threads[e],NULL,handle_client,&thread_data[e]);
	pthread_detach(client_threads[e]);
    
    }while(isexit());
	
	return 0; 
} 

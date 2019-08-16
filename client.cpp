// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include<bits/stdc++.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 

//global_variable
int sock = 0, valread; 
struct sockaddr_in serv_addr; 
char buffer[1024] = {0}; 
//global_variable

using namespace std;
int main() 
{ 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	cout<<"give_us_host"<<endl;
	
	cin.clear();
	cout.clear();
	fflush(stdin);
	fflush(stdout);

	char  s[100];
	cin>>s;
	cout<<"posr_number";
	int posrt_nu;
	cin>>posrt_nu;

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(posrt_nu); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 

	if(inet_pton(AF_INET, s, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
} 
	while(true){

	char message[100];
	cin>>message;
	
	send(sock , message , strlen(message) , 0 ); 
	printf("Hello message sent\n"); 
	
	valread = read( sock , buffer, 1024); 
	printf("%s\n",buffer ); 
	
	if(message[0] == 'E') break;
	}
	
	return 0; 
} 

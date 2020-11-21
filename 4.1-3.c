#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int socket_desc;
	struct sockaddr_in server;
	char *message;
	char server_reply[100];

	//Create socket
	socket_desc=socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc==-1)
	{
		printf("Could not create socket");
	}

	server.sin_addr.s_addr=inet_addr("192.168.56.104");//Please enter the ip address of your serverVM
	server.sin_family=AF_INET;
	server.sin_port=htons( 8887 );

	//Connect to remote server
	if(connect(socket_desc,(struct sockaddr *)&server,sizeof(server))<0)
	{
		puts("connect error");
		return 1;
	}
	
	puts("Connected\n");
	//Send some data
	message = "connect";
	if(send(socket_desc,message,strlen(message),0)<0)
	{
		puts("Send Failed");
		return 1;
	}

	puts("Data Send\n");
	//Receive a reply from a server
	if(recv(socket_desc,server_reply, 2000, 0) <0)
	{
		puts("recv failed");
		return 1;
	}

	puts("Reply received\n");
	puts(server_reply);

	close(socket_desc);	
	return 0;
}

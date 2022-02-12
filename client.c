#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_LINE 4096
#define SRVR_PORT 80

int main(int argc, char** argv)
	{
		int sockfd, n;
		int sendBytes;
		struct sockaddr_in servaddr;
		char sendLine[MAX_LINE];
		char recvLine[MAX_LINE];
		
		sockfd = socket(AF_INET,  SOCK_STREAM, 0);	
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SRVR_PORT);
		inet_pton(AF_INET, inet_ntoa(*(struct in_addr*)gethostbyname(argv[1])->h_addr), 
				&servaddr.sin_addr);
		connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
		sprintf(sendLine, "GET / HTTP/1.1 \r\n\r\n");
		sendBytes = strlen(sendLine);
		
		write(sockfd, sendLine, sendBytes);
		
		memset(recvLine, 0, MAX_LINE);
		while(read(sockfd, recvLine, MAX_LINE-1) > 0)
			{
				printf("%s", recvLine);
				memset(recvLine, 0, MAX_LINE);
			}
		printf("\n");

		return 0;
	}

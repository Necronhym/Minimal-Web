#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_LINE 4096
#define SRVR_PORT 80

int main(int argc, char** argv)
	{
		int listenfd, connfd, n;
		struct sockaddr_in servaddr;
		uint8_t		buff[MAX_LINE+1];
		uint8_t		recvLine[MAX_LINE+1];
		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(SRVR_PORT);
		bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
		listen(listenfd, 10);
		while(1)
			{
				struct sockaddr_in addr;
				socklen_t addr_len;
				fflush(stdout);
				connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
				memset(recvLine, 0, MAX_LINE);
				while( (n = read(connfd, recvLine, MAX_LINE-1)) > 0 )
					{
						fprintf(stdout, "%s\n", recvLine);
						if(recvLine[n-1] == '\n')
							{
								break;
							}
					}
				memset(recvLine, 0, MAX_LINE);
				//Confirm request	
				snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\n");
				write(connfd, (char*)buff, strlen((char*)buff));
				memset(buff, 0, MAX_LINE);
				//read index and write:
				FILE *fptr = fopen("index.html", "r");
				while(fgets(buff, MAX_LINE, fptr) != NULL)
					{
						write(connfd, (char*)buff, strlen((char*)buff));
						memset(buff, 0, MAX_LINE);
					}
				memset(buff, 0, MAX_LINE);
				close(connfd);
			}
		return 0;
	}

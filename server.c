#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sigumt.h>

int main(int argc, char *argv[] ){

	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[5000];
	
	time_t ticks;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET; // should be always set to this
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //ip addr of a host server 
	serv_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while(1){

		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		if(connfd < 0) {
			error("Error in server.");
		}
		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "This is SigUMT server. Commands: send, \n");
		write(connfd, sendBuff, strlen(sendBuff));
		read_from_socket(connfd);

		close(connfd);
		sleep(1);
	}

}

void read_from_socket(int connfd){
	int status;
	char result[1024];
	
	status = read(connfd, result, sizeof(result));
	if(status < 0){
		error("Error in reading.");
		exit(0);
	}
	printf("read: %s", result);
}

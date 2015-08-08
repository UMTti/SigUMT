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
	serv_addr.sin_port = htons(5000);        // Port number

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while(1){

		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		if(connfd < 0) {
			error("Error in server.");
		}
		ticks = time(NULL);  // what is this
		snprintf(sendBuff, sizeof(sendBuff), "This is SigUMT server. Commands: send \n");
		write(connfd, sendBuff, strlen(sendBuff));
		char* ptr = malloc(15);
		read_from_socket(connfd, ptr);

		close(connfd);
		sleep(1);
	}

}

void handle_input(int connfd, char* ptr){
	char sendBuff[5000];
	printf("ptr on %s", ptr);
	if(contains_word(ptr, "send")){
		printf("User wants to send something \n");
		int bytes = get_bytes(ptr, 5);
		snprintf(sendBuff, sizeof(sendBuff), "Please send %d bytes data now.\n", bytes);
		write(connfd, sendBuff, strlen(sendBuff));
	} else {
		printf("Not defined command\n");
	}

}

int get_bytes(char* ptr, int start_index){
	if(start_index<0) return -1;
	int i = start_index;
	char* length = malloc(15);
	int a = 0;
	while(i<strlen(ptr)){
		if(ptr[i] == ' '){
			return atoi(length);
		} else {
			length[a] = ptr[i];
		}
		i++;
		a++;
	}
	return atoi(length);
}

int contains_word(char* base, char* word){
	unsigned int i;
	for(i=0;i<strlen(word);i++){
		if(base[i] != word[i]){
			return 0;
		}
	}
	return 1;
}

void read_from_socket(int connfd, char* ptr){
	int status;
	char* result = malloc(20);
	memset(result, '\0', sizeof(result));	
	status = read(connfd, result, sizeof(result));
	if(status < 0){
		error("Error in reading.");
		exit(0);
	}
	strcpy(ptr, result);
	ptr[14] = '\0';
	handle_input(connfd, ptr);
}

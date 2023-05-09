#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>


int main(int argc, char * argv[]){

	int sockfd, port_number;
	struct sockaddr_in server_address;
	
	int message_size_max = 1024;
	char message[message_size_max];

	if(argc != 3){
		fprintf(stderr, "Please provide requeired number of inputs\n");
		exit(1);
	}

	port_number = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		fprintf(stderr, "Error, a problem occured while opening socket\n");
		exit(1);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(argv[1]);
	server_address.sin_port = htons(port_number);
	
	if(connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address) ) < 0){
		fprintf(stderr, "Error, connection failed\n");
		exit(1);
	}
	
	while(1){
	
		bzero(message, message_size_max);
		printf("Please enter your message: ");
		fgets(message, message_size_max, stdin);

		if(write(sockfd, message, strlen(message)) < 0){
			fprintf(stderr, "Error, a problem occured while writing\n");
			exit(1);
		}
		
		bzero(message, message_size_max);
		
		if(read(sockfd, message, message_size_max) < 0){
			fprintf(stderr, "Error, a problem occured while reading\n");
			exit(1);
		}

		time_t t;
		time(&t);
		
		printf("[%s] Server: %s", strtok(ctime(&t), "\n"), message);

		if(!strncmp("close", message, 5)){
			break;
		}
	}

	close(sockfd);
	return 0;
}

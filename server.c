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

	int sockfd, newsockfd, port_number;
	struct sockaddr_in server_address, client_address;

	int message_size_max = 1024;
	char message[message_size_max];
	
	socklen_t length;

	if(argc != 2){
		fprintf(stderr, "Please provide required number of inputs\n");
		exit(1);	
	}

	port_number = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0){
		fprintf(stderr, "Error, a problem occured while creating socket\n");
		exit(1);
	}
	
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port_number);

	if(bind(sockfd, (struct sockaddr *)  &server_address, sizeof(server_address)) < 0){
		fprintf(stderr, "Error, a problem occured while binding socket\n");
		exit(1);
	}
		
	
	listen(sockfd, 5);
	
	length = sizeof(client_address);
	
	newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &length);

	if(newsockfd < 0){
		fprintf(stderr, "Error, a problem occured while accepting request\n");
		exit(1);	
	}
	
	
	while(1){

		bzero(message, message_size_max);

		if(read(newsockfd, message, message_size_max) < 0){
			fprintf(stderr, "Error, a problem occured while reading\n");		
			exit(1);		
		}
		
		time_t t;
		time(&t);
		
		printf("[%s] Client: %s",strtok(ctime(&t), "\n"), message);
		
		bzero(message, message_size_max);
		printf("Please enter your message: ");
		fgets(message, message_size_max, stdin);

		if(write(newsockfd, message, message_size_max) < 0){
			fprintf(stderr, "Error, a problem occured while writing\n");
			exit(1);
		}
		
		if(strncmp("close", message, 5) == 0){
			break;		
		}
	}


	close(newsockfd);
	close(sockfd);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <ctype.h> 
#include "list.h"

#define PORT "21165"   // port number for ServerA
#define HOST "localhost"

List listA;

void read_file(){

	 
	memset(&listA, 0, sizeof(List));
	Init (&listA);
	
	
	
	char line[128];
	char* delim = "::";

	FILE *fp = NULL;
	fp = fopen("backendA.txt","r");
	if(fp == NULL){
		exit(0);
	}
	while(fgets(line, 128, fp) != NULL) {
		//printf("%s\n", line );
	char* word;
	word = (char*) malloc(27);
	memset(word, '\0', sizeof(word));
	strcpy(word, strtok(line, delim));
	word[strlen(word) - 1] = 0;
	int i = 0;
	while(word[i]) {
		word[i] = tolower(word[i]);
		i++;
	}
	char* define;
	define = (char*) malloc(100);
	memset(define, '\0', sizeof(define));
	strcpy(define, strtok(NULL, delim));
	//printf("%s\n", word );
	//printf("%s\n",define );
		Append(&listA, word, define);
		
		//free(word);
		//free(define);

	}
	// if (Find(&listA, "fother") == NULL) {
	// 	printf("no find\n");
	// } else 
  	//printf("%s\n", Find(&listA, "fother")->define);
    
    fclose(fp);
}

char* search (char input[]) {
	
	if (Find(&listA, input) == NULL) {
		//printf("no find\n");
		return NULL;
	} else {
		char * res = Find(&listA, input)->define ;
		//printf("%s\n", res );
		return res;
	}
}

int main(void){
	read_file();
	
	// set up UDP   From Beej
	int sockfd;
	struct addrinfo hints, *servinfo, *p;  
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int rv;

	memset(&hints, 0, sizeof hints);  // make sure the struct is empty
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_DGRAM; // UDP dgram sockets
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(HOST, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 0;
	}
// loop through all the results and bind to the first we can----Beej
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("serverA: socket");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("serverA: bind");
			continue;
		}
		break;
	}
	
	if (p == NULL) {
		fprintf(stderr, "serverA: failed to bind\n");
		return 0;
	}
	freeaddrinfo(servinfo);
	printf( "The ServerA is up and running using UDP on port %s.\n", PORT);


	while(1) {
		sin_size = sizeof their_addr;
		char input[27];
		memset(input, '\0', 27*sizeof(char));
		int func;
		

		recvfrom(sockfd, input, sizeof input, 0,(struct sockaddr *)&their_addr, &sin_size);
			
		
			//printf("no coming\n");
			
		
		//printf("input is %s\n", input );
		recvfrom(sockfd, (char*) &func, sizeof func, 0,(struct sockaddr *)&their_addr, &sin_size);
		if (func == 1) {
			printf("The ServerA received input <%s> and operation <search> \n", input);
			char* res = search(input);
			if (res == NULL) {
				//printf("no find\n");
				char empty[99];
				memset(empty, '\0', 99 * sizeof(char));
				strcpy(empty, "0");
				 sendto(sockfd, empty, sizeof empty, 0,(struct sockaddr *) &their_addr, sin_size);
				 printf("The ServerA finished sending the output to AWS \n");
		
			} else {
				//printf("%s\n", res );
				char data[99];
				memset(data, '\0', 99 * sizeof(char));
				strcpy(data, res);
				 sendto(sockfd, data, sizeof data, 0,(struct sockaddr *) &their_addr, sin_size);
				 printf("The ServerA finished sending the output to AWS \n");
			}
			

		} else if (func == 2) {
			printf("The ServerA received input <%s> and operation <prefix> \n", input);
			int count = 0;
			char data[999];
			memset(data, '\0', 999*sizeof(char));
			ListElem* e = First(&listA);
			for (int i = 0; i < listA.num_members; i++) {
				if (strncmp(input, e->word, strlen(input)) == 0) {
					count++;
					
					char temp[27];
					memset(temp, '\0', 27*sizeof(char));
					strcpy(temp, e->word);
					strcat(data, temp);
					strcat(data, " ");

				}
				e = Next(&listA, e);
			}
			printf("The serverA has found <%d> matches\n", count);
			if (count == 0) {
				char empty[999];
				memset(empty, '\0', 999 * sizeof(char));
				strcpy(empty, "0");
				 sendto(sockfd, empty, sizeof empty, 0,(struct sockaddr *) &their_addr, sin_size);
			} else {
				sendto(sockfd, data, sizeof data, 0,(struct sockaddr *) &their_addr, sin_size);
			}
			
			//printf(" data is %s\n", data);
			printf("The ServerA finished sending the output to AWS \n");
		}
		
		
		
		
		
		
	}
	
}
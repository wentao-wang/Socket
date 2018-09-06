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
#include <signal.h>
#include <ctype.h> 

#define AWSPORT "25165"   
#define HOST "localhost"



void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}


int main(int argc, char* argv[]){

	
	
	//set up TCP --from Beej
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(HOST, AWSPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can----Beej
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect. \n");
		exit(0);
		return 2;
	}
	freeaddrinfo(servinfo); // all done with this structure
	printf("The client is up and running. \n");

	if (argc < 3) {
		fprintf(stderr,"Error: please input the function name and word\n");
        exit(1);
	}

	char input_t[27];
	memset(input_t, '\0', 27*sizeof(char));
	if (argc == 3) {
		char* input = argv[2];
		strcpy(input_t, input);
		

		
	} else {
		char* input1 = argv[2];
		char* input2 = argv[3];
		char input_t2[27];
		memset(input_t2, '\0', 27*sizeof(char));
		strcpy(input_t2, input1);
		strcat(input_t2, " ");
		strcat(input_t2, input2);
		strcpy(input_t, input_t2);
		//printf("multi input is %s\n", input_t);

	}
	int i = 0;
		while(input_t[i]) {
			input_t[i] = tolower(input_t[i]);
			i++;
		}
	char* func = argv[1];
	char func_t[7];
	memset(func_t,'\0', 7*sizeof(char));
	strcpy(func_t, func);
	//printf("result1 is %s\n", func_t );
	int j = 0;
	while(func_t[j]) {
		func_t[j] = tolower(func_t[j]);
		j++;
	}
	
	//printf("result12 is %s\n", func_t );
	if (strcmp(func_t, "search") != 0 && strcmp(func_t, "prefix") != 0) {
		//printf("error  <%s> and <%s> to AWS.\n", input_t, func_t);
		fprintf(stderr,"Error: only take search or prefix function\n");
        exit(1);
	}
	
	// const char* data[2];
	// data[0] = func_t;
	// data[1] = input_t;
	send(sockfd, func_t, strlen(func_t), 0);
	//send(sockfd, "\0", 2, 0);
	send(sockfd, input_t, strlen(input_t), 0);
	send(sockfd, "\0", 2, 0);

	printf("The client sent <%s> and <%s> to AWS.\n", input_t, func_t);
	if (strcmp(func_t, "search") == 0) {
		char res[99];
		memset(res, '\0', 99*sizeof(char));
		recv(sockfd, res, sizeof res, 0);
		if (strcmp(res, "0") == 0) {
			printf("Found no match for <%s>\n", input_t);
		} else {
			printf("Found a match for <%s>:\n", input_t);
			printf("%s\n", res);
		}
	}
	if (strcmp(func_t, "prefix") == 0) {
		char res[3000];
		memset(res, '\0', 3000*sizeof(char));
		recv(sockfd, res, sizeof res, 0);
		printf("Found matches for <%s>:\n", input_t);
		printf("%s\n", res);
	}
	
	
}
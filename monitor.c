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

#define AWSPORT "26165"   
#define HOST "localhost"



void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}


int main(int argc, char* argv[]){
	printf("The Monitor is up and running. \n");

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

	
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("monitor: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("monitor: connect");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "monitor: failed to connect. \n");
		exit(0);
		return 2;
	}
	freeaddrinfo(servinfo); // all done with this structure
	//printf("The Monitor is up and running. \n");
while(1) {
	char res[99];
	//memset(res, '\0', 99*sizeof(char));
	char word[28];
	memset(word, '\0', 28 * sizeof(char));
	recv(sockfd, res, sizeof res, 0);
	recv(sockfd, word, sizeof word, 0);
	if (strcmp(res, "0") == 0) {
		printf("Found no match for <%s>\n", word);
	} else {
		printf("Found a match for <%s>:\n", word);
		printf("%s\n", res);
		printf("One distance match is <%s>\n", word);

	}

}
	
}
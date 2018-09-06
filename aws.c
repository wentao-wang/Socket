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

#define TCPPORT "25165"   //TCP port
#define TCPPORT2 "26165"   //TCP port2
#define UDPPORT "24165"		//UDP port
#define HOST "localhost"
#define BACKLOG 10 // how many pending connections queue will hold
#define PORTA "21165"
#define PORTB "22165"
#define PORTC "23165"



char data[99];
char data2[999];
// UDP connect to the back-server A B C
char* searnpre(char input[], char server, int f){
	int mysock;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char* backserver_port;

    if(server == 'A') 
    	backserver_port = PORTA;
    
    //printf("go to server A %s\n", input);
    else if (server == 'B')
    	backserver_port = PORTB;
    else if(server == 'C')
    	backserver_port = PORTC;

    //printf("port number: %s \n", backserver_port);

    //beej code -- udp client code
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(HOST, backserver_port, &hints, &servinfo))
			!= 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}
	// loop through all the results and make a socket----beej code
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((mysock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		exit(2);
	}


	//using UDP to send the data
	char input_t[27];
	//memset(input_t, '0', 27*sizeof(char));
	strcpy(input_t, input);
	//printf("input is %s\n",input_t );

	if (sendto(mysock, input_t, strlen(input_t), 0, p->ai_addr,p->ai_addrlen) == -1){
		puts("not sent error");
	}
	
	if (sendto(mysock, (char*) &f, sizeof f, 0, p->ai_addr,p->ai_addrlen) == -1) {
		perror("not sent error");
	}
	if (f == 1) {
		printf("The AWS sent <%s> and <search> to Backend-Server %c. \n", input, server);
	} else {
		printf("The AWS sent <%s> and <prefix> to Backend-Server %c. \n", input, server);
	}
	

	 
	 memset(data,'\0', 99*sizeof(char));

	 recvfrom(mysock, data, sizeof data, 0 , NULL, NULL);
	//printf("the result is %s \n", data);
	return data; 
}

char* searnpre2(char input[], char server, int f){
	int mysock;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char* backserver_port;

    if(server == 'A') 
    	backserver_port = PORTA;
    
    //printf("go to server A %s\n", input);
    else if (server == 'B')
    	backserver_port = PORTB;
    else if(server == 'C')
    	backserver_port = PORTC;

    //printf("port number: %s \n", backserver_port);

    //beej code -- udp client code
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(HOST, backserver_port, &hints, &servinfo))
			!= 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}
	// loop through all the results and make a socket----beej code
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((mysock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		exit(2);
	}


	//using UDP to send the data
	char input_t[27];
	//memset(input_t, '0', 27*sizeof(char));
	strcpy(input_t, input);
	//printf("input is %s\n",input_t );

	if (sendto(mysock, input_t, strlen(input_t), 0, p->ai_addr,p->ai_addrlen) == -1){
		puts("not sent error");
	}
	
	if (sendto(mysock, (char*) &f, sizeof f, 0, p->ai_addr,p->ai_addrlen) == -1) {
		perror("not sent error");
	}
	if (f == 1) {
		printf("The AWS sent <%s> and <search> to Backend-Server %c. \n", input, server);
	} else {
		printf("The AWS sent <%s> and <prefix> to Backend-Server %c. \n", input, server);
	}
	

	 
	 memset(data2,'\0', 999*sizeof(char));

	 recvfrom(mysock, data2, sizeof data2, 0 , NULL, NULL);
	//printf("the result is %s \n", data);
	return data2; 
}


//TCP part
//beej code
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

struct m_info
{
	int fd;
	int pnum;
};
struct m_info monitor(){
	struct m_info m;
	//act as TCP server for users --beej code
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(HOST, TCPPORT2, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
	}
	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(2);
	}
	freeaddrinfo(servinfo); // all done with this structure
	//listen
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	//accept 
	
		sin_size = sizeof their_addr;	
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");			
			exit(1);
		}


		// get the port of monitor
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
		struct sockaddr_in addrClient;
		memset(&addrClient, 0, sizeof(addrClient));
		int len = sizeof(addrClient);
		getpeername(new_fd, (struct sockaddr *) &addrClient, (socklen_t *) &len);
		int monitor_port = addrClient.sin_port;

			m.fd = new_fd;
			m.pnum = monitor_port;
			return m;		

	
}
int sentMonitor(int monitor_port, int new_fd, char* input, char* define ) {
		//printf("sentmi input is %s\n", input);
					char res[99];
					memset(res, '\0', 99 * sizeof(char));
					strcpy(res,define);
					char word[28];
					memset(word, '\0', 28 * sizeof(char));
					strcpy(word, input);

					if(strcmp(res, "0") == 0) {
						printf("The AWS send <0> match to the monitor via TCP port %d\n", monitor_port);

					} else {
						printf("The AWS send <%s> and <%s> to the monitor via TCP port %d for search\n", word,word, monitor_port);
					}
					send(new_fd, res, sizeof res, 0);
					send(new_fd, word, sizeof word, 0);
}

int sentMonitor2(int monitor_port, int new_fd, char* input, char* data ) {
		//printf("sentmi input is %s\n", input);
					char res[999];
					memset(res, '\0', 999 * sizeof(char));
					strcpy(res,data);
					char word[28];
					memset(word, '\0', 28 * sizeof(char));
					strcpy(word, input);

					
					
						
					
					send(new_fd, res, sizeof res, 0);
					send(new_fd, word, sizeof word, 0);
}


int main(){
	
	
	//act as TCP server for users --beej code
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(HOST, TCPPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	freeaddrinfo(servinfo); // all done with this structure

	//listen
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	
	printf( "The AWS is up and running. \n");

	struct m_info m;
	m = monitor();
	//accept 
	while(1){
		sin_size = sizeof their_addr;	
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");			
			exit(1);
		}


		// get the port of client
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
		struct sockaddr_in addrClient;
		memset(&addrClient, 0, sizeof(addrClient));
		int len = sizeof(addrClient);
		getpeername(new_fd, (struct sockaddr *) &addrClient, (socklen_t *) &len);
		int client_port = addrClient.sin_port;


		//receive all the inforamtion from client
		 
		char input[27];
		char func[7];
		char res[100];
		memset(input, '\0', 27*sizeof(char));
		memset(func, '\0', 7*sizeof(char));
		//memset(res, '\0', 100*sizeof(char));


		recv(new_fd, res, 50, 0);	
		//recv(new_fd, func, sizeof func, 0);
		//recv(new_fd, input, sizeof input, 0);
 		//printf("res is %s\n",  res);
		strncpy(func, res, 6);
		strncpy(input, res + 6, 27);
		//printf("%s\n", input);
		//printf("%s\n", func);
		char w[27];
		memset(w, '\0', 27 * sizeof(char));
		strcpy(w, input);
		//printf("w is %s\n", w);
		printf("The AWS received input = <%s> and function = <%s> from the client using TCP over port %d. \n", input, func, client_port);

		if (strcmp(func, "search") == 0) {
			char res1[99];
			memset(res1, '\0', 99*sizeof(char));
			strcpy(res1, searnpre(input, 'A', 1));
			// printf("res1 %s\n", res1 );
			//printf("result A is %s\n", res1);
			if (strcmp(res1, "0") == 0) {
				printf("The AWS received <0> similar words from Backend-Server <A> using UDP over port <21165> \n");
			} else {
				printf("The AWS received <1> similar words from Backend-Server <A> using UDP over port <21165> \n");

			}
			// printf("res1 %s\n", res1 );
			char res2[99];
			memset(res2, '\0', 99*sizeof(char));
			strcpy(res2,searnpre(input, 'B', 1));
			// printf("res2 %s\n", res2 );
			//printf("result B is %s\n", res2);
			if (strcmp(res2, "0") == 0) {
				printf("The AWS received <0> similar words from Backend-Server <B> using UDP over port <22165> \n");
			} else {
				printf("The AWS received <1> similar words from Backend-Server <B> using UDP over port <22165> \n");

			}
			// printf("res2 %s\n", res2 );
			char res3[99];
			memset(res3, '\0', 99*sizeof(char));
			strcpy(res3,searnpre(input, 'C', 1));
			//printf("result C is %s\n", res3);
			// printf("res3 %s\n", res3 );
			if (strcmp(res3, "0") == 0) {
				printf("The AWS received <0> similar words from Backend-Server <C> using UDP over port <23165> \n");
			} else {
				printf("The AWS received <1> similar words from Backend-Server <C> using UDP over port <23165> \n");

			}
			// printf("res3 %s\n", res3 );
			// printf("res1 %s\n", res1 );
			// printf("res2 %s\n", res2 );
			// printf("res3 %s\n", res3 );
			if ((strcmp(res1, "0") == 0) && (strcmp(res2, "0") == 0) && (strcmp(res3, "0") == 0)) {
					printf("The AWS send <0> match to client \n");
					char res[99];
					memset(res, '\0', 99 * sizeof(char));
					strcpy(res, "0");
					
					send(new_fd, res, sizeof(res), 0);
					sentMonitor(m.pnum, m.fd, w, res);
					
			} else if (strcmp(res1, "0") != 0) {
					printf("The AWS send <1> match to client \n");
					char res[99];
					memset(res, '\0', 99 * sizeof(char));
					strcpy(res, res1);
					//printf("ww is %s\n", w);
					send(new_fd, res, sizeof(res), 0);
					sentMonitor(m.pnum, m.fd, w, res);
			} else if (strcmp(res2, "0") != 0){
					printf("The AWS send <1> match to client \n");
					char res[99];
					memset(res, '\0', 99 * sizeof(char));
					strcpy(res, res2);
					
					send(new_fd, res, sizeof(res), 0);
					sentMonitor(m.pnum, m.fd, w, res);
					
			} else {
					printf("The AWS send <1> match to client \n");
					char res[99];
					memset(res, '\0', 99 * sizeof(char));
					strcpy(res, res3);
					
					send(new_fd, res, sizeof(res), 0);
					sentMonitor(m.pnum, m.fd, w, res);
			}
			
		}

		if (strcmp(func, "prefix") == 0) {
			int count;
			char* delim = " ";
			char res1[999];
			memset(res1, '\0', 999*sizeof(char));
			strcpy(res1, searnpre2(input, 'A', 2));
			//printf("data2 is %s\n", res1);
			if (strcmp(res1, "0") == 0) {
				printf("The AWS received <0> matches from Backend-Server <A> using UDP over port <21165> \n");
			} else {

				printf("The AWS received <n> matches from Backend-Server <A> using UDP over port <21165> \n");
				char res11[999];
				memset(res11, '\0', 999 * sizeof(char));
				strcpy(res11, res1);
					//printf("ww is %s\n", w);
				send(new_fd, res11, sizeof(res11), 0);
				//sentMonitor2(m.pnum, m.fd, w, res11);

			}
			char res2[999];
			memset(res2, '\0', 999*sizeof(char));
			strcpy(res2, searnpre2(input, 'B', 2));
			//printf("data2 is %s\n", res1);
			if (strcmp(res2, "0") == 0) {
				printf("The AWS received <0> matches from Backend-Server <B> using UDP over port <22165> \n");
			} else {

				printf("The AWS received <n> matches from Backend-Server <B> using UDP over port <22165> \n");
				char res22[999];
				memset(res22, '\0', 999 * sizeof(char));
				strcpy(res22, res2);
					//printf("ww is %s\n", w);
				send(new_fd, res22, sizeof(res22), 0);
				//sentMonitor2(m.pnum, m.fd, w, res22);
			}
			char res3[999];
			memset(res3, '\0', 999*sizeof(char));
			strcpy(res3, searnpre2(input, 'C', 2));
			//printf("data2 is %s\n", res1);
			if (strcmp(res3, "0") == 0) {
				printf("The AWS received <0> matches from Backend-Server <C> using UDP over port <23165> \n");
			} else {

				printf("The AWS received <n> matches from Backend-Server <C> using UDP over port <23165> \n");
				char res33[999];
				memset(res33, '\0', 999 * sizeof(char));
				strcpy(res33, res3);
					//printf("ww is %s\n", w);
				send(new_fd, res33, sizeof(res33), 0);
				//sentMonitor2(m.pnum, m.fd, w, res33);

			}
			printf("The AWS send <n> match to client \n");
			printf("The AWS send <n> matches to the monitor via TCP port %d for prefix\n",  m.pnum);

		}
		
		

		

		

		

		
		
		close(new_fd); 

	}
	
}
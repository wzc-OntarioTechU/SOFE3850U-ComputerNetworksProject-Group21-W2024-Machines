/* A simple echo server using TCP */
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>

#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN		256	/* buffer length */
#define PACKET_SIZE	100 /* packet length */

void send_file(int);

int main(int argc, char **argv)
{
	int 	sd, new_sd, client_len, port;
	struct	sockaddr_in server, client;

	switch(argc){
		case 1:
			port = SERVER_TCP_PORT;
			break;
		case 2:
			port = atoi(argv[1]);
			break;
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}

	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	/* Bind an address to the socket	*/
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
		fprintf(stderr, "Can't bind name to socket\n");
		exit(1);
	}

	/* queue up to 1 connect requests  */
	listen(sd, 1);

	while(1){
		client_len = sizeof(client);
		new_sd = accept(sd, (struct sockaddr *)&client, &client_len);
		if(new_sd < 0){
			fprintf(stderr, "Can't accept client \n");
			exit(1);
		}

		(void) close(sd);
		send_file(new_sd);
		(void) close(new_sd);
		break;
	}

}

/* Function to send the requested file */
void send_file(int sd){
	FILE *fp;
	struct stat st;
	char fnbuf[BUFLEN];
	char pbuf[PACKET_SIZE];
	int n;

	/* read the filename from the client */	
	printf(" was requested.");
	read(sd, fnbuf, BUFLEN);
	write(0, fnbuf, strlen(fnbuf));
	
	/* search the server's directory */
	if((fp = fopen(fnbuf, "r")) == NULL){
		char error_message[] = "\02File is not found\n";
		printf("\n%s", error_message);
		write(sd, error_message, strlen(error_message) + 1);
		return;
	}
	stat(fnbuf, &st);
	if(st.st_size <= 100){
		char error_message[] = "\02File is too small (<100 bytes)\n";
		printf("\n%s", error_message);	
		write(sd, error_message, strlen(error_message) + 1);
		return;
	}

	printf("\nFile was found\n");
	/* loops until all packets are sent */
	while ((n = fread(pbuf, 1, PACKET_SIZE, fp)) > 0) {
		write(sd, pbuf, n);
	}
	fclose(fp);
	close(sd);
}

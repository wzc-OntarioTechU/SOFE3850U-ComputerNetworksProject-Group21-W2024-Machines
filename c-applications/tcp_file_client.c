/* A simple echo client using TCP */
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>



#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN		256	/* buffer length */
#define PACKET_SIZE	100 /* packet length */

int main(int argc, char **argv)
{
	FILE *fp;
	int 	n, i, bytes_to_read;
	int 	sd, port;
	struct	hostent		*hp;
	struct	sockaddr_in server;
	char	*host, *bp, rbuf[BUFLEN], sbuf[BUFLEN], *filename;

	switch(argc){
	case 2:
		host = argv[1];
		port = SERVER_TCP_PORT;
		break;
	case 3:
		host = argv[1];
		port = atoi(argv[2]);
		break;
	default:
		fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
		exit(1);
	}

	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (hp = gethostbyname(host)) 
	  bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	else if ( inet_aton(host, (struct in_addr *) &server.sin_addr) ){
	  fprintf(stderr, "Can't get server's address\n");
	  exit(1);
	}

	/* Connecting to the server */
	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
	  fprintf(stderr, "Can't connect \n");
	  exit(1);
	}

	/* get file path */
	printf("Enter filepath: \n");
	while(n=read(0, sbuf, BUFLEN)){
	  write(sd, sbuf, n);		/* send it out */
	}

	  bp = rbuf;
	  bytes_to_read = PACKET_SIZE;
	  /* read in the file packets */
	  while ((i = read(sd, bp, bytes_to_read)) > 0){
		bp += i;
		bytes_to_read -=i;
		/* inspect first byte of data */
		if(bp[0] == '\x02'){
			printf("Error, the file was not found on the server");
			close(sd);
			return(0);
		}
		/* reset buffer pointer and bytes_to_read after bytes_to_read = 0, prepares to read the next file packet */
        if (bytes_to_read == 0) {
            bp = rbuf;
            bytes_to_read = PACKET_SIZE;
        }
	  }

	/* write to a local file */
	fp = fopen(sbuf, "w");
	fwrite(rbuf, sizeof(char), bytes_written, fp);
	fclose(fp);
	/* output updated directory and the file contents */
	execvp("ls", NULL);
	execvp("cat", filename);

	close(sd);
	return(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct pdu {
	char type;
	char data[100];
};

int main() {
	int sd;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t addrSize = sizeof(clientAddr);

	// Create UDP socket
	sd = socket(AF_INET, SOCK_DGRAM, 0);

	// Bind to server address
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(12345); // Server port
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	struct pdu rpdu;
	recvfrom(sd, &rpdu, sizeof(rpdu), 0, (struct sockaddr*)&clientAddr, &addrSize);

	printf("Received FILENAME PDU from client: %s\n", rpdu.data);
    
	close(sd);
	return 0;
}


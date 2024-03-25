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
	struct sockaddr_in serverAddr;

	// Create UDP socket
	sd = socket(AF_INET, SOCK_DGRAM, 0);

	// Set up server address
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8000); // Server port
	serverAddr.sin_addr.s_addr = inet_addr("10.0.2.15"); // Server IP

	struct pdu spdu;
	spdu.type = 'C'; // Set the type to FILENAME PDU
	printf("Enter the filename: ");
	scanf("%s", spdu.data);

	// Send FILENAME PDU to server
	sendto(sd, &spdu, sizeof(spdu), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	close(sd);
	return 0;
}


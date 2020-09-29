/* Implements an echo service on UDP. The server listens for UDP datagrams
 * on port PORT. When a datagram is received the server sends the data from the
 * datagram back in an answering datagram.
 */

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

#define PORT "8888"

int main(void)
{
	int socket_fd;
	struct addrinfo hints;
	struct addrinfo *results;
	struct sockaddr_storage client_addr;
	char recv_data[1024];
	socklen_t addrlen = sizeof(client_addr);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, PORT, &hints, &results);

	if ((socket_fd = socket(results->ai_family,
				results->ai_socktype,
				results->ai_protocol)) < 0) {
		printf("Error: could not create socket\n");
		return 1;
	}

	if (bind(socket_fd, results->ai_addr, results->ai_addrlen) != 0) {
		printf("Error: could not bind port to address\n");
		return 1;
	}

	printf("Listening\n");

	while (1) {
		memset(&recv_data, 0, sizeof(recv_data));
		int bytes_read = recvfrom(socket_fd, recv_data, 1024, 0,
					 (struct sockaddr *) &client_addr, &addrlen);
		recv_data[bytes_read] = '\0';
		printf("Datagram received\n");
		sendto(socket_fd, &recv_data, 1024, 0,
		      (struct sockaddr *) &client_addr, addrlen);
		printf("Answering datagram sent\n");
	}
	close(socket_fd);
	return 0;
}

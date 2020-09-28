/* Implements an echo service on UDP. The server listens for UDP datagrams
 * on port 8888. When a datagram is received the server sends the data from the
 * datagram back in an answering datagram.
 */

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int socket_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char recv_data[1024];
	socklen_t addrlen = sizeof(client_addr);

	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Error: could not create socket\n");
		return 1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8888);
	memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

	if (bind(socket_fd, (struct sockaddr *) &server_addr,
			     sizeof(server_addr)) != 0) {
		printf("Error: could not bind port to address\n");
		return 1;
	}

	printf("Listening on %s:%i", inet_ntoa(server_addr.sin_addr),
               server_addr.sin_port);

	while (1) {
		memset(&recv_data, 0, sizeof(recv_data));
		int bytes_read = recvfrom(socket_fd, recv_data, 1024, 0,
					 (struct sockaddr *) &client_addr, &addrlen);
		recv_data[bytes_read] = '\0';
		printf("Datagram received from %s\n",
		       inet_ntoa(client_addr.sin_addr));
		sendto(socket_fd, &recv_data, 1024, 0,
		      (struct sockaddr *) &client_addr, addrlen);
		printf("Answering datagram sent to %s\n",
		       inet_ntoa(client_addr.sin_addr));
	}
	close(socket_fd);
	return 0;
}

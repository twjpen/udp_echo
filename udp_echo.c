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

	// create socket file descriptor. AF_INET for IPv4, SOCK_DGRAM for UDP
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Error: could not create socket\n");
		return 1;
	}

	// gives the server structure an address and port
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8888);
	memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

	/* The bind() function takes  as its second argument a pointer to
	 * a sockaddr struct. Because our server_addr is a sockaddr struct
	 * We have to cast sockaddr_in to sockaddr to make this work. This
	 * is an example of explicit type casting.
	 * The line (struct sockaddr *) &server_addr makes the struct
	 * sockaddr * pointer that bind() expects point to the address of
	 * server_addr, which is a sockaddr_in structure.
	 *
	 * the sin_zero potion of sockaddr_in pads its size so it is the
	 * same as sockaddr. However sin_zero might nog always automatically
	 * be zero. So we explicitly set it to zero to avoid unexpected
	 * behaviour.
	 */

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
		// The same trick as before. We cast client_addr to sockaddr.
		recv_data[bytes_read] = '\0'; // append NULL to received data
		printf("Datagram received from %s\n",
					inet_ntoa(client_addr.sin_addr));
		sendto(socket_fd, &recv_data, 1024, 0, (struct sockaddr *)
						&client_addr, addrlen);
		printf("Answering datagram sent to %s\n",
					inet_ntoa(client_addr.sin_addr));
	}
	close(socket_fd);
	return 0;
}

/**
 * @file
 * @brief Simple SSDP client for discovering devices on local network.
 *
 * https://en.wikipedia.org/wiki/Simple_Service_Discovery_Protocol
 * https://openconnectivity.org/upnp-specs/UPnP-arch-DeviceArchitecture-v2.0-20200417.pdf
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

static bool send_discover_request(int socket_fd)
{
	const char *request = "M-SEARCH * HTTP/1.1\r\n"
	                      "HOST: 239.255.255.250:1900\r\n"
	                      "MAN: \"ssdp:discover\"\r\n"
	                      "MX: 1\r\n"  // max response delay in seconds
	                      "ST: ssdp:all\r\n"  // all devices and services
	                      "\r\n";

	const size_t request_length = strlen(request);

	const struct sockaddr_in dest = {
		.sin_family = AF_INET,
		.sin_port = htons(1900),
		.sin_addr.s_addr = htonl(0xEFFFFFFA)  // 239.255.255.250
	};

	ssize_t length = sendto(socket_fd, request, request_length, 0, (struct sockaddr*) &dest, sizeof dest);
	if (length < 0)
	{
		return false;
	}

	return true;
}

static bool receive_response(int socket_fd)
{
	char buffer[2048];

	const size_t max_length = (sizeof buffer) - 1;  // space for null character

	ssize_t length = recvfrom(socket_fd, buffer, max_length, 0, NULL, NULL);
	if (length < 0)
	{
		return false;
	}

	buffer[length] = '\0';

	// show the received response
	puts(buffer);
	fflush(stdout);

	return true;
}

static int run(int socket_fd)
{
	if (!send_discover_request(socket_fd))
	{
		perror("sendto");
		return 1;
	}

	struct pollfd socket_pollfd = {
		.fd = socket_fd,
		.events = POLLIN,
		.revents = 0
	};

	int status = poll(&socket_pollfd, 1, 2000);  // 2 seconds timeout

	while (status > 0)
	{
		if (!receive_response(socket_fd))
		{
			perror("recvfrom");
			return 1;
		}

		status = poll(&socket_pollfd, 1, 1000);  // 1 second timeout
	}

	if (status < 0)
	{
		perror("poll");
		return 1;
	}

	return 0;
}

int main()
{
	int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_fd < 0)
	{
		perror("socket");
		return 1;
	}

	int status_code = run(socket_fd);

	close(socket_fd);

	return status_code;
}

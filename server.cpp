#include <iostream>
#include <sys/socket.h>


int main(int argc, char *argv[])
{
	if (argc != 2)
	{std::cout << "usage: ./a.out <PORT>\n";

		return 1;
	}
	int fd = socket(PF_LOCAL, SOCK_STREAM, 0);

	int port = -1;
	port = atoi(argv[1]);

	if (port < 0)
	{
		std::cout << "Error with port request\n";
	}

	std::cout << "fd: " << fd << std::endl;
	std::cout << "requested Port: " << port << std::endl;

	const struct sockaddr* address;
	socklen_t address_len;
	bind(fd, &address, sizeof(address));

	return 0;
}

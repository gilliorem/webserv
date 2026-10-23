#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: ./a.out <PORT>\n";

		return 1;
	}
	int sock_fd = socket(PF_INET, SOCK_STREAM, 0);

	if (sock_fd == -1)
	{
		std::cerr << "Socket FAIL\n";
		return 1;
	}

	int port = -1;
	port = atoi(argv[1]);

	if (port < 0)
	{
		std::cout << "Error with port request\n";
		return 1;
	}

	std::cout << "sock_fd: " << sock_fd << std::endl;
	std::cout << "requested Port: " << port << std::endl;


     // int bind(int socket, const struct sockaddr *address, socklen_t address_len);

	struct sockaddr_in address;
	address.sin_family = PF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) == -1)
	{
		std::cerr << "Binding: error\n";
		close(sock_fd);
		return 1;
	}

	if (listen(sock_fd, SOMAXCONN) == -1)
	{
		std::cerr << "listen error\n";
		close (sock_fd);
		return 1;
	}

	std::cout << "Listening ...\n";

	while (true)
	{
		int clientFd = accept(sock_fd, NULL, NULL);
		if (clientFd == -1)
		{
			std::cerr << "Error: client fd\n";
			close(sock_fd);
			return 1;
		}
		char buffer[1024];
		ssize_t bytes_read;
		bytes_read = read(clientFd, buffer, sizeof(buffer) - 1);
		if (bytes_read == -1)
		{
			std::cerr << "Read error\n";
			close(clientFd);
			continue ;
		}
		else if (bytes_read == 0)
		{
			std::cout << "client closed connection" << std::endl;
			close (clientFd);
			continue;
		}
		else
		{
			buffer[bytes_read] = '\0';
			std::cout << "Request:\n" << buffer << std::endl;
		}

		std::cout << "request has been read successfully \n";

		std::string firstLine = "HTTP/1.1 200 OK\r\n";
		//std::string body = "/Users/regilliot/Code/forty-two/webservgit/cockpic.png";
		std::string body = "8==============================>\r\n";
		std::string emptyLine = "\r\n"; 
		//std::string contentDisp = "Content-Disposition: inline; filename='./cockpic.png'\r\n";

		std::string contentLen = "Content-Length: " + std::to_string(body.size()) + "\r\n";
	//	std::string contentType = "Content-Type: image/png\r\n";
		std::string contentType = "Content-Type: text/html\r\n";

		std::string header = firstLine + contentLen + contentType + emptyLine;

		write(clientFd, header.c_str(), header.size());
		write(clientFd, body.c_str(), body.size());
		close(clientFd);
	}
	close(sock_fd);
	std::cout << "End\n";

	return 0;
}

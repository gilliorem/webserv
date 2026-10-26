#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <unordered_map>

// server response (8===============>)


class Request
{
	public:
		std::vector<std::string> firstLine;
		std::string method;
};
class Response
{
	public:
		class Header
		{
			public:
			std::string firstLine; 
			std::unordered_map<unsigned int, std::string> status =
			{
				{200, "Success"},
				{300, "Redirection"},
				{400, "Client Error"},
				{500, "Server error"}
			};
			std::string contentLength;
			std::string contentType;
		};
		
		std::string body;
};

bool methodCheck(std::string &method, Request& request)
{
	if (method != "POST" && method != "GET" && method != "DELETE")
	{
		std::cout << "wrong method\n";
		return false;
	}
	request.method = method;
	return true;
}

// POST / http1

// <d><word><d><word>

std::vector<std::string> split(const std::string& content, const std::string& delimeter)
{
	std::vector<std::string> res;
	std::istringstream input(content);
	std::string line;
	size_t prevDelimiter= 0;
	
	std::vector<size_t> delimetersPos;
	int start = 0;
	
	size_t pos = 0;	
	while (pos != std::string::npos)
	{
		pos = line.find(delimeter, start);
		delimetersPos.push_back(pos);
		start += pos;
	}
	/*
	while (std::getline(input, line))
	{
		res.push_back(line.substr(prevDelimiter), line.find(delimiter));
		delimiterPos = line.find(delimiter);
	}
	*/
	
	for (auto it = delimetersPos.begin(); it != delimetersPos.end(); it++)
	{
		std::cout << *it << " ";
	}

	std::cout << std::endl;

	return res;

}


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: ./a.out <PORT>\n";

		return 1;
	}
			     // ipv4 | 
	int sock_fd = socket(PF_INET, SOCK_STREAM, 0); // create socket 

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
		char buffer[1024]; // client request:   GET / HTTP/1.1
//							Host: localhost:8081
//	      						User-Agent: curl/7.81.0
//	      						Accept: */*

		ssize_t bytes_read;
		bytes_read = read(clientFd, buffer, sizeof(buffer) - 1); // read request 
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

		Request request;
		// check request buffer
		std::string line;
		std::istringstream strBuffer(buffer);
		std::getline(strBuffer, line);

		std::cout << "line: " << line << std::endl;

		//std::vector<std::string> request.firstLine;
		request.firstLine = split(line, " ");
		for (auto it = request.firstLine.begin(); it != request.firstLine.end(); it++)
		{
			std::cout << "word: " << *it << std::endl;
		}


		std::cout << "request has been read successfully \n";

		Response response;
		//Response::Header responseHeader;
		//responseHeader.firstLine = "HTTP/1.1" + responseHeader[200];
		//responseHeader.status = getStatus();
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

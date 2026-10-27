#include <fstream>
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
		std::string path;
		std::string version;
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

bool checkMethod(std::string& method)
{
	if (method != "POST" && method != "GET" && method != "DELETE")
	{
		std::cout << "wrong method\n";
		return false;
	}
	return true;
}

bool checkPath(std::string& path)
{
	if (path.length() == 0)
	{
		std::cout << "wrong path\n";
		return false;
	}
	return true;
}

bool checkVersion(const std::string& version)
{
	if (version != "HTTP/1.1")
		return false;
	return true;
}

// POST / http1

// <d><word><d><word>

// find
// substr

std::vector<std::string> split(const std::string& content, const std::string& delimeter)
{
	std::vector<std::string> res;
	
	size_t start = 0;
	size_t found = 0;	
	while (true)
	{
		found = content.find(delimeter, start);
		if (found == std::string::npos)
		{
			std::cout << "final start: " << start << std::endl;
			res.push_back(content.substr(start));
			break;
		}
		res.push_back(content.substr(start, found - start));
		std::cout << "start: " << start << std::endl; 
		std::cout << "found: " << found << std::endl; 
		start = found + 1;
	}
	return res;
}

bool checkFile(std::string& hostPath)
{

	std::ifstream in_file(hostPath);
	if (!in_file)
	{
		std::cout << "Error opening file " << hostPath << std::endl;
		return false;
	}
	return true;
}

/*
 std::unordered_map<unsigned int, std::string> status =
 {
 {200, "Success"},
 {300, "Redirection"},
 {400, "Client Error"},
 {500, "Server error"}
 };
 */

bool getStatus(Request req, Response& res)
{
	
	// if the <host> is incorrect: 404 
	if (!checkFile(req.path))
	{
		std::cout << res.Header::status[2][0] << ": " << res.Header::status[2][1] << std::endl;
	}
	return true;
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
		std::cout << "line len: " << line.length() << std::endl;
		request.firstLine = split(line, " ");
		if (!checkMethod(request.firstLine[0]))
			return 1;
		request.method = request.firstLine[0];
		if (!checkPath(request.firstLine[1]))
			return 1;
		request.path = request.firstLine[1];
		if (!checkVersion(request.firstLine[2]))
			return 1;
		request.version = request.firstLine[2];

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

// #include <filesystem> C++17
#include <sys/stat.h>  // stat
#include <sys/types.h>
#include <sys/uio.h>
#include <fstream>
#include <vector>
#include <sstream>	   // std::stringstream
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>    // read
// #include <unordered_map> C++11
#include <map>
#include <cstdlib>	   // std::atoi

// server response (8===============>)

class Request
{
	public:
		std::vector<std::string> firstLine;
		std::string method;
		std::string path;
		std::string version;
        std::map<std::string, std::string> headers;
        std::string body;

        bool parse(char* buffer, int clientFd, const std::string& root);
};

std::vector<std::string> split(const std::string& content, const std::string& delimiter)
{
	std::vector<std::string> res;
	size_t start = 0;

	while (true)
	{
		size_t found = content.find(delimiter, start);
		if (found == std::string::npos)
		{
			res.push_back(content.substr(start));
			break;
		}
		res.push_back(content.substr(start, found - start));
		start = found + delimiter.length();
	}
	return res;
}

bool checkMethod(const std::string& method)
{
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		std::cout << "wrong method\n";
		return false;
	}
	return true;
}

// handle directory case - huge file - filepath contain '..' (shouldnt be able to go back)
bool checkPath(const std::string& path, const std::string& root)
{
	if (path.length() == 0 || path.length() > 1024)
	{
		std::cout << "wrong path\n";
		return false;
	}

	std::string globalPath = root + path;
	struct stat fileInfo;

	if (stat(globalPath.c_str(), &fileInfo) != 0)
	{
		std::cerr << "File " << globalPath << " doesnt exist\n";
		return false;
	}

	return true;
}

bool checkVersion(const std::string& version)
{
	if (version != "HTTP/1.1")
	{
		std::cout << "wrong http version\n";
		std::cout << version << std::endl;
		return false;
	}
	return true;
}

bool Request::parse(char* buffer, int clientFd, const std::string& root)
{
        std::vector<std::string> header_body = split(buffer, "\r\n\r\n");
        std::cout << "header:\n" << header_body[0] << std::endl;
        std::cout << "body:\n" << header_body[1] << std::endl;
        
		std::vector<std::string> headerLines = split(header_body[0], "\r\n"); // store our lines

        // Request firstLine: GET / HTTP/1.1
		std::cout << "first line: " << headerLines[0] << std::endl;
		this->firstLine = split(headerLines[0], " ");

        if (this->firstLine.size() != 3)
        {
            std::string response = 
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			return false;
        }
		if (!checkMethod(this->firstLine[0]))
        {
            // Return HTTP 501 Not Implemented
            std::string response = 
                "HTTP/1.1 501 Not Implemented\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			return false;
        }
		this->method = this->firstLine[0];
		if (!checkPath(this->firstLine[1], root))
        { 
            // Return HTTP 404 Not Found
            std::string response = 
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			return false;
        }
		this->path = this->firstLine[1];
		if (!checkVersion(this->firstLine[2]))
        {
            // Return HTTP 505 HTTP Version Not Supported
            std::string response = 
                "HTTP/1.1 505 HTTP Version Not Supported\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			return false;
        }
		this->version = this->firstLine[2];

        // Request Headers
        for (size_t i = 1; i < headerLines.size(); i++)
        {
            std::cout << "headerLines[" << i << "]: " << std::endl;
            std::vector<std::string> key_value = split(headerLines[i], ": ");
            std::cout << "key: " << key_value[0] << std::endl;
            std::cout << "value: " << key_value[1] << std::endl;
            this->headers[key_value[0]] = key_value[1];
        }

        // Request Body
        this->body = header_body[1];
		std::cout << "request has been read successfully \n";
        return true;
}

class Response
{
	public:
		class Header
		{
			public:
				std::string firstLine; 
				std::map<unsigned int, std::string> status;
				std::string contentLength;
				std::string contentType;

				Header()
				{
					status[200] = "Success";
					status[300] = "Redirection";
					status[400] = "Client Error";
					status[500] = "Server error";
				}
		};
		
		std::string body;
};


bool checkFile(const std::string& hostPath)
{
	std::ifstream in_file(hostPath.c_str());
	if (!in_file)
	{
		std::cout << "Error opening file " << hostPath << std::endl;
		return false;
	}
	return true;
}

/*
bool getStatus(Request req, Response& res)
{
	// if the <host> is incorrect: 404 
	if (!checkFile(req.path))
	{
		//std::cout << res.Header::status[2][0] << ": " << res.Header::status[2][1] << std::endl;
	}
	return true;
}
*/

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
	port = std::atoi(argv[1]);

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
		close(sock_fd);
		return 1;
	}

	std::cout << "Listening ...\n";

	std::string macRoot = "/Users/regilliot/code/forty-two/webserv";
	std::string fortyTwoRoot = "/home/regillio/Desktop/webserv";
	while (true)
	{
		int clientFd = accept(sock_fd, NULL, NULL);
		if (clientFd == -1)
		{
			std::cerr << "Error: client fd\n";
			close(sock_fd);
			return 1;
		}

//  Parse request
//      client request:   
//          GET / HTTP/1.1
//		    Host: localhost:8080
//	        User-Agent: curl/7.81.0
//	        Accept: */*
//
		char buffer[1024]; 
		ssize_t bytes_read = read(clientFd, buffer, sizeof(buffer) - 1); // read request 

		if (bytes_read == -1)
		{
			std::cerr << "Read error\n";
			close(clientFd);
			continue ;
		}
		else if (bytes_read == 0)
		{
			std::cout << "client closed connection" << std::endl;
			close(clientFd);
			continue;
		}
		else
		{
			buffer[bytes_read] = '\0';
			std::cout << "Request:\n" << buffer << std::endl;
			std::cout << "End of request\n";
		}

		// check request buffer
		Request request;
        if (!request.parse(buffer, clientFd, fortyTwoRoot))
            continue;
        /*
        std::vector<std::string> header_body = split(buffer, "\r\n\r\n");
        std::cout << "header:\n" << header_body[0] << std::endl;
        std::cout << "body:\n" << header_body[1] << std::endl;
        
		std::vector<std::string> headerLines = split(header_body[0], "\r\n"); // store our lines

        // Request firstLine: GET / HTTP/1.1
		std::cout << "first line: " << headerLines[0] << std::endl;
		request.firstLine = split(headerLines[0], " ");

        if (request.firstLine.size() != 3)
        {
            std::string response = 
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			continue;
        }
		if (!checkMethod(request.firstLine[0]))
        {
            // Return HTTP 501 Not Implemented
            std::string response = 
                "HTTP/1.1 501 Not Implemented\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			continue;
        }
		request.method = request.firstLine[0];
		if (!checkPath(request.firstLine[1], fortyTwoRoot))
        { 
            // Return HTTP 404 Not Found
            std::string response = 
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			continue;
        }
		request.path = request.firstLine[1];
		if (!checkVersion(request.firstLine[2]))
        {
            // Return HTTP 505 HTTP Version Not Supported
            std::string response = 
                "HTTP/1.1 505 HTTP Version Not Supported\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n"
                "\r\n";

            write(clientFd, response.c_str(), response.size());
			close(clientFd);
			continue;
        }
		request.version = request.firstLine[2];

        // Request Headers
        for (size_t i = 1; i < headerLines.size(); i++)
        {
            std::cout << "headerLines[" << i << "]: " << std::endl;
            std::vector<std::string> key_value = split(headerLines[i], ": ");
            std::cout << "key: " << key_value[0] << std::endl;
            std::cout << "value: " << key_value[1] << std::endl;
            request.headers[key_value[0]] = key_value[1];
        }

        // Request Body
        request.body = header_body[1];
		std::cout << "request has been read successfully \n";
        */

		Response response;
		//Response::Header responseHeader;
		//responseHeader.firstLine = "HTTP/1.1" + responseHeader[200];
		//responseHeader.status = getStatus();
		std::string firstLine = "HTTP/1.1 200 OK\r\n";
		std::ifstream imgfile("./cockpic.png");	
		
		FILE* file = fopen("./cockpic.png", "r");
		
		char filebuffer[40000];

		ssize_t bytesRead = fread(filebuffer, 1, sizeof(filebuffer), file);
		std::cout << "bytes read:" << bytesRead << std::endl;

		//std::string body = "8==============================>\r\n";
		std::string body;
		body.resize(bytesRead);
		for (int i = 0; i < bytesRead; i++)
			body[i] = filebuffer[i];
		//std::cout << "Body: " << body << std::endl;
		std::string emptyLine = "\r\n"; 
		//std::string contentDisp = "Content-Disposition: inline; filename='./cockpic.png'\r\n";

		std::stringstream ss;
		ss << body.size();

		std::string contentLen = "Content-Length: " + ss.str() + "\r\n";
		std::string contentType = "Content-Type: image/png\r\n";
		//std::string contentType = "Content-Type: text/html\r\n";

		std::string header = firstLine + contentLen + contentType + emptyLine;

		write(clientFd, header.c_str(), header.size());
		write(clientFd, body.c_str(), body.size());
		close(clientFd);
	}
	close(sock_fd);
	std::cout << "End\n";

	return 0;
}

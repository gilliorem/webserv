#include <sys/stat.h>   // stat, S_ISDIR
#include <sys/types.h>
#include <sys/uio.h>
#include <fstream>      // std::ifstream
#include <vector>
#include <sstream>	    // std::stringstream
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>     // read, close
#include <map>
#include <cstdlib>	    // std::atoi
#include "utils.hpp"    // split

class Response
{
	public:
        std::string firstLine; 
        int statusCode;
        std::map<std::string, std::string> headers;
		std::string body;

        Response(int statusCode, const std::string& body);

        std::string getStatusText() const;
        std::string generate();
		
};

Response::Response(int statusCode, const std::string& body): statusCode(statusCode), body(body)
{
}

/*
   status[200] = "Success";
   status[300] = "Redirection";
   status[400] = "Client Error";
   status[500] = "Server error";
*/
std::string Response::getStatusText() const
{
    if (this->statusCode == 200)
        return "OK";
    if (this->statusCode == 400)
        return "Bad Request";
    if (this->statusCode == 404)
        return "Not Found";
    if (this->statusCode == 500)
        return "Internal Server Error";
    if (this->statusCode == 501)
        return "Not Implemented";
    if (this->statusCode == 505)
        return "HTTP Version Not Supported";

    return "Unknown";
}

std::string Response::generate()
{
    std::string response;

    std::stringstream ss;
    ss << statusCode;

    std::stringstream ssSize;
    ssSize << body.size();

    response += "HTTP/1.1 " + ss.str() + " " + getStatusText() + "\r\n";
    //response += "Server: nrgi\r\n";
    response += "Content-Length: " + ssSize.str() + "\r\n";
    response += "Content-Type: " + headers["Content-Type"] + "\r\n";
    response += "\r\n";
    response += body;

    return response;
}

// Can use binary mode for all file types: .txt, .html, .css, .png, .jpg, .pdf
bool readFile(const std::string& path, std::string& content)
{
    std::ifstream file(path.c_str(), std::ios::binary);

    if (!file)
        return false;

    // Read the file buffer directly into a stringstream
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (!buffer)
        return false;

    // Store the stream content into a single string variable
    content = buffer.str();

    return true;
}

// Content-Type: HTML, CSS, JS, PNG, JPG, TXT
std::string getContentType(const std::string& path)
{
    std::string::size_type pos = path.rfind('.');

    if (pos == std::string::npos)
        return "application/octet-stream";

    std::string extension = path.substr(pos);

    if (extension == ".html" || extension == ".htm")
        return "text/html";
    if (extension == ".css")
        return "text/css";
    if (extension == ".js")
        return "text/javascript";
    if (extension == ".txt")
        return "text/plain";
    if (extension == ".json")
        return "application/json";
    if (extension == ".png")
        return "image/png";
    if (extension == ".jpg" || extension == ".jpeg")
        return "image/jpeg";
    if (extension == ".gif")
        return "image/gif";
    if (extension == ".svg")
        return "image/svg+xml";
    if (extension == ".pdf")
        return "application/pdf";

    return "application/octet-stream";
}

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

    // Handle directory
    if (S_ISDIR(fileInfo.st_mode))
    {
        std::cout << globalPath << " is a directory" << std::endl;
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

        if (header_body.size() < 2)
        {
            close(clientFd);
            return false;
        }

        std::cout << "header:\n" << header_body[0] << std::endl;
        std::cout << "body:\n" << header_body[1] << std::endl;
        
		std::vector<std::string> headerLines = split(header_body[0], "\r\n"); // store our lines

        // Request firstLine: GET / HTTP/1.1
		std::cout << "first line: " << headerLines[0] << std::endl;
		this->firstLine = split(headerLines[0], " ");

        if (this->firstLine.size() != 3)
        {
            Response response(400, "");
            std::string message = response.generate();
            write(clientFd, message.c_str(), message.size());
			close(clientFd);
			return false;
        }

		if (!checkMethod(this->firstLine[0]))
        {
            Response response(501, "");
            std::string message = response.generate();
            write(clientFd, message.c_str(), message.size());
			close(clientFd);
			return false;
        }
		this->method = this->firstLine[0];

		if (!checkPath(this->firstLine[1], root))
        { 
            Response response(404, "");
            std::string message = response.generate();
            write(clientFd, message.c_str(), message.size());
			close(clientFd);
			return false;
        }
		this->path = this->firstLine[1];

		if (!checkVersion(this->firstLine[2]))
        {
            Response response(505, "");
            std::string message = response.generate();
            write(clientFd, message.c_str(), message.size());
			close(clientFd);
			return false;
        }
		this->version = this->firstLine[2];

        // Request Headers
        for (size_t i = 1; i < headerLines.size(); i++)
        {
            std::cout << "headerLines[" << i << "]: " << std::endl;
            std::vector<std::string> key_value = split(headerLines[i], ": ");

            if (key_value.size() < 2)
            {
                Response response(400, "Bad Request");
                response.headers["Content-Type"] = "text/plain";

                std::string message = response.generate();
                write(clientFd, message.c_str(), message.size());
                close(clientFd);
                return false;
            }

            std::cout << "key: " << key_value[0] << std::endl;
            std::cout << "value: " << key_value[1] << std::endl;
            this->headers[key_value[0]] = key_value[1];
        }

        // Request Body
        this->body = header_body[1];
		std::cout << "request has been read successfully \n";
        return true;
}


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

	/* get from config file */
	//Config conf("webserv.conf");
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

	//std::string macRoot = "/Users/regilliot/code/forty-two/webserv";
	//std::string fortyTwoRoot = "/home/regillio/Desktop/webserv";
	std::string macRoot = "/Users/chenyuchi/CommonCore/webserv";
	std::string fortyTwoRoot = "/home/yucchen/CommonCore/webserv";
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
        if (!request.parse(buffer, clientFd, macRoot))
            continue;

        std::string content;
        std::string filePath = macRoot + request.path;

        if (!readFile(filePath, content))
        {
            Response response(404, "Not Found");
            std::string message = response.generate();
            write(clientFd, message.c_str(), message.size());
        }
        else
        {
		    Response response(200, content);
            response.headers["Content-Type"] = getContentType(filePath);

            std::string message = response.generate();
            write(clientFd, message.c_str(), message.size());
        }
        /*
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
        */
		close(clientFd);
	}
	close(sock_fd);
	std::cout << "End\n";

	return 0;
}

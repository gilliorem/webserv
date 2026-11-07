#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>

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
			res.push_back(content.substr(start));
			break;
		}
		res.push_back(content.substr(start, found - start));
		start = found + 1;
	}
	return res;
}


class Config
{
	public:
		std::vector<std::string> fileContent;

		std::string listenLine;
		//std::unordered_map<std::string, unsigned int> firstLine;
		std::unordered_map<std::string, std::string> root;
		std::unordered_map<std::string, unsigned int> size;

		

		Config(std::string filePath)
		{
			std::string fileLine;
			std::ifstream file(filePath);
			while (std::getline(file, fileLine, '\n'))
			{
				std::size_t found = fileLine.find("listen");
				if (found != std::string::npos)
				{
					this->listenLine = fileLine;
				}
				else
				{
					std::cout << "file line: " << fileLine << std::endl;
				}
				this->fileContent.push_back(fileLine);
			}
		}
};



int main()
{
	// find server
	// find http
	//
	// key <space> value

	//std::cout << "file content:";
	Config conf("webserv.conf");
	return 0;
	std::string fileLine;
	std::vector<std::string> fileContent;
	//std::cin >> fileContent;
	std::ifstream file("webserv.conf");
	while (file >> fileLine)
	{
		fileContent.push_back(fileLine);
	}
	for (std::vector<std::string>::iterator it = fileContent.begin(); it != fileContent.end(); ++it)
		std::cout << *it << std::endl;

	/* In file content, we need to find:
	 * 'server'
	 * 'http'
	 * 'listen'
	 * 'root'
	 * ...
	 */
	
	//std::cout << "file content: \n" << fileContent;
	file.close();

	return 0;
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t bytesRead;

	pFile = std::fopen("webserv.conf", "r");
	if (!pFile) {std::cout << "error opening file"; return 1;}

 	fseek(pFile, 0, SEEK_END);

	lSize = ftell(pFile);
	//std::cout << "l size:" << lSize << std::endl;
	rewind (pFile);


	/* size_t fread ( void * ptr, size_t size, size_t count, FILE * stream ); */
	// say we have the size file before reading

	buffer = (char*)malloc(sizeof(lSize));
	bytesRead = fread(buffer,1,lSize,pFile);
	std::cout << "bytes read:" << bytesRead << std::endl;
	std::cout << "l size:" << lSize << std::endl;

	if (bytesRead != lSize)
	{std::cout << "Error reading file\n"; return 1;}

	std::cout << "file content: \n" << buffer << std::endl;
	fclose(pFile);
	free(buffer);
	return 0;

}

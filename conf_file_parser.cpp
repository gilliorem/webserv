#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <map>

std::vector<std::string> split(const std::string& content, const std::string& delimiter)
{
	std::vector<std::string> res;
	size_t start = 0;
	size_t found = 0;

	while (true)
	{
		found = content.find(delimiter, start);
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


class Config
{
	public:
		std::vector<std::string> fileContent;
		std::vector<std::string> lineContent;

		std::map<std::string, unsigned int> listenPort;
		std::map<std::string, std::string> root;
		std::map<std::string, unsigned int> reqSize;
		std::map<std::string, unsigned int> fileSize;

		Config(std::string filePath)
		{
			std::string fileLine; // temp
			std::ifstream file(filePath);
			
			while (std::getline(file, fileLine, '\n'))
			{
				this->lineContent = split(fileLine, " ");
				std::size_t listenFound = fileLine.find("listen");
				std::size_t rootFound = fileLine.find("root");
				std::size_t sizeReqFound = fileLine.find("max_size_req");
				std::size_t sizeFileFound = fileLine.find("max_size_file");
				this->fileContent.push_back(fileLine);
				if (listenFound != std::string::npos)
				{
					this->listenPort[this->lineContent[0]] = std::atoi(this->lineContent[1].c_str());	
					std::map<std::string, unsigned int>::iterator it = listenPort.begin();	
					std::cout << "key:" <<  it->first << "|";
					std::cout << "value:" <<  it->second;
					std::cout << std::endl;
					continue;
				}
				if (rootFound != std::string::npos)
				{
					this->root[this->lineContent[0]] = this->lineContent[1];
					std::map<std::string, std::string>::iterator it = root.begin();	
					std::cout << "key:" <<  it->first << "|";
					std::cout << "value:" <<  it->second;
					std::cout << std::endl;
					continue;
				}
				if (sizeReqFound != std::string::npos)
				{
					this->reqSize[this->lineContent[0]] = std::atoi(this->lineContent[1].c_str());	
					std::map<std::string, unsigned int>::iterator it = reqSize.begin();	
					std::cout << "key:" <<  it->first << "|";
					std::cout << "value:" <<  it->second;
					std::cout << std::endl;
					continue;
				}
				if (sizeFileFound != std::string::npos)
				{
					this->fileSize[this->lineContent[0]] = std::atoi(this->lineContent[1].c_str());	
					std::map<std::string, unsigned int>::iterator it = fileSize.begin();	
					std::cout << "key:" <<  it->first << "|";
					std::cout << "value:" <<  it->second;
					std::cout << std::endl;
					continue;
				}
				else
				{
					std::cout << "file line: " << fileLine << std::endl;
				}
			}
		}
};



int main()
{
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

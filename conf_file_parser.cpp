#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include "utils.hpp"
#include <map>

/* the config class is responsible for parsing the config file:
 * it will make sure the file exist with all the required information 
 * it will save those info in the `fileContent` vector that will later been use to construct a Request */
/* the `fileContent` will save a list of mapped information such as
 * listen port {key value} 
 * 
 */

std::vector<std::map<std::string, std::string>> parseConfigLine(const std::string &filePath)
{
	std::vector <std::map <std::string, std::string>> linesKeyValue;

	std::string line;
	std::ifstream file(filePath);
	std::map<std::string, std::string> keyValue;
	while (std::getline(file, line, '\n'))
	{
		if (line.length() > 1)
		{
			std::vector<std::string> lineContent = split(line, " ");
			if (lineContent.size() > 1)
			{
				keyValue[lineContent[0]] = lineContent[1];
			}
		}
	}
	linesKeyValue.push_back(keyValue);
	return linesKeyValue;
}

void printParsedFile(std::vector <std::map <std::string, std::string>> &linesKeyValue)
{
	for (std::vector<std::map <std::string, std::string>>::iterator it = linesKeyValue.begin(); it != linesKeyValue.end(); it++)
	{
		for (std::map <std::string, std::string>::iterator mapIt = (*it).begin(); mapIt != (*it).end(); mapIt++)
		{
			std::cout << "key:" << mapIt->first << "|value:" << mapIt->second << std::endl;
		}
	}
}

std::string getValue(std::vector <std::map <std::string, std::string>> &linesKeyValue, const std::string &key)
{
	for (std::vector<std::map <std::string, std::string>>::iterator it = linesKeyValue.begin(); it != linesKeyValue.end(); it++)
	{
		for (std::map <std::string, std::string>::iterator mapIt = (*it).begin(); mapIt != (*it).end(); mapIt++)
		{
			//std::cout << "key:" << mapIt->first << "|value:" << mapIt->second << std::endl;
			//std::cout << "key:" << mapIt->first << std::endl;
			if (mapIt->first == key)
				return mapIt->second;
		}
	}
	return "";
}

class Config
{
	public:
	std::vector<std::map <std::string, std::string>> fileContent;
	std::string listenValue;
	std::string addr;
	int port;
	std::string root;
	std::string location;

	Config(const std::string &filePath)
	{
		this->fileContent = parseConfigLine(filePath);	
		this->listenValue = getValue(this->fileContent, "listen");
		this->root= getValue(this->fileContent, "root");
		this->location = getValue(this->fileContent, "location");
		std::size_t findPort = this->listenValue.find(":");
		if (findPort != std::string::npos)
		{
			this->addr = this->listenValue.substr(0, findPort);
			this->port = std::atoi(this->listenValue.substr(findPort+1).c_str());
		}
	}

};

/*
class Config
{
	public:
		std::vector<std::map<std::string, std::string>> fileContent;
		std::string line;
		std::map<std::string, std::string> listenPort;
		std::map<std::string, std::string> root;
		std::map<std::string, std::string> reqSize;
		std::map<std::string, std::string> fileSize;
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
				if (listenFound != std::string::npos)
				{
					this->listenPort[this->lineContent[0]] = std::atoi(this->lineContent[1].c_str());	
					std::map<std::string, std::string>::iterator it = listenPort.begin();	
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
					std::map<std::string, std::string>::iterator it = reqSize.begin();	
					std::cout << "key:" <<  it->first << "|";
					std::cout << "value:" <<  it->second;
					std::cout << std::endl;
					continue;
				}
				if (sizeFileFound != std::string::npos)
				{
					this->fileSize[this->lineContent[0]] = std::atoi(this->lineContent[1].c_str());	
					std::map<std::string, std::string>::iterator it = fileSize.begin();	
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
*/

int main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		// can also use a default config file if no conf file has been provide.
		std::cout << "need to provide config file\n";
		return 1;
	}
	/*
	std::map <std::string, std::string> keyValue;
	keyValue["key"] = "value";
	linesKeyValue.push_back(keyValue);
	*/

	std::string filePath = argv[1];
	
	Config c(filePath);
	std::cout << c.listenValue << std::endl;
	std::cout << c.addr << std::endl;
	std::cout << c.port << std::endl;
	std::cout << c.root << std::endl;
	std::cout << c.location << std::endl;

	return 0;

	/*

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


	// size_t fread ( void * ptr, size_t size, size_t count, FILE * stream ); 
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
	*/

}

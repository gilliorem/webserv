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

// parse line by line
// first line

struct configLine
{
	std::string addr;
	int port;
};

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
				//std::cout << "key: " << lineContent[0] << " | Value: " << lineContent[1] << std::endl;
				keyValue[lineContent[0]] = lineContent[1];
				//std::cout << "Key: " << keyValue[lineContent[0]] << " | Value: " << keyValue[lineContent[1]] << std::endl;
				//std::cout << "Key: " << key << " | Value: " << value << std::endl;
				//std::cout << "key:" << lineContent[0] << std::endl;
				//std::cout << "value:" << lineContent[1] << std::endl;
				linesKeyValue.push_back(keyValue);
			}
		}
	}
	return linesKeyValue;
}



void printParsedFile(std::vector <std::map <std::string, std::string>> &linesKeyValue)
{
	std::cout << "v size: " << linesKeyValue.size() << std::endl;
	//for (std::vector <std::map <std::string, std::string>>::iterator it = linesKeyValue.begin(); it != linesKeyValue.end(); ++it)
	for (std::vector<std::map <std::string, std::string>>::iterator it = linesKeyValue.begin(); it != linesKeyValue.end(); it++)
	//for (int i = 0; i < linesKeyValue.size(); i++)
	{
		//std::cout << "here\n";
		//std::map <std::string, std::string>::iterator mapIt;
		//mapIt = (*it).begin();
		//std::cout << mapIt->first << std::endl;
		//std::cout << mapIt->second << std::endl;
		int i = 0;
		for (std::map <std::string, std::string>::iterator mapIt = (*it).begin(); mapIt != (*it).end(); mapIt++)
		{
			std::cout << "map size: " << (*it).size() << std::endl;
			std::cout << "i: " << i;
			std::cout << "key:" << mapIt->first << " | value:" << mapIt->second << std::endl;
			i++;
		}
	}
}
		//   std::map <std::string, std::string>::iterator it = listenPort.begin();	
		/*
		for (std::map <std::string, std::string>::iterator it = keyValue.begin(); it != keyValue.end(); it++)
		{
			std::cout << "key:" << it->first << " | value:" << it->second << std::endl;
		}
		*/
		//std::map <std::string, std::string>::iterator mapIt;
		//mapIt = *it;

/*
std::vector<std::string> parseConfigLine(const std::string &filePath)
{
	std::string line;	
	std::ifstream file(filePath); // check return value
	std::vector<std::string> lineContent;
	while (std::getline(file, line, '\n'))
	{
		lineContent = split(line, " ");
	}

	return lineContent;
}

class Config
{
	public:
	std::string addr;
	int port;

	Config(std::string filePath);
};
*/

class Config
{
	public:
		//std::vector<std::string> fileContent; 
		std::vector<std::map<std::string, std::string>> fileContent;
		std::vector<std::string> lineContent;
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

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		// can also use a default config file if no conf file has been provide.
		std::cout << "need to provide config file\n";
		return 1;
	}
	std::vector <std::map <std::string, std::string>> linesKeyValue;
	linesKeyValue = parseConfigLine("webserv.conf");
	printParsedFile(linesKeyValue);
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

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


bool isWord(std::vector<std::string> &fileContent, const std::string& word)
{

	for (std::vector<std::string>::iterator it = fileContent.begin(); it != fileContent.end(); it++)
	{
		if (*it == word)
			return true;
	}
	return false;
}

void parser()
{
	/* PARTS */
	std::string general = "--//GENERAL//--";;
	std::string server = "--//SERVER//--";;

	int partsName = 2;
	std::vector<std::string>partWords;
	partWords.push_back(general);
	partWords.push_back(server);

	/* GENERAL VARS */
	int partsVars = 4;
	std::string user = "user";
	std::string root = "root";

	std::string macPath = "/Users/regilliot/code/forty_two/webserv/www/";
	std::string linuxPath = "/home/regillio/Desktop/webserv/www/";
	std::string errorPagePath = linuxPath + "errorPage/";

	/* SERVER VARS */

	int serverVars = 2;

	/* KEYS */
	std::string location = "location";
	std::string localhost = "localhost";

	/* VALUES */
	std::string port;
	std::string locationPath = "";

	#ifndef MAC
		errorPagePath = macPath + "errorPage/";
	#endif

	errorPagePath = linuxPath + "errorPage/";
	std::vector<std::string>fileContent;
	std::string fileWord;
	std::ifstream file("webserv.mac.conf");
	while (file >> fileWord)
	{
		fileContent.push_back(fileWord);
	}
	bool isGeneral = isWord(fileContent, general);

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
			if (mapIt->first == key)
				return mapIt->second;
		}
	}
	//return defaultValue(key);
	return "default";
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
		this->root = getValue(this->fileContent, "root");
		this->location = getValue(this->fileContent, "location");
		std::size_t findPort = this->listenValue.find(":");
		if (findPort != std::string::npos)
		{
			this->addr = this->listenValue.substr(0, findPort);
			this->port = std::atoi(this->listenValue.substr(findPort+1).c_str());
		}
	}

};

int main(int argc, const char *argv[])
{
	parser();
	return 0;
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
	return 0;
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

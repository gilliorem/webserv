#include <sstream>
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

template<typename T>
class Expected
{
	private:
		std::string _message;
		T _value;
		bool _hasValue;
		Expected(){}
	public:
		

};

class Config
{
	public:
	std::vector<std::string> lines;
	std::vector<std::string> firstLine;
	std::string secondLine;
	std::vector <std::pair <std::string, std::string>> keyValuePair;
	
	std::string listenValue;
	std::string addr;
	int port;
	std::string root;
	std::string location;

	void getLines(const std::string& filePath)
	{
		std::ifstream file(filePath);	
		std::string line;
		while (std::getline(file, line))
		{
			lines.push_back(line);
		}
	}
	bool parseFirstLine()
	{
		std::cout << "First line: " << lines[0] << std::endl;
		/* split by spaces */
		std::istringstream iss(lines[0]);
		std::string word;
		int wordCount = 0;
		while (iss >> word)
		{
			std::cout << "word: " << word << std::endl;
			wordCount ++;
			firstLine.push_back(word);
		}
		if (wordCount != 2)
		{
			std::cout << "Error parsing first line: incorrect number of words.\n";
			std::cout << "Word count: " << wordCount << std::endl;
			return false;
		}
		std::cout << "trimed first line: ";
		for (auto it: firstLine)
			std::cout << it;
		std::cout << std::endl;
		if (firstLine[0] != "error_404")
		{
			std::cout << "Error parsing first line: incorrect error key\n";
			return false;
		}
		this->keyValuePair.push_back(std::make_pair(firstLine[0], firstLine[1]));
		std::cout << "first line pair:" << std::endl << "key: " << firstLine[0] << std::endl << "value: " << firstLine[1] << std::endl;
		// check error page file		
		return true;
	}
	bool parseMainContext()
	{
		if (parseFirstLine() == false)
			return false;
		return true;
	}
	bool parseSecondLine()
	{
		std::cout << "second line: " << this->lines[1];
		this->secondLine = this->lines[1];

		return true;
	}
	Config() {}
	Expected<Config> parser()
	{

	}

	/*
	Config(const std::string &filePath)
	{
		this->keyValuePair; 
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
	*/
};

bool parser(Config& conf)
{
	//Expected <std::string>expectedConfig(config);
	bool error = false;

	/* PARTS */
	std::string serverBeg = "--//SERVER_BEGIN//--";;
	std::string serverEnd = "--//SERVER_END//--";;

	int parts = 2;
	std::vector<std::string>partWords;
	partWords.push_back(serverBeg);
	partWords.push_back(serverEnd);

	/* GENERAL VARS */
	int mainVars = 3;
	std::string error_page = "error_404";

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
	/* reading the file word by word (`>>` split on spaces) */
	while (file >> fileWord)
	{
		fileContent.push_back(fileWord);
	}
	//parseMainContext();
	bool isServerBeg = isWord(fileContent, serverBeg);
	bool isServerEnd = isWord(fileContent, serverEnd);
	if (isServerBeg == false)
	{
		std::cout << "Error: Main context not found\n";
		return false;
		//return Expected<T>("Main context not found\n");
	}
	return true;
	//return Expected<T>(config);
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


int main(int argc, const char *argv[])
{
	//Config conf;
	Config::parser("webserv.conf");
}

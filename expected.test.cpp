#include <string>
#include <fstream>
#include <iostream>

// Expected v1: "general"
// Expected v2: "Error: general not found"

class Conf
{
	public:
		std::string mainContext;
		Conf(const std::vector<std::string> &fileContent)
		{
			this->mainContext = fileContent[0];
		}
};

template <typename T>
class Expected
{
	public:
	bool hasValue;
	std::string message;
	T Object;
	Expected(const std::string& message){
		if (this->hasValue == false){
			this->message = "Error:";
		} 
		else
		{
			this->message = message;
		}
		std::cout << this->message << std::endl;
	} 
	~Expected(){}
};

Expected<std::string> parser(std::string& filename)
{
	std::ifstream file(filename);
	std::vector<std::string> fileContent;
	std::string fileword;
	while (file >> fileword)
	{
		fileContent.push_back(fileword);
	}
	Conf conf(fileContent);
	Expected <std::string>expectedConf("general");
	std::cout << expectedConf.message << std::endl;
	return expectedConf;
}

int main()
{
	Expected <std::string> parser("webserv.mac.conf");
	return 0;
}

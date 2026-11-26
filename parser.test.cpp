#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

std::vector<std::string>getLinesFromFile(const std::string& filePath)
{
    std::vector<std::string> lines;
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line))
    {
        //std::cout << "line: " << line << std::endl;
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> splitSocketAddr(const std::string socketAddr)
{
    size_t findColon = socketAddr.find(":");
    std::vector<std::string> socketPair;
    std::string addr = socketAddr.substr(0, findColon);
    std::string port = socketAddr.substr(findColon + 1);
    std::cout << "addr: " << addr << std::endl;
    std::cout << "port: " << port << std::endl;
    socketPair.push_back(addr);
    socketPair.push_back(port);

    return socketPair;
}

std::vector<std::string>getWords(const std::string &line)
{
    std::vector<std::string> words;
    std::istringstream iss(line);
    std::string word;
    while (iss >> word)
    {
        //std::cout << "word: " << word << std::endl;
        words.push_back(word);
    }
    return words;
}

template <typename T>
class Expected
{
   private:
    T _value; // Config conf // 
    std::string _message;
    bool _hasValue;
    Expected (const T& v, const std::string& msg, bool hasV): _value(v), _message(msg), _hasValue(hasV) { }

   public:
    T& value()
    { return this->_value; }
    std::string& message()
    { return this->_message; }
    bool hasValue() 
    { return this->_hasValue; }
    static Expected success(const T& v)
    {
        return Expected(v, "", true);
    }
    static Expected fail(const std::string& errorMsg)
    {
        return Expected(T(), errorMsg, false); // instant construction-destruction for the v object
    }

};

// parse listen line
//  parse listenKey
//   parse listenValue
//    parse port

bool isDigitOnly(const std::string& str)
{
    if (str.empty()) return false;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        unsigned c = static_cast<unsigned char>(*it);
        if (!std::isdigit(c))
            return false;
    }
    return true;
}

class Listen 
{
    public:
        std::vector<std::string> pair;
        std::string key;
        std::string value;

        std::string addr;
        std::string portStr;

        Listen():pair(0), key(""), value(""), addr("localhost"), portStr("80") { }
        static Expected<Listen> parser(const std::string &listenLine)
        {
            std::string listenKey = "";
            std::string listenValue = "";
            std::vector<std::string> listenPair = getWords(listenLine);
            std::string socketAddr = "";
            std::string socketPort = "";
            if (listenPair.size() != 2)
            {
                return Expected<Listen>::fail("The listen pair has not exactly 2 elements");
            }
            else 
            {
                listenKey = listenPair[0];
                listenValue = listenPair[1];
                
                std::vector<std::string> socketPair = splitSocketAddr(listenValue);
                socketAddr = socketPair[0];
                socketPort = socketPair[1]; 
            }
            Listen lst;
            lst.pair = listenPair;
            lst.key = listenKey;
            lst.value = listenValue;
            lst.addr = socketAddr;
            lst.portStr = socketPort;
            return Expected<Listen>::success(lst);
        }
};

class Config
{
    private:
        int _port;
        std::string _location;
    public:
        Config():_port(0), _location(""){ }
        int port() { return this->_port; }
        std::string location() { return this->_location; }

        static Expected<Config> parser(const std::string &filePath)
        {
            std::vector<std::string> lines = getLinesFromFile(filePath); 
            if (lines.size() < 3)
            {
                return Expected<Config>::fail("Error: missing lines");
            }
            int pPort = -1;
            std::string pLocation = "";
            size_t listenPos=std::string::npos;
            for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
            {
                //std::cout << "*it:" << *it << std::endl;
                listenPos = (*it).find("listen");
                if (listenPos != std::string::npos)
                {
                    std::cout << "Found listen keyword" << std::endl;
                    std::string listenLine = *it;
                    Expected<Listen> resLstn = Listen::parser(listenLine);
                    Listen lstn = resLstn.value();
                    std::cout << "Lstn key: " << lstn.key << std::endl;
                    std::cout << "Lstn value: " << lstn.value << std::endl;
                    std::cout << "Lstn port addr: " << lstn.addr << std::endl;
                    std::cout << "Lstn port number: " << lstn.portStr << std::endl;
                    
                    if (!isDigitOnly(lstn.portStr))
                        return Expected<Config>::fail("Port number non-all digits");
                    if (stoi(lstn.portStr) < 0 || stoi(lstn.portStr)> 49152)
                        return Expected<Config>::fail("Port number is out of range");
                    pPort = stoi(lstn.portStr);
                    break;
                }
            }
            Config cfg;
            cfg._port = pPort;
            cfg._location = pLocation;
            return Expected<Config>::success(cfg);
        }
};

int main(int argc, char* argv[])
{

	if (argc != 2) { std::cout << "need valid file as an argument\n"; return 1; }
    Expected<Config> res = Config::parser(argv[1]);

    if (!res.hasValue())
    {
	    std::cerr << "Error parsing conf file:" << res.message() << std::endl;
    }

    Config cfg = res.value();
    std::cout << "Port:" << cfg.port() << std::endl;
    std::cout << "Location:" << cfg.location() << std::endl;

    return 0;
}

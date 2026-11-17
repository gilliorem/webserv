#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/*
std::vector<std::string> tokenize(std::string& raw)
{
	std::vector<std::string> tokens;	
	
	std::string tmp;
	//std::ifstream(tmp);
	std::istringstream iss(tmp);
	std::string tok;
	while (tmp >> raw)
}
*/

static std::vector<std::string> tokenize(const std::string& raw)
{
    std::string spaced;
    for (std::size_t i = 0; i < raw.size(); ++i)
    {
        char c = raw[i];
        if (c == '{' || c == '}' || c == ';')
        {
            spaced += ' ';
            spaced += c;
            spaced += ' ';
        }
        else
            spaced += c;
    }

    std::cout << "spaced:" << spaced << std::endl;

    std::vector<std::string> tokens;
    /* in order to process the raw content */
    std::istringstream iss(spaced); // this send to the stream ? -> the string spaced in being push into the stream channel so it can be processed
    std::string tok; // individual token.
    while (iss >> tok)
        tokens.push_back(tok);
    int i = 0;
    for (auto it: tokens)
    {
	    i++;
	    if (i == 9)
	    {
		    std::cout << i << ":";
		    std::cout << "'" << it << "'" << std::endl;
	    }
	    else
	    {
		    std::cout << i << ":";
		    std::cout << "'" << it << "'" << ", " << std::endl;
	    }
    }
    return tokens;
}

int main()
{
	std::string rawOne = "General{server{location /;}}";
	std::cout << "raw: " << rawOne << std::endl;
	tokenize(rawOne);
	return 0;
	std::string raw = 
		"--//GENERAL//--    \n"
		"	--//SERVER//--	\n"
		"	root /var/www\n"
		"  location   	/\n";
	
	std::vector<std::string> tokens;
	tokens = tokenize(raw);

	for (auto it: tokens)
		std::cout << it << " ";
	std::cout << std::endl;
}

#include "utils.hpp"

std::vector<std::string> split(const std::string& content, const std::string& delimiter)
{
	std::vector<std::string> res;
	size_t start = 0;

	while (true)
	{
		size_t found = content.find(delimiter, start);
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


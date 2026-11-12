#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
# include <sstream>
#include "utils.hpp"


int sum(int a, int b)
{
	return a + b;
}

// 10+2+3

size_t findSign(const std::string& expression)
{
	if (expression.find("+") != std::string::npos || expression.find("-") != std::string::npos)
	{
		if (expression.find("+") != std::string::npos)
			return expression.find("+");
		else
			return expression.find("-");
	}
	else
		return -1;
}

std::string identifySign(const std::string& expression)
{
	if (expression.find("+") != std::string::npos || expression.find("-") != std::string::npos)
	{
		if (expression.find("+") != std::string::npos)
			return ("+");
		else
			return ("-");
	}
	else
		return "";
}

int main(int argc, char* argv[])
{
	// consume the string
	//   find sign ->
	//    split on 'sign'
	//     send the two numbers before and after the sign to the add function
	//      resize my expression

	//const std::string& expression = argv[1];
	std::string input = argv[1];
	std::string sign;
	
	std::vector<std::string> numbers;
	std::vector<std::string> signs;

	std::string expression = input;

	int res = 0;

	while (true)
	{
		std::cout << "expression:" << expression << std::endl;
		if (identifySign(expression) == "")
			break;
		if (findSign(expression) != -1)
		{
			if (identifySign(expression) == "+")
				numbers = split(expression, "+");
			else
				numbers = split(expression, "-");
			expression = expression.substr(findSign(expression));
			res += sum(stoi(numbers[0]), stoi(numbers[1]));
		}
		std::cout << "numbers:" << std::endl;
		for (std::vector<std::string>::iterator it = numbers.begin(); it != numbers.end(); ++it)
		{
			std::cout << *it << " ";
		}
	}
	std::cout << "res:" << res << std::endl;
	return 0;
}

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include "utils.hpp"

// stoai
int main(int argc, char* argv[])
{
	if (argc != 2) return 1;
	std::string expression = argv[1];

	//std::cout << "Math expression:" << expression << std::endl;

	std::vector<std::string> numbersSum;
	std::vector<std::string> numbersMinus;
	

	if (expression.find("+", 0) != std::string::npos)
		numbersSum = split(expression, "+");

	if (expression.find("-", 0) != std::string::npos)
		numbersMinus = split(expression, "-");

	std::cout << "Sum:\n";

	for (std::vector<std::string>::iterator it = numbersSum.begin(); it != numbersSum.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "Minus:\n";
	for (std::vector<std::string>::iterator it = numbersMinus.begin(); it != numbersMinus.end(); it++)
	{
		std::cout << *it << " ";
	}


}

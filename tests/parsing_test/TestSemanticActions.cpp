#include "TestSemanticActions.hpp"

void	Sum::execute(std::vector<std::string>& input, std::vector<std::string>& output) {
	for (size_t i = 0; i < input.size(); i++) {
		std::cout << input[i] << " ";
		output.push_back(input[i]);
	}
	std::cout << std::endl;
}

void	Product::execute(std::vector<std::string>& input, std::vector<std::string>& output) {
	for (size_t i = 0; i < input.size(); i++) {
		std::cout << input[i] << " ";
		output.push_back(input[i]);
	}
	std::cout << std::endl;
}

void	Number::execute(std::vector<std::string>& input, std::vector<std::string>& output) {
	for (size_t i = 0; i < input.size(); i++) {
		std::cout << input[i] << " ";
		output.push_back(input[i]);
	}
	std::cout << std::endl;
}

#include "TestSemanticActions.hpp"

void	Sum::execute(std::stack<std::string>& result) {
	std::string&	str = result.top();
	int	left;
	int	right;

	result.pop();
	if (str == "+") { 
		left = std::stoi(result.top());
		result.pop();
		right = std::stoi(result.top());
		result.pop();
		result.push(std::to_string(left + right));
	}
	else if (str == "-") { 
		left = std::stoi(result.top());
		result.pop();
		right = std::stoi(result.top());
		result.pop();
		result.push(std::to_string(left - right));
	}
}

void	Sum::execute(std::stack<std::string>& result, Token const & token) {
	(void)result;
	(void)token;
}

Sum::~Sum() {

}

// void	Product::execute(std::stack<std::string>& result) {
// 	std::string&	str = result.top();
// 	int	left;
// 	int	right;

// 	result.pop();
// 	if (str == "*") { 
// 		left = std::stoi(result.top());
// 		result.pop();
// 		right = std::stoi(result.top());
// 		result.pop();
// 		result.push(std::to_string(left * right));
// 	}
// 	else if (str == "/") { 
// 		left = std::stoi(result.top());
// 		result.pop();
// 		right = std::stoi(result.top());
// 		result.pop();
// 		result.push(std::to_string(left / right));
// 	}
// }

// void	Product::execute(std::stack<std::string>& result, Token const & token) {
// 	(void)result;
// 	(void)token;
// }

// Product::~Product() {

// }

// void	Number::execute(std::stack<std::string>& result, Token const & token) {
// 	result.push(token.value);
// }

// void	Number::execute(std::stack<std::string>& result) {
// 	(void)result;
// }

// Number::~Number() {
	
// }

// void	Factor::execute(std::stack<std::string>& result) {
// 	result.pop();
// 	result.pop();
// }

// void	Factor::execute(std::stack<std::string>& result, Token const & token) {
// 	(void)result;
// 	(void)token;
// }

// Factor::~Factor() {

// }

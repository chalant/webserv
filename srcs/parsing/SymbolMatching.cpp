#include "GrammarSymbol.hpp"

bool	SubsetSymbolMatching::match(const Token& token, const std::string& value) {
	for (size_t i = 0; i < token.value.size(); i++) {
		if (value.find_first_of(token.value[i]) == std::string::npos)
			return false;
	}
	return true;
}

bool	EqualSymbolMatching::match(const Token& token, const std::string& value) {
	return value == token.value;
}

bool	FalseSymbolMatching::match(const Token& token, const std::string& value) {
	(void)value;
	(void)token;
	return false;
}

//TODO: match ascii characters
bool	WordSymbolMatching::match(const Token& token, const std::string& value) {
	(void)value;
	(void)token;
	return false;
}

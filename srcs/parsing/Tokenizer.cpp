#include "../../includes/parsing/Tokenizer.hpp"
#include <fstream>

bool	is_separator(const std::vector<std::string>& separators, char c) {
	for (size_t i = 0; i < separators.size(); i++) {
		for (size_t j = 0; j < separators[i].length(); j++) {
			if (separators[i][j] == c)
				return true;
		}
	}
	return false;
}

//checks if the substring is a reserved symbol and returns the end index of that symbol
size_t	is_reserved(const std::vector<std::string>& reserved_symbols, const std::string& str, size_t start) {
	for (size_t i = 0; i < reserved_symbols.size(); i++) {
		size_t	end = start;
		for (size_t j = 0; j < reserved_symbols[i].length(); j++) {
			if (str[end] != reserved_symbols[i][j])
				break;
			end++;
		}
		if (end - start == reserved_symbols[i].length())
			return end;
	}
	return start;
}

//appends the token to the end of the token list.
size_t	add_reserved(std::vector<Token>& tokens, const std::vector<std::string>& separators, const std::string& str, size_t start) {
	size_t	end = is_reserved(separators, str, start);
	if (end == start)
		return start;
	tokens.push_back(Token(str.substr(start, end - start), true));
	return add_reserved(tokens, separators, str, end);
}

Token::Token(const std::string value, bool reserved):
	value(value),
	reserved(reserved) {	
}

Token& Token::operator=(const Token& other) {
	reserved = other.reserved;
	return *this;
}

Tokenizer::Tokenizer(const std::vector<std::string> separators, const std::vector<std::string> reserved_symbols):
	m_separators(separators),
	m_reserved_symbols(reserved_symbols) {
	m_tokens = std::vector<Token>();
}

Tokenizer::~Tokenizer() {
}

void	Tokenizer::makeTokens(const std::string& str) {
	size_t	start = 0;
	size_t	i = 0;

	while (i < str.length()) {
		while (is_separator(m_separators, str[i]))
			i++;
		i = add_reserved(m_tokens, m_reserved_symbols, str, i);
		start = i;
		while (i < str.length() && is_reserved(m_reserved_symbols, str, i) == i && !is_separator(m_separators, str[i]))
			i++;
		if (i - start > 0) {
			m_tokens.push_back(Token(str.substr(start, i - start), false));
		}
	}
}

const std::vector<Token>&	Tokenizer::tokenize(const std::string& str) {
	m_tokens.clear();
	makeTokens(str);
	return m_tokens;
}

//fixme: should load the whole file other-wise tokens might get unproperly split.
//todo: set buffer size in the header...
const std::vector<Token>&	Tokenizer::tokenize(std::ifstream& stream) {
	//todo: raise an error here.
	std::string	line;
	if (!stream.is_open())
		return m_tokens;
	m_tokens.clear();
	while (std::getline(stream, line)) {
		makeTokens(line);
	}
	return m_tokens;
}

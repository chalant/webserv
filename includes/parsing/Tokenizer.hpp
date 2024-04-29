#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

#include <vector>
#include <string>

struct Token
{
	const std::string	value;
	bool				reserved;
	Token(const std::string value, bool	reserved);
};

class Tokenizer
{
	private:
		const std::vector<std::string>	m_separators;
		const std::vector<std::string>	m_reserved_symbols;
		std::vector<Token>				m_tokens;
		void							makeTokens(const std::string& str);
	public:
		Tokenizer(const std::vector<std::string> separators, const std::vector<std::string> reserved_symbols);
		~Tokenizer();
		std::vector<Token> const &	tokenize(std::ifstream& stream);
		std::vector<Token> const &	tokenize(const std::string& str);
};


#endif

#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

#include <iostream>

struct Token
{
	const std::string	value;
	int					flags;
	bool				reserved;
	Token(const std::string value, int flags);
};


#endif

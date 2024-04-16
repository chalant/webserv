#ifndef	TESTSEMANTICACTIONS_HPP
# define TESTSEMANTICACTIONS_HPP

#include "SemanticAction.hpp"

class Sum: public ASemanticAction {
	public:
		void	execute(std::vector<std::string>& input, std::vector<std::string>& output);
};

class Product: public ASemanticAction {
	public:
		void	execute(std::vector<std::string>& input, std::vector<std::string>& output);
};

class Number: public ASemanticAction {
	public:
		void	execute(std::vector<std::string>& input, std::vector<std::string>& output);
};

#endif

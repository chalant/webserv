#ifndef GRAMMAR_HPP
# define GRAMMAR_HPP

#include <vector>
#include "SemanticAction.hpp"

class NonTerminalSymbol;

class GrammarRule;

class Grammar {
	private:
		std::vector<GrammarRule>	m_rules;
		size_t						m_size;
	public:
		Grammar();
		~Grammar();
		GrammarRule				*addRule(NonTerminalSymbol const & symbol);
		size_t					size() const;
		const	GrammarRule*	getRule(int index) const;
};

#endif

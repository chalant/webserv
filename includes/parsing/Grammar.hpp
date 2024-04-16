#ifndef GRAMMAR_HPP
# define GRAMMAR_HPP

#include <iostream>
#include <vector>
#include "Tokenizer.hpp"
#include "SemanticAction.hpp"
#include "GrammarRule.hpp"

class Grammar {
	private:
		std::vector<GrammarRule*>	m_rules;
	public:
		Grammar();
		~Grammar();
		void					addRule(GrammarRule* rule);
		const	GrammarRule*	getRule(int index) const;
};

#endif

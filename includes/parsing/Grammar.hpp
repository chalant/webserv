#ifndef GRAMMAR_HPP
# define GRAMMAR_HPP

#include <iostream>
#include <vector>
#include "Tokenizer.hpp"
#include "SemanticAction.hpp"

enum GrammarSymbolType {
	EMPTY,
	TERMINAL,
	NON_TERMINAL
};

class	GrammarSymbol {
	protected:
		const std::string	m_value;
		int					m_rule_idx;
		GrammarSymbolType	m_type;
	public:
		GrammarSymbol(const std::string value, GrammarSymbolType type);
		~GrammarSymbol();
		const std::string&		getValue(void) const;
		GrammarSymbolType		getType(void) const;
		int						getRuleIndex(void) const;
		void					setRuleIndex(int index);
		bool 					match(const Token& token);
};

class	GrammarRule {
	private:
		int								m_rule_idx;
		const std::string				m_name;
		std::vector<GrammarSymbol*>		m_symbols;
	public:
		GrammarRule(ASemanticAction	*action, const std::string name);
		GrammarRule(const std::string name);
		~GrammarRule();
		void					applySemanticAction(std::vector<GrammarSymbol>& symbols);
		int						getRuleIndex() const;
		const std::string&		getName() const;
		void					setRuleIndex(int rule_idx);
		void					addSymbol(GrammarSymbol *symbol);
};

class Grammar {
	private:
		std::vector<GrammarRule*> rules;
	public:
		Grammar();
		~Grammar();
		void					addRule(GrammarRule *rule);
		const	GrammarRule&	getRule(int index) const;
};

#endif

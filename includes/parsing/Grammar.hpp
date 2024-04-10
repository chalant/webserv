#ifndef ConfigurationGrammar_HPP
# define ConfigurationGrammar_HPP

#include <iostream>
#include <vector>
#include "Tokenizer.hpp"
#include "SemanticAction.hpp"

class	AGrammarSymbol {
	private:
		const std::string	m_name;
		int					m_id;
	public:
		AGrammarSymbol(const std::string name);
		const std::string&		getName(void);
		int						getID(void);
		bool					operator==(const AGrammarSymbol& other);
		virtual	bool 			match(const Token& token) = 0;
};

class	GrammarRule {
	private:
		int							rule_id;
		const std::string			name;
		std::vector<AGrammarSymbol>	symbols;
	public:
		GrammarRule(ASemanticAction& action);
		void					applySemanticAction(GrammarRule *left, GrammarRule *right);
		int						getRuleID() const;
		void					setRuleID(int rule_idx);
		const std::string&		getName() const;
		void					addSymbol(AGrammarSymbol *symbol);
};

class Grammar {
	private:
		std::vector<GrammarRule> rules;
	public:
		Grammar();
		~Grammar();
		void					addRule(GrammarRule *rule);
		const	GrammarRule&	getRule(int index) const;
};

#endif

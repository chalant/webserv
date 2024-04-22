#if !defined(NONTERMINALSYMBOL_HPP)
#define NONTERMINALSYMBOL_HPP

#include "GrammarSymbol.hpp"
#include "Grammar.hpp"

class GrammarRule;

class NonTerminalSymbol: public GrammarSymbol {
	private:
		const GrammarRuleID&	m_rule_id;
	public:
		NonTerminalSymbol(const GrammarRuleID& rule);
		bool					matchRule(const GrammarRule& rule);
		int						ruleID() const;
		const std::string&		getValue(void) const;
};

#endif

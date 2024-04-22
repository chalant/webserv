#if !defined(TERMINALSYMBOL_HPP)
#define TERMINALSYMBOL_HPP

#include "GrammarSymbol.hpp"

class TerminalSymbol: public GrammarSymbol {
	private:
		const GrammarRuleID&	m_rule_id;
		ASymbolMatching&		m_matching;
	public:
		TerminalSymbol(const GrammarRuleID& rule_id, ASymbolMatching& matching);
		bool				terminal(void) const;
		bool 				match(const Token& token) const;
		const std::string&	getValue(void) const;
		int					ruleID() const;
};

#endif

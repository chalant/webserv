#include "TerminalSymbol.hpp"

TerminalSymbol::TerminalSymbol(const GrammarRuleID& rule_id, ASymbolMatching& matching):m_rule_id(rule_id), m_matching(matching) {
}

int		TerminalSymbol::ruleID() const {
	return m_rule_id.id;
}

bool	TerminalSymbol::match(const Token& token) const {
	return m_matching.match(token, m_rule_id.name);
}

bool	TerminalSymbol::terminal(void) const {
	return true;
}

const std::string&	TerminalSymbol::getValue(void) const {
	return m_rule_id.name;
}

#include "NonTerminalSymbol.hpp"

NonTerminalSymbol::NonTerminalSymbol(const GrammarRuleID& rule_id):m_rule_id(rule_id) {
}

int	NonTerminalSymbol::ruleID() const {
	return m_rule_id.id;
}

bool	NonTerminalSymbol::matchRule(const GrammarRule& rule) {
	return m_rule_id.id == rule.ruleID();
}

const std::string&	NonTerminalSymbol::getValue(void) const {
	return m_rule_id.name;
}

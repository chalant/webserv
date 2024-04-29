#include "GrammarRule.hpp"

GrammarRuleID::GrammarRuleID(const std::string name, int id): name(name), id(id) {
}

bool	GrammarRuleID::operator==(const GrammarRuleID& other) {
	return other.id == id;
}

GrammarRule::GrammarRule(GrammarRuleID& rule_id): m_rule_id(rule_id) {
}

GrammarRule::~GrammarRule() {
}

int	GrammarRule::getRuleIndex() const {
	return m_rule_idx;
}

const std::string&	GrammarRule::getName() const {
	return m_rule_id.name;
}

void	GrammarRule::setRuleIndex(int rule_idx) {
	m_rule_idx = rule_idx;
}

void	GrammarRule::addSymbol(GrammarSymbol* symbol) {
	m_symbols.push_back(symbol);
}

bool	GrammarRule::operator==(const GrammarRule& other) {
	return m_rule_id == other.m_rule_id && m_rule_idx == other.m_rule_idx;
}

GrammarSymbol*	GrammarRule::getSymbol(int index) const {
	if (static_cast<size_t>(index) >= m_symbols.size())
		return NULL;
	return	m_symbols[index];
}

int	GrammarRule::ruleID(void) const {
	return m_rule_id.id;
}

size_t	GrammarRule::size(void) const {
	return m_symbols.size();
}

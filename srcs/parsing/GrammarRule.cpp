#include "GrammarRule.hpp"

GrammarRuleID::GrammarRuleID(const std::string name, int id): name(name), id(id) {
}

GrammarRule::GrammarRule(ASemanticAction& action, GrammarRuleID& rule_id): m_action(action), m_rule_id(rule_id) {
}

GrammarRule::~GrammarRule() {

}

const ASemanticAction&	GrammarRule::getSemanticAction() const {
	return m_action;
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

GrammarSymbol*	GrammarRule::getSymbol(int index) const {
	return	m_symbols[index];
}

size_t	GrammarRule::size() const {
	return m_symbols.size();
}
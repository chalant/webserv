#include "Grammar.hpp"

Grammar::Grammar() {
	m_size = 0;
}

Grammar::~Grammar() {
}

GrammarRule*	Grammar::addRule(NonTerminalSymbol const & symbol) {
	m_size++;
	m_rules.push_back(GrammarRule(symbol));
	return &m_rules[m_rules.size() - 1];
}

const GrammarRule	*Grammar::getRule(int index) const {
	return &m_rules[index];
}
size_t	Grammar::size() const {
	return m_size;
}

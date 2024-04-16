#include "Grammar.hpp"

Grammar::Grammar() {
}

Grammar::~Grammar() {
}

void	Grammar::addRule(GrammarRule* rule) {
	m_rules.push_back(rule);
}

const	GrammarRule*	Grammar::getRule(int index) const {
	return m_rules.at(index);
}

size_t	Grammar::size() const {
	return m_size;
}

#include "Grammar.hpp"
#include "GrammarSymbol.hpp"

// GrammarSymbol::GrammarSymbol(std::string const value, GrammarSymbolType type, ASymbolMatching& matching):m_value(value), m_type(type), m_matching(matching) {
// 	m_rule_idx = 0;
// }

GrammarSymbol::~GrammarSymbol() {

}

int	GrammarSymbol::getRuleIndex() const {
	return m_rule_idx;
}

void	GrammarSymbol::setRuleIndex(int index) {
	m_rule_idx = index;
}

bool	GrammarSymbol::match(const Token& token) const {
	(void)token;
	return false;
}

bool	GrammarSymbol::terminal(void) const {
	return false;
}

bool	GrammarSymbol::matchRule(const GrammarRule& rule) {
	(void)rule;
	return false;
}

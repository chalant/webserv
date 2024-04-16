#include "Grammar.hpp"
#include "GrammarSymbol.hpp"

GrammarSymbol::GrammarSymbol(const std::string value, GrammarSymbolType type, ASymbolMatching& matching):m_value(value), m_type(type), m_matching(matching) {
	m_rule_idx = 0;
}

GrammarSymbol::~GrammarSymbol() {

}

const std::string&	GrammarSymbol::getValue() const {
	return m_value;
}

GrammarSymbolType	GrammarSymbol::getType() const {
	return m_type;
}

int	GrammarSymbol::getRuleIndex() const {
	return m_rule_idx;
}

void	GrammarSymbol::setRuleIndex(int index) {
	m_rule_idx = index;
}

bool	GrammarSymbol::match(const Token& token) {
	return m_matching.match(token, m_value);
}

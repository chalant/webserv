#include "Grammar.hpp"

GrammarSymbol::GrammarSymbol(const std::string value, GrammarSymbolType type) {

}

GrammarSymbol::~GrammarSymbol() {

}

const std::string&	GrammarSymbol::getValue() const {
	return m_value;
}

GrammarSymbolType	GrammarSymbol::getType() const {

}

int	GrammarSymbol::getRuleIndex() const {

}

void	GrammarSymbol::setRuleIndex(int index) {

}

bool	GrammarSymbol::match(const Token& token) {
	
}

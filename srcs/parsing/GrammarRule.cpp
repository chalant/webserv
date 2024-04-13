#include "Grammar.hpp"

GrammarRule::GrammarRule(ASemanticAction* action, const std::string name) {

}

GrammarRule::~GrammarRule() {

}

void	GrammarRule::addSymbol(GrammarSymbol *symbol) {

}

const std::string&	GrammarRule::getName() const {

}

int	GrammarRule::getRuleIndex() const {

}

void	GrammarRule::setRuleIndex(int rule_idx) {

}

void	GrammarRule::applySemanticAction(std::vector<GrammarSymbol>& symbols) {

}

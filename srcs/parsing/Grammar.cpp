#include "parsing/Grammar.hpp"

Grammar::Grammar() {
	std::vector<GrammarRule*>	rules;
}

Grammar::~Grammar() {
	for (size_t	i = 0; i < rules.size(); i++) {
		delete rules[i];
	}
}

void	Grammar::addRule(GrammarRule *rule) {
	rules.push_back(rule);
}

const GrammarRule&	Grammar::getRule(int index) const {
	return	*rules[index];
}

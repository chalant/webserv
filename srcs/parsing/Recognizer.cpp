#include "parsing/Parser.hpp"

bool	contains_item(std::vector<EarleyItem>& set, EarleyItem& item) {
	for (size_t i = 0; i < set.size(); i++) {
		if (set[i] == item) {
			return true;
		}
	}
	return false;
}

AGrammarSymbol	*next_symbol(const Grammar& grammar, EarleyItem& item) {
	const GrammarRule*	rule;

	rule = grammar.getRule(item.ruleIndex());
	if (static_cast<size_t>(item.next()) >= rule->size())
		return (NULL);
	return grammar.getRule(item.ruleIndex())->getSymbol(item.next());
}

Recognizer::Recognizer() {
	// m_sets = std::vector<std::vector<EarleyItem> >();
	m_state_idx = 0;
	m_symbol = NULL;
}

Recognizer::~Recognizer() {

}

void	Recognizer::scan(std::vector<std::vector<EarleyItem> >& sets, Token const & token, EarleyItem const & item) {
	if (m_symbol->match(token)) {
		EarleyItem	next_item(item.ruleIndex(), item.start(), item.next() + 1);
		sets[m_state_idx + 1].push_back(next_item);
	}
}

void	Recognizer::complete(Grammar const & grammar, std::vector<std::vector<EarleyItem> >& sets, std::vector<EarleyItem>& current_set, int item_index) {
	EarleyItem		*old_item;
	EarleyItem		*current_item;
	AGrammarSymbol	*next;

	current_item = &current_set[item_index];
	for (size_t i = 0; i < sets[current_item->start()].size(); i++) {
		current_item->completed(true);
		old_item = &sets[current_item->start()][i];
		next = next_symbol(grammar, *old_item);
		if (next && next->matchRule(*grammar.getRule(current_item->ruleIndex()))) {
			EarleyItem	new_item(old_item->ruleIndex(), old_item->start(), old_item->next() + 1);
			if (!contains_item(current_set, new_item)) {
				current_set.push_back(new_item);
				current_item = &current_set[item_index];
			}
		}
	}
}

void	Recognizer::predict(const Grammar& grammar, std::vector<EarleyItem>& current_set) {
	for (size_t i = 0; i < grammar.size(); i++) {
		if (m_symbol->matchRule(*grammar.getRule(i))) {
			EarleyItem	item(i, m_state_idx, 0);
			if (!contains_item(current_set, item)) {
				current_set.push_back(item);
			}
		}
	}
}

void	Recognizer::recognize(std::vector<Token> const & tokens, Grammar const & grammar, std::vector<std::vector<EarleyItem> >& sets) {
	std::vector<EarleyItem>	*current_set;
	EarleyItem				*current_item;
	//initialize sets to the number of tokens + 1.
	sets.clear();
	for (size_t i = 0; i < tokens.size() + 1; i++) {
		sets.push_back(std::vector<EarleyItem>());
	}
	// populate first set.
	for (size_t i = 0; i < grammar.size(); i++) {
		if (grammar.getRule(0)->getName() == grammar.getRule(i)->getName()) {
			sets[0].push_back(EarleyItem(i, 0, 0));
		}
	}
	// populate the rest of the sets.
	for (size_t i = 0; i < tokens.size() + 1; i++) {
		current_set = &sets[i];
		m_state_idx = i;
		for (size_t j = 0; j < current_set->size(); j++) {
			current_item = &(*current_set)[j];
			m_symbol = next_symbol(grammar, *current_item);
			if (m_symbol == NULL) {
				complete(grammar, sets, sets[i], j);
			}
			else if (m_symbol->terminal() && static_cast<size_t>(i) < tokens.size()) {
				scan(sets, tokens[i], *current_item);
			}
			else {
				predict(grammar, sets[i]);
			}
		}
	}
}

void	Recognizer::print(Grammar const & grammar, std::vector<std::vector<EarleyItem> >& sets) {
	EarleyItem			*item;
	const GrammarRule	*rule;

	for (size_t i = 0; i < sets.size(); i++) {
		std::cout << "===========" << "(" << i << ")"<< "===========" << std::endl << std::endl;
		for (size_t j = 0; j < sets[i].size(); j++) {
			item = &sets[i][j];
			rule = grammar.getRule(item->ruleIndex());
			std::cout << rule->getName() << " -> ";
			for (int k = 0; k < static_cast<int>(rule->size()) + 1; k++) {
				if (k == item->next())
					std::cout << "• ";
				if (k != static_cast<int>(rule->size()))
					std::cout << rule->getSymbol(k)->getValue() << " ";
			}
			std::cout << "(" << item->start() << ")" << std::endl;
		}
		std::cout << std::endl;
	}
}

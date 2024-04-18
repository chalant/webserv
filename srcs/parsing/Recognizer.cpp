#include "Parser.hpp"

bool	contains_item(std::vector<EarleyItem>& set, EarleyItem& item) {
	for (size_t i = 0; i < set.size(); i++) {
		if (set[i] == item) {
			return true;
		}
	}
	return false;
}

GrammarSymbol	*next_symbol(const Grammar& grammar, EarleyItem& item) {
	const GrammarRule*	rule;

	rule = grammar.getRule(item.ruleIndex());
	if (static_cast<size_t>(item.next()) >= rule->size())
		return (NULL);
	return grammar.getRule(item.ruleIndex())->getSymbol(item.next());
}

Recognizer::Recognizer(const Grammar& grammar):m_grammar(grammar) {
	m_sets = std::vector<std::vector<EarleyItem> >();
	m_item = NULL;
}

Recognizer::~Recognizer() {

}

void	Recognizer::scan() {
	EarleyItem		*current_item;

	current_item = &m_set->at(m_j);
	if (m_symbol->match(*m_token)) {
		std::cout << "Match " << m_token->value << std::endl;
		EarleyItem	item(current_item->ruleIndex(), current_item->start(), current_item->next() + 1);
		m_sets[m_state_idx + 1].push_back(item);
	}
}

void	Recognizer::complete() {
	EarleyItem		*old_item;
	EarleyItem		*current_item;
	GrammarSymbol	*next;

	current_item = &m_set->at(m_j);
	for (size_t i = 0; i < m_sets[current_item->start()].size(); i++) {
		current_item->completed(true);
		old_item = &m_sets[current_item->start()][i];
		next = next_symbol(m_grammar, *old_item);
		if (next->getValue() == m_grammar.getRule(current_item->ruleIndex())->getName()) {
			EarleyItem	new_item(old_item->ruleIndex(), old_item->start(), old_item->next() + 1);
			if (!contains_item(*m_set, new_item)) {
				m_set->push_back(new_item);
			}
		}
		current_item = &m_set->at(m_j);
	}
}

void	Recognizer::predict() {
	for (size_t i = 0; i < m_grammar.size(); i++) {
		if (m_symbol->getValue() == m_grammar.getRule(i)->getName()) {
			EarleyItem	item(i, m_state_idx, 0);
			if (!contains_item(*m_set, item)) {
				m_set->push_back(item);
			}
		}
	}
}

void	Recognizer::recognize(std::vector<Token>& tokens) {
	//initialize sets to the number of tokens.
	m_sets.clear();
	for (size_t i = 0; i < tokens.size() + 1; i++) {
		m_sets.push_back(std::vector<EarleyItem>());
	}
	// populate first set.
	for (size_t i = 0; i < m_grammar.size(); i++) {
		if (m_grammar.getRule(0)->getName() == m_grammar.getRule(i)->getName()) {
			m_sets[0].push_back(EarleyItem(i, 0, 0));
		}
	}
	// populate the rest of the sets.
	for (size_t i = 0; i < tokens.size() + 1; i++) {
		m_set = &m_sets[i];
		m_state_idx = i;
		m_token = &tokens[i];
		for (size_t j = 0; j < m_set->size(); j++) {
			m_j = j;
			m_item = &m_set->at(j);
			m_symbol = next_symbol(m_grammar, *m_item);
			if (m_symbol == NULL) {
				complete();
			}
			else if (m_symbol->getType() == TERMINAL) {
				scan();
			}
			else if (m_symbol->getType() == NON_TERMINAL) {
				predict();
			}
		}
	}
}

void	Recognizer::print() {
	EarleyItem			*item;
	const GrammarRule	*rule;

	for (size_t i = 0; i < m_sets.size(); i++) {
		std::cout << "===========" << "(" << i << ")"<< "===========" << std::endl << std::endl;
		for (size_t j = 0; j < m_sets[i].size(); j++) {
			item = &m_sets[i][j];
			rule = m_grammar.getRule(item->ruleIndex());
			std::cout << rule->getName() << " -> ";
			for (int k = 0; k < (int)rule->size(); k++) {
				if (k == item->next())
					std::cout << "• ";
				if (k != (int)rule->size())
					std::cout << rule->getSymbol(k)->getValue() << " ";
			}
			std::cout << "(" << item->start() << ")" << std::endl;
		}
		std::cout << std::endl;
	}
}

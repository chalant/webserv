#include "Parser.hpp"

bool	contains_item(std::vector<EarleyItem>& set, EarleyItem& item) {
	for (size_t i = 0; i < set.size(); i++) {
		if (set[i] == item)
			return true;
	}
	return false;
}

GrammarSymbol	*next_symbol(const Grammar& grammar, EarleyItem& item) {
	return grammar.getRule(item.getRuleIndex())->getSymbol(item.getNext());
}

Recognizer::Recognizer(const Grammar& grammar):m_grammar(grammar) {
	
}

Recognizer::~Recognizer() {

}

void	Recognizer::scan() {
	if (m_symbol->match(*m_token)) {
		EarleyItem	item(m_item->getNext() + 1, m_item->getStart(), m_item->getRuleIndex());
		m_sets[m_state_idx + 1].push_back(item);
	}
}

void	Recognizer::complete() {
	EarleyItem		*old_item;
	GrammarSymbol	*next;

	m_item->setCompleted(true);
	for (size_t i = 0; i < m_sets[m_item->getStart()].size(); i++) {
		old_item = &m_sets[m_item->getStart()][i];
		next = next_symbol(m_grammar, *old_item);
		if (next->getValue() == m_grammar.getRule(i)->getName()) {
			EarleyItem	new_item(old_item->getRuleIndex(), old_item->getStart(), old_item->getNext() + 1);
			if (!contains_item(*m_set, new_item)) {
				m_set->push_back(new_item);
			}
		}
	}
}

void	Recognizer::predict() {
	for (size_t i = 0; i < m_grammar.size(); i++) {
		if (m_symbol->getValue() == m_grammar.getRule(i)->getName()) {
			EarleyItem	item(i, 0, m_state_idx);
			if (!contains_item(*m_set, item))
				m_set->push_back(item);
		}
	}
}

void	Recognizer::recognize(std::vector<Token*>& tokens) {
	std::vector<EarleyItem> *set;
	GrammarSymbol			*m_symbol;
	//todo: initialize set first

	for (size_t i = 0; i < tokens.size(); i++) {
		m_set = &m_sets[i];
		m_state_idx = i;
		for (size_t j = 0; j < m_set->size(); j++) {
			m_item = &m_set->at(j);
			m_symbol = next_symbol(m_grammar, *m_item);
			if (m_symbol->getType() == TERMINAL)
				scan();
			else if (m_symbol->getType() == NON_TERMINAL)
				predict();
			else if (m_symbol->getType() == EMPTY)
				complete();
		}
	}
}

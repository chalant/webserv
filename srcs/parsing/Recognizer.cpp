#include "Parser.hpp"



Recognizer::Recognizer(const Grammar& grammar):m_grammar(grammar) {
}

Recognizer::~Recognizer() {

}

void	Recognizer::scan() {

}

void	Recognizer::complete() {

}

void	Recognizer::predict() {

}

void	Recognizer::recognize(std::vector<Token*>& tokens) {
	std::vector<EarleyItem> *set;
	EarleyItem				*item;
	GrammarSymbol			*m_symbol;
	//todo: initialize set first

	for (size_t i = 0; i < tokens.size(); i++) {
		set = &m_sets[i];
		m_state_idx = i;
		for (size_t j = 0; j < set->size(); j++) {
			item = &set->at(j);
			m_symbol = m_grammar.getRule(item->getRuleIndex())->getSymbol(item->getNext());
			if (m_symbol->getType() == TERMINAL)
				scan();
			else if (m_symbol->getType() == NON_TERMINAL)
				predict();
			else if (m_symbol->getType() == EMPTY)
				complete();
		}
	}
}

#include "ParseTree.hpp"

ParseTree::ParseTree(void) {
	m_subtrees = std::vector<ParseTree>();
	m_start = 0;
	m_end = 0;
	m_is_leaf = false;
	m_rule_idx = 0;
	m_rule_id = 0;
}

ParseTree::~ParseTree(void) {
}

void	ParseTree::addSubtree(int start, int end, GrammarRule& rule) {
}

void	ParseTree::setSpan(int start, int end) {
	m_start = start;
	m_end = end;
}

int	ParseTree::start(void) const {
	return m_start;
}

int	ParseTree::end(void) const {
	return m_end;
}

ParseTree&	ParseTree::operator[](int index) {
	return m_subtrees[index];
}

bool		ParseTree::leaf(void) const {
	return m_is_leaf;
}

void	ParseTree::leaf(bool value) {
	m_is_leaf = value;
}

void	ParseTree::ruleIndex(int value) {
	m_rule_idx = value;
}

int		ParseTree::ruleIndex(void) const {
	return m_rule_idx;
}

int		ParseTree::ruleID(void) const {
	return m_rule_id;
}

void	ParseTree::ruleID(int value) {
	m_rule_id = value;
}

size_t	ParseTree::size() const {
	return m_subtrees.size();
}

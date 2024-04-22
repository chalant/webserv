#include "Parser.hpp"
#include "ParseTree.hpp"

//inverts the earley sets and turns them into a graph, that will be used to guide the parsing tree building
//processs.
void	buildChart(std::vector<std::vector<EarleyEdge> >& chart, std::vector<std::vector<EarleyItem> >& sets) {
	EarleyItem	*item;

	chart.reserve(sets.size());
	for (size_t i = 0; i < sets.size(); i++) {
		for (size_t j = 0; j < sets[i].size(); j++) {
			item = &sets[i][j];
			chart[item->start()].push_back(EarleyEdge(*item, i));
		}
	}
}

//looks for the longest edge and sets it to be the starting point of our tree.
void	setRootNode(ParseTree& tree, const Grammar& grammar, std::vector<std::vector<EarleyEdge> >& chart) {
	EarleyEdge	*longest = &chart[0][0];

	for (size_t i = 0; i < chart[0].size(); i++) {
		if (chart[0][i].end() >= longest->end()) {
			longest = &chart[0][i];
		}
	}
	tree.ruleID(grammar.getRule(longest->ruleIndex())->ruleID());
	tree.setSpan(longest->start(), longest->end());
}

EarleyEdge::EarleyEdge(EarleyItem const & item, int end):m_item(item), m_end(end)  {
}

int	EarleyEdge::start() const {
	return m_item.start();
}

int	EarleyEdge::end() const {
	return m_end;
}

int	EarleyEdge::ruleIndex() const {
	return m_item.ruleIndex();
}

SearchState::SearchState(int rule_index, int depth, int node): rule_index(rule_index), depth(depth), node(node) {
}

Parser::Parser(Grammar const & grammar): m_grammar(grammar) {
	m_earley_sets = std::vector<std::vector<EarleyItem> >();
	m_chart = std::vector<std::vector<EarleyEdge> >();
	m_recognizer = Recognizer();
}

bool	Parser::m_processTerminal(ParseTree& parse_tree, SearchState state, const GrammarSymbol& symbol, const std::vector<Token>& tokens) {
	if (state.node >= parse_tree.end())
		return false;
	if (!symbol.match(tokens[state.node]))
		return false;
	//successful terminal match -> todo: perform semantic action here
	parse_tree[state.depth].setSpan(state.node, state.node);
	//if we've reached the bounds of the span, search is finished.
	if (state.node >= parse_tree.end())
		return true;
	//otherwise, we advance one step in the symbols and the input and continue the search.
	return m_searchPath(parse_tree, SearchState(state.rule_index, state.depth + 1, state.node + 1), tokens);
}

bool	Parser::m_searchPath(ParseTree& parse_tree, SearchState state, const std::vector<Token>& tokens) {
	GrammarSymbol	*symbol;

	symbol = m_grammar.getRule(state.rule_index)->getSymbol(state.depth);
	if (symbol->terminal())
		return m_processTerminal(parse_tree, state, *symbol, tokens);
	std::vector<EarleyEdge>&	edges = m_chart[state.node];
	//for each edge, perform searches for rules that match the symbol.
	for (size_t i = 0; i < edges.size(); i++) {
		if (m_grammar.getRule(edges[i].ruleIndex())->ruleID() != symbol->ruleID())
			continue;
		//advance one step in the symbols and perform a new search.
		SearchState	new_state(state.rule_index, state.depth + 1, edges[i].end());
		//if the search successful, add subtree  -> could perform semantic actions here ?
		if (m_searchPath(parse_tree, new_state, tokens)) {
		}
		//sub-parse was completed
		if (new_state.node == parse_tree.end())
			return true;
	}
	return false;
}

//recursively builds the parse tree using deapth-first search.
void	Parser::m_buildTree(ParseTree& parse_tree, const std::vector<Token>& tokens) {
	std::vector<EarleyEdge>&	edges = m_chart[parse_tree.start()];
	EarleyEdge					*edge;

	//reached leaf 
	if (parse_tree.start() == parse_tree.end())
		return ;
	for (size_t i = 0; i < edges.size(); i++) {
		edge = &edges[i];
		if (edge->end() == parse_tree.end() &&
			m_grammar.getRule(edge->ruleIndex())->ruleID() == parse_tree.ruleID()) {
				parse_tree.ruleIndex(edge->ruleIndex());
			// if a successful path was found, stop search.
			if (m_searchPath(parse_tree, SearchState(edge->ruleIndex(), 0, parse_tree.start()), tokens))
				break ;
		}
	}
	//recursively perform searches on subtrees...
	for (size_t i = 0; i < parse_tree.size(); i++) {
		m_buildTree(parse_tree[i], tokens);
	}
	//todo: apply semantic actions here.
}

void	Parser::parse(std::vector<Token> const & tokens) {
	ParseTree	parse_tree;

	//the recognizer should throw an exception if a parse is not possible. (syntax error)
	m_recognizer.recognize(tokens, m_grammar, m_earley_sets);
	buildChart(m_chart, m_earley_sets);
	setRootNode(parse_tree, m_grammar, m_chart);
	m_buildTree(parse_tree, tokens);
}

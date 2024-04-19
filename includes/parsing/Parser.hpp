#ifndef PARSER_HPP
# define PARSER_HPP

#include <vector>
#include "Grammar.hpp"

struct EarleyEdge {
	int	start;
	int	end;
	int	rule_idx;
};

class EarleyItem {
	private:
		int		m_rule_idx;
		int		m_start;
		int		m_next;
		bool	m_completed;
	public:
		EarleyItem(int rule_idx, int start, int next);
		bool	operator==(const EarleyItem& other);
		int		ruleIndex() const;
		int		start() const;
		int		next() const;
		bool	completed() const;
		void	completed(bool value);
};

struct ParseTree {
	int		start;
	int		end;
	bool	is_leaf;
	std::vector<ParseTree>	subtrees;
};

class Recognizer {
	private:
		int						m_state_idx;
		GrammarSymbol			*m_symbol;
		void			scan(std::vector<std::vector<EarleyItem> >& sets, Token const & token, EarleyItem const & item);
		void			complete(Grammar const & grammar, std::vector<std::vector<EarleyItem> >& sets, std::vector<EarleyItem>& current_set, int item_index);
		void			predict(Grammar const & grammar, std::vector<EarleyItem>& current_set);
	public:
		Recognizer();
		~Recognizer();
		void			recognize(std::vector<Token> const & tokens, Grammar const & grammar, std::vector<std::vector<EarleyItem> >& sets);
		void			print(Grammar const & grammar, std::vector<std::vector<EarleyItem> >& sets);
};

class Parser {
	private:
		std::vector<std::vector<EarleyItem> >	m_earley_sets;
		std::vector<std::vector<EarleyEdge> >	m_chart;
		Recognizer								m_recognizer;
		ParseTree								m_parse_tree;
	public:
		Parser();
		void			parse(const Grammar& grammar);
};

#endif

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
		int		getRuleIndex() const;
		int		getStart() const;
		int		getNext() const;
		int		isCompleted() const;
		void	setCompleted(bool value);
};

struct ParseTree {
	int		start;
	int		end;
	bool	is_leaf;
	std::vector<ParseTree>	subtrees;
};

class Recognizer {
	private:
		const Grammar&	m_grammar;
		Token			*m_token;
		GrammarSymbol	*m_symbol;
		int				m_state_idx;
		std::vector<std::vector<EarleyItem>>	m_sets;
		void			scan(void);
		void			complete(void);
		void			predict(void);
	public:
		Recognizer(const Grammar& grammar);
		~Recognizer();
		void			recognize(std::vector<Token*>& tokens);
};

class EarleyParser {
	private:
		std::vector<std::vector<EarleyItem>>	m_earley_sets;
		std::vector<std::vector<EarleyEdge>>	m_chart;
		Recognizer								m_recognizer;
		ParseTree								m_parse_tree;
	public:
		void			parse(const Grammar& grammar);
};

#endif

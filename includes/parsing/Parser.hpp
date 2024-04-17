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
		const Grammar&	m_grammar;
		int				m_state_idx;
		std::vector<std::vector<EarleyItem> >	m_sets;
		// variables.
		std::vector<EarleyItem>	*m_set;
		Token			*m_token;
		GrammarSymbol	*m_symbol;
		EarleyItem		*m_item;
		void			scan(void);
		void			complete(void);
		void			predict(void);
	public:
		Recognizer(const Grammar& grammar);
		~Recognizer();
		void			recognize(std::vector<Token>& tokens);
		void			print(void);
};

class EarleyParser {
	private:
		std::vector<std::vector<EarleyItem> >	m_earley_sets;
		std::vector<std::vector<EarleyEdge> >	m_chart;
		Recognizer								m_recognizer;
		ParseTree								m_parse_tree;
	public:
		void			parse(const Grammar& grammar);
};

#endif

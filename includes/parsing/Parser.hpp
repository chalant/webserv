#ifndef IConfigurationPARSER_HPP
# define IConfigurationPARSER_HPP

#include <vector>
#include "Grammar.hpp"

struct EarleyEdge {
	int	start;
	int	end;
	int	rule_idx;
};

struct EarleyItem {
	int		rule_idx;
	int		start;
	int		next;
	bool	completed;
};

struct ParseTree {
	std::vector<ParseTree>	subtrees;
};


class EarleyRecognizer {
	private:
		Grammar	grammar;
		void	scan(void);
		void	complete(void);
		void	predict(void);
	public:
		EarleyRecognizer(const Grammar& grammar);
		void	recognize(std::vector<std::vector<EarleyItem>> &sets);
};

class EarleyParser {
	private:
		std::vector<std::vector<EarleyItem>>	m_earley_sets;
		std::vector<std::vector<EarleyEdge>>	m_chart;
		EarleyRecognizer						m_recognizer;
		ParseTree								m_parse_tree;
	public:
		EarleyParser(const Grammar& grammar);
		void	parse();
};

#endif

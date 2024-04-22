#if !defined(PARSETREE_HPP)
#define PARSETREE_HPP

#include <vector>
#include <string>
#include "GrammarRule.hpp"

class EarleyEdge;

class ParseTree {
	private:
		int						m_rule_id;
		bool					m_is_leaf;
		int						m_start;
		int						m_end;
		int						m_rule_idx;
		std::vector<ParseTree>	m_subtrees;
	public:
		ParseTree(void);
		~ParseTree(void);
		ParseTree& operator[](int index);
		void	setSpan(int start, int end);
		int		start(void) const;
		int		end(void) const;
		int		ruleIndex(void) const;
		void	ruleIndex(int value);
		bool	leaf(void) const;
		void	leaf(bool value);
		void	addSubtree(int start, int end, GrammarRule& rule);
		int		ruleID(void) const;
		void	ruleID(int id);
		size_t	size(void) const;
};

#endif

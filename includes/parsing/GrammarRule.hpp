#if !defined(GRAMMARRULE_HPP)
#define GRAMMARRULE_HPP

#include "GrammarRule.hpp"
#include "SemanticAction.hpp"
#include "GrammarSymbol.hpp"

struct GrammarRuleID {
	const std::string	name;
	int					id;
	GrammarRuleID(const std::string name, int id);
};

class	GrammarRule {
	private:
		int								m_rule_idx;
		ASemanticAction					&m_action;
		GrammarRuleID&					m_rule_id;
		std::vector<GrammarSymbol*>		m_symbols;
	public:
		GrammarRule(ASemanticAction &action, GrammarRuleID& rule_id);
		~GrammarRule();
		const	ASemanticAction&	getSemanticAction() const;
		int							getRuleIndex() const;
		const std::string&			getName() const;
		void						setRuleIndex(int rule_idx);
		void						addSymbol(GrammarSymbol *symbol);
		GrammarSymbol*				getSymbol(int index) const;
		size_t						size() const;
};

#endif

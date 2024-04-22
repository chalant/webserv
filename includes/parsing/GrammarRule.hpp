#if !defined(GRAMMARRULE_HPP)
#define GRAMMARRULE_HPP

#include "GrammarRule.hpp"
#include "SemanticAction.hpp"
#include "GrammarSymbol.hpp"

class GrammarSymbol;

struct GrammarRuleID {
	const std::string	name;
	int					id;
	GrammarRuleID(const std::string name, int id);
	bool				operator==(const GrammarRuleID& other);
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
		const	ASemanticAction&	getSemanticAction(void) const;
		int							getRuleIndex(void) const;
		int							ruleID(void) const;
		const std::string&			getName(void) const;
		void						setRuleIndex(int rule_idx);
		void						addSymbol(GrammarSymbol *symbol);
		GrammarSymbol*				getSymbol(int index) const;
		bool						operator==(const GrammarRule& other);
		size_t						size(void) const;
};

#endif

#if !defined(GRAMMARSYMBOL_HPP)
#define GRAMMARSYMBOL_HPP

#include "Tokenizer.hpp"
#include "Grammar.hpp"

struct GrammarRuleID;

class GrammarRule;

enum GrammarSymbolType {
	TERMINAL,
	NON_TERMINAL
};

class ASymbolMatching {
	public:
		virtual	bool	match(const Token& token, const std::string& value) = 0;
};

class EqualSymbolMatching: public ASymbolMatching {
	public:
		bool	match(const Token& token, const std::string& value);
};

class SubsetSymbolMatching: public ASymbolMatching {
	public:
		bool	match(const Token& token, const std::string& value);
};

class FalseSymbolMatching: public ASymbolMatching {
	public:
		bool	match(const Token& token, const std::string& value);
};

class WordSymbolMatching: public ASymbolMatching {
	public:
		bool	match(const Token& token, const std::string& value);
};

class GrammarSymbol {
	private:
		int	m_rule_idx;
	public:
		virtual						~GrammarSymbol() = 0;
		int							getRuleIndex(void) const;
		void						setRuleIndex(int index);
		virtual bool 				match(const Token& token) const;
		virtual bool				matchRule(GrammarRule const & rule);
		virtual	bool				terminal(void) const;
		virtual	int					ruleID() const = 0;
		virtual const std::string&	getValue(void) const = 0;
};

#endif

#if !defined(GRAMMARSYMBOL_HPP)
#define GRAMMARSYMBOL_HPP

#include "Tokenizer.hpp"

struct GrammarRuleID;

enum GrammarSymbolType {
	EMPTY,
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
		std::string			m_value;
		int					m_rule_idx;
		GrammarSymbolType	m_type;
		ASymbolMatching&	m_matching;
	public:
		GrammarSymbol(const std::string value, GrammarSymbolType type, ASymbolMatching& matching);
		~GrammarSymbol();
		const std::string&		getValue(void) const;
		GrammarSymbolType		getType(void) const;
		int						getRuleIndex(void) const;
		void					setRuleIndex(int index);
		bool 					match(const Token& token);
};

#endif

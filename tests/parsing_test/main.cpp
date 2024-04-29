#include "Grammar.hpp"
#include "Parser.hpp"
#include "TestSemanticActions.hpp"
#include "TerminalSymbol.hpp"
#include "NonTerminalSymbol.hpp"

int	main(void) {
	Grammar	arithmetic;
	// Sum			sum_action;
	// Product		product_action;
	// Factor		factor_action;
	// Number									number_action;
	// RelayAction<std::stack<std::string> >	relay;
	// std::vector<ASemanticAction<std::stack<std::string> > > sem_actions;
	//GrammarRuleBuilder	builder;

	//rule_id = builder.createNonTerminalSymbol("sum");
	//rule_id.addRule();
	//rule_id.addRule();
	//rule_id.addTerminalSymbol()
	//builder.addRule("sum");
	//builder.addRule()

	SubsetSymbolMatching	subset_matching;
	EqualSymbolMatching		equal_matching;

	NonTerminalSymbol	sum("sum",0);
	NonTerminalSymbol	product("product",1);
	NonTerminalSymbol	factor("factor",2);
	NonTerminalSymbol	num("number",7);
	TerminalSymbol		pm("+-",3,subset_matching);
	TerminalSymbol		md("*/",4,subset_matching);
	TerminalSymbol		open_par("(",5,subset_matching);
	TerminalSymbol		close_par(")",6,subset_matching);
	TerminalSymbol		numbers("0123456789",8,subset_matching);

	// sem_actions.push_back(sum_action);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(product_action);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(factor_action);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(number_action);

	GrammarRule	*rule = arithmetic.addRule(sum);
	rule->addSymbol(&sum);
	rule->addSymbol(&pm);
	rule->addSymbol(&product);
	
	rule = arithmetic.addRule(sum);
	rule->addSymbol(&product);
	
	rule = arithmetic.addRule(product);
	rule->addSymbol(&product);
	rule->addSymbol(&md);
	rule->addSymbol(&factor);
	
	rule = arithmetic.addRule(product);
	rule->addSymbol(&factor);
	
	rule = arithmetic.addRule(factor);
	rule->addSymbol(&open_par);
	rule->addSymbol(&sum);
	rule->addSymbol(&close_par);
	
	rule = arithmetic.addRule(factor);
	rule->addSymbol(&num);
	
	rule = arithmetic.addRule(num);
	rule->addSymbol(&numbers);
	rule->addSymbol(&num);
	
	rule = arithmetic.addRule(num);
	rule->addSymbol(&numbers);
	
	// arithmetic.addRule(&sum2);
	// arithmetic.addRule(&rule);
	// arithmetic.addRule(&product2);
	// arithmetic.addRule(&factor1);
	// arithmetic.addRule(&factor2);
	// arithmetic.addRule(&number1);
	// arithmetic.addRule(&number2);

	std::vector<Token> tokens = std::vector<Token>();
	tokens.push_back(Token("1", 0));
	tokens.push_back(Token("+", 0));
	tokens.push_back(Token("(", 0));
	tokens.push_back(Token("2", 0));
	tokens.push_back(Token("*", 0));
	tokens.push_back(Token("3", 0));
	tokens.push_back(Token("+", 0));
	tokens.push_back(Token("4", 0));
	tokens.push_back(Token(")", 0));

	std::vector<std::vector<EarleyItem>>	sets = std::vector<std::vector<EarleyItem> >();

	Recognizer	recognizer;
	recognizer.recognize(tokens, arithmetic, sets);
	recognizer.print(arithmetic, sets);

	Parser	parser(arithmetic);
	parser.parse(tokens);
	parser.print(tokens);

	return 0;
}
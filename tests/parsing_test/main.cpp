#include "Grammar.hpp"
#include "Parser.hpp"
#include "TestSemanticActions.hpp"

int	main(void) {
	Grammar		arithmetic;
	Sum			sum_action;
	Product		product_action;
	Number		number_action;
	RelayAction	relay;

	GrammarRuleID	sum_id("sum", 0);
	GrammarRuleID	product_id("product", 1);
	GrammarRuleID	factor_id("factor", 2);
	GrammarRuleID	number_id("number", 3);

	GrammarRule		sum1(sum_action, sum_id);
	GrammarRule		sum2(relay, sum_id);
	GrammarRule		product1(product_action, product_id);
	GrammarRule		product2(relay, product_id);
	GrammarRule		factor1(relay, factor_id);
	GrammarRule		factor2(relay, factor_id);
	GrammarRule		number1(relay, number_id);
	GrammarRule		number2(number_action, number_id);

	SubsetSymbolMatching	subset_matching;
	FalseSymbolMatching		false_matching;
	EqualSymbolMatching		equal_matching;

	GrammarSymbol	sum(sum_id.name, NON_TERMINAL, false_matching);
	GrammarSymbol	product(product_id.name, NON_TERMINAL, false_matching);
	GrammarSymbol	factor(factor_id.name, NON_TERMINAL, false_matching);
	GrammarSymbol	pm("+-", TERMINAL, subset_matching);
	GrammarSymbol	md("*/", TERMINAL, subset_matching);
	GrammarSymbol	open_par("(", TERMINAL, subset_matching);
	GrammarSymbol	close_par(")", TERMINAL, subset_matching);
	GrammarSymbol	num("number", NON_TERMINAL, false_matching);;
	GrammarSymbol	numbers("0123456789", TERMINAL, subset_matching);

	sum1.addSymbol(&sum);
	sum1.addSymbol(&pm);
	sum1.addSymbol(&product);
	sum2.addSymbol(&product);
	product1.addSymbol(&product);
	product1.addSymbol(&md);
	product1.addSymbol(&factor);
	product2.addSymbol(&factor);

	factor1.addSymbol(&open_par);
	factor1.addSymbol(&sum);
	factor1.addSymbol(&close_par);
	factor2.addSymbol(&num);
	number1.addSymbol(&numbers);
	number1.addSymbol(&num);
	number2.addSymbol(&numbers);


	arithmetic.addRule(&sum1);
	arithmetic.addRule(&sum2);
	arithmetic.addRule(&product1);
	arithmetic.addRule(&product2);
	arithmetic.addRule(&factor1);
	arithmetic.addRule(&factor2);
	arithmetic.addRule(&number1);
	arithmetic.addRule(&number2);

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
	//tokens.push_back(Token(")", 0));

	Recognizer	recognizer(arithmetic);
	recognizer.recognize(tokens);
	recognizer.print();

	return 0;
}
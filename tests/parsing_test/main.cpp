#include "Grammar.hpp"
#include "Parser.hpp"
#include "TestSemanticActions.hpp"
#include "TerminalSymbol.hpp"
#include "NonTerminalSymbol.hpp"

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
	GrammarRuleID	plus_minus("+-", 4);
	GrammarRuleID	mul_div("*/", 5);
	GrammarRuleID	open_p("(", 6);
	GrammarRuleID	close_p(")", 7);
	GrammarRuleID	num_set("0123456789", 8);

	GrammarRule		sum1(sum_action, sum_id);
	GrammarRule		sum2(relay, sum_id);
	GrammarRule		product1(product_action, product_id);
	GrammarRule		product2(relay, product_id);
	GrammarRule		factor1(relay, factor_id);
	GrammarRule		factor2(relay, factor_id);
	GrammarRule		number1(relay, number_id);
	GrammarRule		number2(number_action, number_id);

	SubsetSymbolMatching	subset_matching;
	EqualSymbolMatching		equal_matching;

	NonTerminalSymbol	sum(sum_id);
	NonTerminalSymbol	product(product_id);
	NonTerminalSymbol	factor(factor_id);
	TerminalSymbol		pm(plus_minus, subset_matching);
	TerminalSymbol		md(mul_div, subset_matching);
	TerminalSymbol		open_par(open_p, subset_matching);
	TerminalSymbol		close_par(close_p, subset_matching);
	NonTerminalSymbol	num(number_id);
	TerminalSymbol		numbers(num_set, subset_matching);

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

	std::vector<std::vector<EarleyItem> >	sets =  std::vector<std::vector<EarleyItem> >();

	Recognizer	recognizer;
	recognizer.recognize(tokens, arithmetic, sets);
	recognizer.print(arithmetic, sets);

	return 0;
}
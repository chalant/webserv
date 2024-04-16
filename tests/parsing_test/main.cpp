#include "Grammar.hpp"
#include "TestSemanticActions.hpp"

int	main(void) {
	Grammar		arithmetic;
	Sum			sum_action;
	Product		product_action;
	Number		number_action;
	RelayAction	relay;

	GrammarRuleID	sum_id;
	sum_id.id = 0;
	sum_id.name = std::string("sum");
	GrammarRuleID	product_id;
	product_id.id = 1;
	product_id.name = std::string("product");
	GrammarRuleID	factor_id;
	product_id.id = 2;
	product_id.name = std::string("factor");
	GrammarRuleID	number_id;
	product_id.id = 3;
	product_id.name = std::string("number");


	GrammarRule		sum1(sum_action, sum_id);
	GrammarRule		sum2(relay, sum_id);
	GrammarRule		product1(product_action, product_id);
	GrammarRule		product2(relay, product_id);
	GrammarRule		factor1(relay, factor_id);
	GrammarRule		factor2(relay, factor_id);
	GrammarRule		number(number_action, number_id);

	SubsetSymbolMatching	subset_matching;
	FalseSymbolMatching	false_matching;

	GrammarSymbol	sum(sum_id.name, NON_TERMINAL, false_matching);
	GrammarSymbol	product(product_id.name, NON_TERMINAL, false_matching);
	GrammarSymbol	factor(factor_id.name, NON_TERMINAL, false_matching);
	GrammarSymbol	pm("+-", TERMINAL, subset_matching);
	GrammarSymbol	md("*/", TERMINAL, subset_matching);
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
	factor1.addSymbol(&sum);
	factor2.addSymbol(&num);
	number.addSymbol(&numbers);

	arithmetic.addRule(&sum1);
	arithmetic.addRule(&sum2);
	arithmetic.addRule(&product1);
	arithmetic.addRule(&product2);
	arithmetic.addRule(&factor1);
	arithmetic.addRule(&factor2);
	arithmetic.addRule(&number);

	return 0;
}
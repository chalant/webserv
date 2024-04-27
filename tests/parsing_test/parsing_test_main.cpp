#include "Grammar.hpp"
#include "Parser.hpp"
#include "TestSemanticActions.hpp"
#include "TerminalSymbol.hpp"
#include "NonTerminalSymbol.hpp"

int	main(void) {
	// Sum			sum_action;
	// Product		product_action;
	// Factor		factor_action;
	// Number									number_action;
	// RelayAction<std::stack<std::string> >	relay;
	// std::vector<ASemanticAction<std::stack<std::string> > > sem_actions;

	//create symbols
	SubsetSymbolMatching	subset_matching;
	EqualSymbolMatching		equal_matching;
	DigitMatching			digit_matching;
	SubStringMatching		substring_matching;
	AlphabeticMatching		alpha_matching;
	PrintableAsciiMatching	printable_ascii_matching;

	NonTerminalSymbol	sum("sum",0);
	NonTerminalSymbol	product("product",1);
	NonTerminalSymbol	factor("factor",2);
	NonTerminalSymbol	num("number",7);
	TerminalSymbolSet	pm("+-", 3, {"+","-"}, subset_matching);
	TerminalSymbolSet	md("*/", 4, {"*","/"}, subset_matching);
	TerminalSymbol		open_par("(", 5);
	TerminalSymbol		close_par(")", 6);
	TerminalSymbolSet	numbers("[0-9]", 8, {}, digit_matching);

	// sem_actions.push_back(sum_action);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(product_action);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(factor_action);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(relay);
	// sem_actions.push_back(number_action);

	//create grammar;
	Grammar	arithmetic;
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

	std::vector<Token> tokens = std::vector<Token>();
	tokens.push_back(Token("1", 0));
	tokens.push_back(Token("+", 0));
	tokens.push_back(Token("(", 0));
	tokens.push_back(Token("2", 0));
	tokens.push_back(Token("*", 0));
	tokens.push_back(Token("3", 0));
	tokens.push_back(Token("+", 0));
	tokens.push_back(Token("40", 0));
	tokens.push_back(Token(")", 0));

	std::vector<std::vector<EarleyItem>>	sets = std::vector<std::vector<EarleyItem> >();

	Recognizer	recognizer;
	recognizer.recognize(tokens, arithmetic, sets);
	recognizer.print(arithmetic, sets);

	Parser	parser(arithmetic);
	parser.parse(tokens);
	parser.print(tokens);

	Grammar				nginx_configuration_grammar;

	NonTerminalSymbol	configuration("configuration", 0);
	NonTerminalSymbol	configuration_element("configuration_element", 1);
	NonTerminalSymbol	block("block", 2);
	NonTerminalSymbol	directive("directive", 3);
	NonTerminalSymbol	parameters("parameters", 4);
	NonTerminalSymbol	parameter("parameter", 5);
	NonTerminalSymbol	comment("comment", 6);
	NonTerminalSymbol	value("value", 7);
	NonTerminalSymbol	directive_name("directive_name", 8);
	NonTerminalSymbol	identifier("identifier", 9);
	NonTerminalSymbol	text("text", 10);

	TerminalSymbol		curl_open("{", 11);
	TerminalSymbol		curl_close("}", 12);
	TerminalSymbol		semicolon(";", 13);
	TerminalSymbol		hashtag("#", 14);
	TerminalSymbolSet	boolean("boolean", 15, {"on", "off"}, substring_matching);
	TerminalSymbol		underscore("_", 16);
	TerminalSymbolSet	letters("letters", 17, {}, alpha_matching);
	TerminalSymbolSet	digits("digits", 18, {}, digit_matching);
	TerminalSymbolSet	string_("string", 19, {}, printable_ascii_matching);

	//configuration
	rule = nginx_configuration_grammar.addRule(configuration);
	rule->addSymbol(&configuration_element);
	rule->addSymbol(&configuration);
	
	//configuration element
	rule = nginx_configuration_grammar.addRule(configuration_element);
	rule->addSymbol(&directive);
	rule = nginx_configuration_grammar.addRule(configuration_element);
	rule->addSymbol(&block);
	rule = nginx_configuration_grammar.addRule(configuration_element);
	rule->addSymbol(&comment);
	rule = nginx_configuration_grammar.addRule(configuration_element);

	//directive
	rule = nginx_configuration_grammar.addRule(directive);
	rule->addSymbol(&directive_name);
	rule->addSymbol(&parameters);
	rule->addSymbol(&semicolon);

	//block
	rule = nginx_configuration_grammar.addRule(block);
	rule->addSymbol(&curl_open);
	rule->addSymbol(&configuration);
	rule->addSymbol(&curl_close);

	//comment
	rule = nginx_configuration_grammar.addRule(comment);
	rule->addSymbol(&hashtag);
	rule->addSymbol(&text);

	//parameters
	rule = nginx_configuration_grammar.addRule(parameters);
	rule->addSymbol(&value);
	rule->addSymbol(&parameters);
	rule = nginx_configuration_grammar.addRule(parameters);
	rule->addSymbol(&value);

	//value
	rule = nginx_configuration_grammar.addRule(value);
	rule->addSymbol(&string_);
	rule = nginx_configuration_grammar.addRule(value);
	rule->addSymbol(&boolean);
	rule = nginx_configuration_grammar.addRule(value);

	//directive_name
	rule = nginx_configuration_grammar.addRule(directive_name);

	//identifier
	rule = nginx_configuration_grammar.addRule(identifier);
	rule->addSymbol(&letters);
	rule = nginx_configuration_grammar.addRule(identifier);
	rule->addSymbol(&underscore);
	rule = nginx_configuration_grammar.addRule(identifier);
	rule->addSymbol(&letters);
	rule->addSymbol(&identifier);

	//text
	rule = nginx_configuration_grammar.addRule(text);
	rule->addSymbol(&string_);
	rule = nginx_configuration_grammar.addRule(text);
	rule->addSymbol(&string_);
	rule->addSymbol(&text);

	return 0;
}

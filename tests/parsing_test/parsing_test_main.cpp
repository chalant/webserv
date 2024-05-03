#include "Grammar.hpp"
#include "Parser.hpp"
#include "TestSemanticActions.hpp"
#include "TerminalSymbol.hpp"
#include "NonTerminalSymbol.hpp"
#include "ConfigurationLoader.hpp"
#include "MockLogger.hpp"

#include <fstream>

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

	std::vector<std::vector<EarleyItem>>	sets = std::vector<std::vector<EarleyItem> >();

	Tokenizer	tokenizer({" "}, {"+", "-", "*", "/", "(", ")"});
	Recognizer	recognizer;

	std::ifstream	fi("test_file.txt");
	const std::vector<Token>&	toks = tokenizer.tokenize("1 1 + 12   ");
	
	// for (size_t i = 0; i < toks.size(); i++) {
	// 	std::cout << "Tokens " << toks[i].value << std::endl;
	// }

	recognizer.recognize(toks, arithmetic, sets);
	//recognizer.print(arithmetic, sets);

	Parser	parser(arithmetic);
	parser.parse(toks);
	//parser.print(toks);

	// Tokenizer	nginx_tokenizer({" ", "\n"}, {"#", "{", "}", ";", "~"});

	// std::ifstream	nginx_conf("test_configuration_file.conf");
	// const std::vector<Token>&	nginx_toks = nginx_tokenizer.tokenize(nginx_conf);
	
	// // for (size_t i = 0; i < nginx_toks.size(); i++) {
	// // 	std::cout << "Tokens " << nginx_toks[i].value << std::endl;
	// // }

	// Grammar				nginx_configuration_grammar;

	// NonTerminalSymbol	configuration("configuration", 0);
	// NonTerminalSymbol	block_element("configuration-element", 1);
	// NonTerminalSymbol	block("block", 2);
	// NonTerminalSymbol	block_param("block-param", 22);
	// NonTerminalSymbol	directive("directive", 3);
	// NonTerminalSymbol	parameters("parameters", 4);
	// NonTerminalSymbol	parameter("parameter", 5);
	// NonTerminalSymbol	comment_list("comment-list", 23);
	// NonTerminalSymbol	comment("comment", 6);
	// NonTerminalSymbol	value("value", 7);
	// NonTerminalSymbol	directive_name("directive-name", 8);
	// NonTerminalSymbol	identifier("identifier", 9);
	// NonTerminalSymbol	text("text", 10);

	// TerminalSymbol		curl_open("{", 11);
	// TerminalSymbol		curl_close("}", 12);
	// TerminalSymbol		semicolon(";", 13);
	// TerminalSymbol		hashtag("#", 14);
	// TerminalSymbol		newline("\n", 15);
	// TerminalSymbol		regex("~", 16);
	// TerminalSymbolSet	boolean("boolean", 17, {"on", "off"}, substring_matching);
	// TerminalSymbol		underscore("_", 18);
	// TerminalSymbolSet	letters("letters", 19, {}, alpha_matching);
	// TerminalSymbolSet	digits("digits", 20, {}, digit_matching);
	// TerminalSymbolSet	string_("string", 21, {}, printable_ascii_matching);

	// //configuration
	// rule = nginx_configuration_grammar.addRule(configuration);
	// rule->addSymbol(&block_element);
	// rule = nginx_configuration_grammar.addRule(configuration);
	// rule->addSymbol(&block_element);
	// rule->addSymbol(&configuration);
	
	// //configuration element
	// rule = nginx_configuration_grammar.addRule(block_element);
	// rule->addSymbol(&directive);
	// rule = nginx_configuration_grammar.addRule(block_element);
	// rule->addSymbol(&block);
	// rule = nginx_configuration_grammar.addRule(block_element);
	// rule->addSymbol(&comment_list);

	// //directive
	// rule = nginx_configuration_grammar.addRule(directive);
	// rule->addSymbol(&directive_name);
	// rule->addSymbol(&parameters);
	// rule->addSymbol(&semicolon);

	// //block
	// rule = nginx_configuration_grammar.addRule(block);
	// rule->addSymbol(&string_);
	// rule->addSymbol(&curl_open);
	// rule->addSymbol(&configuration);
	// rule->addSymbol(&curl_close);

	// rule = nginx_configuration_grammar.addRule(block);
	// rule->addSymbol(&string_);
	// rule->addSymbol(&block_param);
	// rule->addSymbol(&curl_open);
	// rule->addSymbol(&configuration);
	// rule->addSymbol(&curl_close);

	// rule = nginx_configuration_grammar.addRule(block_param);
	// rule->addSymbol(&regex);
	// rule->addSymbol(&string_);

	// rule = nginx_configuration_grammar.addRule(block_param);
	// rule->addSymbol(&string_);

	// //comment-list
	// rule = nginx_configuration_grammar.addRule(comment_list);
	// rule->addSymbol(&comment);
	// rule = nginx_configuration_grammar.addRule(comment_list);
	// rule->addSymbol(&comment);
	// rule->addSymbol(&comment_list);

	// //comment
	// rule = nginx_configuration_grammar.addRule(comment);
	// rule->addSymbol(&hashtag);
	// rule->addSymbol(&text);
	// rule = nginx_configuration_grammar.addRule(comment);
	// rule->addSymbol(&hashtag);

	// //parameters
	// rule = nginx_configuration_grammar.addRule(parameters);
	// rule->addSymbol(&value);
	// rule = nginx_configuration_grammar.addRule(parameters);
	// rule->addSymbol(&value);
	// rule->addSymbol(&parameters);

	// //value
	// rule = nginx_configuration_grammar.addRule(value);
	// rule->addSymbol(&string_);
	// rule = nginx_configuration_grammar.addRule(value);
	// rule->addSymbol(&boolean);

	// //directive_name
	// rule = nginx_configuration_grammar.addRule(directive_name);
	// rule->addSymbol(&string_);

	// //identifier
	// rule = nginx_configuration_grammar.addRule(identifier);
	// rule->addSymbol(&letters);
	// rule = nginx_configuration_grammar.addRule(identifier);
	// rule->addSymbol(&underscore);
	// rule = nginx_configuration_grammar.addRule(identifier);
	// rule->addSymbol(&letters);
	// rule->addSymbol(&identifier);

	// //text
	// rule = nginx_configuration_grammar.addRule(text);
	// rule->addSymbol(&string_);
	// rule = nginx_configuration_grammar.addRule(text);
	// rule->addSymbol(&string_);
	// rule->addSymbol(&text);

	// std::vector<std::vector<EarleyItem>>	nginx_sets = std::vector<std::vector<EarleyItem> >();
	// Recognizer	nginx_recognizer;
	// nginx_recognizer.recognize(nginx_toks, nginx_configuration_grammar, nginx_sets);
	// nginx_recognizer.print(nginx_configuration_grammar, nginx_sets);
	// Parser	nginx_parser(nginx_configuration_grammar);
	// nginx_parser.parse(nginx_toks);
	// nginx_parser.print(nginx_toks);

	MockLogger	logger;
	ConfigurationLoader	loader = ConfigurationLoader(logger);
	loader.loadConfiguration("test_configuration_file.conf");

	return 0;
}

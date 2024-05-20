#include "../../../includes/parsing/Grammar.hpp"
#include "../../../includes/parsing/NonTerminalSymbol.hpp"
#include "../../../includes/parsing/Parser.hpp"
#include "../../../includes/parsing/TerminalSymbol.hpp"

#include <fstream>

int main(void)
{
    // create symbols
    SubsetSymbolMatching subset_matching;
    EqualSymbolMatching equal_matching;
    DigitMatching digit_matching;
    SubStringMatching substring_matching;
    AlphabeticMatching alpha_matching;
    PrintableAsciiMatching printable_ascii_matching;

    NonTerminalSymbol sum("sum", 0);
    NonTerminalSymbol product("product", 1);
    NonTerminalSymbol factor("factor", 2);
    NonTerminalSymbol num("number", 7);

    std::vector<std::string> values; // to be c++98 compliant, initialize the
                                     // vector with the values
    values.push_back("+");
    values.push_back("-");

    TerminalSymbolSet pm("+-", 3, values, subset_matching);

    values.clear();
    values.push_back("*");
    values.push_back("/");

    TerminalSymbolSet md("*/", 4, values, subset_matching);
    TerminalSymbol open_par("(", 5);
    TerminalSymbol close_par(")", 6);

    values.clear();

    TerminalSymbolSet numbers("[0-9]", 8, values, digit_matching);

    // create grammar;
    Grammar arithmetic;
    GrammarRule *rule = arithmetic.addRule(sum);

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

    std::vector<std::vector<EarleyItem> > sets =
        std::vector<std::vector<EarleyItem> >();

    std::vector<std::string> separators; // to be c++98 compliant, initialize
                                         // the vector with the values
    std::vector<std::string>
        reserved_symbols; // to be c++98 compliant, initialize the vector
                          // with the values
    separators.push_back(" ");
    reserved_symbols.push_back("+");
    reserved_symbols.push_back("-");
    reserved_symbols.push_back("*");
    reserved_symbols.push_back("/");
    reserved_symbols.push_back("(");
    reserved_symbols.push_back(")");

    Tokenizer tokenizer(separators, reserved_symbols);
    Recognizer recognizer;

    std::ifstream fi("test_file.txt");
    const std::vector<Token> &toks = tokenizer.tokenize("1 + 1");

    // for (size_t i = 0; i < toks.size(); i++) {
    // 	std::cout << "Tokens " << toks[i].value << std::endl;
    // }

    recognizer.recognize(toks, arithmetic, sets);
    recognizer.print(arithmetic, sets);

    Parser parser(arithmetic);
    parser.parse(toks);
    fi.close();
    // parser.print(toks);

    // Tokenizer	nginx_tokenizer({" ", "\n"}, {"#", "{", "}", ";", "~"});

    // std::ifstream	nginx_conf("test_configuration_file.conf");
    // const std::vector<Token>&	nginx_toks =
    // nginx_tokenizer.tokenize(nginx_conf);

    // // for (size_t i = 0; i < nginx_toks.size(); i++) {
    // // 	std::cout << "Tokens " << nginx_toks[i].value << std::endl;
    // // }

    // std::vector<std::vector<EarleyItem> >	nginx_sets =
    // std::vector<std::vector<EarleyItem> >(); Recognizer
    // nginx_recognizer; nginx_recognizer.recognize(nginx_toks,
    // nginx_configuration_grammar, nginx_sets);
    // nginx_recognizer.print(nginx_configuration_grammar, nginx_sets);
    // Parser	nginx_parser(nginx_configuration_grammar);
    // nginx_parser.parse(nginx_toks);
    // nginx_parser.print(nginx_toks);

    return 0;
}

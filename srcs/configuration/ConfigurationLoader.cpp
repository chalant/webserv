#include "../../includes/configuration/ConfigurationLoader.hpp"
#include "../../includes/configuration/Defaults.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/parsing/Grammar.hpp"
#include "../../includes/parsing/NonTerminalSymbol.hpp"
#include "../../includes/parsing/Parser.hpp"
#include "../../includes/parsing/TerminalSymbol.hpp"
#include <fstream>

static void add_block(const std::vector<Token> &tokens, const Grammar &grammar,
                      ParseTree &parse_tree, ConfigurationBlock &block,
                      Defaults &defaults);
static void add_directive(const std::vector<Token> &tokens,
                          ParseTree &parse_tree, ConfigurationBlock &block);
static void get_values(const std::vector<Token> &tokens, ParseTree &parse_tree,
                       std::vector<std::string> &result);

static void build_config(const std::vector<Token> &tokens,
                         const Grammar &grammar, ParseTree &parse_tree,
                         ConfigurationBlock &block, Defaults &defaults)
{
    const GrammarRule *rule = grammar.getRule(parse_tree.ruleIndex());
    const std::string rule_name = rule->getName();
    if (rule_name == "block")
    {
        add_block(tokens, grammar, parse_tree, block, defaults);
        return;
    }
    else if (rule_name == "directive")
    {
        add_directive(tokens, parse_tree, block);
        return;
    }
    // go down the parse tree and build sub-blocks.
    for (size_t i = 0; i < parse_tree.size(); i++)
    {
        build_config(tokens, grammar, *parse_tree[ i ], block, defaults);
    }
}

// retrieves the list of strings from the parse tree.
static void get_values(const std::vector<Token> &tokens, ParseTree &parse_tree,
                       std::vector<std::string> &result)
{
    ParseTree *next = parse_tree[ 1 ];
    result.push_back(tokens[ (*parse_tree[ 0 ])[ 0 ]->tokenIndex() ].value);
    // goes down the tree branch
    while (next != NULL)
    {
        result.push_back(tokens[ (*(*next)[ 0 ])[ 0 ]->tokenIndex() ].value);
        next = (*next)[ 1 ];
    }
}

static void add_directive(const std::vector<Token> &tokens,
                          ParseTree &parse_tree, ConfigurationBlock &block)
{
    // the first sub-child is the directive name and the second is the
    // parameters list.
    std::vector<std::string> &params = block.addDirective(
        tokens[ (*parse_tree[ 0 ])[ 0 ]->tokenIndex() ].value);
    get_values(tokens, *parse_tree[ 1 ], params);
}

static void add_block(const std::vector<Token> &tokens, const Grammar &grammar,
                      ParseTree &parse_tree, ConfigurationBlock &block,
                      Defaults &defaults)
{
    ConfigurationBlock *new_block;
    const std::string rule_name =
        grammar.getRule(parse_tree[ 1 ]->ruleIndex())->getName();

    // check if it is a block with at block_parameters and retreive the
    // block_parameters.
    if (rule_name == "block-parameters")
    {
        int start = 0;
        // NOTE: the ConfigurationBlock could have a regex mode...
        new_block = new ConfigurationBlock(
            block, tokens[ parse_tree[ 0 ]->tokenIndex() ].value, defaults);
        if (tokens[ (*parse_tree[ 1 ])[ 0 ]->tokenIndex() ].value == "~")
        {
            start = 1;
            new_block->isRegex(true);
        }
        std::vector<std::string> &params = new_block->setParameters();
		get_values(tokens, *(*parse_tree[ 1 ])[ start ], params);
        build_config(tokens, grammar, *parse_tree[ 3 ], *new_block, defaults);
    }
    else
    {
        new_block = new ConfigurationBlock(
            block, tokens[ parse_tree[ 0 ]->tokenIndex() ].value, defaults);
        // recursively add blocks or directives on the current block. (skipping
        // the open brace.)
        build_config(tokens, grammar, *parse_tree[ 2 ], *new_block, defaults);
    }
    block.addBlock(tokens[ parse_tree[ 0 ]->tokenIndex() ].value, new_block);
}

ConfigurationLoader::ConfigurationLoader(ILogger &logger) : m_logger(logger)
{
    // Log the creation of the configuration loader.
    this->m_logger.log(VERBOSE, "ConfigurationLoader created.");

    m_config = NULL;
}

// todo: delete all the blocks.
ConfigurationLoader::~ConfigurationLoader() { delete m_config; }

IConfiguration &ConfigurationLoader::loadConfiguration(const std::string &path)
{
    std::ifstream conf_stream(path.c_str());
    if (!conf_stream.is_open())
        throw InvalidConfigFileError();

    // Log the loading of the configuration file.
    this->m_logger.log(INFO, "Loading configuration file: '" + path + "'.");

    Grammar grammar;
    SubsetSymbolMatching subset_matching;
    EqualSymbolMatching equal_matching;
    DigitMatching digit_matching;
    SubStringMatching substring_matching;
    AlphabeticMatching alpha_matching;
    PrintableAsciiMatching printable_ascii_matching;

    NonTerminalSymbol configuration("configuration", 0);
    NonTerminalSymbol block_element("configuration-element", 1);
    NonTerminalSymbol block("block", 2);
    NonTerminalSymbol block_parameters("block-parameters", 22);
    NonTerminalSymbol directive("directive", 3);
    NonTerminalSymbol parameters("parameters", 4);
    NonTerminalSymbol comment_list("comment-list", 23);
    NonTerminalSymbol comment("comment", 6);
    NonTerminalSymbol value("value", 7);
    NonTerminalSymbol directive_name("directive-name", 8);
    NonTerminalSymbol identifier("identifier", 9);
    NonTerminalSymbol text("text", 10);

    TerminalSymbol curl_open("{", 11);
    TerminalSymbol curl_close("}", 12);
    TerminalSymbol semicolon(";", 13);
    TerminalSymbol hashtag("#", 14);
    TerminalSymbol newline("\n", 15);
    TerminalSymbol regex("~", 16);
    std::vector<std::string> bools;
    bools.push_back("on");
    bools.push_back("off");
    std::vector<std::string> empty;
    TerminalSymbolSet boolean("boolean", 17, bools, substring_matching);
    TerminalSymbol underscore("_", 18);
    TerminalSymbolSet letters("letters", 19, empty, alpha_matching);
    TerminalSymbolSet digits("digits", 20, empty, digit_matching);
    TerminalSymbolSet string_("string", 21, empty, printable_ascii_matching);

    // configuration
    GrammarRule *rule = grammar.addRule(configuration);
    rule->addSymbol(&block_element);
    rule = grammar.addRule(configuration);
    rule->addSymbol(&block_element);
    rule->addSymbol(&configuration);

    // configuration element
    rule = grammar.addRule(block_element);
    rule->addSymbol(&directive);
    rule = grammar.addRule(block_element);
    rule->addSymbol(&block);
    rule = grammar.addRule(block_element);
    rule->addSymbol(&comment_list);

    // directive
    rule = grammar.addRule(directive);
    rule->addSymbol(&directive_name);
    rule->addSymbol(&parameters);
    rule->addSymbol(&semicolon);

    // block
    rule = grammar.addRule(block);
    rule->addSymbol(&string_);
    rule->addSymbol(&curl_open);
    rule->addSymbol(&configuration);
    rule->addSymbol(&curl_close);

    rule = grammar.addRule(block);
    rule->addSymbol(&string_);
    rule->addSymbol(&block_parameters);
    rule->addSymbol(&curl_open);
    rule->addSymbol(&configuration);
    rule->addSymbol(&curl_close);

    rule = grammar.addRule(block_parameters);
    rule->addSymbol(&regex);
    rule->addSymbol(&parameters);

    rule = grammar.addRule(block_parameters);
    rule->addSymbol(&parameters);

    // comment-list
    rule = grammar.addRule(comment_list);
    rule->addSymbol(&comment);
    rule = grammar.addRule(comment_list);
    rule->addSymbol(&comment);
    rule->addSymbol(&comment_list);

    // comment
    rule = grammar.addRule(comment);
    rule->addSymbol(&hashtag);
    rule->addSymbol(&text);
    rule = grammar.addRule(comment);
    rule->addSymbol(&hashtag);

    // parameters
    rule = grammar.addRule(parameters);
    rule->addSymbol(&value);
    rule = grammar.addRule(parameters);
    rule->addSymbol(&value);
    rule->addSymbol(&parameters);

    // value
    rule = grammar.addRule(value);
    rule->addSymbol(&string_);
    rule = grammar.addRule(value);
    rule->addSymbol(&boolean);

    // directive_name
    rule = grammar.addRule(directive_name);
    rule->addSymbol(&string_);

    // identifier
    rule = grammar.addRule(identifier);
    rule->addSymbol(&letters);
    rule = grammar.addRule(identifier);
    rule->addSymbol(&underscore);
    rule = grammar.addRule(identifier);
    rule->addSymbol(&letters);
    rule->addSymbol(&identifier);

    // text
    rule = grammar.addRule(text);
    rule->addSymbol(&string_);
    rule = grammar.addRule(text);
    rule->addSymbol(&string_);
    rule->addSymbol(&text);

    std::vector<std::string> separators;
    separators.push_back(" ");
    separators.push_back("\n");
    separators.push_back("\t");

    std::vector<std::string> reserved_symbols;

    reserved_symbols.push_back("#");
    reserved_symbols.push_back("{");
    reserved_symbols.push_back("}");
    reserved_symbols.push_back(";");
    reserved_symbols.push_back("~");

    Tokenizer tokenizer(separators, reserved_symbols);
    Parser parser(grammar);
    const std::vector<Token> &tokens = tokenizer.tokenize(conf_stream);

    ParseTree &parse_tree = parser.parse(tokens);
    if (m_config)
        delete m_config;
    // initial block.
    m_config = new ConfigurationBlock(m_logger, "main", m_defaults);

    for (size_t i = 0; i < parse_tree.size(); i++)
    {
        build_config(tokens, grammar, *parse_tree[ i ], *m_config, m_defaults);
    }
    conf_stream.close();

    // Log the end of loading of the configuration file.
    this->m_logger.log(VERBOSE, "Configuration file loaded successfully.");

    return *m_config;
}

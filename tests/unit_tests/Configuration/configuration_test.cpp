#include "Grammar.hpp"
#include "Parser.hpp"
#include "TerminalSymbol.hpp"
#include "NonTerminalSymbol.hpp"
#include "ConfigurationLoader.hpp"
#include "MockLogger.hpp"

#include <fstream>

int	main() {
	MockLogger	logger;
	ConfigurationLoader	loader = ConfigurationLoader(logger);
	const IConfiguration&	block = loader.loadConfiguration("test_configuration_file.conf");
	block.print(0);
}

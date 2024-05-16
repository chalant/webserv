#include "../../../includes/configuration/ConfigurationLoader.hpp"
#include "../../mock_includes/MockLogger.hpp"
#include <iostream>

int	main() {
	MockLogger				logger;
	ConfigurationLoader		loader(logger);
	const IConfiguration&	block = loader.loadConfiguration("test_configuration_file.conf");
	block.print(0);
	// test defaults
	const std::string& val = block.getString("server_name");
	(void)val;
	const std::vector<IConfiguration *> blocks = block.getBlocks("location");
	// try to access index
	blocks[0]; blocks[1]; blocks[2];
}

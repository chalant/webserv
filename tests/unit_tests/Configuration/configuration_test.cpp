#include "../../../includes/configuration/ConfigurationLoader.hpp"
#include "../../mock_includes/MockLogger.hpp"

int	main() {
	MockLogger	logger;
	ConfigurationLoader	loader(logger);
	const IConfiguration&	block = loader.loadConfiguration("test_configuration_file.conf");
	block.print(0);
}

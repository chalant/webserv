#include "../../../includes/configuration/ConfigurationLoader.hpp"
#include "../../mock_includes/MockLogger.hpp"
#include <cassert>
#include <iostream>

int main()
{
    MockLogger logger;
    ConfigurationLoader loader(logger);
    IConfiguration &block =
        loader.loadConfiguration("test_configuration_file.conf");
    block.print(0);
    //  test defaults
    const std::string &val = block.getString("server_name");
    //(void)val;
    const BlockList &blocks = block.getBlocks("location");
    // try to access index
    blocks[ 0 ];
    blocks[ 1 ];
    blocks[ 2 ];
    // blocks.at(10);
    //  std::vector<IConfiguration *>::iterator	itr = blocks.begin();
    //  for (; itr != blocks.end(); itr++)
    //  {
    //  	std::cout << "BLOCKS! " << (*itr)->getName() << std::endl;
    //  }
    // test if the default is valid
    std::vector<std::string> blk =
        block.getBlocks("limit_except")[ 0 ]->getParameters();
    assert(blk[ 0 ] == "GET");
    return 0;
}

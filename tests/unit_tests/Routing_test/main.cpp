#include "../../../includes/network/Socket.hpp"
#include "../../../includes/configuration/IConfiguration.hpp"
#include "../../../includes/buffer/BufferManager.hpp"
#include "../../../includes/logger/Logger.hpp"
#include "../../../includes/response/Router.hpp"
#include "../../../includes/configuration/ConfigurationLoader.hpp"
#include "../../mock_includes/MockLogger.hpp"

#include <iostream>

int main()
{
    MockLogger logger;
    ConfigurationLoader loader(logger);
    const IConfiguration &block = loader.loadConfiguration("test_configuration_router.conf");
    // block.print(0);

    Router router(block, logger);
    size_t i = 0;
    while (i < router.getRouteCount())
    {
        std::cout << "Route :" << router.getRoutes()[i++].getUri() << std::endl;
        // std::cout << "Method: " << it->getMethod() << std::endl << std::endl;
    }
    return (0);
}
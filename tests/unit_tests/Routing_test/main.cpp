#include "../../../includes/buffer/BufferManager.hpp"
#include "../../../includes/configuration/ConfigurationLoader.hpp"
#include "../../../includes/configuration/IConfiguration.hpp"
#include "../../../includes/logger/Logger.hpp"
#include "../../../includes/network/Socket.hpp"
#include "../../../includes/response/Router.hpp"
#include "../../mock_includes/MockLogger.hpp"

#include <cassert>
#include <iostream>

int main()
{
    MockLogger logger;
    ConfigurationLoader loader(logger);
    const IConfiguration &block =
        loader.loadConfiguration("test_configuration_router.conf");
    HttpHelper httpHelper;
    Router router(block, logger, httpHelper);
    size_t i = 0;
    while (i < router.getRouteCount())
    {
        std::cout << "Route :" << router.getRoutes()[ i ].getUri() << std::endl;
        std::cout << "Method: "
                  << httpHelper.httpMethodStringMap(
                         router.getRoutes()[ i ].getMethod())
                  << std::endl
                  << std::endl;
        i++;
    }
    assert(i == router.getRouteCount());
    return (0);
}

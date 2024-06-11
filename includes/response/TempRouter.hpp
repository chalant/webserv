#ifndef TEMPROUTER_HPP
#define TEMPROUTER_HPP

/*
 * TempRouter: Selects the right 'ResponseGenerator' based on URI (etc.)
 * in the 'IRequest' (each locationblock in the conf file corresponds
 * to a 'ResponseGenerator' mapping, ie the TempRouter selects the correct
 * locationblock)
 */

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpHelper.hpp"
#include "../logger/ILogger.hpp"
#include "../request/IRequest.hpp"
#include "IResponse.hpp"
#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include "ITempRouter.hpp"
#include "URIMatcher.hpp"

class TempRouter : public ITempRouter
{
private:
    IConfiguration &m_configuration;
    ILogger &m_logger;
    HttpHelper m_http_helper;

    // std::vector<IRoute *>			m_routes;
    std::vector<std::vector<IRoute *> *> m_routes;
    std::vector<IConfiguration *> m_servers;
    std::map<std::string, IResponseGenerator *> m_response_generators;
    std::map<std::string, IURIMatcher *> m_uri_matchers;

    // Method to compare two routes by path length
    static bool m_sortRoutes(const IRoute *a, const IRoute *b);
    IResponseGenerator *
    m_createCGIResponseGenerator(const std::string &type,
                                 const std::string &bin_path, ILogger &logger);
    void m_createRoutes(IConfiguration &server, std::vector<IRoute *> &routes);

public:
    TempRouter(IConfiguration &Configuration, ILogger &logger);
    ~TempRouter();

    virtual IRoute *getRoute(IRequest *req, IResponse *res);
    virtual Triplet_t execRoute(IRoute *route, IRequest *req, IResponse *res);
};

#endif // TEMPROUTER_HPP
// Path: includes/response/TempRouter.hpp

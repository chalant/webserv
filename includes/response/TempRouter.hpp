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

class TempRouter : public ITempRouter
{
private:
    IConfiguration &m_configuration;
    ILogger &m_logger;
    HttpHelper m_http_helper;

    std::vector<IRoute *> m_routes;
    std::map<std::string, IResponseGenerator *> m_response_generators;

    // Method to compare two routes by path length
    static bool m_sortRoutes(const IRoute *a, const IRoute *b);
	IResponseGenerator	*m_createCGIResponseGenerator(const std::string type, ILogger &logger);

public:
    TempRouter(IConfiguration &Configuration, ILogger &logger);
    ~TempRouter();

    virtual Triplet_t	execRoute(IRequest *req, IResponse *res);
	virtual	IRoute		*getRoute(IRequest *req, IResponse *res);
	virtual	Triplet_t	execRoute(IRoute *route, IRequest *req, IResponse *res);
};

#endif // TEMPROUTER_HPP
// Path: includes/response/TempRouter.hpp

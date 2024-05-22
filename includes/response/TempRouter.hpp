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
#include "IRoute.hpp"
#include "ITempRouter.hpp"

class TempRouter : public ITempRouter
{
private:
    IConfiguration &m_configuration;
    ILogger &m_logger;
    HttpHelper m_http_helper;
    IRoute *m_route;

public:
    TempRouter(IConfiguration &Configuration, ILogger &logger);
    ~TempRouter();

    virtual Triplet_t execRoute(IRequest *req, IResponse *res);
};

#endif // TEMPROUTER_HPP
// Path: includes/response/TempRouter.hpp

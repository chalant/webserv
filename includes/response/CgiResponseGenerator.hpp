#ifndef CGIRESPONSEGENERATOR_HPP
#define CGIRESPONSEGENERATOR_HPP

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpStatusCodeHelper.hpp"
#include "../logger/ILogger.hpp"
#include "../request/IRequest.hpp"
#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class CgiResponseGenerator : public IResponseGenerator
{
private:
    ILogger &m_logger;
    HttpStatusCodeHelper m_http_status_code_helper;

    void m_setCgiArguments(const std::string &script_name, const IRoute &route,
                          const IConfiguration &configuration,
                          std::vector<char *> &cgi_args);
    void m_setCgiEnvironment(const std::string &script_name, const IRoute &route,
                            const IRequest &request,
                            std::vector<char *> &cgi_env);
    char *m_getCgiInterpreterPath(const std::string &script_name,
                                 const IConfiguration &configuration) const;
    char *m_getScriptPath(const std::string &script_name,
                         const IRoute &route) const;
    std::string m_getPathTranslated(std::string &script_name,
                                   const IRoute &route) const;
    void m_cleanUp(char *cgi_args[], char *cgi_env[] = NULL,
                  int response_pipe_fd[ 2 ] = NULL, int body_pipe_fd[ 2 ] = NULL,
                  short option = 0x0) const;

public:
    CgiResponseGenerator(ILogger &logger);
    ~CgiResponseGenerator();

    virtual Triplet_t generateResponse(const IRoute &route,
                                       const IRequest &request,
                                       IResponse &response,
                                       const IConfiguration &configuration,
                                       const std::string &script_name = "");
};

#endif // CGIRESPONSEGENERATOR_HPP
// Path: includes/CgiResponseGenerator.hpp

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
    ILogger &_logger;
    HttpStatusCodeHelper _httpStatusCodeHelper;

    void _setCgiArguments(const std::string &scriptName, const IRoute &route,
                          const IConfiguration &configuration,
                          std::vector<char *> &cgiArgs);
    void _setCgiEnvironment(const std::string &scriptName, const IRoute &route,
                            const IRequest &request,
                            std::vector<char *> &cgiEnv);
    char *_getCgiInterpreterPath(const std::string &scriptName,
                                 const IConfiguration &configuration) const;
    char *_getScriptPath(const std::string &scriptName,
                         const IRoute &route) const;
    std::string _getPathTranslated(std::string &scriptName,
                                   const IRoute &route) const;
    void _cleanUp(char *cgiArgs[], char *cgiEnv[] = NULL,
                  int responsePipefd[ 2 ] = NULL, int bodyPipefd[ 2 ] = NULL,
                  short option = 0x0) const;

public:
    CgiResponseGenerator(ILogger &logger);
    ~CgiResponseGenerator();

    virtual Triplet_t generateResponse(const IRoute &route,
                                       const IRequest &request,
                                       const IConfiguration &configuration,
                                       const std::string &scriptName = "");
};

#endif // CGIRESPONSEGENERATOR_HPP
// Path: includes/CgiResponseGenerator.hpp

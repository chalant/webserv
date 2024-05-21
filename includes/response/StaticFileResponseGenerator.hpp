#ifndef STATICFILERESPONSEGENERATOR_HPP
#define STATICFILERESPONSEGENERATOR_HPP

#include "../logger/ILogger.hpp"
#include "IResponseGenerator.hpp"

class StaticFileResponseGenerator : public IResponseGenerator
{
private:
    const std::map<std::string, std::string> _mimeTypes;
    ILogger &_logger;

    std::map<std::string, std::string> _initialiseMimeTypes() const;
    std::string _getMimeType(const std::string &filePath) const;

public:
    StaticFileResponseGenerator(ILogger &logger);
    ~StaticFileResponseGenerator();
    Triplet_t generateResponse(const IRoute &route, const IRequest &request,
                               IResponse &response,
                               const IConfiguration &configuration,
                               const std::string &scriptName = "");
};

#endif // STATICFILERESPONSEGENERATOR_HPP
// Path: includes/response/StaticFileResponseGenerator.hpp

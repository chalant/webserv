#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include <string>

class Route : public IRoute
{
private:
    const std::string m_path; // The path of the route (prefix)
    const bool m_is_regex; // If the path is a regex
    const std::vector<HttpMethod> m_methods; // The method of the route
    const std::string
        m_root; // The root directory of the route (where the files are stored)
    const std::string
        m_index; // The file to serve if the request uri is a directory
    const std::string m_cgi_script; // The script to execute if the request is a CGI
    IResponseGenerator &m_response_generator;

public:
    Route(const std::string path, const bool is_regex, const std::vector<HttpMethod> methods, const std::string root,
          const std::string index, const std::string cgi_script, IResponseGenerator &response_generator);
    ~Route();
    std::string getPath() const;
    std::string getRoot() const;
    std::string getIndex() const;
    std::string getCgiScript() const;
    bool isAllowedMethod(const HttpMethod method) const;
    bool isRegex() const;
    IResponseGenerator *getResponseGenerator() const;
};

#endif // ROUTE_HPP
       // Path: includes/response/Route.hpp

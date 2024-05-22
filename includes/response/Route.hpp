#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include <string>

class Route : public IRoute
{
private:
    const std::string m_path; // The path of the route (prefix)
    const std::string
        m_root; // The root directory of the route (where the files are stored)
    const std::string
        m_index; // The file to serve if the request uri is a directory
    IResponseGenerator &m_response_generator;

public:
    Route(const std::string path, const std::string root,
          const std::string index, IResponseGenerator &response_generator);
    ~Route();
    std::string getPath() const;
    std::string getRoot() const;
    std::string getIndex() const;
    IResponseGenerator *getResponseGenerator() const;
};

#endif // ROUTE_HPP
       // Path: includes/response/Route.hpp

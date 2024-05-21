#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include <string>

class Route : public IRoute
{
private:
    const std::string _path; // The path of the route (prefix)
    const std::string
        _root; // The root directory of the route (where the files are stored)
    const std::string
        _index; // The file to serve if the request uri is a directory
    IResponseGenerator &_responseGenerator;

public:
    Route(const std::string path, const std::string root,
          const std::string index, IResponseGenerator &responseGenerator);
    ~Route();
    std::string getPath() const;
    std::string getRoot() const;
    std::string getIndex() const;
    IResponseGenerator *getResponseGenerator() const;
};

#endif // ROUTE_HPP
       // Path: includes/response/Route.hpp

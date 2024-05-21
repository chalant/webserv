#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include <string>

class Route : public IRoute
{
private:
    const std::string _path;
    const std::string _root;
    IResponseGenerator &_responseGenerator;

public:
    Route(const std::string path, const std::string root,
          IResponseGenerator &responseGenerator);
    ~Route();
    std::string getPath() const;
    std::string getRoot() const;
    IResponseGenerator *getResponseGenerator() const;
};

#endif // ROUTE_HPP
       // Path: includes/response/Route.hpp

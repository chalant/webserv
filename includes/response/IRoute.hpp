#ifndef IROUTE_HPP
#define IROUTE_HPP

#include <string>

class IResponseGenerator;

class IRoute
{
public:
    virtual ~IRoute() {};

    virtual std::string getRoot() const = 0;
    virtual std::string getPath() const = 0;
    virtual std::string getIndex() const = 0;
    virtual IResponseGenerator *getResponseGenerator() const = 0;
};

#endif // IROUTE_HPP
       // Path: includes/response/IRoute.hpp

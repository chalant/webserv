#ifndef MOCKROUTE_HPP
#define MOCKROUTE_HPP

/*
 * MockRoute class
 *
 * This class represents a mock route
 *
 */

#include "../../includes/response/IRoute.hpp"

class MockRoute : public IRoute
{
private:
    std::string _root;
    std::string _prefix;
public:
    MockRoute(){};
    ~MockRoute(){};

    virtual std::string getUri() const;
    virtual void setUri(std::string newUri);
    virtual HttpMethod getMethod() const;
    virtual void setMethod(HttpMethod newMethod);
    virtual std::string getRoot() const;
    virtual std::string getPrefix() const;
    virtual void setRoot(std::string &root);
    virtual void setPrefix(std::string &prefix);
};

#endif // MOCKROUTE_HPP
// Path: tests/mock_includes/MockRoute.hpp

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
    std::string _path;

public:
    MockRoute();
    ~MockRoute();

    virtual std::string getRoot() const;
    virtual std::string getPath() const;
    virtual void setRoot(std::string &root);
    virtual void setPath(std::string &prefix);
    virtual IResponseGenerator *getResponseGenerator() const;
};

#endif // MOCKROUTE_HPP
// Path: tests/mock_includes/MockRoute.hpp

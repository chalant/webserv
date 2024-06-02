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
    std::string m_root;
    std::string m_path;

public:
    MockRoute();
    ~MockRoute();

    virtual std::string getRoot() const;
    virtual std::string getPath() const;
    virtual std::string getIndex() const;
    virtual void setRoot(std::string &root);
    virtual void setPath(std::string &prefix);
    virtual bool isAllowedMethod(const HttpMethod method) const;
    virtual bool isRegex() const;
    virtual IResponseGenerator *getResponseGenerator() const;
};

#endif // MOCKROUTE_HPP
// Path: tests/mock_includes/MockRoute.hpp

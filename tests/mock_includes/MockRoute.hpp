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
    std::string m_cgi_script;

public:
    MockRoute();
    ~MockRoute();

    virtual std::string getRoot() const;
    virtual std::string getPath() const;
    virtual std::string getIndex() const;
    virtual std::string getCgiScript() const;
    virtual void setRoot(std::string &root);
    virtual void setPath(std::string &prefix);
    virtual void setCgiScript(std::string &cgi_script);
    virtual bool isAllowedMethod(const HttpMethod method) const;
    virtual bool isRegex() const;
	virtual bool isCGI() const;
    virtual IResponseGenerator *getResponseGenerator() const;
	virtual bool match(const std::string &value);
	virtual void				setResponseGenerator(IResponseGenerator *generator);
};

#endif // MOCKROUTE_HPP
// Path: tests/mock_includes/MockRoute.hpp

/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpMethodHelper.hpp"
#include "../logger/ILogger.hpp"
#include "../request/IRequest.hpp"
#include "../request/Request.hpp"
#include "../response/IResponse.hpp"
#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include "../../includes/response/UploadResponseGenerator.hpp"
#include "IResponseGenerator.hpp"
#include "IRouter.hpp"
#include "IRoute.hpp"
#include <string>
#include <vector>

typedef std::pair<int, std::pair<int, int> > Triplet_t;

class Response;

// Route structure with function pointer for handler
class Route : public IRoute
{
public:
    Route(IResponseGenerator *response_generator);
	~Route();
    std::string	getUri() const;
    void setUri(std::string new_uri);
    void appendUri(const std::string &new_string);

	// HostName
    std::string getHostname() const;
	void setHostname(std::string new_hostname);

	// Port
	std::string getPort() const;
	void setPort(std::string new_port);

	// Method
    std::vector<HttpMethod> getMethod() const;
    void setMethod(const std::vector<std::string> new_method, HttpHelper &http_helper);

	// Root
	std::string getRoot() const;
	void setRoot(std::string new_root);

	// Prefix
	std::string getPath() const;
	void setPath(std::string new_path);

	// Regex flag
    bool getRegex() const;
    void setRegex(bool new_regex);

	// Index
	std::string getIndex() const;
	void setIndex(std::string new_index);

	// getResponseGenerator
	IResponseGenerator *getResponseGenerator(void) const;

    //void (*handler)(IRequest *, IResponse *);

	// Operator overloads
    bool operator<(const Route &other) const;
    Route &operator=(const Route &other);

private:
    std::string	m_host_name;
    std::string	m_path;
	std::string	m_port;
    std::string	m_uri;
	std::string	m_index;
    bool		m_regex_flag;
    std::string   m_root;
    std::vector<HttpMethod>  m_methods;
    IResponseGenerator *m_response_generator;
};

// Router class using std::map for route storage
class Router : public IRouter
{
public:
    Router(IConfiguration &Configuration, ILogger &logger,
           HttpHelper m_http_helper);
    ~Router();
    Triplet_t           execRoute(IRequest *req, IResponse *res);
    std::vector<IRoute*>  getRoutes(void) const;
    size_t              getRouteCount(void) const;

private:
    void m_createServerRoutes(IConfiguration *server_block);
    void m_createRoutes(IConfiguration *server_block);
    IConfiguration &m_configuration;
    std::vector<IRoute*> m_routes;
    ILogger &m_logger;
    HttpHelper m_http_helper;
};

#endif

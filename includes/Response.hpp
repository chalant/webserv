#ifndef RESPONSE_HPP
#define RESPONSE_HPP

/*Response: Contains the elements of the response*/

#include <string>
#include <map>
#include "constants/RequestHelper.hpp"

class Response
{
private:
    std::string _status_line;
    std::string _body;
    std::map<HttpHeader, std::string> _headers;

public:
    Response();
    Response(const Response &copy);
    Response &operator=(const Response &other);
    ~Response();
    std::string get_status_line();
    std::string get_headers();
    std::string get_body();
    void set_status_line(std::string status_line);
    void set_headers(std::string headers);
    void set_body(std::string body);
    std::string getStatusCodeString() const { return "200"; };
    std::string getResponseSizeString() const { return "0"; };
    std::map<std::string, std::string> getHeadersString() const { return std::map<std::string, std::string>(); };
};

#endif // RESPONSE_HPP
// Path: includes/Response.hpp

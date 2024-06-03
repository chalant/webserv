#include "../../includes/response/Route.hpp"

// Constructor
Route::Route(const std::string path, const bool is_regex, const std::vector<HttpMethod> methods, const std::string root,
             const std::string index, const std::string cgi_script, IResponseGenerator &response_generator)
    : m_path(path), m_is_regex(is_regex), m_methods(methods),m_root(root), m_index(index), m_cgi_script(cgi_script),
      m_response_generator(response_generator)
{
}

// Destructor
Route::~Route() {}

// Get the path
std::string Route::getPath() const { return m_path; }

// Get the root
std::string Route::getRoot() const { return m_root; }

// Get the index
std::string Route::getIndex() const { return m_index; }

// Get the CGI script
std::string Route::getCgiScript() const { return m_cgi_script; }

// Check if the method is allowed
bool Route::isAllowedMethod(const HttpMethod method) const
{
    for (std::vector<HttpMethod>::const_iterator it = m_methods.begin(); it != m_methods.end(); ++it)
    {
        if (*it == method)
            return true;
    }
    return false;
}

// Check if the path is a regex
bool Route::isRegex() const { return m_is_regex; }

// Get the response generator
IResponseGenerator *Route::getResponseGenerator() const
{
    return &m_response_generator;
}

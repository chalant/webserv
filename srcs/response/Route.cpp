#include "../../includes/response/Route.hpp"

// Constructor
Route::Route(const std::string path, const std::string root,
             IResponseGenerator &responseGenerator)
    : _path(path), _root(root), _responseGenerator(responseGenerator)
{
}

// Destructor
Route::~Route() {}

// Get the path
std::string Route::getPath() const { return this->_path; }

// Get the root
std::string Route::getRoot() const { return this->_root; }

// Get the response generator
IResponseGenerator *Route::getResponseGenerator() const
{
    return &this->_responseGenerator;
}

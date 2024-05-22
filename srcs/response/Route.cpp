#include "../../includes/response/Route.hpp"

// Constructor
Route::Route(const std::string path, const std::string root,
             const std::string index, IResponseGenerator &response_generator)
    : m_path(path), m_root(root), m_index(index),
      m_response_generator(response_generator)
{
}

// Destructor
Route::~Route() {}

// Get the path
std::string Route::getPath() const { return this->m_path; }

// Get the root
std::string Route::getRoot() const { return this->m_root; }

// Get the index
std::string Route::getIndex() const { return this->m_index; }

// Get the response generator
IResponseGenerator *Route::getResponseGenerator() const
{
    return &this->m_response_generator;
}

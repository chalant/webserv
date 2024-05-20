#ifndef DEFAULTMATCHER_HPP
#define DEFAULTMATCHER_HPP

#include "IURIMatcher.hpp"

class DefaultMatcher : public IURIMatcher
{
private:
    const std::string &m_path;

public:
    DefaultMatcher(const std::string &path);
    ~DefaultMatcher();
    bool match(const std::string &uri);
};

#endif // DEFAULTMATCHER_HPP

#ifndef URIMATCHER_HPP
#define URIMATCHER_HPP

#include <string>

class IURIMatcher
{
public:
	virtual ~IURIMatcher(){};
	virtual bool match(const std::string &uri) = 0;
};

#endif // URIMATCHER_HPP

#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

#include <string>
#include <map>

class Defaults
{
private:
	std::map<const std::string, std::string> m_pairs;

public:
	Defaults();
	~Defaults();
	const std::string &operator[](const std::string &key);
};

#endif // DEFAULTS_HPP

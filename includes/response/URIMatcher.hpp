#ifndef URIMATCHER_HPP
#define URIMATCHER_HPP

#include <string>
#include <vector>

class IURIMatcher
{
public:
    virtual ~IURIMatcher() {};
    virtual bool match(const std::string &uri) = 0;
};

class DefaultMatcher : public IURIMatcher
{
	private:
		const std::string &m_path;

	public:
		DefaultMatcher(const std::string &path): m_path(path) {}
		~DefaultMatcher() {}
		bool match(const std::string &uri) { return uri.find(m_path) != std::string::npos; }
};

class ExtensionMatcher : public IURIMatcher
{
	private:
		const std::vector<std::string> &m_extensions;

	public:
		ExtensionMatcher(const std::vector<std::string> &extensions): m_extensions(extensions) {}
		~ExtensionMatcher() {}
		bool match(const std::string &uri)
		{
			size_t last_dot = uri.find_last_of('.');
			std::string extension = uri.substr(last_dot + 1);
			for (size_t i = 0; i < m_extensions.size(); i++) { if (extension == m_extensions[i]) { return true; } }
			return false;
		}
};

#endif // URIMATCHER_HPP

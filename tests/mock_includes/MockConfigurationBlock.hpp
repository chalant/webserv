#ifndef MOCKCONFIGURATIONBLOCK_HPP
#define MOCKCONFIGURATIONBLOCK_HPP

/*
 * MockConfigurationBlock class is a mock implementation of the IConfiguration interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IConfiguration interface
 * during unit testing without relying on the actual implementation.
 */

#include "configuration/IConfiguration.hpp" // Include the base interface header
#include "logger/ILogger.hpp"                 // Include the logger interface header
#include <stdexcept>                        // Include the standard exception header

class MockConfigurationBlock : public IConfiguration
{
private:
    std::map<std::string, std::vector<IConfiguration *> > _blocks;
    std::map<std::string, std::vector<std::string> *> _directives;
    ILogger &_logger;
    const std::string _name;

public:
    // Constructor
    MockConfigurationBlock(ILogger &logger, const std::string name);

    // Destructor
    virtual ~MockConfigurationBlock();

    // TEST methods
    void setBlock(const std::string &parameter, IConfiguration *block);
    void setString(const std::string &parameter, const std::string &value);
    void setSize_t(const std::string &parameter, size_t value);
    void setStringVector(const std::string &parameter, const std::vector<std::string> &value);
    void setInt(const std::string &parameter, int value);

    // Methods implementing the IConfiguration interface
    // These methods are overridden to provide custom behavior for testing.

    virtual const std::vector<IConfiguration *> &getBlocks(const std::string &parameter) const;
    virtual const std::vector<std::string> &getStringVector(const std::string &parameter) const;
    virtual const std::string &getString(const std::string &parameter, size_t index = 0) const;
    virtual int getInt(const std::string &parameter, size_t index = 0) const;
    virtual size_t getSize_t(const std::string &parameter, size_t index = 0) const;
    virtual bool getBool(const std::string &parameter, size_t index = 0) const;
    virtual void print(size_t depth) const;
    virtual const std::string &getName() const;
};

#endif // MOCKCONFIGURATIONBLOCK_HPP
// Path: tests/mock_headers/MockLogger.hpp

#ifndef MOCKCONFIGURATION_HPP
#define MOCKCONFIGURATION_HPP

/*
 * MockConfiguration class is a mock implementation of the IConfiguration interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IConfiguration interface
 * during unit testing without relying on the actual implementation.
 */

#include "configuration/IConfiguration.hpp" // Include the base interface header
#include "configuration/IBlock.hpp"         // Include the IBlock header
#include <stdexcept>                        // Include the standard exception header

class MockConfiguration : public IConfiguration
{
private:
    std::map<std::string, std::vector<IBlock *>> _blocks;
    std::map<std::string, std::string> _strings;
    std::map<std::string, std::vector<std::string>> _stringVectors;
    std::map<std::string, int> _ints;
    std::map<std::string, size_t> _size_ts;
    std::map<std::string, bool> _bools;

public:
    // Constructor
    MockConfiguration();

    // Destructor
    virtual ~MockConfiguration();

    // Methods implementing the IConfiguration interface
    // These methods are overridden to provide custom behavior for testing.

    // Setters
    virtual void setInt(const std::string &parameter, int value);

    // Getters
    virtual const std::vector<IBlock *> getBlocks(const std::string &parameter) const;
    virtual const std::vector<std::string> getStringVector(const std::string &parameter) const;
    virtual const std::string getString(const std::string &parameter) const;
    virtual int getInt(const std::string &parameter) const;
    virtual size_t getSize_t(const std::string &) const;
    virtual bool getBool(const std::string &parameter) const;
};

#endif // MOCKCONFIGURATION_HPP
// Path: tests/mock_headers/MockLogger.hpp

#ifndef MOCKSESSION_HPP
#define MOCKSESSION_HPP

/*
 * MockSession class
 *
 * This class is a mock implementation of the ISession interface.
 * It is used for testing purposes.
 *
 */

#include "connection/ISession.hpp"

class MockSession : public ISession
{
public:
    // Destructor
    virtual ~MockSession();

    virtual void touch();
    virtual bool hasExpired() const;
    virtual void setData(const std::string &, const std::string &);
    virtual std::string getData(const std::string &) const;
};

#endif // MOCKSESSION_HPP
// Path: tests/mock_srcs/MockSession.cpp

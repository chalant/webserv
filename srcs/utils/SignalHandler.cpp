#include "../../includes/utils/SignalHandler.hpp"
#include <csignal>
#include <iostream>
#include "../../includes/exception/WebservExceptions.hpp"

SignalHandler::SignalHandler()
{
	m_sigint_received = false;
}

SignalHandler::~SignalHandler()
{
}

void SignalHandler::m_sigintHandler(int param, siginfo_t*info, void *context)
{
	static_cast<void>(param);
	static_cast<void>(info);
	SignalHandler *handler = static_cast<SignalHandler *>(context);
	if (!handler)		
		exit (1); // Should not happen
	handler->m_sigint_received = true;
}


void SignalHandler::sigint()
{
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = m_sigintHandler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void SignalHandler::checkState()
{
	if (m_sigint_received)
		throw SigintException();
}

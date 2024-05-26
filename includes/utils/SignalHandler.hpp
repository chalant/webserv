#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP

#include <csignal>

class SignalHandler
{
	private:
		static void m_sigintHandler(int param, siginfo_t*info, void *context);
		bool m_sigint_received;
	public:
		SignalHandler();
		~SignalHandler();

		void	sigint();
		void checkState();
};

#endif

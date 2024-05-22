#include "../../includes/utils/SignalHandler.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <csignal>
#include <iostream>

void siginthandler(int param)
{
	static_cast<void>(param);
	std::cout << "Exiting Webserv.. " << std::endl;
	std::exit(1);
}


void SignalHandler::sigint()
{
	std::signal(SIGINT, siginthandler);
}



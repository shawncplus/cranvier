#include <anachronism/nvt.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <exception>

#include "includes/server.hpp"

Cranvier::Server* server;

void handle_server_event(telnet_nvt* nvt, telnet_event* event)
{
	server->on_event(nvt, event);
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: cran <port>\n";
			return 1;
		}

		server = new Cranvier::Server(boost::lexical_cast<unsigned int>(argv[1]), &handle_server_event);
		server->start();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

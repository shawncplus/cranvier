#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <exception>
#include <iostream>

#include "includes/mud.hpp"

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: cran <port>\n";
			return 1;
		}

		boost::asio::io_service service;
		server = new Cranvier::Server(boost::lexical_cast<unsigned int>(argv[1]), service);//, &handle_server_event);

		service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

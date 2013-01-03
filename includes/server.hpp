#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <list>
#include <anachronism/nvt.h>

using boost::asio::ip::tcp;
using namespace boost::assign;
typedef boost::shared_ptr<tcp::socket> socket_ptr;

namespace Cranvier
{
	class Server
	{
		private:
			unsigned int port;
			boost::asio::io_service* service;
			tcp::acceptor* acc;
			void (*event_callback)(telnet_nvt *, telnet_event *);

//		std::list players;
		public:
			static std::map<unsigned char, const char*> TELNET_OPS;

			Server(unsigned int port, void (*event_callback)(telnet_nvt *, telnet_event *)) : port( port ), event_callback ( event_callback ) { }

			void start();
			void session(socket_ptr sock);
			void on_event(telnet_nvt* nvt, telnet_event* event);
	};
}

#ifndef CRANVIER_SERVER_H
#define CRANVIER_SERVER_H

#include <boost/asio.hpp>

#include "session.hpp"
#include "player.hpp"

using boost::asio::ip::tcp;

namespace Cranvier
{
	class Server
	{
		private:
			unsigned int _port;
			boost::asio::io_service& _service;
			tcp::acceptor _acc;
			std::vector<Cranvier::Player*> _players;

			void _start_accept();
			void _accept(Cranvier::Session* session, const boost::system::error_code& error);

		public:
			Server(unsigned int port, boost::asio::io_service& service) :
				_port( port ),
				_service ( service ),
				_acc(service, tcp::endpoint(tcp::v4(), port))
			{
				this->_start_accept();
			}
	};
}

#endif

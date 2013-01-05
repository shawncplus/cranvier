#include <boost/bind.hpp>

#include "includes/server.hpp"

/**
 * Bind for the socket's thread
 * @param socket_ptr sock
 * @return void
 */
void Cranvier::Server::_accept(Cranvier::Session* session, const boost::system::error_code& error)
{
	if (!error)
	{
		session->enter();
		Cranvier::Player* player = new Cranvier::Player(session);
		this->_players.push_back(player);
	}
	else
	{
		delete session;
	}

	this->_start_accept();
}

/**
 * Start the server
 * @return void
 */
void Cranvier::Server::_start_accept()
{
	Cranvier::Session* session = new Cranvier::Session(this->_service);
	this->_acc.async_accept(session->getSocket(), boost::bind(&Server::_accept, this, session, boost::asio::placeholders::error));
}


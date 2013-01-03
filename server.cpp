#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <list>
#include <anachronism/nvt.h>

#include "includes/server.hpp"

using boost::asio::ip::tcp;
using namespace boost::assign;

typedef boost::shared_ptr<tcp::socket> socket_ptr;


/**
 * Util map for debug of telnet ops
 */
std::map <unsigned char, const char*> Cranvier::Server::TELNET_OPS = map_list_of<unsigned char, const char*>
	(IAC_SE, "IAC_SE")
	(IAC_NOP, "IAC_NOP")
	(IAC_DM, "IAC_DM")
	(IAC_BRK, "IAC_BRK")
	(IAC_IP, "IAC_IP")
	(IAC_AO, "IAC_AO")
	(IAC_AYT, "IAC_AYT")
	(IAC_EC, "IAC_EC")
	(IAC_EL, "IAC_EL")
	(IAC_GA, "IAC_GA")
	(IAC_SB, "IAC_SB")
	(IAC_WILL, "IAC_WILL")
	(IAC_WONT, "IAC_WONT")
	(IAC_DO, "IAC_DO")
	(IAC_DONT, "IAC_DONT")
	(IAC_IAC, "IAC_IAC")
;

/**
 * Start the server
 * @return void
 */
void Cranvier::Server::start()
{

	this->service = new boost::asio::io_service;
	this->acc = new tcp::acceptor(*this->service, tcp::endpoint(tcp::v4(), this->port));
	for (;;)
	{
		socket_ptr sock(new tcp::socket(*this->service));
		this->acc->accept(*sock);
		boost::thread t(boost::bind(&Server::session, this, sock));
	}
}

/**
 * Bind for the socket's thread
 * @param socket_ptr sock
 * @return void
 */
void Cranvier::Server::session(socket_ptr sock)
{
	const int max_length = 1024;

	try
	{
		// Create an NVT
		telnet_nvt* nvt = telnet_nvt_new(&sock, this->event_callback, NULL, NULL);
		for (;;)
		{
			char data[max_length] = "";


			boost::system::error_code error;
			size_t length = sock->read_some(boost::asio::buffer(data), error);

			telnet_receive(nvt, (const telnet_byte*)data, strlen(data), NULL);

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.
			std::cout << data << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

/**
 * Handle a telnet event
 * @param telnet_nvt* nvt
 * @param telnet_event* event
 * @return void
 */
void Cranvier::Server::on_event(telnet_nvt* nvt, telnet_event* event)
{
	switch (event->type)
	{
		// A data event (normal text received)
		case TELNET_EV_DATA:
			{
				telnet_data_event* ev = (telnet_data_event*)event;
				printf("[IN]: %.*s\n", ev->length, ev->data);
				break;
			}

			// Outgoing data emitted by the NVT
		case TELNET_EV_SEND:
			{
				telnet_send_event* ev = (telnet_send_event*)event;
				printf("[OUT]: %.*s\n", ev->length, ev->data);
				break;
			}
		case TELNET_EV_COMMAND:
			{
				telnet_command_event* ev = (telnet_command_event*)event;
				printf("[COMMAND]: %s\n", Cranvier::Server::TELNET_OPS[ev->command]);
				switch (ev->command)
				{
					case IAC_IP:
						socket_ptr sock;
						telnet_get_userdata(nvt, (void**)&sock);
						sock->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
						sock->close();
						// Free the NVT
						telnet_nvt_free(nvt);
						break;
				}
				break;
			}
	}
}

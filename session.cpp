#include <anachronism/nvt.h>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr.hpp>
#include <cstdlib>
#include <map>
#include <vector>

#include <iostream>

#include "includes/session.hpp"
#include "includes/interp.hpp"

using boost::asio::ip::tcp;

/**
 * Hackety hack
 */
void Cranvier::Session::telnet_on_event(telnet_nvt* nvt, telnet_event* event)
{
	Session* newsession = NULL;
	telnet_get_userdata(nvt, (void**)&newsession);
	newsession->on_event(event);
}


/**
 * Entry point for the connection, attempt to read some data in
 * @return void
 */
void Cranvier::Session::enter()
{
	this->_socket.async_read_some(boost::asio::buffer(this->_data, BUFFER_MAX),
		boost::bind(&Cranvier::Session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

/**
 * Simple write halper
 * @param char* data
 * @return void
 */
void Cranvier::Session::write(std::string data)
{
	if (data.length() && data.length() < BUFFER_MAX)
	{
		boost::asio::async_write(this->_socket, boost::asio::buffer(data, data.length()),
			boost::bind(&Cranvier::Session::handle_write, this, boost::asio::placeholders::error)
		);
	}
}

/**
 * ... handle reads, what else did you think this would do
 * @param boost::system::error_code& error Errors attached to the read
 * @param size_t transferred Number of bytes transfered
 * @return void
 */
void Cranvier::Session::handle_read(const boost::system::error_code& error, size_t transferred)
{
	if (!error)
	{
		telnet_receive(this->_nvt, (const telnet_byte*)this->_data, transferred, NULL);
	}
	else
	{
		delete this;
	}
}

/**
 * Handle any writes to the player otherwise flush the connection
 * @param boost::system::error_code& error Errors attached to the write
 * @return void
 */
void Cranvier::Session::handle_write(const boost::system::error_code& error)
{
	if (!error)
	{
		this->_socket.async_read_some(boost::asio::buffer(this->_data, BUFFER_MAX),
			boost::bind(&Cranvier::Session::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
	else
	{
		delete this;
	}
}

/**
 * Handle a telnet event
 * @param telnet_nvt* nvt
 * @param telnet_event* event
 * @return void
 */
void Cranvier::Session::on_event(telnet_event* event)
{
	switch (event->type)
	{
	// A data event (normal text received)
	case TELNET_EV_DATA:
		{
			telnet_data_event* ev = (telnet_data_event*)event;
			std::cout << "[IN]: [" << ev->data << "]\n";
			std::stringstream outstream;
			outstream << ev->data;
			std::string evdata = outstream.str();


			this->write(evdata);
			try
			{
				Cranvier::CommandInterpreter command_interpreter;
//				command_interpreter.registerCommand("shout", boost::bind(&testCommand));
//				command_interpreter.parseInput(evdata);
			}
			catch (std::runtime_error& err)
			{
				std::cerr << "[BAD COMMAND]: " << err.what() << std::endl;
			}
			catch (std::exception& exc)
			{
				std::cerr << "[GENERAL ERROR]: " << exc.what() << std::endl;
			}

			break;
		}

		// Outgoing data emitted by the NVT
	case TELNET_EV_SEND:
		{
			telnet_send_event* ev = (telnet_send_event*)event;
			std::cout << "[OUT]:" << ev->data << std::endl;
			break;
		}
	case TELNET_EV_COMMAND:
		{
			telnet_command_event* ev = (telnet_command_event*)event;
			std::cout << "[COMMAND]:" << Cranvier::TELNET_OPS[ev->command] << std::endl;
			break;
		}
	case TELNET_EV_WARNING:
		{
			telnet_warning_event* ev = (telnet_warning_event*)event;
			std::cout << "[WARNING]:" <<  ev->message << std::endl;
			break;
		}
	}
}

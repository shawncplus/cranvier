#ifndef CRANVIER_SESSION_H
#define CRANVIER_SESSION_H

#include <anachronism/nvt.h>
#include <boost/asio.hpp>
#include <boost/assign/list_of.hpp>
#include <map>

using boost::asio::ip::tcp;

namespace Cranvier
{
	static std::map<unsigned char, const char*> TELNET_OPS = boost::assign::map_list_of<unsigned char, const char*>
		(IAC_SE,   "IAC_SE")
		(IAC_NOP,  "IAC_NOP")
		(IAC_DM,   "IAC_DM")
		(IAC_BRK,  "IAC_BRK")
		(IAC_IP,   "IAC_IP")
		(IAC_AO,   "IAC_AO")
		(IAC_AYT,  "IAC_AYT")
		(IAC_EC,   "IAC_EC")
		(IAC_EL,   "IAC_EL")
		(IAC_GA,   "IAC_GA")
		(IAC_SB,   "IAC_SB")
		(IAC_WILL, "IAC_WILL")
		(IAC_WONT, "IAC_WONT")
		(IAC_DO,   "IAC_DO")
		(IAC_DONT, "IAC_DONT")
		(IAC_IAC,  "IAC_IAC")
	;

	/**
	 * Session class which represents a connection, gets assigned to a player if successfully connected
	 */
	class Session
	{
		private:
			tcp::socket _socket;
			telnet_nvt* _nvt;
			enum { BUFFER_MAX = 1024 };
			char _data[BUFFER_MAX];


		public:
			Session(boost::asio::io_service& io_service) : _socket(io_service)
			{
				this->_nvt = telnet_nvt_new(this, &Cranvier::Session::telnet_on_event, NULL, NULL);
			}

			/**
			 * Hackety hack
			 */
			static void telnet_on_event(telnet_nvt* nvt, telnet_event* event);


			/**
			 * Returns the socket bound to this connection
			 * @return boost::asio::ip::tcp::socket&
			 */
			tcp::socket& getSocket() { return this->_socket; }

			/**
			 * Entry point for the connection, attempt to read some data in
			 * @return void
			 */
			void enter();

			/**
			 * Simple write halper
			 * @param char* data
			 * @return void
			 */
			void write(std::string data);

			/**
			 * ... handle reads, what else did you think this would do
			 * @param boost::system::error_code& error Errors attached to the read
			 * @param size_t transferred Number of bytes transfered
			 * @return void
			 */
			void handle_read(const boost::system::error_code& error, size_t transferred);

			/**
			 * Handle any writes to the player otherwise flush the connection
			 * @param boost::system::error_code& error Errors attached to the write
			 * @return void
			 */
			void handle_write(const boost::system::error_code& error);

			/**
			 * Handle a telnet event
			 * @param telnet_nvt* nvt
			 * @param telnet_event* event
			 * @return void
			 */
			void on_event(telnet_event* event);
	};
}
#endif

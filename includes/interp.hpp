#ifndef CRANVIER_COMMANDINTERP_H
#define CRANVIER_COMMANDINTERP_H

#include <boost/function.hpp>
#include <exception>
#include <map>
#include <vector>

#include "player.hpp"

namespace Cranvier
{
	class CommandNotFoundException : public std::exception { };
	class CommandInterpreter
	{
		typedef boost::function<void (Cranvier::Player*, std::vector<std::string>)> command;
		typedef std::map<std::string, command> commandmap;
		private:
			commandmap _command_list;

		public:
			void registerCommand(std::string commandname, command commandFunction);

			void parseInput(std::string, Cranvier::Player*);
	};
}

#endif

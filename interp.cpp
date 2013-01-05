#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>
#include <exception>
#include <map>
#include <utility>
#include <vector>

#include "includes/interp.hpp"

void Cranvier::CommandInterpreter::registerCommand(std::string commandname, Cranvier::CommandInterpreter::command commandFunction)
{
	this->_command_list.insert(std::make_pair(commandname, commandFunction));
}

void Cranvier::CommandInterpreter::parseInput(std::string input, Cranvier::Player* player)
{
	boost::algorithm::trim(input);
	if (input.empty())
	{
		return;
	}

	std::vector<std::string> command_args;
	boost::split(command_args, input, boost::is_any_of(" \t\n\r"));

	if (!command_args.size())
	{
		return;
	}

	std::string command_search = command_args[0];
	command_args.erase(command_args.begin());

	CommandInterpreter::commandmap::iterator it = this->_command_list.find(command_search);
	if (it == this->_command_list.end())
	{
		throw CommandNotFoundException();
	}

	it->second(player, command_args);
}

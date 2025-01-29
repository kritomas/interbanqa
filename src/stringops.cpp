#include "stringops.hpp"
#include <sstream>

std::vector<std::string> parseCommand(const std::string& command)
{
	std::istringstream stream(command);

	std::vector<std::string> arguments;
	std::string arg;
	while (stream >> arg)
	{
		arguments.emplace_back(arg);
	}
	return arguments;
}
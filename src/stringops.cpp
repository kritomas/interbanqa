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
std::string reassembeCommand(const std::vector<std::string>& arguments)
{
	if (arguments.size() <= 0)
	{
		return "";
	}
	std::string cmd = arguments[0];
	for (int index = 1; index < arguments.size(); ++index)
	{
		cmd += " " + arguments[index];
	}
	return cmd;
}

std::vector<std::string> splitString(const std::string& string, const std::string& with)
{
	std::vector<std::string> res;

	size_t startIndex = 0, endIndex = 0;

	while (startIndex < string.size() && endIndex < string.size())
	{
		endIndex = string.find(with, startIndex);
		if (endIndex != startIndex)
		{
			res.push_back(string.substr(startIndex, endIndex - startIndex));
		}
		startIndex = endIndex + with.size();
	}

	return res;
}
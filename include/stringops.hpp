#ifndef STRINGOPS_HPP
#define STRINGOPS_HPP

#include <vector>
#include <string>

std::vector<std::string> parseCommand(const std::string& command);
std::string reassembeCommand(const std::vector<std::string>& arguments);

std::vector<std::string> splitString(const std::string& string, const std::string& with);

#endif
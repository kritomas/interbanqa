#ifndef STRINGOPS_HPP
#define STRINGOPS_HPP

#include <vector>
#include <string>

std::vector<std::string> parseCommand(const std::string& command);

std::vector<std::string> splitString(const std::string& string, const std::string& with);

#endif
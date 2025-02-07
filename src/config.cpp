#include "config.hpp"
#include <fstream>
#include "boost/regex.hpp"
#include "exception.hpp"
#include "json.hpp"

const char CONFIG_PATH[] = "config.json";
const char CONFIG_MIN_PORT_NAME[] = "min_port";
const char CONFIG_MAX_PORT_NAME[] = "max_port";
const char CONFIG_PORT_NAME[] = "port";
const char CONFIG_ADDRESS_NAME[] = "address";
const char CONFIG_PREFIX_LENGTH_NAME[] = "prefix";
const char CONFIG_TIMEOUT_NAME[] = "timeout";

namespace config
{
	int MIN_PORT = 0;
	int MAX_PORT = 0;
	int PORT = 0;
	std::string ADDRESS = "0.0.0.0";
	int PREFIX_LENGTH = 24;
	double TIMEOUT = 5;
}

void initConfig()
{
	using json = nlohmann::json;
	std::ifstream file(CONFIG_PATH);
	if (!file.is_open())
	{
		throw InterbanqaException("Couldn't open config");
	}
	json raw;
	try
	{
		raw = json::parse(file);
	}
	catch (...)
	{
		throw InterbanqaException("Invalid JSON");
	}

	if (!raw.contains(CONFIG_MIN_PORT_NAME))
	{
		throw InterbanqaException("Config entry min_port not found");
	}
	if (!raw.contains(CONFIG_MAX_PORT_NAME))
	{
		throw InterbanqaException("Config entry max_port not found");
	}
	if (!raw.contains(CONFIG_PORT_NAME))
	{
		throw InterbanqaException("Config entry port not found");
	}
	if (!raw.contains(CONFIG_ADDRESS_NAME))
	{
		throw InterbanqaException("Config entry address not found");
	}
	if (!raw.contains(CONFIG_PREFIX_LENGTH_NAME))
	{
		throw InterbanqaException("Config entry prefix not found");
	}
	if (!raw.contains(CONFIG_TIMEOUT_NAME))
	{
		throw InterbanqaException("Config entry timeout not found");
	}

	if (!raw[CONFIG_MIN_PORT_NAME].is_number_unsigned())
	{
		throw InterbanqaException("Config entry min_port must be an unsigned integer");
	}
	if (!raw[CONFIG_MAX_PORT_NAME].is_number_unsigned())
	{
		throw InterbanqaException("Config entry max_port must be an unsigned integer");
	}
	if (!raw[CONFIG_PORT_NAME].is_number_unsigned())
	{
		throw InterbanqaException("Config entry port must be an unsigned integer");
	}
	if (raw[CONFIG_PORT_NAME] < raw[CONFIG_MIN_PORT_NAME] || raw[CONFIG_PORT_NAME] > raw[CONFIG_MAX_PORT_NAME])
	{
		throw InterbanqaException("port must be between min_port and max_port");
	}
	if (!raw[CONFIG_ADDRESS_NAME].is_string())
	{
		throw InterbanqaException("Config entry address must be a string");
	}
	boost::regex addr_regex("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
	if (!boost::regex_match((std::string)raw[CONFIG_ADDRESS_NAME], addr_regex))
	{
		throw InterbanqaException("Config entry address must be an IPv4 address");
	}
	if (!raw[CONFIG_PREFIX_LENGTH_NAME].is_number_unsigned())
	{
		throw InterbanqaException("Config entry prefix must be an unsigned integer");
	}
	if (raw[CONFIG_PREFIX_LENGTH_NAME] < 0 || raw[CONFIG_PREFIX_LENGTH_NAME] > 32)
	{
		throw InterbanqaException("Config entry prefix must be 0-32");
	}
	if (!raw[CONFIG_TIMEOUT_NAME].is_number())
	{
		throw InterbanqaException("Config entry timeout must be a number");
	}

	config::MIN_PORT = raw[CONFIG_MIN_PORT_NAME];
	config::MAX_PORT = raw[CONFIG_MAX_PORT_NAME];
	config::PORT = raw[CONFIG_PORT_NAME];
	config::ADDRESS = raw[CONFIG_ADDRESS_NAME];
	config::PREFIX_LENGTH = raw[CONFIG_PREFIX_LENGTH_NAME];
	config::TIMEOUT = raw[CONFIG_TIMEOUT_NAME];
}
#include "config.hpp"
#include <fstream>
#include <stdexcept>
#include "json.hpp"

const char CONFIG_PATH[] = "config.json";
const char CONFIG_PORT_NAME[] = "port";
const char CONFIG_ADDRESS_NAME[] = "address";
const char CONFIG_TIMEOUT_NAME[] = "timeout";

namespace config
{
	int PORT = 0;
	std::string ADDRESS = "0.0.0.0";
	double TIMEOUT = 5;
}

void initConfig()
{
	using json = nlohmann::json;
	std::ifstream file(CONFIG_PATH);
	if (!file.is_open())
	{
		throw std::runtime_error("Couldn't open config");
	}
	json raw;
	try
	{
		raw = json::parse(file);
	}
	catch (...)
	{
		throw std::runtime_error("Invalid JSON");
	}

	if (!raw.contains(CONFIG_PORT_NAME))
	{
		throw std::runtime_error("Config entry port not found");
	}
	if (!raw.contains(CONFIG_ADDRESS_NAME))
	{
		throw std::runtime_error("Config entry address not found");
	}
	if (!raw.contains(CONFIG_TIMEOUT_NAME))
	{
		throw std::runtime_error("Config entry timeout not found");
	}

	if (!raw[CONFIG_PORT_NAME].is_number_unsigned())
	{
		throw std::runtime_error("Config entry port must be an unsigned integer");
	}
	if (!raw[CONFIG_ADDRESS_NAME].is_string())
	{
		throw std::runtime_error("Config entry address must be a string");
	}
	if (!raw[CONFIG_TIMEOUT_NAME].is_number())
	{
		throw std::runtime_error("Config entry timeout must be a number");
	}

	config::PORT = raw[CONFIG_PORT_NAME];
	config::ADDRESS = raw[CONFIG_ADDRESS_NAME];
	config::TIMEOUT = raw[CONFIG_TIMEOUT_NAME];
}
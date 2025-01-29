#include "config.hpp"
#include <fstream>
#include <stdexcept>
#include "json.hpp"

const char CONFIG_PATH[] = "config.json";
const char CONFIG_PORT_NAME[] = "port";

namespace config
{
	int PORT;
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

	if (!raw[CONFIG_PORT_NAME].is_number_unsigned())
	{
		throw std::runtime_error("Config entry port must be an unsigned integer");
	}

	config::PORT = raw[CONFIG_PORT_NAME];
}
#ifndef CONFIG_JSON
#define CONFIG_JSON

#include <string>

namespace config
{
	extern int PORT;
	extern std::string ADDRESS;
}

void initConfig();

#endif
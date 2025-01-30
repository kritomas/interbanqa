#ifndef CONFIG_JSON
#define CONFIG_JSON

#include <string>

namespace config
{
	extern int PORT;
	extern std::string ADDRESS;
	extern double TIMEOUT;
}

void initConfig();

#endif
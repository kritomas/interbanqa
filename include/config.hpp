#ifndef CONFIG_JSON
#define CONFIG_JSON

#include <string>

namespace config
{
	extern int PORT;
	extern std::string ADDRESS;
	extern int NETBITS;
	extern double TIMEOUT;
}

/**
 * This must be executed before any server is started.
 */
void initConfig();

#endif
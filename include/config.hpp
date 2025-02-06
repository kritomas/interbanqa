#ifndef CONFIG_JSON
#define CONFIG_JSON

#include <string>

namespace config
{
	extern int MIN_PORT;
	extern int MAX_PORT;
	extern int PORT;
	extern std::string ADDRESS;
	extern int PREFIX_LENGTH;
	extern double TIMEOUT;
}

/**
 * This must be executed before any server is started.
 */
void initConfig();

#endif
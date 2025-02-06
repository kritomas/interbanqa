#include "database/singleton.hpp"
#include "log.hpp"
#include "server.hpp"
#include "config.hpp"
#include <string>

int main(int argc, char* argv[])
{
	runtime_log.start("runtime.log");
	runtime_log.log("Initializing Interbanqa", LOG_INFO);
	try
	{
		initConfig();

		DBSingleton::instance();

		Server server;
		server.start();
	}
	catch (const std::exception& error)
	{
		std::cout << "Critical error: " << error.what() << std::endl;
		runtime_log.log((std::string)"Critical error: " + error.what(), LOG_ERROR);
		return -1;
	}

	return 0;
}
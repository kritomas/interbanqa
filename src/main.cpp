#include "database/singleton.hpp"
#include "log.hpp"
#include "server.hpp"
#include "config.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
	runtime_log.start("runtime.log");
	runtime_log.log("Initializing Interbanqa", LOG_INFO);

	initConfig();

	DBSingleton::instance();

	Server server;
	server.start();

	return 0;
}
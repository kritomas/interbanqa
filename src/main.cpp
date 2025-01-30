#include "database/singleton.hpp"
#include "server.hpp"
#include "config.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
	initConfig();

	DBSingleton::instance();

	Server server;
	server.start();

	std::string cmd;
	while (server.running)
	{
		std::getline(std::cin, cmd);

		if (cmd == "exit") server.running = false;

		if (std::cin.eof()) server.running = false;
	}

	return 0;
}
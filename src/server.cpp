#include "server.hpp"
#include <thread>
#include <iostream>
#include "config.hpp"
#include "log.hpp"
#include "stringops.hpp"
#include "database/account.hpp"

Server::~Server()
{
	std::cout << "Server terminating" << std::endl;
	runtime_log.log("Server terminating", LOG_INFO);
	connection.close();
}

void Server::start()
{
	connection.host(config::ADDRESS, config::PORT);
	std::cout << "Server hosted at " << config::ADDRESS << " port " << config::PORT << std::endl;

	runtime_log.log("Interbanqa server listening at " + config::ADDRESS + ", port " + std::to_string(config::PORT), LOG_INFO);
	std::string cmd;
	bool running = true;
	while (running)
	{
		std::getline(std::cin, cmd);

		if (cmd == "exit") running = false;

		if (std::cin.eof()) running = false;
	}
}
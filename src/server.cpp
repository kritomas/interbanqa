#include "server.hpp"
#include <thread>
#include <iostream>
#include "config.hpp"

void runThread(Server* server)
{
	server->run();
}

void Server::run()
{
	while (running)
	{
		// TODO
	}
}

Server::~Server()
{
	running = false;
	thread.join();
	std::cout << "Server terminating" << std::endl;
}

void Server::start()
{
	running = true;
	connection.host(config::ADDRESS, config::PORT);
	std::cout << "Server hosted at " << config::ADDRESS << " port " << config::PORT << std::endl;

	thread = std::thread(runThread, this);
}
#include "server.hpp"
#include <thread>
#include <iostream>
#include "config.hpp"
#include "stringops.hpp"

void runThread(Server* server)
{
	server->run();
}

void Server::respond(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
	connection.send(message + "\r\n", socket);
}

void Server::bankCode(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
	respond("BC " + config::ADDRESS, socket);
}

void Server::run()
{
	while (running)
	{
		while (connection.pending() > 0)
		{
			Packet packet = connection.next();
			auto socket = packet.socket();
			std::vector<std::string> arguments = parseCommand(packet.data());
			if (arguments.size() <= 0) continue;
			if (commands.count(arguments[0]))
			{
				(this->*commands[arguments[0]])(arguments, socket);
			}
			else
			{
				respond("ER Command not found", socket);
			}
		}
	}
}

Server::Server()
{
	commands["BC"] = &Server::bankCode;
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
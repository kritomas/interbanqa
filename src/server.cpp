#include "server.hpp"
#include <thread>
#include <iostream>
#include "config.hpp"
#include "stringops.hpp"
#include "database/account.hpp"

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
void Server::accountCreate(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
	Account account = Account::create();
	respond("AC " + std::to_string(account.number()) + "/" + config::ADDRESS, socket);
}
void Server::accountDeposit(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
	if (arguments.size() < 3)
	{
		throw std::runtime_error("Not enough arguments");
	}
	auto raw_addr = splitString(arguments[1], "/");
	if (raw_addr.size() < 2)
	{
		throw std::runtime_error("Illegal address");
	}
	if (raw_addr[1] == config::ADDRESS)
	{
		int number = std::stoi(raw_addr[0]);
		Account account = Account::get(number);
		account.deposit(std::stoll(arguments[2]));
		respond("AD", socket);
	}
	else
	{
		throw std::runtime_error("Not implemented"); // TODO
	}
}
void Server::accountWithdrawal(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
	if (arguments.size() < 3)
	{
		throw std::runtime_error("Not enough arguments");
	}
	auto raw_addr = splitString(arguments[1], "/");
	if (raw_addr.size() < 2)
	{
		throw std::runtime_error("Illegal address");
	}
	if (raw_addr[1] == config::ADDRESS)
	{
		int number = std::stoi(raw_addr[0]);
		Account account = Account::get(number);
		account.withdraw(std::stoll(arguments[2]));
		respond("AW", socket);
	}
	else
	{
		throw std::runtime_error("Not implemented"); // TODO
	}
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
			try
			{
				if (commands.count(arguments[0]))
				{
					(this->*commands[arguments[0]])(arguments, socket);
				}
				else
				{
					throw std::runtime_error("Command not found");
				}
			}
			catch (const std::exception& e)
			{
				respond((std::string)"ER " + e.what(), socket);
			}
		}
	}
}

Server::Server()
{
	commands["BC"] = &Server::bankCode;
	commands["AC"] = &Server::accountCreate;
	commands["AD"] = &Server::accountDeposit;
	commands["AW"] = &Server::accountWithdrawal;
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
#include "server.hpp"
#include <thread>
#include <iostream>
#include "config.hpp"
#include "log.hpp"
#include "stringops.hpp"
#include "database/account.hpp"

void runThread(Server* server)
{
	server->run();
}

void Server::respond(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
	runtime_log.log("Response to " + socket->remote_endpoint().address().to_string() + ": " + message, LOG_INFO);
	connection.send(message + "\r\n", socket);
}

std::string Server::bankCode(const std::vector<std::string>& arguments)
{
	return "BC " + config::ADDRESS;
}
std::string Server::accountCreate(const std::vector<std::string>& arguments)
{
	Account account = Account::create();
	return "AC " + std::to_string(account.number()) + "/" + config::ADDRESS;
}
std::string Server::accountDeposit(const std::vector<std::string>& arguments)
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
		return "AD";
	}
	else
	{
		throw std::runtime_error("Not implemented"); // TODO
	}
}
std::string Server::accountWithdrawal(const std::vector<std::string>& arguments)
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
		return "AW";
	}
	else
	{
		throw std::runtime_error("Not implemented"); // TODO
	}
}
std::string Server::accountBalance(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 2)
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
		return "AB " + std::to_string(account.balance());
	}
	else
	{
		throw std::runtime_error("Not implemented"); // TODO
	}
}
std::string Server::accountRemove(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 2)
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
		Account::remove(number);
		return "AR";
	}
	else
	{
		throw std::runtime_error("Not implemented"); // TODO
	}
}
std::string Server::bankTotalAmount(const std::vector<std::string>& arguments)
{
	return "BA " + std::to_string(Account::funds());
}
std::string Server::bankNumberOfClients(const std::vector<std::string>& arguments)
{
	return "BN " + std::to_string(Account::count());
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
			std::string cmdToLog = arguments[0];
			for (int index = 1; index < arguments.size(); ++index)
			{
				cmdToLog += " " + arguments[index];
			}
			runtime_log.log("Request from " + socket->remote_endpoint().address().to_string() + ": " + cmdToLog, LOG_INFO);
			try
			{
				if (commands.count(arguments[0]))
				{
					std::future<std::string> awaited_response = std::async(std::launch::async, commands[arguments[0]], this, arguments);
					if (awaited_response.wait_for(std::chrono::milliseconds((int)(1000*config::TIMEOUT))) == std::future_status::timeout)
					{
						throw std::runtime_error("Timed out");
					}
					respond(awaited_response.get(), socket);
				}
				else
				{
					throw std::runtime_error("Command not found");
				}
			}
			catch (const std::exception& e)
			{
				runtime_log.log("When handling request for " + socket->remote_endpoint().address().to_string() + ": " + e.what(), LOG_ERROR);
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
	commands["AB"] = &Server::accountBalance;
	commands["AR"] = &Server::accountRemove;
	commands["BA"] = &Server::bankTotalAmount;
	commands["BN"] = &Server::bankNumberOfClients;
}

Server::~Server()
{
	running = false;
	thread.join();
	std::cout << "Server terminating" << std::endl;
	runtime_log.log("Server terminating", LOG_INFO);
}

void Server::start()
{
	running = true;
	connection.host(config::ADDRESS, config::PORT);
	std::cout << "Server hosted at " << config::ADDRESS << " port " << config::PORT << std::endl;

	runtime_log.log("Interbanqa server listening at " + config::ADDRESS + ", port " + std::to_string(config::PORT), LOG_INFO);
	thread = std::thread(runThread, this);
}
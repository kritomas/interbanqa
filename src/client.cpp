#include "client.hpp"
#include "networking/socket.hpp"
#include "log.hpp"
#include "database/account.hpp"
#include "config.hpp"
#include "stringops.hpp"

void Client::respond(const std::string& message)
{
	runtime_log.log("Response to " + socket->raw()->remote_endpoint().address().to_string() + ": " + message, LOG_INFO);
	socket->send(message + "\r\n");
}

std::string Client::bankCode(const std::vector<std::string>& arguments)
{
	return "BC " + config::ADDRESS;
}
std::string Client::accountCreate(const std::vector<std::string>& arguments)
{
	Account account = Account::create();
	return "AC " + std::to_string(account.number()) + "/" + config::ADDRESS;
}
std::string Client::accountDeposit(const std::vector<std::string>& arguments)
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
std::string Client::accountWithdrawal(const std::vector<std::string>& arguments)
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
std::string Client::accountBalance(const std::vector<std::string>& arguments)
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
std::string Client::accountRemove(const std::vector<std::string>& arguments)
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
std::string Client::bankTotalAmount(const std::vector<std::string>& arguments)
{
	return "BA " + std::to_string(Account::funds());
}
std::string Client::bankNumberOfClients(const std::vector<std::string>& arguments)
{
	return "BN " + std::to_string(Account::count());
}

std::unordered_map<std::string, std::string(*)(const std::vector<std::string>& arguments)> Client::commands;

Client::Client(std::shared_ptr<Socket> socket)
{
	this->socket = socket;

	commands["BC"] = &Client::bankCode;
	commands["AC"] = &Client::accountCreate;
	commands["AD"] = &Client::accountDeposit;
	commands["AW"] = &Client::accountWithdrawal;
	commands["AB"] = &Client::accountBalance;
	commands["AR"] = &Client::accountRemove;
	commands["BA"] = &Client::bankTotalAmount;
	commands["BN"] = &Client::bankNumberOfClients;
}

void Client::run()
{
	while (socket->isOpen())
	{
		while (socket->pending() > 0)
		{
			Packet packet = socket->next();
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
					std::future<std::string> awaited_response = std::async(std::launch::async, commands[arguments[0]], arguments);
					if (awaited_response.wait_for(std::chrono::milliseconds((int)(1000*config::TIMEOUT))) == std::future_status::timeout)
					{
						throw std::runtime_error("Timed out");
					}
					respond(awaited_response.get());
				}
				else
				{
					throw std::runtime_error("Command not found");
				}
			}
			catch (const std::exception& e)
			{
				runtime_log.log("When handling request for " + socket->remote_endpoint().address().to_string() + ": " + e.what(), LOG_ERROR);
				respond((std::string)"ER " + e.what());
			}
		}
	}
}
#include "client.hpp"
#include <thread>
#include "bank.hpp"
#include "exception.hpp"
#include "networking/connection.hpp"
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

std::string actuallyForwardRequest(const std::string& cmd, std::string address, int port)
{
	Connection connection;
	connection.connectV4(address, std::to_string(port));
	connection.send(cmd + "\r\n");
	while (connection.pending() <= 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	Packet response = connection.next();
	std::string answer = reassembeCommand(parseCommand(response.data()));
	runtime_log.log("Received '" + answer + "' from " + address + ", port " + std::to_string(port), LOG_INFO);
	return answer;
}

std::string Client::forwardRequest(const std::vector<std::string>& arguments, std::string address)
{
	std::string cmd = reassembeCommand(arguments);
	runtime_log.log("Forwarding request '" + cmd + "' to " + address, LOG_WARNING);
	std::vector<std::future<std::string>> requests;
	requests.reserve(config::MAX_PORT - config::MIN_PORT + 1);
	for (int port = config::MIN_PORT; port <= config::MAX_PORT; ++port)
	{
		requests.emplace_back(std::async(std::launch::async, actuallyForwardRequest, cmd, address, port));
	}
	const std::chrono::time_point start = std::chrono::system_clock::now();
	const std::chrono::time_point end = start + std::chrono::milliseconds((int)(1000 * config::TIMEOUT));
	for (auto& r : requests)
	{
		try
		{
			auto status = r.wait_until(end);
			if (status == std::future_status::timeout) InterbanqaException("Timeout");
			else return r.get();
		}
		catch (const boost::wrapexcept<boost::system::system_error>& error)
		{
			switch (error.code().value())
			{
				case boost::asio::error::host_unreachable:
				case boost::asio::error::connection_refused:
					break;
				default:
					throw;
			}
		}
	}
	throw InterbanqaException("Bank not found");
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
		throw InterbanqaException("Not enough arguments");
	}
	auto raw_addr = splitString(arguments[1], "/");
	if (raw_addr.size() < 2)
	{
		throw InterbanqaException("Illegal address");
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
		return forwardRequest(arguments, raw_addr[1]);
	}
}
std::string Client::accountWithdrawal(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 3)
	{
		throw InterbanqaException("Not enough arguments");
	}
	auto raw_addr = splitString(arguments[1], "/");
	if (raw_addr.size() < 2)
	{
		throw InterbanqaException("Illegal address");
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
		return forwardRequest(arguments, raw_addr[1]);
	}
}
std::string Client::accountBalance(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 2)
	{
		throw InterbanqaException("Not enough arguments");
	}
	auto raw_addr = splitString(arguments[1], "/");
	if (raw_addr.size() < 2)
	{
		throw InterbanqaException("Illegal address");
	}
	if (raw_addr[1] == config::ADDRESS)
	{
		int number = std::stoi(raw_addr[0]);
		Account account = Account::get(number);
		return "AB " + std::to_string(account.balance());
	}
	else
	{
		return forwardRequest(arguments, raw_addr[1]);
	}
}
std::string Client::accountRemove(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 2)
	{
		throw InterbanqaException("Not enough arguments");
	}
	auto raw_addr = splitString(arguments[1], "/");
	if (raw_addr.size() < 2)
	{
		throw InterbanqaException("Illegal address");
	}
	if (raw_addr[1] == config::ADDRESS)
	{
		int number = std::stoi(raw_addr[0]);
		Account::remove(number);
		return "AR";
	}
	else
	{
		return forwardRequest(arguments, raw_addr[1]);
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
std::string Client::robberyPlan(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 2)
	{
		throw InterbanqaException("Not enough arguments");
	}
	long long int target = std::stoll(arguments[1]);

	std::multiset<Bank> banks = Bank::listBanks();

	long long int total = 0;
	for (auto& b : banks)
	{
		total += b.balance;
	}
	if (total < target)
	{
		throw InterbanqaException("Not enough finances in network");
	}

	std::set<std::string> to_rob;
	int clients = 0;
	total = 0;
	for (auto it = banks.rbegin(); it != banks.rend() && total < target; ++it)
	{
		total += it->balance;
		clients += it->clients;
		to_rob.emplace(it->address);
	}

	std::string bank_addrs = "";
	bool do_comma = false;
	for (auto& a : to_rob)
	{
		if (do_comma)
		{
			bank_addrs += ", ";
		}
		bank_addrs += a;
		do_comma = true;
	}

	return "RP for " + std::to_string(total) + "$ (" + std::to_string(clients) + " clients): " + bank_addrs;
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
	//commands["RP"] = &Client::robberyPlan; // BORKED
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
			std::string cmdToLog = reassembeCommand(arguments);
			runtime_log.log("Request from " + socket->remote_endpoint().address().to_string() + ": " + cmdToLog, LOG_INFO);
			try
			{
				if (commands.count(arguments[0]))
				{
					std::future<std::string> awaited_response = std::async(std::launch::async, commands[arguments[0]], arguments);
					auto status = awaited_response.wait_for(std::chrono::milliseconds((int)(1000*config::TIMEOUT)));
					if (status == std::future_status::timeout)
					{
						throw InterbanqaException("Timed out");
					}
					respond(awaited_response.get());
				}
				else
				{
					throw InterbanqaException("Command not found");
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
#include "bank.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <future>
#include "config.hpp"
#include "stringops.hpp"
#include "networking/connection.hpp"

double Bank::balancePerClient() const
{
	if (clients == 0)
	{
		return 0;
	}
	return balance / (double)clients;
}

bool Bank::operator<(const Bank& other) const
{
	return balancePerClient() < other.balancePerClient();
}

Bank fetchBank(std::string address)
{
	Bank res;
	res.address = address;
	std::vector<std::string> response = parseCommand(Client::forwardRequest(parseCommand("BA"), address));
	if (response.size() <= 1) throw std::runtime_error("Invalid response");
	if (response[0] == "ER") throw std::runtime_error("Remote returned error");
	res.balance = std::stoll(response[1]);

	response = parseCommand(Client::forwardRequest(parseCommand("BN"), address));
	if (response.size() <= 1) throw std::runtime_error("Invalid response");
	if (response[0] == "ER") throw std::runtime_error("Remote returned error");
	res.clients = std::stoi(response[1]);
	return res;
}

std::multiset<Bank> Bank::listBanks()
{
	boost::asio::ip::address_v4 addr = boost::asio::ip::make_address_v4(config::ADDRESS);
	boost::asio::ip::network_v4 network = boost::asio::ip::network_v4(addr, config::PREFIX_LENGTH);

	boost::asio::ip::address_v4 first_address = network.address();
	boost::asio::ip::address_v4 last_address = network.broadcast();

	uint32_t first = first_address.to_uint();
	uint32_t last = last_address.to_uint();

	std::vector<std::future<Bank>> requests;
	requests.reserve(last - first);

	for (uint32_t ip = first; ip < last; ++ip)
	{
		boost::asio::ip::address_v4 current_ip(ip);
		requests.emplace_back(std::async(std::launch::async, fetchBank, current_ip.to_string()));
	}

	std::multiset<Bank> res;
	const std::chrono::time_point start = std::chrono::system_clock::now();
	const std::chrono::time_point end = start + std::chrono::milliseconds((int)(150 * config::TIMEOUT));
	for (auto& r : requests)
	{
		try
		{
			auto status = r.wait_until(end);
			if (status == std::future_status::timeout) throw std::runtime_error("Timeout");
			else res.emplace(r.get());
		}
		catch (const std::exception& e)
		{
			// TODO: Log
		}
	}
	return res;
}
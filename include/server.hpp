#ifndef SERVER_HPP
#define SERVER_HPP

#include "networking/connection.hpp"
#include <unordered_map>

class Server
{
private:
	Connection connection;
	std::thread thread;

	void respond(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket>& socket);

	std::string bankCode(const std::vector<std::string>& arguments);
	std::string accountCreate(const std::vector<std::string>& arguments);
	std::string accountDeposit(const std::vector<std::string>& arguments);
	std::string accountWithdrawal(const std::vector<std::string>& arguments);
	std::string accountBalance(const std::vector<std::string>& arguments);
	std::string accountRemove(const std::vector<std::string>& arguments);
	std::string bankTotalAmount(const std::vector<std::string>& arguments);
	std::string bankNumberOfClients(const std::vector<std::string>& arguments);

	std::unordered_map<std::string, std::string (Server::*)(const std::vector<std::string>& arguments)> commands;

	void run();

	friend void runThread(Server* server);
public:
	bool running = false;

	Server();

	~Server();

	void start();
};

#endif
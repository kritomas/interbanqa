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

	void bankCode(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket);
	void accountCreate(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket);
	void accountDeposit(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket);
	void accountWithdrawal(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket);
	void accountBalance(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket);
	void accountRemove(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket);

	std::unordered_map<std::string, void (Server::*)(const std::vector<std::string>& arguments, std::shared_ptr<boost::asio::ip::tcp::socket>& socket)> commands;

	void run();

	friend void runThread(Server* server);
public:
	bool running = false;

	Server();

	~Server();

	void start();
};

#endif
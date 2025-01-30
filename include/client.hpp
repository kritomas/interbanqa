#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class Socket;

class Client
{
private:
	std::shared_ptr<Socket> socket;

	void respond(const std::string& message);

	static std::string forwardRequest(const std::vector<std::string>& arguments, std::string address);

	static std::string bankCode(const std::vector<std::string>& arguments);
	static std::string accountCreate(const std::vector<std::string>& arguments);
	static std::string accountDeposit(const std::vector<std::string>& arguments);
	static std::string accountWithdrawal(const std::vector<std::string>& arguments);
	static std::string accountBalance(const std::vector<std::string>& arguments);
	static std::string accountRemove(const std::vector<std::string>& arguments);
	static std::string bankTotalAmount(const std::vector<std::string>& arguments);
	static std::string bankNumberOfClients(const std::vector<std::string>& arguments);

	static std::unordered_map<std::string, std::string(*)(const std::vector<std::string>& arguments)> commands;

public:
	Client(std::shared_ptr<Socket> socket);

	void run();
};

#endif
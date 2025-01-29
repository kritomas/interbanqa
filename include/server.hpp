#ifndef SERVER_HPP
#define SERVER_HPP

#include "networking/connection.hpp"

class Server
{
private:
	Connection connection;
	std::thread thread;

	void run();

	friend void runThread(Server* server);
public:
	bool running = false;

	~Server();

	void start();
};

#endif
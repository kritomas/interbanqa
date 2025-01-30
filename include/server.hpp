#ifndef SERVER_HPP
#define SERVER_HPP

#include "networking/connection.hpp"

class Server
{
private:
	Connection connection;
public:
	~Server();

	void start();
};

#endif
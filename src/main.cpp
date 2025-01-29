#include <iostream>
#include "networking/connection.hpp"

int main(int argc, char* argv[])
{
	Connection connection;
	int port = std::stoi(argv[1]);
	connection.hostV4(port);
	std::cout << "Running on: " << port << std::endl;

	bool running = true;
	while (running)
	{
		while (connection.pending() > 0)
		{
			Packet packet = connection.next();

			std::cout << "Incoming: " << packet.data() << std::endl;

			if (packet.data() == "exit")
			{
				connection.send("Server shutting down\r\n", packet.socket());
				running = false;
			}
		}
	}

	connection.close();

	return 0;
}
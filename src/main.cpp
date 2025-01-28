#include <iostream>
#include "kge2D/networking/connection.hpp"

int main(int argc, char* argv[])
{
	kge2D::tcp::Connection connection;
	int port = std::stoi(argv[1]);
	connection.hostV4(port);
	std::cout << "Running on: " << port << std::endl;

	bool running = true;
	while (running)
	{
		while (connection.pending() > 0)
		{
			kge2D::tcp::Packet packet = connection.next();

			std::cout << "Incoming: " << packet.data() << std::endl;

			if (packet.data() == "exit\r")
			{
				connection.send("Server shutting down\r\n", packet.socket());
				running = false;
			}
		}
	}

	connection.close();

	return 0;
}
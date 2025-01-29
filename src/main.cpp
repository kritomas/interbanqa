#include "server.hpp"
#include "config.hpp"
#include <thread>

int main(int argc, char* argv[])
{
	initConfig();

	Server server;
	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return 0;
}
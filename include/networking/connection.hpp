/*
 *	This file was taken out of Kritos Game Engine 2D, and subsequently modified
 *	for use in interbanqa.
 *
 *	MIT License
 *
 *	Copyright (c) 2024 kritomas
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *
 *	Source code: https://gitlab.com/kritomas/kritos-game-engine-2d
 */

#ifndef NETWORKING_CONNECTION_HPP
#define NETWORKING_CONNECTION_HPP

#include "networking/acceptor.hpp"
#include "networking/socket.hpp"

/**
 * A class for handling TCP network connections.
 */
class Connection
{
private:
	std::shared_ptr<Socket> socket;
	std::shared_ptr<Acceptor> acceptor;

public:
	~Connection();

	void close();

	/**
	 * @return The amount of pending received packets, waiting for processing.
	 */
	size_t pending();
	/**
	 * @return The next packet to be processed, which is REMOVED FROM THE QUEUE.
	 */
	Packet next();

	void hostV4(int port);
	void hostV6(int port);
	void host(const std::string& address, int port);
	void connectV4(std::string ip, std::string port);
	void connectV6(std::string ip, std::string port);

	void send(Buffer buffer, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
	void send(Buffer buffer);
};
#endif

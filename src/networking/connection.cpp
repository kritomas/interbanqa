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

#include "networking/connection.hpp"
#include "exception.hpp"

Connection::~Connection()
{
	close();
}

void Connection::close()
{
	if (socket != nullptr)
	{
		socket->close();
		socket = nullptr;
	}
	if (acceptor != nullptr)
	{
		acceptor->close();
		acceptor = nullptr;
	}
}

size_t Connection::pending()
{
	size_t res = 0;
	if (socket != nullptr)
	{
		res += socket->pending();
	}
	if (acceptor != nullptr)
	{
		res += acceptor->pending();
	}
	return res;
}
Packet Connection::next()
{
	if (socket != nullptr)
	{
		return socket->next();
	}
	if (acceptor != nullptr)
	{
		return acceptor->next();
	}
	throw InterbanqaException("error");
}

void Connection::hostV4(int port)
{
	close();
	acceptor.reset(new Acceptor);
	acceptor->hostV4(port);
}
void Connection::hostV6(int port)
{
	close();
	acceptor.reset(new Acceptor);
	acceptor->hostV6(port);
}
void Connection::host(const std::string& address, int port)
{
	close();
	acceptor.reset(new Acceptor);
	acceptor->host(address, port);
}
void Connection::connectV4(std::string ip, std::string port)
{
	close();
	socket.reset(new Socket);
	socket->connectV4(ip, port);
}
void Connection::connectV6(std::string ip, std::string port)
{
	close();
	socket.reset(new Socket);
	socket->connectV6(ip, port);
}

void Connection::send(Buffer buffer, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
	socket->send(boost::asio::buffer(buffer.data(), buffer.size()));
}
void Connection::send(Buffer buffer)
{
	socket->send(buffer);
}
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

#include "kge2D/networking/socket.hpp"
#include <string>
#include <thread>

namespace tcp
{
	void socketReceiveThread(kge2D::tcp::Socket* socket)
	{
		socket->receive();
	}
}

kge2D::tcp::Socket::~Socket()
{
	close();
}

void kge2D::tcp::Socket::close()
{
	receiving = false;
	if (socket != nullptr)
	{
		socket->close();
	}
	incomingLocker.unlock();
	internalLocker.lock();
	if (receiveThread.joinable())
	{
		receiveThread.join();
	}
	internalLocker.unlock();
}
void kge2D::tcp::Socket::receive()
{
	Buffer receiveBuffer;

	while (receiving)
	{
		try
		{
			if (socket != nullptr && socket->is_open())
			{
				internalLocker.lock();
				boost::asio::streambuf buffer;
				try
				{
					//socket->non_blocking(false);
					boost::asio::read_until(*socket, buffer, '\n');
					//socket->non_blocking(true);
					std::getline(std::istream(&buffer), receiveBuffer);
				}
				catch (...)
				{
					internalLocker.unlock();
					throw;
				}
				internalLocker.unlock();

				Packet incomingPacket(receiveBuffer, socket);
				incomingLocker.lock();
				incomingPackets.push_back(incomingPacket);
				incomingLocker.unlock();
			}
			else
			{
				receiving = false;
			}
		}
		catch (const boost::wrapexcept<boost::system::system_error>& error)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			// Silently ignore
		}
	}
}

void kge2D::tcp::Socket::start()
{
	socket->non_blocking(true);
	receiving = true;
	receiveThread = std::thread(::tcp::socketReceiveThread, this);
}

bool kge2D::tcp::Socket::isOpen() const
{
	return (socket != nullptr && socket->is_open());
}

size_t kge2D::tcp::Socket::pending()
{
	incomingLocker.lock();
	size_t res = incomingPackets.size();
	incomingLocker.unlock();
	return res;
}
kge2D::tcp::Packet kge2D::tcp::Socket::next()
{
	incomingLocker.lock();
	Packet res = incomingPackets[0];
	incomingPackets.erase(incomingPackets.begin());
	incomingLocker.unlock();
	return res;
}

void kge2D::tcp::Socket::accept(boost::asio::ip::tcp::acceptor& acceptor)
{
	close();
	if (socket == nullptr)
	{
		socket.reset(new boost::asio::ip::tcp::socket(ioContext));
	}
	acceptor.accept(*socket);
	start();
}
void kge2D::tcp::Socket::connectV4(std::string ip, std::string port)
{
	close();
	if (socket == nullptr)
	{
		socket.reset(new boost::asio::ip::tcp::socket(ioContext));
	}
	boost::asio::ip::tcp::resolver resolver(ioContext);
	boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(boost::asio::ip::tcp::v4(), ip, port);
	boost::asio::connect(*socket, endpoints);
	start();
}
void kge2D::tcp::Socket::connectV6(std::string ip, std::string port)
{
	close();
	if (socket == nullptr)
	{
		socket.reset(new boost::asio::ip::tcp::socket(ioContext));
	}
	boost::asio::ip::tcp::resolver resolver(ioContext);
	boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(boost::asio::ip::tcp::v6(), ip, port);
	boost::asio::connect(*socket, endpoints);
	start();
}

void kge2D::tcp::Socket::send(kge2D::Buffer buffer)
{
	size_designator outgoingSize = buffer.size();
	internalLocker.lock();
	socket->send(boost::asio::buffer(&outgoingSize, sizeof(outgoingSize)));
	socket->send(boost::asio::buffer(buffer.data(), buffer.size()));
	internalLocker.unlock();
}
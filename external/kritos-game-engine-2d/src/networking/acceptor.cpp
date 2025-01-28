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

#include "kge2D/networking/acceptor.hpp"
#include <thread>

void acceptorV4Thread(kge2D::tcp::Acceptor* acceptor)
{
	acceptor->hostV4Thread();
}
void acceptorV6Thread(kge2D::tcp::Acceptor* acceptor)
{
	acceptor->hostV6Thread();
}

kge2D::tcp::Acceptor::Acceptor() : acceptor(ioContext)
{

}

kge2D::tcp::Acceptor::~Acceptor()
{
	close();
}

void kge2D::tcp::Acceptor::close()
{
	accepting = false;
	internalLocker.lock();
	acceptor.close();
	internalLocker.unlock();
	acceptorLocker.lock();
	for (auto& s : sockets)
	{
		if (s != nullptr) s->close();
	}
	acceptorLocker.unlock();
	if (acceptorThread.joinable())
	{
		acceptorThread.join();
	}
	sockets.clear();
	_port = 0;
}

int kge2D::tcp::Acceptor::port() const
{
	return _port;
}

void kge2D::tcp::Acceptor::update()
{
	acceptorLocker.lock();
	for (auto it = sockets.begin(); it != sockets.end();)
	{
		std::shared_ptr<Socket> s = *it;
		if (s->isOpen())
		{
			while (s->pending() > 0)
			{
				incomingPackets.push_back(s->next());
			}

			++it;
		}
		else
		{
			it = sockets.erase(it);
		}
	}
	acceptorLocker.unlock();
}

size_t kge2D::tcp::Acceptor::pending()
{
	update();
	size_t res = incomingPackets.size();
	return res;
}
kge2D::tcp::Packet kge2D::tcp::Acceptor::next()
{
	update();
	Packet res = incomingPackets[0];
	incomingPackets.erase(incomingPackets.begin());
	return res;
}

void kge2D::tcp::Acceptor::hostV4Thread()
{
	while (accepting)
	{
		acceptedSocket.reset(new Socket());
		try
		{
			if (acceptor.is_open())
			{
				internalLocker.lock();
				try
				{
					acceptedSocket->accept(acceptor);
				}
				catch (...)
				{
					internalLocker.unlock();
					throw;
				}
				internalLocker.unlock();
				acceptorLocker.lock();
				sockets.push_back(acceptedSocket);
				acceptorLocker.unlock();
			}
			else
			{
				accepting = false;
			}
		}
		catch (const boost::wrapexcept<boost::system::system_error>& error)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			// Silently ignore
		}
	}
}
void kge2D::tcp::Acceptor::hostV6Thread()
{
	while (accepting)
	{
		acceptedSocket.reset(new Socket());
		try
		{
			if (acceptor.is_open())
			{
				internalLocker.lock();
				try
				{
					acceptedSocket->accept(acceptor);
				}
				catch (...)
				{
					internalLocker.unlock();
					throw;
				}
				internalLocker.unlock();
				acceptorLocker.lock();
				sockets.push_back(acceptedSocket);
				acceptorLocker.unlock();
			}
			else
			{
				accepting = false;
			}
		}
		catch (const boost::wrapexcept<boost::system::system_error>& error)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			// Silently ignore
		}
	}
}

void kge2D::tcp::Acceptor::hostV4(int port)
{
	close();
	_port = port;
	accepting = true;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), _port);
	acceptor.open(endpoint.protocol());
	acceptor.bind(endpoint);
	acceptor.listen();
	acceptor.non_blocking(true);
	acceptorThread = std::thread(acceptorV4Thread, this);
}
void kge2D::tcp::Acceptor::hostV6(int port)
{
	close();
	_port = port;
	accepting = true;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v6(), _port);
	acceptor.open(endpoint.protocol());
	acceptor.bind(endpoint);
	acceptor.listen();
	acceptor.non_blocking(true);
	acceptorThread = std::thread(acceptorV6Thread, this);
}
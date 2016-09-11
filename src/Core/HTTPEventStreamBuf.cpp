#include "HTTPEventStreamBuf.h"
#include <algorithm>

using namespace boost::asio;

namespace ge::Core
{
	HTTPEventStreamBuf::HTTPEventStreamBuf(io_service &ioService)
		: ioService(ioService)
		, acceptor(ioService)
		, acceptorSocket(ioService)
		, line("data: ")
	{
	}

	HTTPEventStreamBuf::HTTPEventStreamBuf(io_service &ioService, Port port)
		: HTTPEventStreamBuf(ioService)
	{
		open(port.port);
	}

	HTTPEventStreamBuf::~HTTPEventStreamBuf()
	{
		if(is_open())
		{
			close();
		}
	}

	auto HTTPEventStreamBuf::open(uint16_t port) -> HTTPEventStreamBuf *
	{
		boost::system::error_code ec;
		auto endpoint = ip::tcp::endpoint(ip::tcp::v4(), port);
		acceptor.open(endpoint.protocol(), ec);
		acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
		// acceptor.set_option(ip::v6_only(false));	// use IPv4 and IPv6
		acceptor.bind(endpoint);

		if(ec != boost::system::errc::success)
		{
			return nullptr;
		}

		acceptor.listen();
		onAccept();
		return this;
	}

	auto HTTPEventStreamBuf::is_open() const -> bool
	{
		return acceptor.is_open();
	}

	auto HTTPEventStreamBuf::close() -> HTTPEventStreamBuf *
	{
		acceptor.close();
		return this;
	}

	// std::streambuf interface
	auto HTTPEventStreamBuf::xsputn(const char *s, std::streamsize n) -> std::streamsize
	{
		auto end = std::find(s, s + n, '\n');
		line.append(s, end);
		
		const auto newlineInserted = end != s + n;
		if(newlineInserted)
		{
			// Deep copy so network writes can continue in the background
			auto finalStr = std::make_shared<std::string>(line + '\n');
			{
				std::unique_lock<std::mutex> lock(mtx);	// Prevent modicications of client list
				for(auto &client : sockets)
				{
					// Copy the shared_ptr, so deletion happens when possible
					async_write(client, buffer(*finalStr), [finalStr](auto &, auto) {});
				}
			}

			line = "data: ";
		}

		return end - s;
	}

	auto HTTPEventStreamBuf::overflow(int c) -> int
	{
		char tmp = c;
		xsputn(&tmp, 1);
		return c;
	}

	void HTTPEventStreamBuf::onAccept()
	{
		acceptor.async_accept(acceptorSocket, [this](boost::system::error_code ec) {
			if(!ec)
			{
				std::unique_lock<std::mutex> lock(mtx);
				sockets.emplace_back(std::move(acceptorSocket));
				auto s = sockets.end() - 1;

				// Fake a HTTP Response to inform the client that this is a text/event-stream
				// There is no guarantee this works because no request has been sent
				const char response[] =
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/event-stream\r\n"
					"Access-Control-Allow-Origin: *\r\n\r\n";
				async_write(*s, buffer(response), [](auto &, auto) {});
				async_read(*s, null_buffers(), [s, this](auto ec, size_t) { return this->onRead(s, ec); });
			}
			onAccept();
		});
	}

	void HTTPEventStreamBuf::onRead(SocketContainer::iterator socket, boost::system::error_code ec)
	{
		if(ec == boost::asio::error::connection_reset)
		{
			sockets.erase(socket);
		}
		async_read(*socket, null_buffers(),
				   [socket, this](auto ec, size_t) { return this->onRead(socket, ec); });
	}
}

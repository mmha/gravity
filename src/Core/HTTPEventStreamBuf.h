#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/container/stable_vector.hpp>
#include <mutex>
#include <streambuf>
#if __has_include(<fmt/format.h>)
	#include <fmt/format.h>
#endif

namespace ge::Core
{
	/** This class creates a very primitive HTTP SSE stream.
	 *  Warning: Not HTTP compliant (does not really parse incoming data, it just assumes the client sent a GET request), but should work in practice.
	 */
	class HTTPEventStreamBuf final : public std::streambuf
	{
		using Socket = boost::asio::ip::tcp::socket;
		using SocketContainer = boost::container::stable_vector<Socket>;

		public:
		struct Port
		{
			uint16_t port;
		};

		explicit HTTPEventStreamBuf(boost::asio::io_service &ioService);
		HTTPEventStreamBuf(boost::asio::io_service &ioService, Port port);
		~HTTPEventStreamBuf() override;

		auto open(uint16_t port) -> HTTPEventStreamBuf *;
		auto is_open() const -> bool;
		auto close() -> HTTPEventStreamBuf *;

		#if __has_include(<fmt/format.h>)
		template <typename... Args>
		void fmt(fmt::CStringRef format, Args &&... args)
		{
			const auto str = fmt::format(format, std::forward<Args>(args)...);
			xsputn(str.c_str(), str.size());
		}
		#endif

		protected:
		// std::streambuf interface
		auto xsputn(const char *s, std::streamsize n) -> std::streamsize override;
		auto overflow(int c = EOF) -> int override;

		private:
		void onAccept();
		void onRead(SocketContainer::iterator socket, boost::system::error_code ec);

		boost::asio::io_service &ioService;
		boost::asio::ip::tcp::acceptor acceptor;
		Socket acceptorSocket;
		boost::container::stable_vector<Socket> sockets;

		std::string line;
		std::mutex mtx;
	};
}

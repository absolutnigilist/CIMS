#include "session.hpp"
#include "handler.hpp"
#include <glog/logging.h>


using boost::asio::ip::tcp;

Session::Session(tcp::socket socket):
 socket_(std::move(socket)){}

void Session::start() {
	do_read();
}
void Session::do_read() {
	auto self = shared_from_this();
	boost::asio::async_read_until(
		socket_, readbuf_, '\n',
		[this, self](boost::system::error_code ec, std::size_t)
		{
			if (ec)
			{
				if (ec == boost::asio::error::eof ||
					ec == boost::asio::error::connection_reset ||
					ec == boost::asio::error::operation_aborted)
				{
					// Ќормальное завершение Ч клиент отключилс€
					LOG(INFO) << "Client disconnected";
				}

				else
				{
					std::cerr << "Read error: " << ec.message() << "\n";
				}
				return; //---прекращаем сессию (shared_ptr сам очиститс€)
			}
			std::istream is(&readbuf_);
			std::string line;
			std::getline(is, line);						// '\n' уже вычитан из буфера
			if (!line.empty() && line.back() == '\r')
			{
				line.pop_back();						// на случай CRLF от клиента
			}
			std::string response = Handler::handle(line, queue_);
			do_write(response);
		});
}
void Session::do_write(const std::string& message) {
	
	auto self = shared_from_this();
	auto buffer = std::make_shared<std::string>(message);
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(*buffer),
		[this, self, buffer](boost::system::error_code ec, std::size_t)
		{
			if (!ec)
			{
				//---ѕосле записи снова читаем
				do_read();
			}
			else
			{
				std::cerr << "Write error: " << ec.message() << "\n";

			}

		});
}
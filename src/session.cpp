#include "session.hpp"

using boost::asio::ip::tcp;

Session::Session(tcp::socket socket):
 socket_(std::move(socket)){}

void Session::start() {
	do_read();
}
void Session::do_read() {
	auto self = shared_from_this();
	socket_.async_read_some(
		boost::asio::buffer(buffer_),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::string data(buffer_.data(), length);
				std::cout << "Received message: " << data << "\n";

				std::string response = "Echo: " + data;
				do_write(response);
			}
			else
			{
				std::cerr << "Read error: " << ec.message() << "\n";
			}
		});
}
void Session::do_write(const std::string& message) {
	auto buffer = std::make_shared<std::string>(message);
	auto self = shared_from_this();
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(*buffer),
		[this, self, buffer](boost::system::error_code ec, std::size_t)
		{
			if (!ec)
			{
				//---После записи снова читаем
				do_read();
			}
			else
			{
				std::cerr << "Write error: " << ec.message() << "\n";

			}

		});
}
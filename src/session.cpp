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
					// ���������� ���������� � ������ ����������
					LOG(INFO) << "Client disconnected";
				}

				else
				{
					std::cerr << "Read error: " << ec.message() << "\n";
				}
				return; //---���������� ������ (shared_ptr ��� ���������)
			}
			std::istream is(&readbuf_);
			std::string line;
			std::getline(is, line);						// '\n' ��� ������� �� ������
			if (!line.empty() && line.back() == '\r')
			{
				line.pop_back();						// �� ������ CRLF �� �������
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
				//---����� ������ ����� ������
				do_read();
			}
			else
			{
				std::cerr << "Write error: " << ec.message() << "\n";

			}

		});
}
#include "server.hpp"

using boost::asio::ip::tcp;

//--------------------------------------------------------------------------------------------
//		����������� �������
//--------------------------------------------------------------------------------------------
Server::Server(boost::asio::io_context& io_context, unsigned short port) :
	io_context_(io_context),
	acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {

	//---�������� ����� ��������
	do_accept();
}
//--------------------------------------------------------------------------------------------
//		������������ �������� �������
//--------------------------------------------------------------------------------------------
void Server::do_accept() {

	acceptor_.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "Client connected: " << socket.remote_endpoint() << "\n";
				std::make_shared<Session>(std::move(socket))->start();
			}
			else
			{
				std::cerr << "Accept error: " << ec.message() << " \n";
			}
			//---���������� ��������� ����� ��������
			do_accept();
		});
}
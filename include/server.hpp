#pragma once

#include <boost/asio.hpp>
#include <memory>
#include "session.hpp"

//--------------------------------------------------------------------------------------------
//		������
//--------------------------------------------------------------------------------------------
class Server {

public:
	//--TCP - ������, ������� ��������� ���������� � ������ Session ��� ������� �������
	Server(boost::asio::io_context& io_context, unsigned short port);

private:
	//---������������ �������� �������
	void do_accept();
	
	//---��������� �������� ����������
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_context& io_context_;
};
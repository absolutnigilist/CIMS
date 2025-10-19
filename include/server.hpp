#pragma once

#include <boost/asio.hpp>
#include <memory>
#include "session.hpp"

//--------------------------------------------------------------------------------------------
//		Сервер
//--------------------------------------------------------------------------------------------
class Server {

public:
	//--TCP - сервер, который принимает соединения и создаёт Session для каждого клиента
	Server(boost::asio::io_context& io_context, unsigned short port);

private:
	//---Ассинхронное ожидание клиента
	void do_accept();
	
	//---Слушатель входящих соединений
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_context& io_context_;
};
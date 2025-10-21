#pragma once

#include <boost/asio.hpp>
#include<memory>
#include <iostream>
#include "queue.hpp"
#include <boost/asio/streambuf.hpp>

//--------------------------------------------------------------------------------------------
//		Session
//--------------------------------------------------------------------------------------------
class Session :public std::enable_shared_from_this<Session> {
public:
	explicit Session(boost::asio::ip::tcp::socket);

	//---Начало обмена сообщениями
	void start();

private:
	//---Асинхронное чтение
	void do_read();
	void do_write(const std::string& messge);

	//---Сокет для общения с клиентом
	boost::asio::ip::tcp::socket socket_;

	//---Буфер для построчного чтения (JSON Lines, \n-делимитер)
	boost::asio::streambuf readbuf_;

	//---Очередь для данного клиента
	Queue<int> queue_;
};
#pragma once
#include <boost/asio.hpp>
#include<memory>
#include <iostream>

//---���� ������ � ���� ������ Session

class Session :public std::enable_shared_from_this<Session> {
public:
	explicit Session(boost::asio::ip::tcp::socket);

	void start();
private:
	//---����������� ������
	void do_read();
	void do_write(const std::string& messge);

	boost::asio::ip::tcp::socket socket_;
	std::array<char, 1024> buffer;

};
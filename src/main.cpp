#include "server.hpp"
#include<iostream>

int main() {
	try
	{
		boost::asio::io_conterxt io_context;
		Server server(io_context, 12345);

		io_context.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;



};
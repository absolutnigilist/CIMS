#include "server.hpp"
#include<iostream>

int main() {
	try
	{
		boost::asio::io_context io_context;
		Server server(io_context, 12345);

		std::cout << "Server started on port 12345...\n";
		io_context.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;



};
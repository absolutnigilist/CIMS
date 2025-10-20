#include "server.hpp"
#include<iostream>
#include <glog/logging.h>

int main(int argc, char* argv[]) {

	// --- ������������� GLOG
	google::InitGoogleLogging(argv[0]);
	google::SetStderrLogging(google::INFO);						//		���������� �� INFO � ���� � stderr
	google::SetLogDestination(google::ERROR, "logs/error_");	//		������ � ���� logs/error_*

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
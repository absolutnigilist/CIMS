#include "server.hpp"
#include<iostream>
#include <glog/logging.h>
#include <filesystem>

namespace fs = std::filesystem;

#ifndef LOG_DIR
#define LOG_DIR "/tmp" 
#endif

int main(int argc, char* argv[]) {

	//---Путь для лог файлов из CMAKE
	fs::path log_dir = LOG_DIR;  

	//---Создаём папку на случай, если она удалена
	std::error_code ec;
	fs::create_directories(log_dir, ec);
	if (ec) std::cerr << "Failed to create log dir: " << ec.message() << "\n";

	FLAGS_logtostderr = false;
	FLAGS_alsologtostderr = 1;				//	Выводить также в консоль
	FLAGS_colorlogtostderr = 1;				//	Цветной вывод в консоль
	FLAGS_max_log_size = 10;				//	10 MB max per file
	FLAGS_stop_logging_if_full_disk = true; //	Не логируем, если диск заполнен
	FLAGS_log_dir = log_dir.string();
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::GLOG_INFO, (log_dir / "info_").string().c_str());

	try
	{
		boost::asio::io_context io_context;
		Server server(io_context, 12345);

		LOG(INFO) << "Server started successfully on port 12345";

		io_context.run();

		LOG(INFO) << "Server stopped gracefully";
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		LOG(ERROR) << "Server exception: " << e.what();
	}
	google::ShutdownGoogleLogging();
	return 0;
}
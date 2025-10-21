#include <glog/logging.h>
#include "handler.hpp"
#include "heapSort.hpp"

std::string Handler::handle(const std::string& input, Queue<int>& queue) {
	try
	{
		//---Парсим строку в JSON
		json::value parsed = json::parse(input);
		if (!parsed.is_object())
		{
			LOG(ERROR) << "Invalid JSON format";
			return makeError("Invalid JSON format");
		}
		const json::object& obj = parsed.as_object();

		//---Проверяем наличие поля cmd
		if (!obj.if_contains("cmd"))
		{
			return makeError("Missing 'cmd' field");
		}
		std::string cmd = obj.at("cmd").as_string().c_str();

		//---Выполняем команду
		if (cmd == "ADD")
		{
			if (!obj.if_contains("values"))
			{
				LOG(ERROR) << "Missing values";
				return makeError("Missing values");
			}
			const auto& arr = obj.at("values").as_array();
			for (auto v : arr) {
				queue.push(v.as_int64());
			}
			return makeOK("Added values");
		}
		else if (cmd == "GET") 
		{
			auto vec = queue.to_vector();
			heapSort(vec);
			json::array arr;
			for (auto v : vec) 
			{
				arr.push_back(v);
			}
			return makeOK("Queue data", { {"values", arr} });
		}
		else if (cmd == "CLEAR")
		{
			queue.clear();
			return makeOK("Queue cleared");

		}
		else
		{
			return makeError("Unknown cmd");
		}
	}
	catch (const std::exception& e)
	{
		return makeError(e.what());
	}

	return "";
}
std::string Handler::makeOK(const std::string& msg, const json::object& data) {
	json::object result = {
		{"status", "ok"},
		{"message", msg},
		{"data", data}
	};
	return json::serialize(result) + "\n";	//	LF-разделитель
}
std::string Handler::makeError(const std::string& msg) {
	json::object result = {
		{"status","error"},
		{"message", msg}
	};
	return json::serialize(result) + "\n";	//	LF-разделитель
}
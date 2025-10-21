#pragma once

#include <boost/json.hpp>
#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>
#include "queue.hpp"

namespace json = boost::json;

class Handler {
public:
	//---Обработка входящего JSON-сообщения
	static std::string handle(const std::string& input, Queue<int>& queue);

private:
	//---Утилиты для формирования ответов
	static std::string makeOK(const std::string& msg, const json::object& data = {});
	static std::string makeError(const std::string& msg);
};

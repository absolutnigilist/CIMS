#pragma once

#include <boost/json.hpp>
#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>

namespace json = boost::json;

class Handler {
public:
	//---��������� ��������� JSON-���������
	static std::string handle(const std::string& input);



	using HandlerFunction = std::function<json::object(const boost::json::object&)>;
};

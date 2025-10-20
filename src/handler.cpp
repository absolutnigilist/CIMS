#include "handler.hpp"

std::string Handler::handle(const std::string& input) {
	try
	{
		//---Парсим строку в JSON
		json::value parsed = json::parse(input);
		if (!parsed.is_object())
		{

		}
	}
	catch (const std::exception&)
	{

	}


}
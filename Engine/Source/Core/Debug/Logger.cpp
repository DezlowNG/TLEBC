/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "Logger.h"

namespace rb
{
	void Logger::Init(std::string path)
	{
		std::string t = path + "log.txt";
		std::remove(t.c_str());
		auto logger = spdlog::basic_logger_mt("log", t.c_str());

		logger->set_pattern("%v");

		logger->info("\t\t\tThank you for staying with us.\n\t\t\t  (c) Yume Games 2020-2021\n");

		logger->set_pattern("[%H:%M:%S] [%l] %v");
	}
}
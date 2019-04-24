#pragma once
#include <string>
#include <ctime>
#include <chrono>


namespace Core 
{
	void error(const std::string& message);
	struct Exception{ std::string message; };
	std::string readFile(const std::string& path);
}
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include "Core.h"

void Core::error(const std::string& message)
{
	MessageBox(NULL, message.c_str(), "Error", MB_ICONERROR);
	std::exit(EXIT_FAILURE);
}

std::string Core::readFile(const std::string& path)
{
	std::ifstream myfile{ path };
	if (!myfile.is_open()) throw Exception{ "Could not open file \"" + path + "\" ." };
	std::stringstream buffer;
	//rdbuf() just returns a pointer to the underlying basic_streambuf object for the given [io]stream object.
	// the operator << has an overload or a pointer to basic_streambuf<...> which writes out the contents of the basic_streambuf<...>.
	buffer << myfile.rdbuf();
	return buffer.str();
}

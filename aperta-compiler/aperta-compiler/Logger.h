#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

namespace aperta_compiler
{
	template<typename T>
	void Print(T value)
	{
		std::cout << value << std::endl;
	}

	void ReadEnter();
}

#endif // LOGGER_H
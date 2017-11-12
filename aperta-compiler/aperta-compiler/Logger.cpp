#include "Logger.h"

namespace aperta_compiler
{
	void ReadEnter()
	{
		Print("Press ENTER to continue...");
		std::cin.get();
	}
}
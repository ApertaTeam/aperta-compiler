#include <cstdlib>
#include "Logger.h"
#include "Lexer.h"
#include <vector>
#include <chrono>

int main()
{
	aperta_compiler::Print("----------------\nAperta compiler\n----------------\n");

	auto started = std::chrono::high_resolution_clock::now();
	std::vector<aperta_compiler::lexer::Token> tokens = aperta_compiler::lexer::GetTokensFromString(std::string("func main[void]()\n{\n\n}"));
	auto ended = std::chrono::high_resolution_clock::now();
	aperta_compiler::Print(std::string("Lexed input in this many milliseconds: ") + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(ended - started).count()));

	for (aperta_compiler::lexer::Token t : tokens)
	{
		aperta_compiler::Print(t.contents);
	}

	aperta_compiler::ReadEnter();
	return EXIT_SUCCESS;
}
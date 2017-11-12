#ifndef LEXER_H
#define LEXER_H

#include <string>

namespace aperta_compiler {

	namespace lexer {

		class Core {

			enum TokenType {
				MUL,
				DIV,
				ADD,
				SUB
			};

			struct Token {
				int line;
				int column;
				TokenType type;
				std::string contents;
			};

			void parseThisShit(int idunnodood);

		};
	}
}

#endif // LEXER_H
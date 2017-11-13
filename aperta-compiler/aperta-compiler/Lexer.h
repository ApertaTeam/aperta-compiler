#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

namespace aperta_compiler
{
	namespace lexer 
	{
		enum TokenType {
			String, // "abc"
			Character, // 'a'
			Number, // 123, 1.23, etc.
			Keyword, // func, etc.
			Identifier, // abc123, _a32, etc. cannot start with a number
			Semicolon, // ;
			ConditonalOp, // ==, !=, >=, <=, ||, &&, <, >
			BinOpEquals, // +=, -=, *=, /=
			BinOp, // +, -, *, /
			Equals, // =
			OpenParen, // (
			CloseParen, // )
			OpenBrack, // [
			CloseBrack, // ]
			OpenBrace, // {
			CloseBrace, // }
			Dot,	// .
			Colon, // :
			Comma, // ,
			Not, // !
			Ampersand, // &
			Tilde // ~
		};

		struct Token {
			int line;
			TokenType type;
			std::string contents;
		};

		extern char* ErrorMessage;

		std::vector<Token> GetTokensFromString(std::string& input);
	}
}

#endif // LEXER_H
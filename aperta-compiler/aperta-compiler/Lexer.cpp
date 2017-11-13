#include "Lexer.h"

namespace aperta_compiler
{
	namespace lexer
	{
		char* ErrorMessage = "";

		inline bool DetectToken(std::string& input, int& pos, char c)
		{
			if (input[pos] == c)
			{
				pos++;
				return true;
			}
			else
			{
				return false;
			}
		}

		inline bool DetectToken(std::string& input, int& pos, char c1, char c2)
		{
			if (input[pos] == c1)
			{
				pos++;
				if (input[pos] == c2) 
				{
					pos++;
					return true;
				}
				else
				{
					pos--;
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		inline bool DetectToken(std::string& input, int& pos, char c1, char c2, char c3)
		{
			if (input[pos] == c1)
			{
				pos++;
				if (input[pos] == c2)
				{
					pos++;
					if (input[pos] == c3)
					{
						pos++;
						return true;
					}
					else
					{
						pos -= 2;
						return false;
					}
				}
				else
				{
					pos--;
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		std::vector<Token> GetTokensFromString(std::string& input)
		{
			std::vector<Token> tokens = std::vector<Token>();
			int line = 1;

			// For safety and ease in detecting unenclosed things.
			input += "\n   ";

			int pos = 0;
			while (pos < (int)input.length())
			{
				// Detect newlines
				if (input[pos] == '\n')
				{
					line++;
				}

				// Skip whitespace
				while (pos < (int)input.length() && iswspace(input[pos]))
				{
					pos++;
				}

				// Single line comment
				if (DetectToken(input, pos, '/', '/'))
				{
					while (pos < (int)input.length() && input[pos] != '\n')
					{
						pos++;
					}
				}
				// Multi line comment
				else if (DetectToken(input, pos, '/', '*'))
				{
					while (pos < (int)input.length() && !DetectToken(input, pos, '*', '/'))
					{
						if (input[pos] == '\n')
							line++;
						pos++;
					}
				}
				// String
				else if (DetectToken(input, pos, '"'))
				{
					std::string contents = "";
					while (pos < (int)input.length() && !DetectToken(input, pos, '"'))
					{
						if (input[pos] == '\\')
						{
							pos++;
							if (input[pos] == '"') {
								contents += '"';
							}
							else if (input[pos] == 'n') {
								contents += 0x0A;
							}
							else if (input[pos] == 'r') {
								contents += 0x0D;
							}
							pos++;
						}
						else
						{
							contents += input[pos];
							if (input[pos] == '\n')
								line++;
							pos++;
						}
					}
					if (pos >= (int)input.length())
					{
						ErrorMessage = "Unenclosed string literal detected.";
						return std::vector<Token>();
					}
					else
					{
						tokens.push_back({ line, String, contents });
					}
				}
				// Character
				else if (DetectToken(input, pos, '\''))
				{
					std::string content = "";
					if (input[pos] == '\\')
					{
						pos++;
						if (input[pos] == '\'') {
							content += '\'';
						}
						else if (input[pos] == 'n') {
							content += 0x0A;
						}
						else if (input[pos] == 'r') {
							content += 0x0D;
						}
						pos++;
					}
					else
					{
						content += input[pos];
						if (input[pos] == '\n')
							line++;
						pos++;
					}
					if (pos >= (int)input.length() || !DetectToken(input, pos, '\''))
					{
						ErrorMessage = "Unenclosed/too large character literal detected.";
						return std::vector<Token>();
					}
					else
					{
						tokens.push_back({ line, Character, content });
					}
				}
				// Number
				else if (isdigit(input[pos]))
				{
					bool usedDot = false;
					std::string contents = "";
					contents += input[pos];
					pos++;
					while (pos < (int)input.length() && (isdigit(input[pos]) || (!usedDot && input[pos] == '.')))
					{
						if (input[pos] == '.')
						{
							if (!usedDot)
							{
								usedDot = true;
							}
							else
							{
								ErrorMessage = "Malformed number.";
								return std::vector<Token>();
							}
						}
						contents += input[pos];
						pos++;
					}
					tokens.push_back({ line, Number, contents });
				}
				// Identifier
				else if (isalpha(input[pos]) || input[pos] == '_')
				{
					std::string contents = "";
					contents += input[pos];
					pos++;
					while (pos < (int)input.length() && (isalpha(input[pos]) || isdigit(input[pos]) || input[pos] == '_'))
					{
						contents += input[pos];
						pos++;
					}
					tokens.push_back({ line, Identifier, contents });
				}
				// Semicolon
				else if (DetectToken(input, pos, ';'))\
				{
					tokens.push_back({ line, Semicolon, ";" });
				}
				// Conditional Operators
				else if (DetectToken(input, pos, '=', '='))
				{
					tokens.push_back({ line, ConditonalOp, "==" });
				}
				else if (DetectToken(input, pos, '!', '='))
				{
					tokens.push_back({ line, ConditonalOp, "!=" });
				}
				else if (DetectToken(input, pos, '>', '='))
				{
					tokens.push_back({ line, ConditonalOp, ">=" });
				}
				else if (DetectToken(input, pos, '<', '='))
				{
					tokens.push_back({ line, ConditonalOp, "<=" });
				}
				else if (DetectToken(input, pos, '|', '|'))
				{
					tokens.push_back({ line, ConditonalOp, "||" });
				}
				else if (DetectToken(input, pos, '&', '&'))
				{
					tokens.push_back({ line, ConditonalOp, "&&" });
				}
				else if (DetectToken(input, pos, '>'))
				{
					tokens.push_back({ line, ConditonalOp, ">" });
				}
				else if (DetectToken(input, pos, '<'))
				{
					tokens.push_back({ line, ConditonalOp, "<" });
				}
				// BinOp Equals
				else if (DetectToken(input, pos, '+', '+'))
				{
					// "++" becomes "+= 1" at compile time.
					tokens.push_back({ line, BinOpEquals, "+=" });
					tokens.push_back({ line, Number, "1" });
				}
				else if (DetectToken(input, pos, '-', '-'))
				{
					// "++" becomes "+= 1" at compile time.
					tokens.push_back({ line, BinOpEquals, "-=" });
					tokens.push_back({ line, Number, "1" });
				}
				else if (DetectToken(input, pos, '+', '='))
				{
					tokens.push_back({ line, BinOpEquals, "+=" });
				}
				else if (DetectToken(input, pos, '-', '='))
				{
					tokens.push_back({ line, BinOpEquals, "-=" });
				}
				else if (DetectToken(input, pos, '*', '='))
				{
					tokens.push_back({ line, BinOpEquals, "*=" });
				}
				else if (DetectToken(input, pos, '/', '='))
				{
					tokens.push_back({ line, BinOpEquals, "/=" });
				}
				// BinOp
				else if (DetectToken(input, pos, '+'))
				{
					tokens.push_back({ line, BinOp, "+" });
				}
				else if (DetectToken(input, pos, '-'))
				{
					tokens.push_back({ line, BinOp, "-" });
				}
				else if (DetectToken(input, pos, '*'))
				{
					tokens.push_back({ line, BinOp, "*" });
				}
				else if (DetectToken(input, pos, '/'))
				{
					tokens.push_back({ line, BinOp, "/" });
				}
				// Equals
				else if (DetectToken(input, pos, '='))
				{
					tokens.push_back({ line, Equals, "=" });
				}
				// OpenParen
				else if (DetectToken(input, pos, '('))
				{
					tokens.push_back({ line, OpenParen, "(" });
				}
				// CloseParen
				else if (DetectToken(input, pos, ')'))
				{
					tokens.push_back({ line, CloseParen, ")" });
				}
				// OpenBrack
				else if (DetectToken(input, pos, '['))
				{
					tokens.push_back({ line, OpenBrack, "[" });
				}
				// CloseBrack
				else if (DetectToken(input, pos, ']'))
				{
					tokens.push_back({ line, CloseBrack, "]" });
				}
				// OpenBrace
				else if (DetectToken(input, pos, '{'))
				{
					tokens.push_back({ line, OpenBrace, "{" });
				}
				// CloseBrace
				else if (DetectToken(input, pos, '}'))
				{
					tokens.push_back({ line, CloseBrace, "}" });
				}
				// Dot
				else if (DetectToken(input, pos, '.'))
				{
					tokens.push_back({ line, Dot, "." });
				}
				// Colon
				else if (DetectToken(input, pos, ':'))
				{
					tokens.push_back({ line, Colon, ":" });
				}
				// Comma
				else if (DetectToken(input, pos, ','))
				{
					tokens.push_back({ line, Comma, "," });
				}
				// Not
				else if (DetectToken(input, pos, '!'))
				{
					tokens.push_back({ line, Not, "!" });
				}
				// Ampersand
				else if (DetectToken(input, pos, '&'))
				{
					tokens.push_back({ line, Ampersand, "&" });
				}
				// Tilde
				else if (DetectToken(input, pos, '~'))
				{
					tokens.push_back({ line, Tilde, "~" });
				}
				else
					pos++;
			}

			return tokens;
		}
	}
}
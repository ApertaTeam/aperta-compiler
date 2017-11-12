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

			for (int pos = 0; pos < (int)input.length(); pos++)
			{
				// Skip whitespace
				while (pos < (int)input.length() && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\v'))
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
							} else if (input[pos] == 'n') {
								contents += 0x0A;
							} else if (input[pos] == 'r') {
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
						tokens.push_back({line, String, contents});
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
						tokens.push_back({line, Character, content});
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
					tokens.push_back({line, Number, contents});
				}

				// Detect newlines
				if (input[pos] == '\n')
				{
					line++;
				}
			}

			return tokens;
		}
	}
}
#include "Parser.h"

#include "Error.h"
#include "utf8.h"

#include <charconv>

std::vector<Token> Parser::parse_file(const LoadedFile& file)
{
	std::vector<Token> tokens;

	auto toknizer = Tokenizer{
		file.filepath, // filename
		file.source.data(), // source
		file.source.data() + file.source.size(), // source_end
	};

	for (;;)
	{
		auto token = toknizer.next();
		if (token.kind == TokenKind_None)
			break;

		tokens.push_back(token);
	}

	return tokens;
}

TokenPrecedence Token::precedence() const
{
	TokenPrecedence prec = TokenPrecedence_None;

	switch (kind)
	{
		case TokenKind_None: break;

		// Literals
		case TokenKind_Bool:
		case TokenKind_Char:
		case TokenKind_Int:
		case TokenKind_Float:
		case TokenKind_Str:
		case TokenKind_Ident:
			break;

		// Delimeters
		case TokenKind_OpenParen:    prec = TokenPrecedence_Call; break;
		case TokenKind_CloseParen:   break;
		case TokenKind_OpenBracket:  prec = TokenPrecedence_Call; break;
		case TokenKind_CloseBracket: break;
		case TokenKind_OpenCurly:    break;
		case TokenKind_CloseCurly:   break;

		// Operators
		case TokenKind_Plus:    prec = TokenPrecedence_Term;  break;
		case TokenKind_Dash:    prec = TokenPrecedence_Term;  break;
		case TokenKind_Star:    prec = TokenPrecedence_Factor; break;
		case TokenKind_Slash:   prec = TokenPrecedence_Factor; break;
		case TokenKind_Percent: prec = TokenPrecedence_Factor; break;

		// Keywords
		case TokenKind_Def:
		case TokenKind_Import:
			break;

		default:
		{
			PANIC("An unhandled TokenKind `{}` was passed to `{}`", kind, __func__);
			break;
		}
	}

	return prec;
}

void sk::Formatter<TokenKind>::format(const TokenKind& kind, std::string_view fmt, sk::Writer& writer)
{
	switch (kind)
	{
		case TokenKind_None:
		{
			writer.write("None");
			break;
		}
		
		// Literals
		case TokenKind_Bool:
		{
			writer.write("Bool");
			break;
		}
		case TokenKind_Char:
		{
			writer.write("Char");
			break;
		}
		case TokenKind_Int:
		{
			writer.write("Int");
			break;
		}
		case TokenKind_Float:
		{
			writer.write("Float");
			break;
		}
		case TokenKind_Str:
		{
			writer.write("Str");
			break;
		}
		case TokenKind_Ident:
		{
			writer.write("Ident");
			break;
		}

		// Delimeters
		case TokenKind_OpenParen:
		{
			writer.write("OpenParen");
			break;
		}
		case TokenKind_CloseParen:
		{
			writer.write("CloseParen");
			break;
		}
		case TokenKind_OpenBracket:
		{
			writer.write("OpenBracket");
			break;
		}
		case TokenKind_CloseBracket:
		{
			writer.write("CloseBracket");
			break;
		}
		case TokenKind_OpenCurly:
		{
			writer.write("OpenCurly");
			break;
		}
		case TokenKind_CloseCurly:
		{
			writer.write("CloseCurly");
			break;
		}

		// Operators
		case TokenKind_Plus:
		{
			writer.write("Plus");
			break;
		}
		case TokenKind_Dash:
		{
			writer.write("Dash");

			break;
		}
		case TokenKind_Star:
		{
			writer.write("Star");
			break;
		}
		case TokenKind_Slash:
		{
			writer.write("Slash");
			break;
		}
		case TokenKind_Percent:
		{
			writer.write("Percent");
			break;
		}

		// Keywords
		case TokenKind_Def:
		{
			writer.write("Def");
			break;
		}
		case TokenKind_Import:
		{
			writer.write("Import");
			break;
		}

		default:
		{
			writer.print("<TokenKind: {}>", static_cast<int>(kind));
			break;
		}
	}
}

void sk::Formatter<TokenPrecedence>::format(const TokenPrecedence& prec, std::string_view fmt, sk::Writer& writer)
{
	switch (prec)
	{
		case TokenPrecedence_None:
		{
			writer.write("None");
			break;
		}
		case TokenPrecedence_Assignment:
		{
			writer.write("Assignment");
			break;
		}
		case TokenPrecedence_Term:
		{
			writer.write("Term");
			break;
		}
		case TokenPrecedence_Factor:
		{
			writer.write("Factor");
			break;
		}
		default:
		{
			writer.print("<TokenPrecedence: {}>", static_cast<int>(prec));
			break;
		}
	}
}

void sk::Formatter<Token>::format(const Token& token, std::string_view fmt, sk::Writer& writer)
{
	bool alternate = fmt == "#";

	writer.print("Token{}", alternate ? " {\n\t" : "{ ");

	switch (token.kind)
	{
		case TokenKind_Bool:
		{
			writer.print("Bool = {}", token.bool_value);
			if (alternate) writer.write("\n\t");
			break;
		}
		case TokenKind_Char:
		{
			writer.print("Char = {:c}", static_cast<int32_t>(token.char_value));
			if (alternate) writer.write("\n\t");
			break;
		}
		case TokenKind_Int:
		{
			writer.print("Int = {}", token.int_value);
			if (alternate) writer.write("\n\t");
			break;
		}
		case TokenKind_Float:
		{
			writer.print("Float = {}", token.float_value);
			if (alternate) writer.write("\n\t");
			break;
		}
		case TokenKind_Str:
		{
			writer.print("Str = {}", token.str_value);
			if (alternate) writer.write("\n\t");
			break;
		}
		case TokenKind_Ident:
		{
			writer.print("Ident = {}, ", token.str_value);
			if (alternate) writer.write("\n\t");
			break;
		}
		default:
		{
			writer.print("kind: {}, ", token.kind);
			if (alternate) writer.write("\n\t");
			break;
		}
	}

	writer.print("location: {}{}}}", token.location, alternate ? ",\n" : " ");
}

CodeLocation Tokenizer::current_location() const
{
	auto loc = CodeLocation{ ln, ch, filename };
	return loc;
}

bool Tokenizer::is_ident_begin(char32_t c)
{
	bool invalid_char = isdigit(c) || ispunct(c) || isspace(c) || iscntrl(c);
	return !invalid_char || c == '_';
}

bool Tokenizer::is_ident_char(char32_t c)
{
	return is_ident_begin(c) || isdigit(c);
}

bool Tokenizer::is_str_begin(char32_t c)
{
	return c == '"';
}

bool Tokenizer::is_char_begin(char32_t c)
{
	return c == '\'';
}

char32_t Tokenizer::peek_char()
{
	if (source == source_end)
		return U'\0';

	char32_t c = utf8::peek_next(source, source_end);

	return c;
}

char32_t Tokenizer::next_char()
{
	if (source == source_end)
		return U'\0';

	char32_t c = utf8::next(source, source_end);

	if (c == U'\n')
	{
		previous_was_newline = true;
		ln++;
		ch = 1; // @NOTE: Maybe this should be 0???
	}
	else
	{
		previous_was_newline = false;
		ch++;
	}

	return c;
}

char32_t Tokenizer::next_if(bool(*f)(char32_t))
{
	auto c = peek_char();
	if (c == 0)
		return c;

	if (!f(c))
		return 0;

	return next_char();
}

bool Tokenizer::next_if_eq(char32_t c)
{
	auto peeked = peek_char();
	if (peeked == 0)
		return 0;

	if (peeked != c)
		return 0;

	return next_char();
}

Token Tokenizer::peek(size_t n)
{
	while (peeked_tokens.size() <= n)
	{
		auto token = next_no_peeking();
		if (token.kind == TokenKind_None)
			return token;

		peeked_tokens.push_back(token);
	}

	return peeked_tokens[n];
}

Token Tokenizer::next()
{
	if (!peeked_tokens.empty())
	{
		Token token = peeked_tokens.front();
		peeked_tokens.erase(peeked_tokens.begin());
		return token;
	}

	return next_no_peeking();
}

Token Tokenizer::next_no_peeking()
{
	skip_whitespace();

	Token token = { TokenKind_None };

	char32_t c = peek_char();
	if (c == 0)
		return token;

	if (c == U'\n')
	{
		token.kind = TokenKind_Newline;
		token.location = current_location();

		next_char();
	}
	else if (isdigit(c))
	{
		token = tokenize_number();
	}
	else if (is_char_begin(c))
	{
		token = tokenize_text_literal(true);
	}
	else if (is_str_begin(c))
	{
		token = tokenize_text_literal(false);
	}
	else if (is_ident_begin(c))
	{
		token = tokenize_ident_or_keyword();
	}
	else
	{
		token = tokenize_punctuation();
	}

	return token;
}

void Tokenizer::skip_whitespace()
{
	while (source != source_end)
	{
		auto c = peek_char();
		switch (c)
		{
			case 0:
				return;

			case U'#':
			{
				while (next_char() != U'\n')
				{
					// skip comment
				}
				break;
			}
			case U'\n':
			{
				if (!previous_was_newline)
					break;
				next_char();
				break;
			}
			case U' ':
			{
				next_char();
				break;
			}

			default:
				return;
		}
	}
}

Token Tokenizer::tokenize_number()
{
	auto loc = current_location();

	const char* number_start = source;
	const char* number_end = number_start;

	while (next_if([](char32_t c) { return isdigit(c) != 0; }) != 0)
	{
		number_end = source;
	}

	const char* reset_point = number_end;
	bool has_fractional_component = false;
	if (next_if_eq(U'.'))
	{
		auto c = peek_char();
		if (c != 0)
		{
			if (!isdigit(c))
			{
				source = reset_point;
			}
			else
			{
				has_fractional_component = true;
				while (next_if([](char32_t c) { return isdigit(c) != 0; }) != 0)
				{
					number_end = source;
				}
			}
		}
	}

	Token token = {};
	if (has_fractional_component)
	{
		double d;
		std::from_chars(number_start, number_end, d);
		token.kind = TokenKind_Float;
		token.location = loc;
		token.float_value = d;
	}
	else
	{
		int64_t d;
		std::from_chars(number_start, number_end, d);
		token.kind = TokenKind_Int;
		token.location = loc;
		token.int_value = d;
	}

	return token;
}

Token Tokenizer::tokenize_text_literal(bool char_literal)
{
	auto loc = current_location();
	auto terminator = next_char();

	const char* start = source;
	const char* end = start;

	for (;;)
	{
		auto c = next_char();
		if (c == 0)
		{
			RAISE(loc, "Unended {} literal.", char_literal ? "Char" : "String");
		}

		if (c == terminator)
			break;

		end = source;
	}

	size_t length = utf8::distance(start, end);

	Token token = {};
	if (char_literal)
	{
		if (length != 1)
		{
			RAISE(loc, "Char literals can only contain 1 character.");
		}

		char32_t c = utf8::peek_next(start, end);
		token.kind = TokenKind_Char;
		token.location = loc;
		token.char_value = c;
	}
	else
	{
		auto str = std::string_view{ start, length };
		token.kind = TokenKind_Str;
		token.location = loc;
		token.str_value = str;
	}

	return token;
}

Token Tokenizer::tokenize_ident_or_keyword()
{
	auto loc = current_location();

	const char* start = source;
	const char* end = start;
	while (next_if(is_ident_char))
	{
		end = source;
	}

	size_t length = utf8::distance(start, end);
	auto word = std::string_view{ start, length };

	Token token = {};
	if (word == "true")
	{
		token.kind = TokenKind_Bool;
		token.location = loc;
		token.bool_value = true;
	}
	else if (word == "false")
	{
		token.kind = TokenKind_Bool;
		token.location = loc;
		token.bool_value = false;
	}
	else if (word == "def")
	{
		token.kind = TokenKind_Def;
		token.location = loc;
	}
	else if (word == "import")
	{
		token.kind = TokenKind_Import;
		token.location = loc;
	}
	else
	{
		token.kind = TokenKind_Ident;
		token.location = loc;
		token.str_value = word;
	}

	return token;
}

Token Tokenizer::tokenize_punctuation()
{
	Token token = {};
	token.location = current_location();

	auto c = next_char();
	switch (next_char())
	{
		// Delimeters
		case U'\n':
		{
			PANIC("newline character reached {}.", __func__);
		}
		case U'(':
		{
			token.kind = TokenKind_OpenParen;
			break;
		}
		case U')':
		{
			token.kind = TokenKind_CloseParen;
			break;
		}
		case U'[':
		{
			token.kind = TokenKind_OpenBracket;
			break;
		}
		case U']':
		{
			token.kind = TokenKind_CloseBracket;
			break;
		}
		case U'{':
		{
			token.kind = TokenKind_OpenCurly;
			break;
		}
		case U'}':
		{
			token.kind = TokenKind_CloseCurly;
			break;
		}

		// Operators
		case U'+':
		{
			token.kind = TokenKind_Plus;
			break;
		}
		case U'-':
		{
			token.kind = TokenKind_Dash;
			break;
		}
		case U'*':
		{
			token.kind = TokenKind_Star;
			break;
		}
		case U'/':
		{
			token.kind = TokenKind_Slash;
			break;
		}
		case U'%':
		{
			token.kind = TokenKind_Percent;
			break;
		}

		default:
		{
			RAISE(token.location, "Invalid operator `{}`", c);
		}
	}
}

#pragma once

#include "utf8.h"
#include "CodeLocation.h"
#include "Interpreter.h"

enum TokenKind
{
	TokenKind_Bool,
	TokenKind_Char,
	TokenKind_Int,
	TokenKind_Float,
	TokenKind_Str,
};

struct Token
{
	TokenKind kind;
	CodeLocation location;

	union
	{
		bool bool_value;
		char32_t char_value;
		int64_t int_value;
		double float_value;
		std::string_view str_value;
	};
};

struct Tokenizer
{
		
};

struct Parser
{
	Tokenizer tokenizer;

	static std::vector<void*> parse_file(const LoadedFile& file);
};
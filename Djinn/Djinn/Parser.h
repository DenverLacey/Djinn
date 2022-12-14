#pragma once

#include "utf8.h"
#include "skrive.h"

#include "CodeLocation.h"
#include "Interpreter.h"
#include "Ast.h"

enum TokenKind
{
	TokenKind_None = 0,

	// Literals
	TokenKind_Bool,
	TokenKind_Char,
	TokenKind_Int,
	TokenKind_Float,
	TokenKind_Str,
	TokenKind_Ident,

	// Delimeters
	TokenKind_Newline,
	TokenKind_Colon,
	TokenKind_Comma,
	TokenKind_OpenParen,
	TokenKind_CloseParen,
	TokenKind_OpenBracket,
	TokenKind_CloseBracket,
	TokenKind_OpenCurly,
	TokenKind_CloseCurly,

	// Operators
	TokenKind_Plus,
	TokenKind_Dash,
	TokenKind_Star,
	TokenKind_Slash,
	TokenKind_Percent,

	// Keywords
	TokenKind_Def,
	TokenKind_Import,
};

enum TokenPrecedence
{
	TokenPrecedence_None = 0,
	TokenPrecedence_Assignment, // =
	TokenPrecedence_Colon,
	TokenPrecedence_Term,       // + -
	TokenPrecedence_Factor,      // * / % 
	TokenPrecedence_Unary,      // -
	TokenPrecedence_Call,		// () . []
};

struct Token
{
	// === Data =======================
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

	// === Methods ====================
	TokenPrecedence precedence() const;
	bool is_none() const;
};

struct Tokenizer
{
	// === Data =======================
	const char* filename;
	const char* source;
	const char* source_end;

	std::vector<Token> peeked_tokens;
	bool previous_was_newline = false;
	
	size_t ln = 1;
	size_t ch = 1;

	// === Methods ====================
	CodeLocation current_location() const;
	static bool is_ident_begin(char32_t c);
	static bool is_ident_char(char32_t c);
	static bool is_str_begin(char32_t c);
	static bool is_char_begin(char32_t c);

	char32_t peek_char();
	char32_t next_char();

	char32_t next_if(bool (*f)(char32_t));
	bool next_if_eq(char32_t c);

	Token peek(size_t n = 0);
	Token next();
	Token next_no_peeking();

	void skip_whitespace();

	Token tokenize_number();
	Token tokenize_text_literal(bool char_literal);
	Token tokenize_ident_or_keyword();
	Token tokenize_punctuation();
};

struct Parser
{
	// === Data =======================
	Tokenizer tokenizer;
	Ast ast;

	// === Methods ====================
	static Ast parse_file(const LoadedFile& file);

	bool check(TokenKind kind);
	Token match(TokenKind kind);
	Token expect(TokenKind kind, const char* err);

	void skip_newline();
	bool skip_check(TokenKind kind);
	Token skip_match(TokenKind kind);
	Token skip_expect(TokenKind kind, const char* err);

	NodeIndex parse_declaration();
	NodeIndex parse_statement();
	NodeIndex parse_expression(bool ban_assignment = true);
	NodeIndex parse_precedence(TokenPrecedence prec);
	NodeIndex parse_prefix(Token token);
	NodeIndex parse_infix(NodeIndex previous, Token token);

	NodeIndex parse_unary(AstKind kind, CodeLocation location);
	NodeIndex parse_binary(AstKind kind, CodeLocation location, NodeIndex lhs, TokenPrecedence prec);

	NodeIndex parse_identifier(const char* err);
	NodeIndex parse_block();
	NodeIndex parse_comma_separated_expressions(AstKind kind, TokenKind terminator, CodeLocation location);

	NodeIndex parse_import_statement(Token import_token);
	NodeIndex parse_function_declaration(Token def_token);
};

namespace sk
{
	template<> struct Formatter<TokenKind>
	{
		static void format(const TokenKind& kind, std::string_view fmt, Writer& writer);
	};

	template<> struct Formatter<TokenPrecedence>
	{
		static void format(const TokenPrecedence& prec, std::string_view fmt, Writer& writer);
	};

	template<> struct Formatter<Token>
	{
		static void format(const Token& token, std::string_view fmt, Writer& writer);
	};
}

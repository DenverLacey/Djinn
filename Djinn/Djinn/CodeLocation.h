#pragma once

#include <string_view>
#include "skrive.h"

struct CodeLocation
{
	size_t ln, ch;
	std::string_view filename;
};

template<> struct sk::Formatter<CodeLocation>
{
	static void format(const CodeLocation& loc, std::string_view fmt, Writer& writer);
};

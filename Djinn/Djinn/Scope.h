#pragma once

#include <string_view>
#include "skrive.h"

struct Scope
{
	Scope* parent;
};

template<> struct sk::Formatter<Scope>
{
	static void format(const Scope& scope, std::string_view fmt, Writer& writer);
};

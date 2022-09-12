#pragma once

#include <string_view>

struct Scope
{
	Scope* parent;
};

namespace sk
{
	class Writer;
	template<typename> struct Formatter;
}

template<> struct sk::Formatter<Scope>
{
	static void format(const Scope& scope, std::string_view fmt, sk::Writer& writer);
};

#pragma once

#include "Scope.h"

void sk::Formatter<Scope>::format(const Scope& scope, std::string_view fmt, sk::Writer& writer)
{
	writer.print("Scope{{ parent: {} }}", scope.parent);
}

#pragma once

#include "Scope.h"
#include "skrive.h"

void sk::Formatter<Scope>::format(const Scope& scope, std::string_view fmt, sk::Writer& writer)
{
	writer.write("Scope{}");
}

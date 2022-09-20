#include "CodeLocation.h"
#include "skrive.h"

void sk::Formatter<CodeLocation>::format(const CodeLocation& loc, std::string_view fmt, Writer& writer)
{
	writer.print("{}:{}:{}", loc.filename, loc.ln, loc.ch);
}

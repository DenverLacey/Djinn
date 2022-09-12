#include <iostream>

#include "Interpreter.h"
#include "skrive.h"
#include "Error.h"

int main()
{
	Interpreter interp;
	interp.scopes.append(Scope{});
	sk::println("{}", interp.scopes);
}

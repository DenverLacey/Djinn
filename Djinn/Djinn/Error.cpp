#pragma once

#include "Error.h"

const char* DjinnException::what() const noexcept
{
	return error.c_str();
}

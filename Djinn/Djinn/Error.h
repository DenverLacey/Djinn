#pragma once

#include <exception>
#include <stdarg.h>

#include "skrive.h"

#define TODO(...) \
	__private_todo(__FILE__, __LINE__, __VA_ARGS__)

template<typename... Ts>
[[noreturn]] void __private_todo(const char* file, int line, const char* fmt, const Ts&... args)
{
	sk::eprint("{}:{}: TODO: ", file, line);
	sk::eprintln(fmt, args...);
	exit(-1);
}

#define RAISE(...) \
	throw DjinnException(__FILE__, __LINE__, __VA_ARGS__);

class DjinnException : std::exception
{
public:
	template<typename... Ts>
	DjinnException(
		const char* file,
		int line,
		const char* fmt,
		const Ts&... args)
	{
		std::stringstream ss;
		auto writer = sk::Writer{ ss };

		writer.print("{}:{}: ERR: ", file, line);
		writer.print(fmt, args...);

		error = ss.str();
	}

	const char* what() const noexcept override;

private:
	std::string error;
};

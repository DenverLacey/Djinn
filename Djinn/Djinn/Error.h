#pragma once

#include <exception>
#include <stdarg.h>
#include <windows.h>

#include "skrive.h"

#include "CodeLocation.h"

#ifndef TODO
#define TODO(...) \
	__private_todo(__FILE__, __LINE__, __VA_ARGS__)
#else
#error "TODO macro already defined."
#endif

template<typename... Ts>
[[noreturn]] void __private_todo(const char* file, int line, const char* fmt, const Ts&... args)
{
	sk::eprint("{}:{}: TODO: ", file, line);
	sk::eprintln(fmt, args...);
	exit(-1);
}

#ifndef PANIC
#define PANIC(...) \
	__private_error(__FILE__, __LINE__, __VA_ARGS__)
#else
#error "PANIC macro already defined."
#endif

#ifndef ENSURE
#define ENSURE(cond, ...) \
	if (!(cond)) __private_error(__FILE__, __LINE__, __VA_ARGS__)
#else
#error "ENSURE macro already defined."
#endif

template<typename... Ts>
[[noreturn]] void __private_error(const char* file, int line, const char* fmt, const Ts&... args)
{
	std::stringstream ss;
	auto writer = sk::Writer{ ss };
	
	writer.print("{}:{}: INTERNAL ERR: ", file, line);
	writer.print(fmt, args...);
	
	std::string message = ss.str();

	UINT result = MessageBoxA(NULL, message.c_str(), "Error", MB_OKCANCEL | MB_ICONERROR | MB_DEFBUTTON1);
	if (result == IDOK)
		DebugBreak();
	else
		exit(EXIT_FAILURE);
}

#ifndef RAISE
#define RAISE(...) \
	throw DjinnException(__VA_ARGS__)
#else
#error "RAISE macro is already defined."
#endif

class DjinnException : std::exception
{
public:
	template<typename... Ts>
	DjinnException(
		CodeLocation loc,
		const char* fmt,
		const Ts&... args)
	{
		std::stringstream ss;
		auto writer = sk::Writer{ ss };

		writer.print("{}: ERR: ", loc);
		writer.print(fmt, args...);

		error = ss.str();
	}

	const char* what() const noexcept override;

private:
	std::string error;
};

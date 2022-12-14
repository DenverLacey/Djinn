#pragma once

// @TODO:
// use this hacky workaround to converge some Formatter template 
// specialisations to a single type.
//
// This will increase the complexity of the code and probably the amount
// of work that the compiler has to do but it will reduce the code size
// and get around MSVC's very strict and somewhat iffy overload 
// resolution
//

#include "skrive/formatter-hk.h"
#include <sstream>

namespace sk {
    inline Writer out = Writer{ std::cout };
    inline Writer err = Writer{ std::cerr };

    template<typename... Ts>
    void print(const char* fmt, const Ts&... args) {
        out.print(fmt, args...);
    }

    template<typename... Ts>
    void println(const char* fmt, const Ts&... args) {
        out.println(fmt, args...);
    }

    template<typename... Ts>
    void eprint(const char* fmt, const Ts&... args) {
        err.print(fmt, args...);
    }

    template<typename... Ts>
    void eprintln(const char* fmt, const Ts&... args) {
        err.println(fmt, args...);
    }

    void print(const char* fmt, Args args);
    void println(const char* fmt, Args args);
    void eprint(const char* fmt, Args args);
    void eprintln(const char* fmt, Args args);

    std::string format(const char* fmt, Args args);

    template<typename... Ts>
    std::string format(const char* fmt, const Ts&... args) {
        std::stringstream ss;
        auto writer = Writer{ ss };
        writer.print(fmt, args...);
        return ss.str();
    }
}

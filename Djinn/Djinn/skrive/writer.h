#pragma once

#include <string_view>
#include <iostream>
#include <assert.h>

#include "formatter.h"

namespace sk {
    class Writer;
    typedef void(*Printer)(const void* obj, std::string_view fmt, Writer& writer);
    
    struct Arg {
        Printer printer;
        const void* value_ptr;
    };

    struct PrintInfo {
        const char* fmt;
        size_t num_args;
        Arg* args;
    };

    class Writer {
    public:
        Writer(std::ostream& stream);

    public:
        void write(const char* s, Format fmt = {});
        void write(size_t n, const char* s, Format fmt = {});

        void write(bool b, Format fmt = {});
        void write(char c, Format fmt = {});
        void write(int d, Format fmt = {});
        void write(long d, Format fmt = {});
        void write(double f, Format fmt = {});
        void write(void* p, Format fmt = {});

        void flush();

        template<typename... Ts>
        void print(const char* fmt, const Ts&... args) {
            if constexpr (sizeof...(Ts) == 0) {
                write(fmt);
            } else {
                auto print_info = PrintInfo {
                    fmt,
                    sizeof...(Ts),
                    new Arg[sizeof...(Ts)]{ { reinterpret_cast<Printer>(Formatter<Ts>::format), &args }... }
                };

                print_from_print_info(print_info);

                delete[] print_info.args;
            }
        }

        template<typename... Ts>
        void println(const char* fmt, const Ts&... args) {
            if constexpr (sizeof...(Ts) == 0) {
                write(fmt);
                write('\n');
                flush();
            } else {
                auto print_info = PrintInfo {
                    fmt,
                    sizeof...(Ts),
                    new Arg[sizeof...(Ts)]{ { reinterpret_cast<Printer>(Formatter<Ts>::format), &args }... }
                };

                print_from_print_info(print_info);
                write('\n');
                flush();

                delete[] print_info.args;
            }
        }      

    private:
        void print_from_print_info(PrintInfo print_info);

    private:
        std::ostream& stream;
    };
}

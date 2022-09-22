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

    class Args {
    public:
        Args(size_t size, Arg* args);
        ~Args();

        Args(const Args& other) = delete;
        Args(Args&& other) = delete;

    public:
        Arg& operator[](size_t index) const;
        size_t size() const;
        Arg* args() const;

    private:
        size_t _size;
        Arg* _args;
    };

    template<typename... Ts>
    Args to_args(const Ts&... args) {
        return Args { sizeof...(Ts), new Arg[sizeof...(Ts)]{ { reinterpret_cast<Printer>(Formatter<Ts>::format), &args }... } };
    }

    class Writer {
    public:
        Writer(std::ostream& stream);

    public:
        void write(const char* s, Format fmt = {});
        void write(size_t n, const char* s, Format fmt = {});

        void write(bool b, Format fmt = {});

        void write(char32_t c, Format fmt = {});

        void write(int16_t d, Format fmt = {});
        void write(int32_t d, Format fmt = {});
        void write(int64_t d, Format fmt = {});
        void write(uint16_t d, Format fmt = {});
        void write(uint32_t d, Format fmt = {});
        void write(uint64_t d, Format fmt = {});
        //void write(size_t d, Format fmt = {});
        
        void write(float f, Format fmt = {});
        void write(double f, Format fmt = {});
        
        void write(void* p, Format fmt = {});

        void flush();

        void print(const char* fmt, const Args& args);
        void println(const char* fmt, const Args& args);

        template<typename... Ts>
        void print(const char* fmt, const Ts&... args) {
            if constexpr (sizeof...(Ts) == 0) {
                write(fmt);
            } else {
                auto packed = to_args(args...);
                print(fmt, packed);
            }
        }

        template<typename... Ts>
        void println(const char* fmt, const Ts&... args) {
            if constexpr (sizeof...(Ts) == 0) {
                write(fmt);
                write("\r\n");
            } else {
                auto packed = to_args(args...);
                println(fmt, packed);
            }
        }

    private:
        std::ostream& stream;
    };
}

#pragma once

#include <string_view>
#include <stdint.h>

namespace sk {
    class Writer;

    template<typename T>
    struct Formatter;

    template<>
    struct Formatter<bool> {
        static void format(const bool& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<char> {
        static void format(const char& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<char32_t>
    {
        static void format(const char32_t& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<int16_t> {
        static void format(const int16_t& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<int32_t> {
        static void format(const int32_t& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<int64_t> {
        static void format(const int64_t& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<uint16_t> {
        static void format(const uint16_t& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<uint32_t> {
        static void format(const uint32_t& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<uint64_t> {
        static void format(const uint64_t& obj, std::string_view fmt, Writer& writer);
    };

    //template<>
    //struct Formatter<size_t> {
    //    static void format(const size_t& obj, std::string_view fmt, Writer& writer);
    //};

    template<>
    struct Formatter<double> {
        static void format(const double& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<float> {
        static void format(const float& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<char*> {
        static void format(const char*& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<const char*>
    {
        static void format(const char*& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<std::string_view> {
        static void format(const std::string_view& obj, std::string_view fmt, Writer& writer);
    };

    template<>
    struct Formatter<std::string> {
        static void format(const std::string& obj, std::string_view fmt, Writer& writer);
    };

    struct Format {
        enum class Align : char {
            Omitted = 0,
            Left    = '<',
            Right   = '>',
            Center  = '^',
        };

        enum class Sign : char {
            Omitted  = 0,
            Both     = '+',
            Negative = '-',
            Space    = ' ',
        };

        enum class Type : char {
            Omitted     = 0,
            String      = 's',
            Char        = 'c',
            CharBig     = 'C',
            Binary      = 'b',
            BinaryBig   = 'B',
            Decimal     = 'd',
            Octal       = 'o',
            Hex         = 'x',
            HexBig      = 'X',
            HexFloat    = 'h',
            HexFloatBig = 'H',
            Exp         = 'e',
            ExpBig      = 'E',
            General     = 'g',
            GeneralBig  = 'G',
            Pointer     = 'p'
        };

        int width      = -1;
        int precision  = -1;
        bool alternate = false;
        char fill      = ' ';
        Align align    = Align::Omitted;
        Sign sign      = Sign::Omitted;
        Type type      = Type::Omitted;

        static Format from(std::string_view fmt);
    };
}

#include "formatter.h"
#include "writer.h"

#include <charconv>

namespace sk {
    void Formatter<bool>::format(const bool& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<char>::format(const char& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<char32_t>::format(const char32_t& obj, std::string_view fmt, Writer& writer)
    {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<int16_t>::format(const int16_t& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<int32_t>::format(const int32_t& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<int64_t>::format(const int64_t& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<uint16_t>::format(const uint16_t& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<uint32_t>::format(const uint32_t& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<uint64_t>::format(const uint64_t& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    //void Formatter<size_t>::format(const size_t& obj, std::string_view fmt, Writer& writer) {
    //    auto format = Format::from(fmt);
    //    writer.write(obj, format);
    //}

    void Formatter<double>::format(const double& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<float>::format(const float& obj, std::string_view fmt, Writer& writer) {
        Formatter<double>::format(obj, fmt, writer);
    }

    void Formatter<char*>::format(const char*& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<const char*>::format(const char*& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj, format);
    }

    void Formatter<std::string_view>::format(const std::string_view& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj.size(), obj.data(), format);
    }

    void Formatter<std::string>::format(const std::string& obj, std::string_view fmt, Writer& writer) {
        auto format = Format::from(fmt);
        writer.write(obj.size(), obj.c_str(), format);
    }

    static bool is_align_char(char c) {
        switch (c) {
            case '<':
            case '>':
            case '^':
                return true;
        }
        return false;
    }

    static bool is_sign_char(char c) {
        switch (c) {
            case '+':
            case '-':
            case ' ':
                return true;
        }
        return false;
    }

    Format Format::from(std::string_view fmt) {
        auto format = Format{};
        if (fmt.empty()) return format;

        // Fill and Alignment
        if (fmt.size() >= 2 && is_align_char(fmt[1])) {
            format.fill = fmt[0];
            format.align = static_cast<Align>(fmt[1]);
            fmt.remove_prefix(2);
        } else if (is_align_char(fmt[0])) {
            format.align = static_cast<Align>(fmt[0]);
            fmt.remove_prefix(1);
        }

        // Sign
        if (!fmt.empty() && is_sign_char(fmt[0])) {
            format.sign = static_cast<Sign>(fmt[0]);
            fmt.remove_prefix(1);
        }

        // Alternate Mode
        if (!fmt.empty() && (fmt[0] == '#')) {
            format.alternate = true;
            fmt.remove_prefix(1);
        }

        // Sign-Aware Zero Padding
        if (!fmt.empty() && (fmt[0] == '0')) {
            format.fill = '0'; // @HACK: Overwrites potential alignment fill
            fmt.remove_prefix(1);
        }

        // Width
        if (!fmt.empty() && (isdigit(fmt[0]))) {
            size_t len = 1;
            while(len <= fmt.size() && isdigit(fmt[len])) len++;
            std::from_chars(&fmt[0], &fmt[len], format.width);
            fmt.remove_prefix(len);
        }

        // Precision
        if (fmt.size() >= 2 && (fmt[0] == '.') && isdigit(fmt[1])) {
            fmt.remove_prefix(1);
            size_t len = 1;
            while(len <= fmt.size() && isdigit(fmt[len])) len++;
            std::from_chars(&fmt[0], &fmt[len], format.precision);
            fmt.remove_prefix(len);
        }

        if (!fmt.empty()) {
            format.type = static_cast<Type>(fmt[0]);
            fmt.remove_prefix(1);
        }

        return format;
    }
}

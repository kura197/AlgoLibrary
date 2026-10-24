#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/templates/dbg_utils_algo.h

#include <array>
#include <atcoder/modint.hpp>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace debug_utils {
namespace detail {

template <typename T>
void write_value(std::ostream& out, const T& value);

template <typename T, typename Allocator>
void write_value(std::ostream& out, const std::vector<T, Allocator>& values);

template <typename T, std::size_t N>
void write_value(std::ostream& out, const std::array<T, N>& values);

template <typename First, typename Second>
void write_value(std::ostream& out, const std::pair<First, Second>& value);

template <typename... Ts>
void write_value(std::ostream& out, const std::tuple<Ts...>& value);

template <typename T>
void write_value(std::ostream& out, const T& value) {
    if constexpr (atcoder::internal::is_modint<T>::value) {
        out << value.val();
    } else {
        out << value;
    }
}

inline void write_value(std::ostream& out, bool value) {
    out << std::boolalpha << value;
}

template <typename Iterator>
void write_range(std::ostream& out, Iterator first, Iterator last, char open, char close) {
    out << open;
    for (bool needs_separator = false; first != last; ++first) {
        if (needs_separator) {
            out << ", ";
        }
        needs_separator = true;
        write_value(out, *first);
    }
    out << close;
}

template <typename T, typename Allocator>
void write_value(std::ostream& out, const std::vector<T, Allocator>& values) {
    write_range(out, values.begin(), values.end(), '[', ']');
}

template <typename T, std::size_t N>
void write_value(std::ostream& out, const std::array<T, N>& values) {
    write_range(out, values.begin(), values.end(), '[', ']');
}

template <typename First, typename Second>
void write_value(std::ostream& out, const std::pair<First, Second>& value) {
    out << '{';
    write_value(out, value.first);
    out << ", ";
    write_value(out, value.second);
    out << '}';
}

template <typename... Ts>
void write_value(std::ostream& out, const std::tuple<Ts...>& value) {
    out << '{';
    std::size_t index = 0;
    std::apply(
        [&](const auto&... elements) {
            ((out << (index++ == 0 ? "" : ", "), write_value(out, elements)), ...);
        },
        value
    );
    out << '}';
}

class StreamStateGuard {
public:
    explicit StreamStateGuard(std::ostream& out)
        : out_(out), flags_(out.flags()), precision_(out.precision()),
          width_(out.width()), fill_(out.fill()) {}

    ~StreamStateGuard() {
        out_.flags(flags_);
        out_.precision(precision_);
        out_.width(width_);
        out_.fill(fill_);
    }

private:
    std::ostream& out_;
    std::ios_base::fmtflags flags_;
    std::streamsize precision_;
    std::streamsize width_;
    char fill_;
};

struct FormatSpec {
    char fill = ' ';
    char align = '\0';
    int width = 0;
    int precision = -1;
    char type = '\0';
};

constexpr bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

inline FormatSpec parse_spec(std::string_view spec) {
    FormatSpec result;
    std::size_t pos = 0;

    if (spec.size() >= 2 && (spec[1] == '<' || spec[1] == '>')) {
        result.fill = spec[0];
        result.align = spec[1];
        pos = 2;
    } else if (pos < spec.size() && (spec[pos] == '<' || spec[pos] == '>')) {
        result.align = spec[pos++];
    }

    while (pos < spec.size() && is_digit(spec[pos])) {
        result.width = result.width * 10 + (spec[pos++] - '0');
    }

    if (pos < spec.size() && spec[pos] == '.') {
        ++pos;
        result.precision = 0;
        while (pos < spec.size() && is_digit(spec[pos])) {
            result.precision = result.precision * 10 + (spec[pos++] - '0');
        }
    }

    if (pos < spec.size()) {
        result.type = spec[pos];
    }
    return result;
}

template <typename T>
void write_formatted(std::ostream& out, const T& value, std::string_view spec_text) {
    const StreamStateGuard guard(out);
    const FormatSpec spec = parse_spec(spec_text);

    out << std::setfill(spec.fill);
    if (spec.align == '<') {
        out << std::left;
    } else if (spec.align == '>') {
        out << std::right;
    }
    if (spec.width > 0) {
        out << std::setw(spec.width);
    }
    if (spec.precision >= 0) {
        out << std::setprecision(spec.precision);
    }

    switch (spec.type) {
    case 'd': out << std::dec; break;
    case 'x': out << std::hex; break;
    case 'X': out << std::hex << std::uppercase; break;
    case 'f': out << std::fixed; break;
    case 'e': out << std::scientific; break;
    case 'E': out << std::scientific << std::uppercase; break;
    default: break;
    }

    write_value(out, value);
}

inline void write_literal(std::ostream& out, std::string_view text) {
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (i + 1 < text.size() &&
            ((text[i] == '{' && text[i + 1] == '{') ||
             (text[i] == '}' && text[i + 1] == '}'))) {
            out << text[i++];
        } else {
            out << text[i];
        }
    }
}

inline std::size_t find_replacement_field(std::string_view format) {
    for (std::size_t i = 0; i < format.size(); ++i) {
        if (format[i] != '{') {
            continue;
        }
        if (i + 1 < format.size() && format[i + 1] == '{') {
            ++i;
            continue;
        }
        return i;
    }
    return std::string_view::npos;
}

inline void format_to(std::ostream& out, std::string_view format) {
    write_literal(out, format);
}

template <typename T, typename... Rest>
void format_to(std::ostream& out, std::string_view format, const T& value, const Rest&... rest) {
    const std::size_t open = find_replacement_field(format);
    if (open == std::string_view::npos) {
        write_literal(out, format);
        return;
    }

    const std::size_t close = format.find('}', open + 1);
    if (close == std::string_view::npos) {
        write_literal(out, format);
        return;
    }

    write_literal(out, format.substr(0, open));
    std::string_view spec = format.substr(open + 1, close - open - 1);
    if (!spec.empty() && spec.front() == ':') {
        spec.remove_prefix(1);
    }
    write_formatted(out, value, spec);
    format_to(out, format.substr(close + 1), rest...);
}

} // namespace detail

inline void emit(std::ostream& out, const std::ostringstream& buffer) {
    const std::string text = buffer.str();
    out.write(text.data(), static_cast<std::streamsize>(text.size()));
}

template <typename T>
void write(std::ostream& out, const T& value) {
    std::ostringstream buffer;
    detail::write_value(buffer, value);
    emit(out, buffer);
}

template <typename... Args>
void print(std::ostream& out, std::string_view format, const Args&... args) {
    std::ostringstream buffer;
    detail::format_to(buffer, format, args...);
    emit(out, buffer);
}

template <typename T>
void dump(std::ostream& out, std::string_view name, const T& value) {
    std::ostringstream buffer;
    buffer << name << " = ";
    detail::write_value(buffer, value);
    buffer << '\n';
    emit(out, buffer);
    out.flush();
}

template <typename... Args>
[[noreturn]] void fail_assertion(
    std::ostream& out,
    std::string_view expression,
    std::string_view file,
    int line,
    std::string_view function,
    std::string_view message_format,
    const Args&... args
) {
    std::ostringstream buffer;
    buffer << "===================================================\n";
    buffer << "Assertion failed: " << expression << '\n';
    buffer << "File: " << file << ':' << line << '\n';
    buffer << "Function: " << function << '\n';
    buffer << "Message: ";
    detail::format_to(buffer, message_format, args...);
    buffer << '\n';
    buffer << "===================================================\n";
    emit(out, buffer);
    out.flush();
    std::abort();
}

} // namespace debug_utils

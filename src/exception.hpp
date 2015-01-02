#pragma once

#include <limits>
#include <stdexcept>

namespace exc {

constexpr size_t FaultLine = std::numeric_limits<size_t>::max();

struct AssertException : public std::runtime_error {
    enum class Type {
        Success,
        Warning,
        Fail,
    } type;
    size_t line;

    AssertException(Type t, size_t line, std::string err):
        std::runtime_error(err),
        type(t),
        line(line)
    {}
};

} // namespace exc

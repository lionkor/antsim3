#ifndef RESULT_H
#define RESULT_H

#include <concepts>
#include <utility>
#include <string>
#include <optional>

template<class T>
class Result
{
    std::optional<T> m_value;
    std::string      m_message;

public:
    template<class S>
    Result(S value)
        : m_value(std::forward<S>(value)) { }

    auto get() { return m_value.value(); }
    auto get_or(T _else) { return m_value.value_or(_else); }

    bool ok() const { return m_value.has_value(); }
    bool error() const { return !ok(); }

    static Result<T> error(const std::string& error_message) {
        Result<T> result;
        result.m_value   = std::nullopt;
        result.m_message = error_message;
        return result;
    }
};

#endif // RESULT_H

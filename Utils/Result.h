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

    std::string message() const { return m_message; }

    static Result<T> error(const std::string& error_message) {
        Result<T> result;
        result.m_value   = std::nullopt;
        result.m_message = error_message;
        return result;
    }
};

class OkResult
{
private:
    const bool        m_ok;
    const std::string m_message;

    OkResult(bool _ok, const std::string& msg)
        : m_ok(_ok)
        , m_message(msg) { }

public:
    operator bool() const { return m_ok; }
    std::string message() const { return m_message; }

    static OkResult ok() { return OkResult { true, "" }; }
    static OkResult error(const std::string& error_message = "none") { return OkResult { false, error_message }; }
};

#endif // RESULT_H

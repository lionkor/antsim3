// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef RESULT_H
#define RESULT_H

#include <utility>
#include <string>
#include <optional>
#include "DebugTools.h"

template<class T>
class Result
{
    std::optional<T> m_value;
    std::string m_message;

public:
    Result()
        : m_value(std::nullopt)
        , m_message("internal: uninitialized result") { }

    Result& set_value(T value) {
        m_value = std::optional<T>(std::forward<T>(value));
        m_message = "Success";
        return *this;
    }

    template<typename... Args>
    Result& set_error(const std::string& format_string, Args&&... args) {
        m_value = std::nullopt;
        m_message = fmt::format(format_string, std::forward<Args>(args)...);
        return *this;
    }

    auto value() { return m_value.value(); }
    auto value_or(T _else) { return m_value.value_or(_else); }

    bool ok() const { return m_value.has_value(); }
    bool error() const { return !ok(); }
    std::string message() const { return m_message; }

    operator bool() const { return ok(); }
};

#endif // RESULT_H

﻿#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <cstdio>
#include <iostream>
#include <libgen.h>
#include <stdlib.h>
// TODO: common.h, precompiled
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/format-inl.h>
#include <fmt/ostream.h>

#include <experimental/source_location>

static const char* const ANSI_RESET = "\u001b[0m";

static const char* const ANSI_BLACK = "\u001b[30m";
static const char* const ANSI_RED = "\u001b[31m";
static const char* const ANSI_GREEN = "\u001b[32m";
static const char* const ANSI_YELLOW = "\u001b[33m";
static const char* const ANSI_BLUE = "\u001b[34m";
static const char* const ANSI_MAGENTA = "\u001b[35m";
static const char* const ANSI_CYAN = "\u001b[36m";
static const char* const ANSI_WHITE = "\u001b[37m";

static const char* const ANSI_BLACK_BOLD = "\u001b[30;1m";
static const char* const ANSI_RED_BOLD = "\u001b[31;1m";
static const char* const ANSI_GREEN_BOLD = "\u001b[32;1m";
static const char* const ANSI_YELLOW_BOLD = "\u001b[33;1m";
static const char* const ANSI_BLUE_BOLD = "\u001b[34;1m";
static const char* const ANSI_MAGENTA_BOLD = "\u001b[35;1m";
static const char* const ANSI_CYAN_BOLD = "\u001b[36;1m";
static const char* const ANSI_WHITE_BOLD = "\u001b[37;1m";

static const char* const ANSI_BOLD = "\u001b[1m";
static const char* const ANSI_UNDERLINE = "\u001b[4m";

#include <string>

#ifndef NOTIMPL
#define NOTIMPL                                                                    \
    fprintf(stderr, "%s%s called but not implemented / unfinished in %s%s:%u%s\n", \
        ANSI_YELLOW, __PRETTY_FUNCTION__, ANSI_BOLD, __FILE__, __LINE__,           \
        ANSI_RESET);

#endif // NOTIMPL

#define FILE_C_STRING std::string(__FILE__).data()
#define FILENAME (std::string(basename(FILE_C_STRING)))
#define nameof(x) #x

#define HERE() fmt::format("{}:{}, {}", FILENAME, __LINE__, __FUNCTION__)

namespace impl {

#ifndef STRIP_ALL

template<typename... Args>
static inline void report_impl(const char* format, Args&&... args) {
    fmt::print(format, std::forward<Args>(args)...);
}

template<typename... Args>
static inline void report_info_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("[INFO] in {}:{} in {}: {}\n", basename(const_cast<char*>(location.file_name())), location.line(), location.function_name(), fmt::format(format, std::forward<Args>(args)...));
}

template<typename... Args>
static inline void report_trace_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("[TRACE] in {}:{} in {}: {}\n", basename(const_cast<char*>(location.file_name())), location.line(), location.function_name(), fmt::format(format, std::forward<Args>(args)...));
}

template<typename... Args>
static inline void report_warning_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("{}{}[WARNING] in {}:{} in {}: {}{}\n", ANSI_RESET, ANSI_YELLOW, basename(const_cast<char*>(location.file_name())), location.line(), location.function_name(), fmt::format(format, std::forward<Args>(args)...), ANSI_RESET);
}

template<typename... Args>
static inline void report_error_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("{}{}[ERROR] in {}:{} in {}: {}{}\n", ANSI_RESET, ANSI_RED, basename(const_cast<char*>(location.file_name())), location.line(), location.function_name(), fmt::format(format, std::forward<Args>(args)...), ANSI_RESET);
}

static inline void report_trace_impl(const std::experimental::source_location& location, const std::string& str) {
    impl::report_impl("[TRACE] in {}:{} in {}: {}\n", basename(const_cast<char*>(location.file_name())), location.line(), location.function_name(), str);
}

static inline void report_warning_impl(const std::experimental::source_location& location, const std::string& str) {
    impl::report_impl("{}{}[WARNING] in {}:{} in {}: {}{}\n", ANSI_RESET, ANSI_YELLOW, basename(const_cast<char*>(location.file_name())), location.line(), location.function_name(), str, ANSI_RESET);
}

static inline void report_error_impl(const std::experimental::source_location& location, const std::string& str) {
    impl::report_impl("{}{}[ERROR] in {}:{} in {}: {}{}\n", ANSI_RESET, ANSI_RED, basename(const_cast<char*>(location.file_name())), location.line(), location.function_name(), str, ANSI_RESET);
}

#else // STRIP_ALL
template<typename... Args>
static inline void report_impl(const char* format, Args&&... args) {
    fmt::print(format, std::forward<Args>(args)...);
}

template<typename... Args>
static inline void report_info_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("[INFO] in {}: {}\n", location.function_name(), fmt::format(format, std::forward<Args>(args)...));
}

template<typename... Args>
static inline void report_trace_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("[TRACE] in {}: {}\n", location.function_name(), fmt::format(format, std::forward<Args>(args)...));
}

template<typename... Args>
static inline void report_warning_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("{}{}[WARNING] in {}: {}{}\n", ANSI_RESET, ANSI_YELLOW, location.function_name(), fmt::format(format, std::forward<Args>(args)...), ANSI_RESET);
}

template<typename... Args>
static inline void report_error_impl(const std::experimental::source_location& location, const std::string& format, Args&&... args) {
    impl::report_impl("{}{}[ERROR] in {}: {}{}\n", ANSI_RESET, ANSI_RED, location.function_name(), fmt::format(format, std::forward<Args>(args)...), ANSI_RESET);
}

static inline void report_trace_impl(const std::experimental::source_location& location, const std::string& str) {
    impl::report_impl("[TRACE] in {}: {}\n", location.function_name(), str);
}

static inline void report_warning_impl(const std::experimental::source_location& location, const std::string& str) {
    impl::report_impl("{}{}[WARNING] in {}: {}{}\n", ANSI_RESET, ANSI_YELLOW, location.function_name(), str, ANSI_RESET);
}

static inline void report_error_impl(const std::experimental::source_location& location, const std::string& str) {
    impl::report_impl("{}{}[ERROR] in {}: {}{}\n", ANSI_RESET, ANSI_RED, location.function_name(), str, ANSI_RESET);
}
#endif

}

#ifndef TRACE
#define TRACE 0
#endif
#if TRACE
#define report_trace(...) impl::report_trace_impl(std::experimental::source_location::current(), __VA_ARGS__)
#else
#define report_trace(...)
#endif

#if 1
#define report(...) impl::report_info_impl(std::experimental::source_location::current(), __VA_ARGS__)
#else
#define report(...)
#endif
#define report_warning(...) impl::report_warning_impl(std::experimental::source_location::current(), __VA_ARGS__)
#define report_error(...) impl::report_error_impl(std::experimental::source_location::current(), __VA_ARGS__)

#define report_function() report("{}{}{}{}{}", ANSI_RESET, ANSI_UNDERLINE, __PRETTY_FUNCTION__, ANSI_RESET, " was called!")

#ifndef ASSERT
#define ASSERT(cond) _assert(__FILE__, __PRETTY_FUNCTION__, __LINE__, #cond, (cond))
#define ASSERT_NOT_REACHABLE() _assert(__FILE__, __PRETTY_FUNCTION__, __LINE__, "reached unreachable code", false)

#define DEBUG 1 // TODO: CHANGE ME!
inline void _assert(const char* file, const char* function, [[maybe_unused]] unsigned line,
    const char* condition_string, bool result) {
    if (!result) {
#if DEBUG
        fprintf(stderr,
            "%sASSERTION FAILED%s at %s%s:%u%s \n\t-> in %s%s%s, Line %u: \n\t\t-> "
            "Failed Condition: %s%s%s\n",
            ANSI_RED_BOLD, ANSI_RESET, ANSI_UNDERLINE, file, line, ANSI_RESET,
            ANSI_BOLD, function, ANSI_RESET, line, ANSI_RED, condition_string,
            ANSI_RESET);
        fprintf(stderr, "%s... terminating with SIGABRT ...%s\n", ANSI_BOLD, ANSI_RESET);
        abort();
#else
        fprintf(stderr,
            "%s=> ASSERTION `%s` FAILED IN RELEASE BUILD%s%s -> "
            "THROWING INSTEAD AND HOPING IT WON'T CRASH%s\n",
            ANSI_RED_BOLD, condition_string, ANSI_RESET, ANSI_RED, ANSI_RESET);
        char filename[1024];
        std::strncpy(filename, file, sizeof(filename));
        throw std::runtime_error(fmt::format("{}:{} ({}): \"{}\" failed.", basename(filename), line, function, condition_string));
#endif
    }
}

inline void dump_hex(const char* data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (i % 16 == 0) {
            printf("\n");
        } else if (i % 8 == 0) {
            printf("  ");
        }
        printf("%02X ", data[i] & 0xff);
    }
    printf("\n");
}

#endif

template<typename T>
concept ImplementsOutstreamOperator = requires(T a) {
    std::ostream() << a;
};

template<typename T>
concept IterableContainer = requires(T a) {
    a.begin();
    a.end();
    a[0];
};

template<typename T>
concept IterableContainerNotString = IterableContainer<T> && !std::is_convertible_v<T, std::string>;

template<IterableContainerNotString ContainerT>
std::ostream& operator<<(std::ostream& os, const ContainerT& container) {
    os << "{ ";
    for (auto iter = container.begin(); iter != container.end(); ++iter) {
        os << *iter;
        if (auto end_copy = container.end(); iter != --end_copy) {
            os << ", ";
        }
    }
    os << " }";
    return os;
}

#endif // DEBUGTOOLS_H

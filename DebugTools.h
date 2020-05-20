#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <cstdio>
#include <cstdlib>
#include <libgen.h>
#include <iostream>
#include <String.h>

static constexpr ConstString ANSI_RESET = "\u001b[0m";

static constexpr ConstString ANSI_BLACK   = "\u001b[30m";
static constexpr ConstString ANSI_RED     = "\u001b[31m";
static constexpr ConstString ANSI_GREEN   = "\u001b[32m";
static constexpr ConstString ANSI_YELLOW  = "\u001b[33m";
static constexpr ConstString ANSI_BLUE    = "\u001b[34m";
static constexpr ConstString ANSI_MAGENTA = "\u001b[35m";
static constexpr ConstString ANSI_CYAN    = "\u001b[36m";
static constexpr ConstString ANSI_WHITE   = "\u001b[37m";

static constexpr ConstString ANSI_BLACK_BOLD   = "\u001b[30;1m";
static constexpr ConstString ANSI_RED_BOLD     = "\u001b[31;1m";
static constexpr ConstString ANSI_GREEN_BOLD   = "\u001b[32;1m";
static constexpr ConstString ANSI_YELLOW_BOLD  = "\u001b[33;1m";
static constexpr ConstString ANSI_BLUE_BOLD    = "\u001b[34;1m";
static constexpr ConstString ANSI_MAGENTA_BOLD = "\u001b[35;1m";
static constexpr ConstString ANSI_CYAN_BOLD    = "\u001b[36;1m";
static constexpr ConstString ANSI_WHITE_BOLD   = "\u001b[37;1m";

static constexpr ConstString ANSI_BOLD      = "\u001b[1m";
static constexpr ConstString ANSI_UNDERLINE = "\u001b[4m";

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

#define TRACE 1
#if TRACE
#define report_trace(...) std::cout << String::format("[TRACE] in ", FILENAME, ":", __LINE__, " in ", __FUNCTION__, ": ", __VA_ARGS__, ANSI_RESET) << std::endl
#else
#define report_trace(x)
#endif

#define report(...) std::cout << String::format(ANSI_WHITE_BOLD, "[INFO] in ", FILENAME, ":", __LINE__, " in ", __FUNCTION__, ": ", __VA_ARGS__, ANSI_RESET) << std::endl
#define report_warning(...) std::cout << String::format(ANSI_YELLOW_BOLD, "[WARNING] in ", FILENAME, ":", __LINE__, " in ", __FUNCTION__, ": ", __VA_ARGS__, ANSI_RESET) << std::endl
#define report_error(...) std::cout << String::format(ANSI_RED_BOLD, "[ERROR] in ", FILENAME, ":", __LINE__, " in ", __FUNCTION__, ": ", __VA_ARGS__, ANSI_RESET) << std::endl

#define report_function() report(ANSI_RESET, ANSI_UNDERLINE, __PRETTY_FUNCTION__, ANSI_RESET, ANSI_WHITE_BOLD, " was called!")

#ifndef ASSERT
#define ASSERT(cond) _assert(__FILE__, __PRETTY_FUNCTION__, __LINE__, #cond, (cond))
#define ASSERT_NOT_REACHABLE() _assert(__FILE__, __PRETTY_FUNCTION__, __LINE__, "reached unreachable code", false)

#define DEBUG 1 // TODO: CHANGE ME!
inline void _assert(const char* file, const char* function, unsigned line,
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
            "%s=> ASSERTION `%s` FAILED IN RELEASE BUILD%s%s -> IGNORING FAILED ASSERTION "
            "& HOPING IT WON'T CRASH%s\n",
            ANSI_RED_BOLD, condition_string, ANSI_RESET, ANSI_RED, ANSI_RESET);
#endif
    }
}

#endif

#endif // DEBUGTOOLS_H

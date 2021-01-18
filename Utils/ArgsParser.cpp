#include "ArgsParser.h"

#include <string>
#include <vector>

ArgsParser::ArgsParser(int argc, char** argv) {
    m_args.resize(size_t(argc) - 1);
    for (size_t i = 1 /* skip filename */; i < size_t(argc); ++i) {
        // construct and copy string from argv
        m_args[i - 1] = std::string(argv[i]);
    }
}

void ArgsParser::register_flag(const std::string& name, bool& result) {
    m_flags.insert({ name, std::ref(result) });
}

void ArgsParser::register_arg(const std::string& name, int& result) {
    m_int_args.insert({ name, std::ref(result) });
}

void ArgsParser::register_arg(const std::string& name, bool& result) {
    m_bool_args.insert({ name, std::ref(result) });
}

void ArgsParser::register_arg(const std::string& name, std::string& result) {
    m_string_args.insert({ name, std::ref(result) });
}

void ArgsParser::register_arg(const std::string& name, std::vector<std::string>& result) {
    m_string_vector_args.insert({ name, std::ref(result) });
}

static inline std::string extract_argname(const std::string& arg) {
    return arg.substr(0, arg.find('='));
}

std::ostream& operator<<(std::ostream& os, const std::pair<std::string, std::reference_wrapper<int>>& pair) {
    os << "(" << pair.first;
    os << ", " << pair.second.get();
    os << ")";
    return os;
}

bool ArgsParser::parse() {
    report("args: {}", m_args);
    report("int args: {}", m_int_args);
    for (size_t i = 0; i < m_args.size(); ++i) {
        auto arg = m_args[i];
        bool short_arg;
        if (arg.find('=') != std::string::npos) {
            report_warning("arguments with key=value pairs are not yet supported.");
            return false;
        }
        if (arg.starts_with("--")) {
            arg.erase(0, 2);
            short_arg = false;
        } else if (arg.starts_with("-")) {
            arg.erase(0, 1);
            short_arg = true;
        } else {
            report_error("invalid argument: \"{}\"", arg);
            return false;
        }

        auto argname = extract_argname(arg);
        report("argname = {}, short = {}", argname, short_arg);
        if (short_arg) {
            if (argname.size() > 1) {
                report_error("invalid argument \"-{}\", did you mean \"--{}\"?", argname, argname);
                return false;
            }
        } else {
            if (argname.size() == 1) {
                report_error("invalid argument \"--{}\", did you mean \"-{}\"?", argname, argname);
                return false;
            }
        }
        if (m_flags.contains(arg)) {
            m_flags.at(arg).get() = true;
            continue;
        } else if (m_bool_args.contains(arg)) {
            ++i;
            if (i >= m_args.size()) {
                report_error("missing value for argument \"{}\"", argname);
            }
            if (m_args[i] == "true") {
                m_bool_args.at(arg).get() = true;
            } else if (m_args[i] == "false") {
                m_bool_args.at(arg).get() = false;
            } else {
                report_error("argument {} expects \"true\" or \"false\", instead got \"{}\"", argname, m_args[i]);
                return false;
            }
            continue;
        } else if (m_int_args.contains(arg)) {
            ++i;
            if (i >= m_args.size()) {
                report_error("missing value for argument \"{}\"", argname);
            }
            try {
                m_int_args.at(arg).get() = std::stoi(m_args[i]);
            } catch (const std::exception& e) {
                report_error("failed to parse value \"{}\" for argument \"{}\": {}", m_args[i], argname, e.what());
            }
            continue;
        }
        // if we get here, we had no matches
        report_error("unknown/invalid argument \"{}\"", arg);
    }
    return true;
}

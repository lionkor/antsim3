#ifndef ARGSPARSER_H
#define ARGSPARSER_H

#include "Core/Object.h"

#include <any>
#include <map>

class ArgsParser : public Object {
    OBJNAME(ArgsParser)
private:
    std::vector<std::string> m_args;
    std::map<std::string, std::reference_wrapper<bool>> m_flags;
    std::map<std::string, std::reference_wrapper<int>> m_int_args;
    std::map<std::string, std::reference_wrapper<bool>> m_bool_args;
    std::map<std::string, std::reference_wrapper<std::string>> m_string_args;
    std::map<std::string, std::reference_wrapper<std::vector<std::string>>> m_string_vector_args;

public:
    ArgsParser(int argc, char** argv);

    void register_flag(const std::string& name, bool& result);
    void register_arg(const std::string& name, int& result);
    void register_arg(const std::string& name, bool& result);
    void register_arg(const std::string& name, std::string& result);
    void register_arg(const std::string& name, std::vector<std::string>& result);

    bool parse();
};

#endif // ARGSPARSER_H

#ifndef CSVLOGGER_H
#define CSVLOGGER_H

#include "DebugTools.h"
#include "Common.h"
#include <fstream>
#include <atomic>
#include <queue>
#include <thread>

class CsvLogger
{
private:
    std::string             m_filename;
    std::ofstream           m_os;
    std::queue<std::string> m_queue;
    std::thread             m_thread;
    bool                    m_kill_thread { false };

public:
    CsvLogger(const std::string& filename);
    ~CsvLogger();

    bool               ok() const { return m_os.is_open(); }
    const std::string& filename() const { return m_filename; }

    template<typename T>
    void log(const T& value) {
        m_queue.emplace(fmt::format("{},", value));
    }
};

#endif // CSVLOGGER_H

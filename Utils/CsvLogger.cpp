#include "CsvLogger.h"

CsvLogger::CsvLogger(const std::string& filename)
    : m_filename(filename)
    , m_os(filename) {
    auto log_function = [&]() -> void {
        report("CsvLogger thread started, writing to file \"{}\"", m_filename);
        while (!m_kill_thread) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            while (!m_queue.empty()) {
                report("logging {}", m_queue.front());
                m_os << m_queue.front();
                m_queue.pop();
            }
        }
        report("CsvLogger thread for file \"{}\" ended", m_filename);
    };
    m_thread = std::thread(log_function);
}

CsvLogger::~CsvLogger() {
    m_kill_thread = true;
    m_thread.join();
}

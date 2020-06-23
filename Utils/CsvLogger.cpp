// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "CsvLogger.h"

CsvLogger::CsvLogger(const std::string& filename)
    : m_filename(filename)
    , m_os(filename) {
    auto log_function = [&]() -> void {
        report("CsvLogger thread started, writing to file \"{}\"", m_filename);
        while (!m_kill_thread) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (!m_mutex.try_lock())
                continue;
            while (!m_queue.empty()) {
                m_os << m_queue.front();
                m_queue.pop();
            }
            m_mutex.unlock();
        }
        report("CsvLogger thread for file \"{}\" ended", m_filename);
    };
    m_thread = std::thread(log_function);
}

CsvLogger::~CsvLogger() {
    m_kill_thread = true;
    m_thread.join();
}

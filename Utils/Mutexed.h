// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef MUTEXED_H
#define MUTEXED_H

#include <mutex>

template<class T>
class Mutexed
{
private:
    std::mutex m_mutex;
    T m_value;

public:
    Mutexed(T&& value)
        : m_value(std::move(value)) { }

    ~Mutexed() {
        m_mutex.unlock();
    }

    void lock() {
        m_mutex.lock();
    }

    bool try_lock() {
        return m_mutex.try_lock();
    }

    void unlock() {
        m_mutex.unlock();
    }

    std::mutex& mutex() { return m_mutex; }

    T& value() { return m_value; }
};

#endif // MUTEXED_H

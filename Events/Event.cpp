// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Events/Event.h"

Event::Event() {
}

void Event::accept() {
    m_accepted.store(true);
}

bool Event::accepted() const {
    return m_accepted.load();
}

std::stringstream Event::to_stream() const {
    auto ss = Object::to_stream();
    ss << "accepted=" << (m_accepted ? "true" : "false") << ";";
    return ss;
}

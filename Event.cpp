#include "Event.h"

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

#include "EventDispatcher.h"
#include "IEventReceiver.h"
#include "DebugTools.h"

#include <boost/container/vector.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

EventDispatcher::EventDispatcher() {
}

void EventDispatcher::register_receiver(IEventReceiver& recv) {
    m_receivers.push_front(&recv);
}

void EventDispatcher::unregister_receiver(IEventReceiver& recv) {
    boost::remove_erase_if(m_receivers, [&](auto& refwrap) { return *refwrap == recv; });
}

void EventDispatcher::dispatch(Event& event) {
    for (auto& recv : m_receivers) {
        if (event.accepted()) {
            report("accepted, breaking");
            break;
        }
        recv->handle(event);
    }
}

std::stringstream EventDispatcher::to_stream() const {
    auto ss = Object::to_stream();
    ss << "receiver_count=" << m_receivers.size() << ";";
    return ss;
}

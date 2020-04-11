#include "IEventReceiver.h"
#include "DebugTools.h"

IEventReceiver::IEventReceiver(EventDispatcher& dispatcher)
    : m_uuid(s_ieventreceiver_uuid_gen())
    , m_dispatcher(dispatcher) {
    // only triggered if we ever run out of guids
    m_dispatcher.register_receiver(*this);
    report_function();
}

IEventReceiver::~IEventReceiver() {
    m_dispatcher.unregister_receiver(*this);
}

bool IEventReceiver::operator==(const IEventReceiver& e) {
    return m_uuid == e.m_uuid;
}

bool IEventReceiver::operator!=(const IEventReceiver& e) {
    return !(*this == e);
}

void IEventReceiver::handle(Event& e) {
    report_function();
    e.accept();
    report("event received!");
}

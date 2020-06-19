#include "IEventReceiver.h"
#include "Utils/DebugTools.h"


IEventReceiver::IEventReceiver(EventDispatcher& dispatcher) 
    : m_dispatcher(dispatcher) {
}

IEventReceiver::~IEventReceiver() {
}

bool IEventReceiver::operator==(const IEventReceiver&) const {
}

void IEventReceiver::handle(Event&) {
}

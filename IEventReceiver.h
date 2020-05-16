#ifndef IEVENTRECEIVER_H
#define IEVENTRECEIVER_H

#include <boost/atomic.hpp>
#include <cstdint>

#include "EventDispatcher.h"
#include "Object.h"

static boost::uuids::random_generator s_ieventreceiver_uuid_gen;

// Interfaces do not inherit from Object
class IEventReceiver
{
private:
    UUID             m_uuid;
    EventDispatcher& m_dispatcher;

public:
    IEventReceiver(EventDispatcher& dispatcher);
    virtual ~IEventReceiver();

    bool operator==(const IEventReceiver&) const;
    bool operator!=(const IEventReceiver&) const;

    // Will reveice all events. Events received here are guaranteed to not yet be accepted.
    virtual void handle(Event&);
};

#endif // IEVENTRECEIVER_H

#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <boost/container/deque.hpp>
#include "Event.h"

class IEventReceiver;

class EventDispatcher : public Object
{
    OBJECT(EventDispatcher)
private:
    boost::container::deque<IEventReceiver*> m_receivers;

public:
    EventDispatcher();

    EventDispatcher(EventDispatcher&&)      = delete;
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;

    void register_receiver(IEventReceiver&);
    void unregister_receiver(IEventReceiver&);
    void dispatch(Event&);
    
    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // EVENTDISPATCHER_H

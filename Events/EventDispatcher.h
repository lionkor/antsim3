// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <boost/container/deque.hpp>
#include "Events/Event.h"

class IEventReceiver;

class EventDispatcher
{
private:
    boost::container::deque<IEventReceiver*> m_receivers;

public:
    EventDispatcher();

    EventDispatcher(EventDispatcher&&) = delete;
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;

    void register_receiver(IEventReceiver&);
    void unregister_receiver(IEventReceiver&);
    void dispatch(Event&);
};

#endif // EVENTDISPATCHER_H

#ifndef EVENT_H
#define EVENT_H

#include <boost/atomic.hpp>
#include "Core/Object.h"

class Event : public Object
{
    OBJNAME(Event)

private:
    boost::atomic<bool> m_accepted { false };

public:
    Event();

    void accept();
    bool accepted() const;

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // EVENT_H

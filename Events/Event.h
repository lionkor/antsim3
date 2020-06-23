// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

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

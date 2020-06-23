// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

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

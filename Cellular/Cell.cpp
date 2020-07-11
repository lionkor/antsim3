// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Cell.h"
#include "Core/World.h"
#include "Utils/Random.h"
#include <algorithm>


bool Cell::operator==(const Object& other) const {
    return uuid() == other.uuid();
}

bool Cell::operator!=(const Object& other) const {
    return uuid() != other.uuid();
}

// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef CELL_H
#define CELL_H

#include "Core/Object.h"
#include "Physics/vec.h"

class World;

class Cell
    : public Object
{
    OBJNAME(Cell)

    // Object interface
public:
    virtual bool operator==(const Object& other) const override;
    virtual bool operator!=(const Object& other) const override;
};


#endif // CELL_H

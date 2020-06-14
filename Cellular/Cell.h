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

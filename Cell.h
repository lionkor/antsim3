#ifndef CELL_H
#define CELL_H

#include "Object.h"
#include "vec.h"

class World;

class Cell
    : public CopyableObject
{
    OBJECT(Cell)

    // Object interface
public:
    virtual bool operator==(const Object& other) const override;
    virtual bool operator!=(const Object& other) const override;
};


#endif // CELL_H

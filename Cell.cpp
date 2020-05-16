#include "Cell.h"
#include "World.h"
#include "Random.h"
#include <algorithm>


bool Cell::operator==(const Object& other) const {
    return uuid() == other.uuid();
}

bool Cell::operator!=(const Object& other) const {
    return uuid() != other.uuid();
}

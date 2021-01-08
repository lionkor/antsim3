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

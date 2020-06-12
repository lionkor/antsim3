#ifndef HID_H
#define HID_H

/*
 * HIDs: Human Interface Devices 
 * This header contains constructs and helpers for dealing with HIDs.
 */

#include "Physics/vec.h"

enum MouseButton : char
{
    Left,
    Right,
    Middle // FIXME: Add more ( 5 button mice at least )
};

struct MouseClick {
    MouseButton   button;
    vec<unsigned> screen_position;
    vec<double>   world_position;
};

#endif // HID_H

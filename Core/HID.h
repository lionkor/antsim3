#ifndef HID_H
#define HID_H

/*
 * HIDs: Human Interface Devices 
 * This header contains constructs and helpers for dealing with HIDs.
 */

#include "Physics/vec.h"
#include "Core/GameWindow.h"
#include <SFML/Graphics.hpp>

namespace HID {

enum MouseButton : char
{
    Left,
    Right,
    Middle // FIXME: Add more ( 5 button mice at least )
};

struct MouseAction {
    HID::MouseButton button;
    vec<int>         screen_position;
    vec<double>      world_position(GameWindow&);
};

MouseAction from_sf_mouse_action(sf::Event);

}
#endif // HID_H

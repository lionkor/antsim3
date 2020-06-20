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

enum MouseButton
{
    Left,
    Right,
    Middle, 
    Extra1,
    Extra2
};

struct MouseAction {
    HID::MouseButton button;
    vec<int>         screen_position;
    vecd      world_position(GameWindow&) const;
};

MouseAction from_sf_mouse_action(sf::Event);

}
#endif // HID_H

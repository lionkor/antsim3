#include "HID.h"


static inline HID::MouseButton from_sf_mouse_button(sf::Mouse::Button button) {
    switch (button) {
    case sf::Mouse::Left:
        return HID::MouseButton::Left;
    case sf::Mouse::Right:
        return HID::MouseButton::Right;
    case sf::Mouse::Middle:
        return HID::MouseButton::Middle;
    default:
        ASSERT_NOT_REACHABLE();
    }
    return HID::MouseButton::Left; // never happens
}

HID::MouseAction HID::from_sf_mouse_action(sf::Event event) {
    HID::MouseAction action;
    switch (event.type) {
    case sf::Event::MouseMoved:
        [[fallthrough]];
    case sf::Event::MouseButtonPressed:
        [[fallthrough]];
    case sf::Event::MouseButtonReleased: {
        action.button          = from_sf_mouse_button(event.mouseButton.button);
        action.screen_position = vec<int>(event.mouseButton.x, event.mouseButton.y);
        break;
    }
    default:
        ASSERT_NOT_REACHABLE();
    }
    return action;
}

vec<double> HID::MouseAction::world_position(GameWindow& window) {
    return window.mapPixelToCoords(sf::Vector2i { screen_position.x, screen_position.y });
}

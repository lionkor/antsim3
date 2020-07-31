// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "HID.h"


static inline HID::MouseButton from_sf_mouse_button(sf::Mouse::Button button) {
    // just cast
    return HID::MouseButton(button);
}

HID::MouseAction HID::from_sf_mouse_action(sf::Event event) {
    HID::MouseAction action;
    action.button = from_sf_mouse_button(event.mouseButton.button);
    switch (event.type) {
    case sf::Event::MouseMoved:
        action.screen_position = { event.mouseMove.x, event.mouseMove.y };
        break;
    case sf::Event::MouseButtonPressed:
        [[fallthrough]];
    case sf::Event::MouseButtonReleased: {
        action.screen_position = { event.mouseButton.x, event.mouseButton.y };
        break;
    }
    default:
        ASSERT_NOT_REACHABLE();
    }
    return action;
}

vecd HID::MouseAction::world_position(GameWindow& window) const {
    return window.mapPixelToCoords(sf::Vector2i { screen_position.x, screen_position.y });
}

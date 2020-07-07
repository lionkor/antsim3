// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "Rendering/Draw.h"
#include "Rendering/SimpleDrawable.h"

class PlayerComponent : public Component
{
    OBJNAME(PlayerComponent)
private:
    std::string    m_name;
    SimpleDrawable m_drawable;
    vec<size_t>    m_size { 10, 10 };
    vecd           m_last_position;
    sf::Text       m_text;
    bool           m_is_player_controlled { false };
    bool           m_dir_pressed[4] { false, false, false, false };
    bool           m_is_initialized { false };
    sf::Clock      m_update_clock;

    void key_released(HID::Key key);
    void key_pressed(HID::Key key);

public:
    PlayerComponent(Entity& e, const std::string& name);
    ~PlayerComponent();

    virtual void on_update() override;

    virtual void on_draw(DrawSurface& surface) override {
        auto         pos    = parent().transform().position();
        sf::Vector2f sf_pos = sf::Vector2f(pos.x, pos.y);
        sf::Vector2f real_position(sf_pos.x, sf_pos.y);
        sf::Vector2i screen_pos = surface.window().mapCoordsToPixel(real_position);
        m_text.setPosition(screen_pos.x, screen_pos.y);
        surface.draw_text(m_text);
        m_drawable.draw(surface);
    }

    void set_is_player_controlled(bool is) { m_is_player_controlled = is; }
    bool is_player_controlled() const { return m_is_player_controlled; }

    const std::string& name() const { return m_name; }
};

#endif // PLAYERCOMPONENT_H

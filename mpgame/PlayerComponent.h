#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "Rendering/Draw.h"

class PlayerComponent : public Component
{
    OBJNAME(PlayerComponent)
private:
    std::string m_name;
    Rectangle m_drawable;
    vec<size_t> m_size { 10, 10 };
    vecd m_last_position;
    sf::Text m_text;
    bool m_is_player_controlled { false };
    bool m_dir_pressed[4] { false, false, false, false };
    bool m_is_initialized { false };
    sf::Clock m_heartbeat_clock;
    sf::Clock m_update_clock;

    void key_released(HID::Key key);
    void key_pressed(HID::Key key);

public:
    PlayerComponent(Entity& e, const std::string& name);
    ~PlayerComponent();

    virtual void on_update(float) override;

    virtual void on_draw(DrawSurface& surface) override;

    void set_is_player_controlled(bool is) { m_is_player_controlled = is; }
    bool is_player_controlled() const { return m_is_player_controlled; }

    const std::string& name() const { return m_name; }
};

#endif // PLAYERCOMPONENT_H

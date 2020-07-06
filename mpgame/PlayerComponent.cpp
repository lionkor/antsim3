#include "PlayerComponent.h"
#include "ClientComponent.h"

PlayerComponent::PlayerComponent(Entity& e, const std::string& name)
    : Component(e)
    , m_name(name)
    , m_drawable(SimpleDrawable::PrimitiveType::Quads, 4)
    , m_last_position(parent().transform().position()) {
    ASSERT(parent().has_parent());

    // init drawable
    auto         transform_pos = parent().transform().position();
    sf::Vector2f pos           = sf::Vector2f(transform_pos.x, transform_pos.y);
    sf::Color    random_color(Random::random(64, 255), Random::random(64, 255), Random::random(64, 255));
    m_drawable[0] = sf::Vertex(pos, random_color);
    m_drawable[1] = sf::Vertex(sf::Vector2f(pos.x + m_size.x, pos.y), random_color);
    m_drawable[2] = sf::Vertex(pos + sf::Vector2f(m_size.x, m_size.y), random_color);
    m_drawable[3] = sf::Vertex(sf::Vector2f(pos.x, pos.y + m_size.y), random_color);
    m_drawable.set_changed();

    on_key_down = [&](GameWindow&, const HID::Key& key) {
        key_pressed(key);
    };

    on_key_up = [&](GameWindow&, const HID::Key& key) {
        key_released(key);
    };
}

PlayerComponent::~PlayerComponent() {
    if (m_is_player_controlled) {
        report_warning("playable player destroyed (ignore on shutdown)");
    }
}


void PlayerComponent::key_released(HID::Key key) {
    switch (key) {
    case HID::Key::W:
        m_dir_pressed[0] = false;
        break;
    case HID::Key::A:
        m_dir_pressed[1] = false;
        break;
    case HID::Key::S:
        m_dir_pressed[2] = false;
        break;
    case HID::Key::D:
        m_dir_pressed[3] = false;
        break;
    default:
        // nothing
        break;
    }
}

void PlayerComponent::key_pressed(HID::Key key) {
    switch (key) {
    case HID::Key::W:
        m_dir_pressed[0] = true;
        break;
    case HID::Key::A:
        m_dir_pressed[1] = true;
        break;
    case HID::Key::S:
        m_dir_pressed[2] = true;
        break;
    case HID::Key::D:
        m_dir_pressed[3] = true;
        break;
    default:
        // nothing
        break;
    }
}

void PlayerComponent::on_update() {
    if (!m_is_initialized) {
        m_is_initialized = true;
        ASSERT(parent().parent()->has_component<ClientComponent>());
        auto* client_comp = parent().parent()->fetch_component<ClientComponent>();
        m_text            = sf::Text(m_name, client_comp->font(), 10);
        m_update_clock.restart();
    }
    if (m_is_player_controlled) {
        vecd vel(0, 0);
        if (m_dir_pressed[0]) {
            vel.y -= 1;
        }
        if (m_dir_pressed[1]) {
            vel.x -= 1;
        }
        if (m_dir_pressed[2]) {
            vel.y += 1;
        }
        if (m_dir_pressed[3]) {
            vel.x += 1;
        }
        if (vel.length() != 0) {
            vel.normalize();
        }
        parent().transform().move_by(vel);
    }
    
    if (m_update_clock.getElapsedTime().asMilliseconds() > 40) {
        auto  pos  = parent().transform().position();
        auto* comp = parent().parent()->fetch_component<ClientComponent>();
        comp->send_packet(UpdatePacket { m_name, pos.x, pos.y });
        m_update_clock.restart();
    }

    auto transform_pos = parent().transform().position();
    if (transform_pos != m_last_position) {
        m_last_position        = transform_pos;
        sf::Vector2f pos       = sf::Vector2f(transform_pos.x, transform_pos.y);
        m_drawable[0].position = pos;
        m_drawable[1].position = sf::Vector2f(pos.x + m_size.x, pos.y);
        m_drawable[2].position = pos + sf::Vector2f(m_size.x, m_size.y);
        m_drawable[3].position = sf::Vector2f(pos.x, pos.y + m_size.y);
        m_drawable.set_changed();
    }
}

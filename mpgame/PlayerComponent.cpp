#include "PlayerComponent.h"
#include "ClientComponent.h"

#include "Utils/Random.h"

PlayerComponent::PlayerComponent(Entity& e, const std::string& name)
    : Component(e)
    , m_name(name)
    , m_last_position(parent().transform().position()) {
    ASSERT(parent().has_parent());

    // init drawable
    auto transform_pos = parent().transform().position();
    sf::Vector2f pos = sf::Vector2f(transform_pos.x, transform_pos.y);
    Color random_color(Random::random(64, 255), Random::random(64, 255), Random::random(64, 255), 255);
    m_drawable.set_position(transform_pos);
    m_drawable.set_color(random_color);

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

vecd vel(Random::random(-10, 10), Random::random(-10, 10));

void PlayerComponent::on_update(float) {
    if (!m_is_initialized) {
        m_is_initialized = true;
        ASSERT(parent().parent()->has_component<ClientComponent>());
        auto* client_comp = parent().parent()->fetch_component<ClientComponent>();
        m_text = sf::Text(m_name, client_comp->font(), 10);
        m_heartbeat_clock.restart();
        m_update_clock.restart();
        // send first update packet
        auto pos = parent().transform().position();
        auto* comp = parent().parent()->fetch_component<ClientComponent>();
        comp->send_packet(UpdatePacket { m_name, pos.x, pos.y });
    }
    if (m_is_player_controlled) {
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

        if (m_heartbeat_clock.getElapsedTime().asSeconds() > 1.0f) {
            m_heartbeat_clock.restart();
            auto pos = parent().transform().position();
            auto* comp = parent().parent()->fetch_component<ClientComponent>();
            comp->send_packet(UpdatePacket(m_name, pos.x, pos.y, UpdatePacket::Heartbeat));
        }
        auto transform_pos = parent().transform().position();
        if (transform_pos != m_last_position && m_update_clock.getElapsedTime().asMilliseconds() >= 50) {
            m_update_clock.restart();
            m_last_position = transform_pos;
            auto pos = parent().transform().position();
            auto* comp = parent().parent()->fetch_component<ClientComponent>();
            comp->send_packet(UpdatePacket { m_name, pos.x, pos.y });
        }
    }
    auto transform_pos = parent().transform().position();
    m_drawable.set_position(transform_pos);
}

void PlayerComponent::on_draw(DrawSurface& surface) {
    auto pos = parent().transform().position();
    sf::Vector2f sf_pos = sf::Vector2f(pos.x, pos.y);
    sf::Vector2f real_position(sf_pos.x, sf_pos.y);
    sf::Vector2i screen_pos = surface.window().mapCoordsToPixel(real_position);
    m_text.setPosition(screen_pos.x, screen_pos.y);
    //surface.draw_text(m_text);
    //m_drawable.draw(surface);
    surface.draw(m_drawable);
}

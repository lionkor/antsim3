/**
 * Main program, where the actual antsim will go
 */

#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>
#include <cmath>
#include <chrono>

#include <functional>

#include "Engine.h"

class SimpleMovementComponent
    : public Component
{
    OBJNAME(SimpleMovementComponent)

public:
    SimpleMovementComponent(Entity& e)
        : Component(e) {
        
        
        on_mouse_down = [](GameWindow& window, const HID::MouseAction& action) {
            report("button {} was pressed at {}, which is {} in world coords!", action.button, action.screen_position, action.world_position(window));
        };
        
        
    }

    virtual void on_update(float) override {
        auto& transform = parent().transform();
        transform.move_by({ 0.1, 0 });
    }
};
/*
class GuiElementComponent
    : public Component
{
    OBJNAME(GuiElementComponent)

    sf::Text m_text_obj;
    sf::Font m_font;

public:
    GuiElementComponent(Entity& e, const std::string& text, const std::string& font_resource_name, sf::Color color = sf::Color::White, const vecu& relative_pos = { 0, 0 }, const vecd& scale = { 1., 1. })
        : Component(e) {
        auto entity_pos = e.transform().position();
        m_text_obj.setPosition(float(entity_pos.x), float(entity_pos.y));
        m_text_obj.setScale(float(scale.x), float(scale.y));
        m_text_obj.setFillColor(color);
        m_text_obj.setString(text);
        auto result = resource_manager().get_resource_by_name(font_resource_name);
        if (result.error()) {
            report_error(result.message());
            return;
        }
        auto& file = result.value().get();
        if (!file.is_valid()) {
            report_error(file.validation_error_message());
            return;
        }
        auto* data_ptr = file.load();
        auto loaded = m_font.loadFromMemory(data_ptr->data(), data_ptr->size());
        if (!loaded) {
            report_error("failed loading \"{}\" as font!", font_resource_name);
            return;
        }
        report("loaded font {}", font_resource_name);
        m_text_obj.setFont(m_font);
    }

    void set_text(const std::string& str) {
        m_text_obj.setString(str);
    }

    std::string text() const {
        return m_text_obj.getString();
    }

    virtual void on_update(float) override {
        const auto entity_pos = parent().transform().position();
        m_text_obj.setPosition(float(entity_pos.x), float(entity_pos.y));
    }
    virtual void on_draw(DrawSurface& surface) override {
        surface.draw(m_text_obj);
    }
};
*/


int main(int, char**) {
    Application app("AntSim3", { 1280, 720 });

    auto& world = app.world();

    auto entity = world.add_entity({ 100, 200 });
    auto shared_entity = entity.lock();
    //(void)shared_entity->add_component<SpriteComponent>(vecd { 0 }, vecd { 100, 100 }, Color::White, "planet.png");
    shared_entity->add_component<SimpleMovementComponent>();
    auto& tilemap_comp = shared_entity->add_component<TileMapComponent>(vec<size_t> { 20, 20 }, 10.0, "atlas1.png", 32);
    tilemap_comp.tilemap().randomize_textures();

    return app.run();
}

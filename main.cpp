/**
 * Main program, where the actual antsim will go
 */

#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>
#include <cmath>
#include <chrono>

#include "Core.h"
#include "Utils.h"
#include "ECS/Entity.h"

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

    virtual void on_update() override {
        auto& transform = parent().transform();
        transform.move_by({ 0.05, 0 });
    }
};

int main(int, char**) {
    Application app("AntSim3", { 1280, 720 });

    auto& world = app.world();
    auto& window = app.window();
    static_cast<void>(window);

    auto entity = world.add_entity({ 100, 200 });
    auto shared_entity = entity.lock();
    shared_entity->add_component<SpriteComponent>(vecd { 10, 10 }, vecd { 100, 100 }, Color::Blue, "sprite1.png");
    shared_entity->add_component<SpriteComponent>(vecd { 50, 50 }, vecd { 50, 50 }, Color::Blue, "sprite1.png");
    report("has SpriteComponent? {}", shared_entity->has_component<SpriteComponent>());
    shared_entity->add_component<SimpleMovementComponent>();

    auto child = shared_entity->add_child();
    child.lock()->add_component<SpriteComponent>(vecd { 120, 0 }, vecd { 90, 90 }, Color::Red, "sprite1.png");

    auto label_ptr = app.add_gui_element(vecu(20, 20), vecd(0.4, 0.4), "Hello, World!").lock();
    label_ptr->on_update = [&]() {
        label_ptr->set_text(fmt::format("screen pos: {}\nworld  pos: {}", window.mouse_screen_pos(), window.mouse_world_pos()));
    };

    std::cout << window << std::endl;

    return app.run();
}

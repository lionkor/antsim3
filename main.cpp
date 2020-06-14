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
    SimpleMovementComponent() {
        on_mouse_down = [](GameWindow& window, const HID::MouseAction& action) {
            report("button {} was pressed at {}, which is {} in world coords!", action.button, action.screen_position, action.world_position(window));
        };
    }

    virtual void on_update() override {
        auto& transform = parent()->transform();
        transform.move_by({ 0.05, 0 });
    }
};

int main(int, char**) {
    Application app(new GameWindow("AntSim3", { 1280, 720 }), new World);

    auto& world  = app.world();
    auto& window = app.window();
    static_cast<void>(window);

    auto entity        = world.add_entity(new Entity({ 100, 200 }));
    auto shared_entity = entity.lock();
    shared_entity->add_component(new SpriteComponent({ 10, 10 }, { 100, 100 }, Color::Blue, "sprite1.png"));
    report("has SpriteComponent? {}", shared_entity->has_component<SpriteComponent>());
    shared_entity->add_component(new SimpleMovementComponent);

    auto child = shared_entity->add_child(new Entity());
    child.lock()->add_component(new SpriteComponent({ 120, 0 }, { 90, 90 }, Color::Red, "sprite1.png"));

    world.update(window);

    return app.run();
}

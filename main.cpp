#include <iostream>
#include <SFML/Graphics.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>
#include <cmath>
#include <chrono>

#include "Core.h"
#include "Utils.h"
#include "ECS/Entity.h"

int main(int, char**) {
    Application app(new GameWindow("AntSim3", { 1280, 720 }), new World);

    auto& world  = app.world();
    auto& window = app.window();
    static_cast<void>(window);

    auto& entity = world.add_entity(new Entity({ 100, 200 }));
    entity.add_component(new SpriteComponent({ 10, 10 }, { 100, 100 }));

    return app.run();
}

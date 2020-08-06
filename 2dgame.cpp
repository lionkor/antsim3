#include "Engine.h"

class WorldComponent : public Component
{
    OBJNAME(WorldComponent)
public:
    WorldComponent(Entity& e)
        : Component(e) {
    }
};

int main() {
    try {
        Application app("2dgame", { 960, 540 }, "Data/2dgame_res.list");
        World& world = app.world();
        [[maybe_unused]] GameWindow& window = app.window();

        auto gameworld_weak = world.add_entity({ 0, 0 });
        if (!gameworld_weak.expired()) {
            auto gameworld = gameworld_weak.lock();
            gameworld->add_component<WorldComponent>();
        }

        return app.run();
    } catch (std::exception& _e) {
        report_error("uncaught exception: \n\t{}", _e.what());
    }
}

#include "Engine.h"

int main(int argc, char** argv) {
    ArgsParser parser(argc, argv);

    int width = 100;
    int height = 100;
    parser.register_arg("w", width);
    parser.register_arg("width", width);
    parser.register_arg("h", height);
    parser.register_arg("height", height);

    bool ok = parser.parse();
    if (!ok) {
        report_error("invalid arguments!");
        return -1;
    }

    report("w, h: {}, {}", width, height);

    Application app("tilemap editor", { 1280, 720 });

    auto tilemap_entity = app.world().add_entity({ 0, 0 });

    tilemap_entity.lock()->add_component<TileMapComponent>();
}

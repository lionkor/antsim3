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
    auto gui_layer = app.window().add_gui_layer<GuiLayer>();
    auto font = app.resource_manager().load_font("mono.ttf");
    ASSERT(!!font);
    auto widget = gui_layer.lock()->add_widget<Widget>(vecd { 10, 10 }, vecd { 40, 20 }, fmt::format("new map {}x{}", width, height), *font);
    widget.lock()->set_resize_to_text(true);
    widget.lock()->set_background_color(Color::White);
    widget.lock()->on_click_callback = [&](Widget& widget, const vecd&) {
        report("{} was clicked!", widget);
        widget.disable();
    };

    auto tilemap_entity = app.world().add_entity({ 0, 0 });

    //tilemap_entity.lock()->add_component<TileMapComponent>();

    return app.run();
}

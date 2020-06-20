#include "doctest.h"

#include "Core/Application.h"

TEST_CASE("Application interface") {
    Application app("test", { 100, 50 });
    CHECK(app.class_name() == "Application");
    CHECK(app.window().title() == "test");
    CHECK(app.window().width<int>() == 100);
    CHECK(app.window().height<int>() == 50);
    // TODO: Check that World has been initialized correctly
}

TEST_CASE("GameWindow interface") {
    Application app("test", { 100, 50 });
    auto&       window = app.window();
    CHECK(window.width<int>() == 100);
    CHECK(window.height<int>() == 50);
    CHECK(window.title() == "test");
    window.set_title("something else");
    CHECK(window.title() == "something else");
    window.set_title("");
    CHECK(window.title() == "");
}

TEST_CASE("World interface") {
    SUBCASE("add entity returns proper ptr and attached properly") {
        Application app("", { 10, 10 });
        World&      world = app.world();

        auto entity_ptr = world.add_entity();
        CHECK(!entity_ptr.expired());
        // only this entity_ptr and the World should have a strong reference
        CHECK(entity_ptr.lock().use_count() == 2);
        // adding an entity takes one update cycle
        world.update(app.window());
        // and removes the first ref again so we dont end up with a dangling ref
        CHECK(entity_ptr.lock().use_count() == 2);
    }
}


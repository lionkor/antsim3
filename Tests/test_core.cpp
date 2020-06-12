#include "doctest.h"

#include "Core/Application.h"

TEST_CASE("Application interface") {
    Application app(new GameWindow("test", { 100, 50 }), new World);
    CHECK(app.class_name() == "Application");
    CHECK(app.window().title() == "test");
    CHECK(app.window().width<int>() == 100);
    CHECK(app.window().height<int>() == 50);
    // TODO: Check that World has been initialized correctly
}

TEST_CASE("GameWindow interface") {
    GameWindow window("test", { 100, 50 });
    CHECK(window.width<int>() == 100);
    CHECK(window.height<int>() == 50);
    CHECK(window.title() == "test");
    window.set_title("something else");
    CHECK(window.title() == "something else");
    window.set_title("");
    CHECK(window.title() == "");
}

TEST_CASE("World interface") {
    SUBCASE("throws with no application attached") {
        World world;
        CHECK(world.class_name() == "World");
        GameWindow window("", { 100, 100 });
        CHECK_THROWS(world.update(window));
    }
    SUBCASE("add entity returns proper ptr and attached properly") {
        GameWindow* window = new GameWindow("", { 10, 10 });
        Application app(std::move(window), new World);
        World& world = app.world();
        Entity* entity     = new Entity;
        
        auto    entity_ptr = world.add_entity(std::move(entity));
        CHECK(!entity_ptr.expired());
        // only this entity_ptr and the World should have a strong reference
        CHECK(entity_ptr.lock().use_count() == 2);
        // adding an entity takes one update cycle
        world.update(app.window());
        // and removes the first ref again so we dont end up with a dangling ref
        CHECK(entity_ptr.lock().use_count() == 2);
        CHECK(**world.begin() == *entity_ptr.lock());
    }
    SUBCASE("clears moved-from") {
        World   world;
        Entity* entity = new Entity;
        world.add_entity(std::move(entity));
        CHECK(entity == nullptr);
    }
}



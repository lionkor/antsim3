#include "doctest.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Core/World.h"
#include "Core/GameWindow.h"
#include "Core/Application.h"

TEST_CASE("Entity ctor") {
    SUBCASE("non const") {
        Entity e;
        CHECK(e.transform().position() == vec<double> { 0, 0 });
        CHECK(e.transform().rotation() == 0.0);
        CHECK(e.parent() == nullptr);
        CHECK(e.children().empty() == true);
        CHECK(e.is_marked_destroyed() == false);
        CHECK(e.has_world() == false);
        CHECK(e.has_parent() == false);
        CHECK(e.has_component<TransformComponent>() == true);
        CHECK_THROWS(e.world());
    }
    SUBCASE("const") {
        const Entity e;
        CHECK(e.transform().position() == vec<double> { 0, 0 });
        CHECK(e.transform().rotation() == 0.0);
        CHECK(e.parent() == nullptr);
        CHECK(e.children().empty() == true);
        CHECK(e.is_marked_destroyed() == false);
        CHECK(e.has_world() == false);
        CHECK(e.has_parent() == false);
        CHECK(e.has_component<TransformComponent>() == true);
        CHECK_THROWS(e.world());
    }
}

TEST_CASE("Entity::destroy without a World") {
    Entity e;
    CHECK(e.is_marked_destroyed() == false);
    e.destroy();
    CHECK(e.is_marked_destroyed() == true);
}

TEST_CASE("Entity::destroy with a world") {
    Application app(new GameWindow("", { 10, 10 }), new World);
    World&      world    = app.world();
    Entity*     e        = new Entity;
    auto        ret_e    = world.add_entity(std::move(e));
    world.update(app.window());
    REQUIRE(ret_e.expired() == false);
    {
        auto lock = ret_e.lock();
        CHECK(lock->is_marked_destroyed() == false);
        lock->destroy();
        CHECK(lock->is_marked_destroyed() == true);
    }
    // after the destroy, in the next update, the world should remove the entity
    world.update(app.window());
    // check if this happened
    CHECK(ret_e.expired() == true);
    CHECK(world.end() - world.begin() == 0);
}

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
    World&      world = app.world();
    Entity*     e     = new Entity;
    auto        ret_e = world.add_entity(std::move(e));
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

TEST_CASE("Entity hierarchy") {
    Application app(new GameWindow("", { 10, 10 }), new World);
    World&      world  = app.world();
    GameWindow& window = app.window();
    SUBCASE("add child") {
        auto    entity_ptr = world.add_entity(new Entity);
        Entity* entity     = new Entity;
        auto    uuid       = entity->uuid();
        auto    child_ptr  = entity_ptr.lock()->add_child(std::move(entity));
        CHECK(entity_ptr.lock()->children().size() == 1);
        CHECK(entity_ptr.lock()->children().front()->uuid() == uuid);
        CHECK(child_ptr.lock()->has_parent() == true);
        CHECK(child_ptr.lock()->has_world() == true);
        CHECK(child_ptr.lock()->has_component<TransformComponent>() == true);
        world.update(window);
        auto& entities = world.entities();
        CHECK(std::find_if(entities.begin(), entities.end(), [&](auto& ptr) -> bool {
            return child_ptr.lock().get() == ptr.get();
        }) != entities.end());
    }
    SUBCASE("destroy child") {
        CHECK(world.entities().empty());
        auto entity_ptr = world.add_entity(new Entity).lock();
        CHECK(entity_ptr->children().empty());
        auto child_ptr = entity_ptr->add_child(new Entity).lock();
        CHECK(entity_ptr->children().empty() == false);
        world.update(window);
        CHECK(entity_ptr->children().size() == 1);
        CHECK(world.entities().size() == 2);
        child_ptr->destroy();
        CHECK(entity_ptr->children().empty());
        world.update(window);
        CHECK(entity_ptr->children().empty());
        CHECK(world.entities().size() == 1);
        CHECK(*world.entities().front() == *entity_ptr);
    }
    SUBCASE("destroy parent") {
        auto entity_ptr = world.add_entity(new Entity).lock();
        auto child_ptr  = entity_ptr->add_child(new Entity).lock();
        world.update(window);
        entity_ptr->destroy();
        world.update(window);
        CHECK(world.entities().empty());
    }
}

TEST_CASE("Components Unique flag") {
    class UniqueComponent : public Component
    {
        OBJNAME(UniqueComponent)
    public:
        virtual bool is_unique() const override { return true; }
    };

    class NonUniqueComponent : public Component
    {
        OBJNAME(NonUniqueComponent)
    public:
        virtual bool is_unique() const override { return false; }
    };

    Application app(new GameWindow("", { 10, 10 }), new World);
    World&      world = app.world();
    auto        e     = world.add_entity(new Entity).lock();
    CHECK_NOTHROW(e->add_component(new UniqueComponent));
    CHECK_THROWS(e->add_component(new UniqueComponent));

    CHECK_NOTHROW(e->add_component(new NonUniqueComponent));
    CHECK_NOTHROW(e->add_component(new NonUniqueComponent));
}

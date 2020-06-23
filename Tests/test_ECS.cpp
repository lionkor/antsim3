// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "doctest.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Core/World.h"
#include "Core/GameWindow.h"
#include "Core/Application.h"

TEST_CASE("Entity ctor") {
    SUBCASE("non const") {
        Application app("", { 10, 10 });
        World&      world = app.world();
        Entity&     e     = *world.add_entity({ 0, 0 }).lock();
        CHECK(e.transform().position() == vecd { 0, 0 });
        CHECK(e.transform().rotation() == 0.0);
        CHECK(e.parent() == nullptr);
        CHECK(e.children().empty() == true);
        CHECK(e.is_marked_destroyed() == false);
        CHECK(e.has_parent() == false);
        CHECK(e.has_component<TransformComponent>() == true);
        CHECK_THROWS(e.world());
    }
    SUBCASE("const") {
        Application   app("", { 10, 10 });
        World&        world = app.world();
        const Entity& e     = *world.add_entity({ 0, 0 }).lock();
        CHECK(e.transform().position() == vecd { 0, 0 });
        CHECK(e.transform().rotation() == 0.0);
        CHECK(e.parent() == nullptr);
        CHECK(e.children().empty() == true);
        CHECK(e.is_marked_destroyed() == false);
        CHECK(e.has_parent() == false);
        CHECK(e.has_component<TransformComponent>() == true);
        CHECK_THROWS(e.world());
    }
}

TEST_CASE("Entity::destroy without a World") {
    Application app("", { 10, 10 });
    World&      world = app.world();
    Entity&     e     = *world.add_entity({ 0, 0 }).lock();

    CHECK(e.is_marked_destroyed() == false);
    e.destroy();
    CHECK(e.is_marked_destroyed() == true);
}

TEST_CASE("Entity::destroy with a world") {
    Application app("", { 10, 10 });
    World&      world = app.world();
    Entity&     e     = *world.add_entity({ 0, 0 }).lock();
    auto        ret_e = world.add_entity(e);
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
}

TEST_CASE("Entity hierarchy") {
    Application app("", { 10, 10 });
    World&      world  = app.world();
    GameWindow& window = app.window();
    SUBCASE("add child") {
        auto entity_ptr = world.add_entity();
        auto child_ptr  = entity_ptr.lock()->add_child();
        CHECK(entity_ptr.lock()->children().size() == 1);
        CHECK(child_ptr.lock()->has_parent() == true);
        CHECK(child_ptr.lock()->has_component<TransformComponent>() == true);
        world.update(window);
        auto& entities = world.entities();
        CHECK(std::find_if(entities.begin(), entities.end(), [&](auto& ptr) -> bool {
            return child_ptr.lock().get() == ptr.get();
        }) != entities.end());
    }
    SUBCASE("destroy child") {
        CHECK(world.entities().empty());
        auto entity_ptr = world.add_entity().lock();
        CHECK(entity_ptr->children().empty());
        auto child_ptr = entity_ptr->add_child().lock();
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
        auto entity_ptr = world.add_entity().lock();
        auto child_ptr  = entity_ptr->add_child().lock();
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
        UniqueComponent(Entity& e)
            : Component(e) { }
        virtual bool is_unique() const override { return true; }
    };

    class NonUniqueComponent : public Component
    {
        OBJNAME(NonUniqueComponent)
    public:
        NonUniqueComponent(Entity& e)
            : Component(e) { }
        virtual bool is_unique() const override { return false; }
    };

    Application app("", { 10, 10 });
    World&      world = app.world();
    auto        e     = world.add_entity().lock();
    CHECK_NOTHROW(e->add_component<UniqueComponent>());
    CHECK_THROWS(e->add_component<UniqueComponent>());

    CHECK_NOTHROW(e->add_component<NonUniqueComponent>());
    CHECK_NOTHROW(e->add_component<NonUniqueComponent>());
}

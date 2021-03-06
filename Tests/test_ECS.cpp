﻿#include "doctest.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Core/World.h"
#include "Core/GameWindow.h"
#include "Core/Application.h"

TEST_CASE("Entity add/remove/...") {
    Application app("Test", { 512, 512 }, false);
    auto& world = app.world();
    SUBCASE("Adding an Entity") {
        CHECK(world.entities().size() == 0);
        WeakPtr<Entity> entity_weak = world.add_entity({ 1, 2 });
        // update once to move from to_add queue to entities vector
        world.update(app.window(), 0.1f);
        SharedPtr<Entity> entity_shared = entity_weak.lock();
        Entity& entity = *entity_shared;
        CHECK(entity.transform().position() == vecd(1, 2));
        CHECK(entity.transform().rotation() == 0.0);
        CHECK(world.entities().size() == 1);
    }
    SUBCASE("Removing an Entity through Entity::destroy()") {
        CHECK(world.entities().size() == 0);
        WeakPtr<Entity> entity_weak = world.add_entity({ 1, 2 });
        // update once to move from to_add queue to entities vector
        world.update(app.window(), 0.1f);
        CHECK(world.entities().size() == 1);
        entity_weak.lock()->destroy();
        CHECK(world.entities().size() == 1);
        CHECK(entity_weak.expired() == false);
        world.update(app.window(), 0.1f);
        CHECK(world.entities().size() == 0);
        CHECK(entity_weak.expired());
    }
}

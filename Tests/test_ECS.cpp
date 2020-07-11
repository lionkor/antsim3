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

TEST_CASE("Entity Tests") {
    Application app("Test", { 512, 512 });
    auto& world = app.world();
    SUBCASE("Adding an Entity") {
        CHECK(world.entities().size() == 0);
        WeakPtr<Entity> entity_weak = world.add_entity({ 1, 2 });
        // update once to move from to_add queue to entities vector
        world.update(app.window());
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
        world.update(app.window());
        CHECK(world.entities().size() == 1);
        entity_weak.lock()->destroy();
        world.update(app.window());
        CHECK(world.entities().size() == 0);
        CHECK(entity_weak.expired());
    }
}

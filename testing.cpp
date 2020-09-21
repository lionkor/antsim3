// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Engine.h"

#include "ECS/ScriptableComponent.h"


int main() {
    Application app("testing!", { 512, 512 });

    auto& world = app.world();

    {
        auto entity = world.add_entity().lock();
        entity->add_component<SpriteComponent>(vecd { -10, -10 }, vecd { 20, 20 }, sf::Color::Red);
        auto& component = entity->add_component<GenericModuleComponent>("Data/c_test.so");
        //        auto& component = entity->add_component<ScriptableComponent>("test_script.lua");
    }

    return app.run();
}

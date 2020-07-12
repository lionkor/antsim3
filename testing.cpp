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
        auto& component = entity->add_component<ScriptableComponent>("test_script.lua");
        component.register_global(69, "nice_number");
    }

    return app.run();
}

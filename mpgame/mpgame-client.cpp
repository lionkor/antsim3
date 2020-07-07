// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "mpgame-common.h"
#include "ClientComponent.h"

int main(int argc, char** argv) {
    Application app("MPGAME Client", { 512, 512 });
    auto&       world = app.world();

    { // scope for shared_ptr lock
        auto player = world.add_entity({ 0, 0 }).lock();
        ASSERT(argc == 2);
        player->add_component<ClientComponent>("localhost", 26999, argv[1]);
    }

    return app.run();
    /*
    Client client { "localhost", 26999, argv[1] };
    report("client running");
    return client.run();
    */
}

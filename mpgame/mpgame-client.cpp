﻿#include "mpgame-common.h"
#include "ClientComponent.h"
#include "Core/Application.h"

int main(int argc, char** argv) {
    Application app("MPGAME Client", { 512, 512 });
    auto& world = app.world();

    { // scope for shared_ptr lock
        auto player = world.add_entity({ 0, 0 }).lock();
        ASSERT(argc == 2);
        (void)player->add_component<ClientComponent>("localhost", 26999, argv[1]);
    }

    return app.run();
}

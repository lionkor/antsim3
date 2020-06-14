#ifndef APPLICATION_H
#define APPLICATION_H

#include <thread>

#include "Utils/DebugTools.h"
#include "Utils/Managed.h"
#include "Utils/Mutexed.h"
#include "Object.h"
#include "World.h"
#include "GameWindow.h"
#include "Utils/ResourceManager.h"

class Application : public Object
{
    OBJNAME(Application)

private:
    Managed<GameWindow> m_window;
    Managed<World>      m_world;
    ResourceManager     m_resource_manager;

public:
    Application(GameWindow*&&, World*&&, const std::string& res_file_path = "Data/res.list");

    GameWindow&            window() { return *m_window; }
    const GameWindow&      window() const { return *m_window; }
    World&                 world() { return *m_world; }
    const World&           world() const { return *m_world; }
    ResourceManager&       resource_manager() { return m_resource_manager; }
    const ResourceManager& resource_manager() const { return m_resource_manager; }

    [[nodiscard]] int run();
};

#endif // APPLICATION_H

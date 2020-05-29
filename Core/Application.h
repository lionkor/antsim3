#ifndef APPLICATION_H
#define APPLICATION_H

#include "Utils/DebugTools.h"
#include "Utils/Managed.h"
#include "Object.h"
#include "World.h"
#include "GameWindow.h"

class Application : public Object
{
    OBJECT(Application)

private:
    Managed<GameWindow> m_window;
    Managed<World>      m_world;

public:
    Application(GameWindow*&&, World*&&);

    GameWindow&       window() { return *m_window; }
    const GameWindow& window() const { return *m_window; }
    World&            world() { return *m_world; }
    const World&      world() const { return *m_world; }

    [[nodiscard]] int run();
};

#endif // APPLICATION_H

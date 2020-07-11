// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

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
#include "Rendering/GuiElement.h"

class Application : public Object
{
    OBJNAME(Application)

private:
    Managed<GameWindow> m_window;
    Managed<World> m_world;
    ResourceManager m_resource_manager;
    std::vector<SharedPtr<GuiElement>> m_gui_elements;

public:
    Application(const std::string& title, sf::Vector2u size, const std::string& res_file_path = "Data/res.list");

    GameWindow& window() { return *m_window; }
    const GameWindow& window() const { return *m_window; }
    World& world() { return *m_world; }
    const World& world() const { return *m_world; }
    ResourceManager& resource_manager() { return m_resource_manager; }
    const ResourceManager& resource_manager() const { return m_resource_manager; }

    std::vector<SharedPtr<GuiElement>>& gui_elements() { return m_gui_elements; }

    template<typename... Args>
    [[nodiscard]] WeakPtr<GuiElement> add_gui_element(Args&&... args);

    [[nodiscard]] int run();

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

template<typename... Args>
[[nodiscard]] WeakPtr<GuiElement> Application::add_gui_element(Args&&... args) {
    auto elem = make_shared<GuiElement>(*this, std::forward<Args>(args)...);
    m_gui_elements.push_back(elem);
    return WeakPtr<GuiElement>(elem);
}

#endif // APPLICATION_H

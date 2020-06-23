// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Application.h"

Application::Application(const std::string& title, sf::Vector2u size, const std::string& res_file_path)
    : m_window(new GameWindow(*this, title, size))
    , m_world(new World(*this))
    , m_resource_manager(res_file_path) {
}

int Application::run() {
    report("starting application with \n\tWorld: {}\n\tWindow: ", *m_world, *m_window);

    try {
        while (m_window->isOpen()) {
            m_world->update(*m_window);
        }
    } catch (std::exception& e) {
        report_error("fatal exception: {}", e.what());
        throw;
    }
    return 0;
}

std::stringstream Application::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_S("window", *m_window);
    TS_PROP_S("world", *m_world);
    TS_END();
}

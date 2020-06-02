#define TRACE 0
#include "Core.h"
#include "Utils.h"
#include "ECS/Component.h"

#include <vector>

struct Cell {
    bool alive;
};

class GridComponent : public Component
{
    OBJECT(GridComponent)
private:
    std::vector<Cell>   m_cells;
    const vec<unsigned> m_size;
    bool                m_initialized { false };
    sf::VertexArray     m_varray;
    std::size_t         m_index;


public:
    GridComponent(std::size_t w, std::size_t h)
        : m_cells(w * h)
        , m_size(w, h)
        , m_varray(sf::PrimitiveType::Quads) {
        for (auto& cell : m_cells) {
            cell.alive = Random::chance(10);
        }
    }

    Cell& at(std::size_t x, std::size_t y) {
        if (x >= m_size.x || y >= m_size.y) {
            report_error("invalid size passed, exiting");
            ASSERT_NOT_REACHABLE();
        }
        return m_cells[x + y * m_size.x]; // x + y * width
    }

    bool in_bounds(std::size_t x, std::size_t y) {
        return x < m_size.x && y < m_size.y;
    }

    virtual void on_update() override {
        auto gc = *this;
        m_varray.clear();
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y; ++y) {
                int nb = 0;
                if (gc.at(x, y).alive)
                    nb -= 1;
                for (int i = -1; i < 2; ++i) {
                    for (int k = -1; k < 2; ++k) {
                        if (gc.in_bounds(x + i, y + k) && gc.at(x + i, y + k).alive) {
                            nb++;
                        }
                    }
                }
                if (nb > 3) {
                    at(x, y).alive = false;
                } else if (nb == 3 && !gc.at(x, y).alive) {
                    at(x, y).alive = true;
                } else if (nb <= 1) {
                    at(x, y).alive = false;
                }

                const auto color = at(x, y).alive ? sf::Color::White : sf::Color::Black;
                m_varray.append(sf::Vertex(sf::Vector2f(x * 10, y * 10), color));
                m_varray.append(sf::Vertex(sf::Vector2f(x * 10 + 10, y * 10), color));
                m_varray.append(sf::Vertex(sf::Vector2f(x * 10 + 10, y * 10 + 10), color));
                m_varray.append(sf::Vertex(sf::Vector2f(x * 10, y * 10 + 10), color));
            }
        }
    }

    virtual void on_draw(DrawSurface& surf) override {
        if (!m_initialized) {
            m_index       = surf.submit_custom_varray(m_varray);
            m_initialized = true;
        }
        surf.update_custom_varray(m_index, m_varray);
    }
};

int main(int, char**) {
    report("--- BEGIN ---");

    Application app(new GameWindow("cells 1.0", { 1280, 720 }), new World);

    GameWindow& window = app.window();
    static_cast<void>(window);
    World& world = app.world();

    world.set_update_interval(100);

    Entity& grid = world.add_entity(new Entity);
    grid.add_component(new GridComponent(320 * 3, 180 * 3));

    auto ret = app.run();
    report("---  END  ---");
    return ret;
}

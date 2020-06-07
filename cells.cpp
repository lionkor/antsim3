#define TRACE 0
#include "Engine.h"
#include "ECS/Component.h"

#include <vector>
#include <queue>

static constexpr size_t index_3to1(size_t x, size_t y, size_t z, size_t height, size_t depth) {
    return x * height * depth + y * depth + z;
}

struct Cell {
    bool alive;
};

class GridComponent : public Component
{
    OBJECT(GridComponent)
private:
    std::vector<Cell>   m_old_cells;
    std::vector<Cell>   m_cells;
    const vec<unsigned> m_size;
    SimpleDrawable      m_drawable;
    std::thread         m_thread;
    std::atomic_bool    m_do_update { false };
    std::atomic_bool    m_kill_thread { false };


public:
    GridComponent(const GridComponent& gc)
        : m_cells(gc.m_cells)
        , m_size(gc.m_size)
        , m_drawable(gc.m_drawable) {
    }

    virtual ~GridComponent() {
        m_kill_thread = true;
        m_thread.join();
    }

    GridComponent(std::size_t w, std::size_t h)
        : m_cells(w * h)
        , m_size(w, h)
        , m_drawable(SimpleDrawable::PrimitiveType::Quads, w * h * 4)
        , m_old_cells(w * h) {
        for (auto& cell : m_cells) {
            cell.alive = Random::chance(10);
        }

        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y; ++y) {
                const auto color = at(x, y).alive ? sf::Color::White : sf::Color::Black;

                m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * 10, y * 10), color);
                m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * 10 + 10, y * 10), color);
                m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * 10 + 10, y * 10 + 10), color);
                m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * 10, y * 10 + 10), color);
            }
        }
        auto update_fn = [&]() {
            while (!m_kill_thread) {
                if (!m_do_update) {
                    continue;
                }
                std::memcpy(m_old_cells.data(), m_cells.data(), sizeof(Cell) * m_cells.size());
                m_do_update = false;
                for (std::size_t x = 0; x < m_size.x; ++x) {
                    for (std::size_t y = 0; y < m_size.y; ++y) {
                        int  nb     = 0;
                        auto gccell = at_old(x, y);
                        if (gccell.alive)
                            nb -= 1;
                        for (int i = -1; i < 2; ++i) {
                            for (int k = -1; k < 2; ++k) {
                                if (in_bounds(x + i, y + k) && at_old(x + i, y + k).alive) {
                                    nb++;
                                }
                            }
                        }
                        Cell&      cell         = at(x, y);
                        const bool alive_before = cell.alive;
                        if (cell.alive && nb > 3) {
                            cell.alive = false;
                        } else if (nb == 3 && !gccell.alive) {
                            cell.alive = true;
                        } else if (cell.alive && nb <= 1) {
                            cell.alive = false;
                        }

                        if (cell.alive == alive_before)
                            continue;

                        const auto color = cell.alive ? sf::Color::White : sf::Color::Black;

                        m_drawable[index_3to1(x, y, 0, m_size.y, 4)].color = color;
                        m_drawable[index_3to1(x, y, 1, m_size.y, 4)].color = color;
                        m_drawable[index_3to1(x, y, 2, m_size.y, 4)].color = color;
                        m_drawable[index_3to1(x, y, 3, m_size.y, 4)].color = color;
                    }
                }
                m_drawable.set_changed();
            }
        };
        m_thread = std::thread(update_fn);
    }

    Cell& at(std::size_t x, std::size_t y) {
        if (x >= m_size.x || y >= m_size.y) {
            report_error("invalid size passed, exiting");
            ASSERT_NOT_REACHABLE();
        }
        return m_cells[x + y * m_size.x]; // x + y * width
    }
    
    Cell& at_old(std::size_t x, std::size_t y) {
        if (x >= m_size.x || y >= m_size.y) {
            report_error("invalid size passed, exiting");
            ASSERT_NOT_REACHABLE();
        }
        return m_old_cells[x + y * m_size.x]; // x + y * width
    }

    constexpr bool in_bounds(std::size_t x, std::size_t y) {
        return x < m_size.x && y < m_size.y;
    }

    virtual void on_update() override {
        /* auto gc = *this;
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y; ++y) {
                int  nb     = 0;
                auto gccell = gc.at(x, y);
                if (gccell.alive)
                    nb -= 1;
                for (int i = -1; i < 2; ++i) {
                    for (int k = -1; k < 2; ++k) {
                        if (gc.in_bounds(x + i, y + k) && gc.at(x + i, y + k).alive) {
                            nb++;
                        }
                    }
                }
                Cell&      cell         = at(x, y);
                const bool alive_before = cell.alive;
                if (cell.alive && nb > 3) {
                    cell.alive = false;
                } else if (nb == 3 && !gccell.alive) {
                    cell.alive = true;
                } else if (cell.alive && nb <= 1) {
                    cell.alive = false;
                }

                if (cell.alive == alive_before)
                    continue;

                const auto color = cell.alive ? sf::Color::White : sf::Color::Black;

                m_drawable[index_3to1(x, y, 0, m_size.y, 4)].color = color;
                m_drawable[index_3to1(x, y, 1, m_size.y, 4)].color = color;
                m_drawable[index_3to1(x, y, 2, m_size.y, 4)].color = color;
                m_drawable[index_3to1(x, y, 3, m_size.y, 4)].color = color;
            }
        }
        m_drawable.set_changed();*/
        m_do_update = true;
    }

    virtual void on_draw(DrawSurface& surf) override {
        m_drawable.draw(surf);
    }
};

int main(int, char**) {
    report("--- BEGIN ---");

    Application app(new GameWindow("cells 1.0", { 1280, 720 }), new World);

    GameWindow& window = app.window();
    static_cast<void>(window);
    World& world = app.world();

    world.set_update_interval(0);

    Entity& grid = world.add_entity(new Entity);
    grid.add_component(new GridComponent(320 * 10, 180 * 10));

    auto ret = app.run();
    report("---  END  ---");
    return ret;
}
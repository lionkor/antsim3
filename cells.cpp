#define TRACE 0
#include "Engine.h"
#include "ECS/Component.h"
#include "Utils/CsvLogger.h"

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
    OBJNAME(GridComponent)
private:
    std::vector<Cell>   m_cells;
    const vec<unsigned> m_size;
    SimpleDrawable      m_drawable;
    std::thread         m_thread;
    std::atomic_bool    m_do_update { false };
    std::atomic_bool    m_kill_thread { false };
    std::vector<Cell>   m_old_cells;


public:
    virtual ~GridComponent() {
        m_kill_thread = true;
        m_thread.join();
    }

    GridComponent(Entity& e, std::size_t w, std::size_t h)
        : Component(e)
        , m_cells(w * h)
        , m_size(w, h)
        , m_drawable(SimpleDrawable::PrimitiveType::Quads, w * h * 4)
        , m_old_cells(w * h) {
        for (auto& cell : m_cells) {
            cell.alive = Random::chance(15);
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
            CsvLogger logger("c.csv");
            while (!m_kill_thread) {
                if (!m_do_update) {
                    //continue;
                }
                std::memcpy(m_old_cells.data(), m_cells.data(), sizeof(Cell) * m_cells.size());
                m_do_update   = false;
                size_t alives = 0;
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
                        Cell& cell = at(x, y);
                        if (cell.alive)
                            ++alives;

                        //if (x <= 1 || y <= 1 || x >= m_size.x - 2 || y >= m_size.y - 2)
                        //   cell.alive = true;

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
                logger.log(alives);
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
        m_do_update = true;
    }

    virtual void on_draw(DrawSurface& surf) override {
        m_drawable.draw(surf);
    }
};

int main(int, char**) {
    report("--- BEGIN ---");

    Application app("cells 1.0", { 1280, 720 });

    GameWindow& window = app.window();
    static_cast<void>(window);
    World& world = app.world(); 
    world.set_update_interval(200);

    auto grid = world.add_entity();
    grid.lock()->add_component<GridComponent>(320 * 5, 180 * 5);

    auto ret = app.run();
    report("---  END  ---");
    return ret;
}

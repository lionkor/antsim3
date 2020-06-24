#include "Engine.h"

static constexpr size_t index_3to1(size_t x, size_t y, size_t z, size_t height, size_t depth) {
    return x * height * depth + y * depth + z;
}


struct Cell {
    enum class Type
    {
        Air,
        Dirt,
    };

    static inline const std::map<Cell::Type, SimpleDrawable::Color> s_color_map = {
        { Type::Air, SimpleDrawable::Color::Cyan },
        { Type::Dirt, SimpleDrawable::Color(0x9b, 0x76, 0x53) }
    };

    static inline const std::map<Cell::Type, double /* density */> s_density_map = {
        { Type::Air, 0.1 },
        { Type::Dirt, 2.0 }
    };

    static inline double density_for_type(Cell::Type type) {
        return s_density_map.at(type);
    }

    static inline const SimpleDrawable::Color& color_for_type(Cell::Type type) {
        return s_color_map.at(type);
    }

    Type   type;
    double density;

    Cell(Type type = Type::Air, double density = -1)
        : type(type)
        , density(density) {
        if (density < 0) {
            density = density_for_type(type);
        }
    }
};

struct CellPair {
    Cell old_cell;
    Cell new_cell;

    inline void swap() {
        old_cell = new_cell;
    }
};


class SandboxComponent : public Component
{
    OBJNAME(SandboxComponent)
private:
    vec<size_t>           m_size;
    SimpleDrawable        m_drawable;
    std::vector<CellPair> m_cells;

public:
    static constexpr size_t CELL_SIZE = 10.0f;

    SandboxComponent(Entity& e, size_t w, size_t h)
        : Component(e)
        , m_size(w, h)
        , m_drawable(SimpleDrawable::PrimitiveType::Quads, w * h * 4) {
        m_cells.resize(w * h);
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y / 2; ++y) {
                at(x, y).new_cell.type = Cell::Type::Air;

                auto color = Cell::color_for_type(at(x, y).new_cell.type);

                m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
            }
        }
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = m_size.y / 2; y < m_size.y; ++y) {
                at(x, y).new_cell.type = Cell::Type::Dirt;

                auto color = Cell::color_for_type(at(x, y).new_cell.type);

                m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
            }
        }
        m_drawable.set_changed();
    }

    CellPair& at(std::size_t x, std::size_t y) {
        if (x >= m_size.x || y >= m_size.y) {
            report_error("invalid size passed, exiting");
            ASSERT_NOT_REACHABLE();
        }
        return m_cells[x + y * m_size.x]; // x + y * width
    }

    virtual void on_update() override {
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y; ++y) {
                auto _at = at(x, y);
                // use old_cell for lookups, new_cell for changes
                Cell&       new_cell = _at.new_cell;
                const Cell& old_cell = _at.old_cell;
                
                

                auto color = Cell::color_for_type(new_cell.type);
                if (m_drawable[index_3to1(x, y, 0, m_size.y, 4)].color != color) {
                    m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE), color);
                    m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE), color);
                    m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                    m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                    m_drawable.set_changed();
                }
                at(x, y).swap();
            }
        }
    }

    virtual void on_draw(DrawSurface& surface) override {
        m_drawable.draw(surface);
    }
};

int main() {
    Application app("Sandbox", { 1280, 720 });
    auto&       window = app.window();
    auto&       world  = app.world();

    window.set_framerate_limit(60);

    auto sandbox = world.add_entity().lock();
    sandbox->add_component<SandboxComponent>(200, 150);

    auto      fps_counter = app.add_gui_element(vecu(10, 10), vecd(0.4, 0.4), "... FPS").lock();
    sf::Clock clock;
    fps_counter->on_update = [&]() {
        auto time = clock.restart().asSeconds();
        fps_counter->set_text(fmt::format("{} FPS", size_t(1.0 / time)));
    };

    return app.run();
}

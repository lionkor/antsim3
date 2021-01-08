#include "Engine.h"

static constexpr size_t index_3to1(size_t x, size_t y, size_t z, size_t height, size_t depth) {
    return x * height * depth + y * depth + z;
}


struct Cell {
    enum Type
    {
        Air,
        Dirt,
        Water,
    };

    static inline const std::map<Cell::Type, SimpleDrawable::Color> s_color_map = {
        { Type::Air, SimpleDrawable::Color::Cyan },
        { Type::Dirt, SimpleDrawable::Color(0x9b, 0x76, 0x53) },
        { Type::Water, SimpleDrawable::Color::Blue },
    };

    static inline const std::map<Cell::Type, double /* density */> s_density_map = {
        { Type::Air, 0.1 },
        { Type::Water, 1.0 },
        { Type::Dirt, 2.0 },
    };

    static inline double density_for_type(Cell::Type type) {
        return s_density_map.at(type);
    }

    static inline const SimpleDrawable::Color& color_for_type(Cell::Type type) {
        return s_color_map.at(type);
    }

    Type type;
    double density;

    Cell(Type _type = Type::Air, double _density = -1)
        : type(_type)
        , density(_density) {
        if (density < 0) {
            density = density_for_type(type);
        }
    }
};


class SandboxComponent : public Component
{
    OBJNAME(SandboxComponent)
private:
    vec<size_t> m_size;
    SimpleDrawable m_drawable;
    std::vector<Cell> m_old_cells;
    std::vector<Cell> m_new_cells;
    std::vector<bool> m_updated_cells;

public:
    static constexpr size_t CELL_SIZE = 10.0f;

    SandboxComponent(Entity& e, size_t w, size_t h)
        : Component(e)
        , m_size(w, h)
        , m_drawable(SimpleDrawable::PrimitiveType::Quads, w * h * 4) {
        m_new_cells.resize(w * h);
        m_old_cells.resize(w * h);
        m_updated_cells.resize(w * h);
        /*
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y / 2; ++y) {
                at_new(x, y) = Cell(Cell::Type::Dirt);

                auto color = Cell::color_for_type(at_new(x, y).type);

                m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
            }
        }
        */
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y; ++y) {
                at_new(x, y) = Cell(Cell::Type(Random::random<size_t>(0, 1)));

                auto color = Cell::color_for_type(at_new(x, y).type);

                m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
            }
        }
        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y / 2; ++y) {
                at_new(x, y) = Cell(Cell::Type::Air);

                auto color = Cell::color_for_type(at_new(x, y).type);

                m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
            }
        }
        m_drawable.set_changed();

        report("first: {}\nlast: {}", at_new(0, 0).density, at_new(w - 1, h - 1).density);

        on_mouse_down = [&](GameWindow& window, const HID::MouseAction& action) -> void {
            auto pos = action.world_position(window);
            // needed for the calculation we're doing :/
            ASSERT(parent().transform().position() == vecd(0, 0));
            auto array_pos = pos / 10;
            if (in_bounds(array_pos.x, array_pos.y)) {
                if (action.button == HID::Left) {
                    draw_circle(array_pos.x, array_pos.y, 10, Cell(Cell::Type::Dirt));
                } else if (action.button == HID::Middle) {
                    draw_circle(array_pos.x, array_pos.y, 10, Cell(Cell::Type::Water));
                } else {
                    draw_circle(array_pos.x, array_pos.y, 10, Cell(Cell::Type::Air));
                }
            }
        };
    }

    void draw_circle(size_t x, size_t y, size_t _radius, Cell&& cell_template) {
        int radius = int(_radius);
        for (int i = -radius; i < radius; ++i) {
            for (int k = -radius; k < radius; ++k) {
                if (in_bounds(x + i, y + k)) {
                    at_new(x + i, y + k) = cell_template;
                }
            }
        }
    }

    Cell& at_new(std::size_t x, std::size_t y) {
        if (x >= m_size.x || y >= m_size.y) {
            report_error("invalid size passed, exiting");
            ASSERT_NOT_REACHABLE();
        }
        return m_new_cells[x + y * m_size.x]; // x + y * width
    }

    const Cell& at_old(std::size_t x, std::size_t y) const {
        if (x >= m_size.x || y >= m_size.y) {
            report_error("invalid size passed, exiting");
            ASSERT_NOT_REACHABLE();
        }
        return m_old_cells[x + y * m_size.x]; // x + y * width
    }

    void mark_updated(size_t x, size_t y) {
        m_updated_cells[x + y * m_size.x] = true;
    }

    bool was_updated(size_t x, size_t y) const {
        return m_updated_cells[x + y * m_size.x];
    }

    void clear_updated() {
        std::fill(m_updated_cells.begin(), m_updated_cells.end(), false);
    }

    virtual void on_update(float) override {
        m_old_cells = m_new_cells;

        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y; ++y) {
                // use old_cell for lookups, new_cell for changes
                Cell& new_cell = at_new(x, y);
                const Cell& old_cell = at_old(x, y);

                if (was_updated(x, y))
                    continue;

                if (in_bounds(x, y - 1)) {
                    auto density_to_match = old_cell.density;
                    auto prev = vecu(x, y);
                    auto pos = vecu(x, y - 1);
                    while (in_bounds(pos.x, pos.y) && at_old(pos.x, pos.y).density > density_to_match) {
                        std::swap(at_new(pos.x, pos.y), at_new(prev.x, prev.y));
                        mark_updated(pos.x, pos.y);
                        mark_updated(prev.x, prev.y);
                        prev = pos;
                        --pos.y;
                    }
                }

                if (was_updated(x, y))
                    continue;

                if (old_cell.type == Cell::Type::Water) {
                    bool go_left { false };
                    bool go_right { false };
                    if (in_bounds(x - 1, y)
                        && !was_updated(x - 1, y)
                        && !was_updated(x, y)
                        && !was_updated(x, y + 1)
                        && at_old(x - 1, y).type != Cell::Type::Water
                        && at_old(x, y + 1).density >= old_cell.density
                        && at_old(x, y - 1).density < old_cell.density
                        && at_old(x - 1, y).density < old_cell.density) {
                        go_left = true;
                    }
                    if (in_bounds(x + 1, y)
                        && !was_updated(x + 1, y)
                        && !was_updated(x, y)
                        && !was_updated(x, y + 1)
                        && at_old(x + 1, y).type != Cell::Type::Water
                        && at_old(x, y + 1).density >= old_cell.density
                        && at_old(x, y - 1).density < old_cell.density
                        && at_old(x + 1, y).density < old_cell.density) {
                        go_right = true;
                    }
                    if (go_left) {
                        std::swap(at_new(x - 1, y), new_cell);
                        mark_updated(x - 1, y);
                        mark_updated(x, y);
                    } else if (go_right) {
                        std::swap(at_new(x + 1, y), new_cell);
                        mark_updated(x + 1, y);
                        mark_updated(x, y);
                    }
                }
            }
        }
        clear_updated();

        for (std::size_t x = 0; x < m_size.x; ++x) {
            for (std::size_t y = 0; y < m_size.y; ++y) {
                // use old_cell for lookups, new_cell for changes
                Cell& new_cell = at_new(x, y);

                auto color = Cell::color_for_type(new_cell.type);
                if (m_drawable[index_3to1(x, y, 0, m_size.y, 4)].color != color) {
                    m_drawable[index_3to1(x, y, 0, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE), color);
                    m_drawable[index_3to1(x, y, 1, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE), color);
                    m_drawable[index_3to1(x, y, 2, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                    m_drawable[index_3to1(x, y, 3, m_size.y, 4)] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE), color);
                }
            }
        }
        m_drawable.set_changed();
    }

    bool in_bounds(size_t x, size_t y) {
        return x < m_size.x && y < m_size.y;
    }

    virtual void on_draw(DrawSurface& surface) override {
        m_drawable.draw(surface);
    }
};

int main() {
    Application app("Sandbox", { 1280, 720 });
    auto& window = app.window();
    auto& world = app.world();

    window.set_framerate_limit(60);

    world.set_update_interval(30);

    auto sandbox = world.add_entity().lock();
    sandbox->add_component<SandboxComponent>(200, 150);

    auto fps_counter = app.add_gui_element(vecu(10, 10), vecd(0.4, 0.4), "... FPS").lock();
    sf::Clock clock;
    fps_counter->on_update = [&](float dt) {
        fps_counter->set_text(fmt::format("{} FPS", size_t(1.0 / dt)));
    };

    return app.run();
}

#include "World.h"
#include "Random.h"
#include "Cell.h"

void World::resize(vec<size_t> new_size) {
    m_size = new_size;
    m_cells.resize(new_size.x * new_size.y);
    // initialize m_cells by constructing each cell seperately
    for (size_t x = 0; x < new_size.x; ++x)
        for (size_t y = 0; y < new_size.y; ++y) {
            Cell& cell = cell_at(x, y).value();
            cell       = std::move(Cell({ x, y }, *this));
        }
}

void World::tick() {
    std::for_each(m_cells.begin(), m_cells.end(), [](auto& c) { c.tick(); });
}

World::World(size_t width, size_t height)
    : m_cells()
    , m_size(width, height) {
    resize({ width, height });
}

bool World::set_cell_at(const vec<size_t>& pos, Cell cell) {
    if (pos.x >= m_size.x || pos.y >= m_size.y)
        return false;
    m_cells.at(pos.x + m_size.x * pos.y) = std::move(cell);
    return true;
}

boost::optional<boost::reference_wrapper<Cell> > World::cell_at(size_t x, size_t y) {
    if (x >= m_size.x || y >= m_size.y)
        return boost::none;
    return boost::ref(m_cells.at(x + m_size.x * y));
}

boost::optional<Cell> World::cell_at(size_t x, size_t y) const {
    if (x >= m_size.x || y >= m_size.y)
        return boost::none;
    return m_cells.at(x + m_size.x * y);
}

boost::optional<boost::reference_wrapper<Cell> > World::find_empty_neighbor(const vec<size_t>& pos) {
    return find_empty_neighbor(pos.x, pos.y);
}

boost::optional<boost::reference_wrapper<Cell> > World::find_empty_neighbor(size_t x, size_t y) {
    boost::container::vector<Cell*> found_cells;
    if (x >= m_size.x || y >= m_size.y)
        return boost::none;
    for (int i = -1; i < 2; ++i) {
        for (int k = -1; k < 2; ++k) {
            if (i == 0 && k == 0)
                continue;
            auto result = cell_at(size_t(int(x) + i), size_t(int(y) + k));
            if (result.has_value()) {
                Cell& cell = result.value();
                found_cells.push_back(&cell);
            }
        }
    }
    return boost::ref(*Random::random_from(found_cells));
}

std::stringstream World::to_stream() const {
    auto ss = Object::to_stream();
    ss << "size=(" << m_size.x << "," << m_size.y << ");";
    return ss;
}

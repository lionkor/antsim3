#ifndef WORLD_H
#define WORLD_H

#include "DebugTools.h"
#include "Object.h"

#include <boost/container/vector.hpp>

#include "vec.h"
#include "Cell.h"

#include <boost/ref.hpp>
#include <boost/optional.hpp>


class World
    : public Object
{
    OBJECT(World)

private:
    boost::container::vector<Cell> m_cells;
    vec<size_t>                    m_size;
    
public:
    World(size_t width, size_t height);
    
    void resize(vec<size_t> new_size);

    // advances the simulation one step
    void tick();

    bool set_cell_at(const vec<size_t>& pos, Cell cell);

    boost::optional<boost::reference_wrapper<Cell>> cell_at(size_t x, size_t y);
    boost::optional<Cell>                           cell_at(size_t x, size_t y) const;

    boost::optional<boost::reference_wrapper<Cell>> find_empty_neighbor(const vec<size_t>& pos);
    boost::optional<boost::reference_wrapper<Cell>> find_empty_neighbor(size_t x, size_t y);
    
    inline vec<size_t> size() const { return m_size; }

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // WORLD_H

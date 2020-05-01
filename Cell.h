#ifndef CELL_H
#define CELL_H

#include "Object.h"
#include "vec.h"

class World;

class Cell
    : public CopyableObject
{
    OBJECT(Cell)

private:
    bool        m_is_alive { true };
    bool        m_is_default_constructed { false };
    bool        _padding[6];
    World*      m_world { nullptr };
    vec<size_t> m_pos { 0, 0 };

    size_t m_multiply_tick_counter { 0 };
    float  m_energy_stored { 10.0f };

    // evolutionarily controlled
    float  m_max_energy { 10.0f };
    size_t m_max_multiply_ticks { 20 };
    float  m_move_cost { 0.1f };

public:
    float mutation_amount = 1.0f;

    Cell();
    Cell(vec<size_t> pos, World& world);
    Cell(const Cell&) = default;

    void tick();
    void try_move();
    void try_eat();
    void try_multiply();
    void kill();
    bool alive() const;

    Cell new_cell_from_this(const vec<size_t>& pos) const;

    // Object interface
public:
    virtual bool operator==(const Object& other) const override;
    virtual bool operator!=(const Object& other) const override;

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};


#endif // CELL_H

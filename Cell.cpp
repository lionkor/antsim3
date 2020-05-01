#include "Cell.h"
#include "World.h"
#include "Random.h"
#include <algorithm>

Cell::Cell()
    : m_is_default_constructed(true) {
}

Cell::Cell(vec<size_t> pos, World& world)
    : m_world(&world)
    , m_pos(std::move(pos)) {}

void Cell::tick() {
    if (!m_is_alive)
        return;
    if (m_is_default_constructed) {
        report_warning(*this << " is default constructed (probably uninitialized in container)");
    }

    report_trace(*this);

    if (m_energy_stored < 0.1f) {
        report_warning("KILL!");
        kill();
    }

    try_multiply();
    try_eat();
    try_move();
    ++m_multiply_tick_counter;
}

void Cell::try_move() {
    if (Random::chance(50)) {
        if (m_energy_stored - m_move_cost > 0)
            m_energy_stored -= m_move_cost;
        else
            m_energy_stored = 0;
    }
}

void Cell::try_eat() {
    if (Random::chance(50) && m_energy_stored + 0.5f < m_max_energy) {
        m_energy_stored += 0.1f;
    }
}

void Cell::try_multiply() {
    ASSERT(m_world);

    if (m_multiply_tick_counter < m_max_multiply_ticks)
        return;
    else
        m_multiply_tick_counter = 0;

    auto maybe_empty_cell = m_world->find_empty_neighbor(m_pos);
    if (!maybe_empty_cell.has_value())
        return;

    report_warning("MULTIPLY!");

    Cell& empty_cell = maybe_empty_cell.value();
    // fill new found empty cell
    m_world->set_cell_at(empty_cell.m_pos, new_cell_from_this(empty_cell.m_pos));
    // fill current cell with new cell
    m_world->set_cell_at(m_pos, new_cell_from_this(empty_cell.m_pos));
}

void Cell::kill() {
    m_is_alive = false;
}

bool Cell::alive() const {
    return m_is_alive;
}

Cell Cell::new_cell_from_this(const vec<size_t>& pos) const {
    Cell new_cell(pos, *m_world);


    float min                     = 1.0f - mutation_amount;
    float max                     = 1.0f + mutation_amount;
    new_cell.m_max_energy         = m_max_energy * Random::random_real<float>(min, max);
    new_cell.m_max_multiply_ticks = size_t(float(m_max_multiply_ticks) * Random::random_real<float>(min, max));
    new_cell.m_move_cost          = m_move_cost * Random::random_real<float>(min, max);

    new_cell.m_energy_stored         = m_energy_stored / 2.0f;
    new_cell.m_multiply_tick_counter = 0;
    new_cell.m_is_alive              = true;

    new_cell.mutation_amount = std::fabs(mutation_amount * Random::random_real<float>(min, max));
    new_cell.mutation_amount = std::clamp(new_cell.mutation_amount, 0.0f, 2.0f);

    return new_cell;
    report_trace(uuid());
    report_trace("  m_max_energy         from " << m_max_energy << " to " << new_cell.m_max_energy);
    report_trace("  m_max_multiply_ticks from " << m_max_multiply_ticks << " to " << new_cell.m_max_multiply_ticks);
    report_trace("  m_move_cost          from " << m_move_cost << " to " << new_cell.m_move_cost);
}

bool Cell::operator==(const Object& other) const {
}

bool Cell::operator!=(const Object& other) const {
}

std::stringstream Cell::to_stream() const {
    auto ss = CopyableObject::to_stream();
    ss << "pos=(" << m_pos.x << "," << m_pos.y << ");";
    ss << "is_alive=" << bool_str(m_is_alive) << ";";
    ss << "world=" << *m_world << ";";
    ss << "multiply_tick_counter=" << m_multiply_tick_counter << ";";
    ss << "energy_stored=" << m_energy_stored << ";";
    ss << "max_energy=" << m_max_energy << ";";
    ss << "max_multiply_ticks=" << m_max_multiply_ticks << ";";
    ss << "move_cost=" << m_move_cost << ";";
    ss << "mutation_amount=" << mutation_amount << ";";
    return ss;
}

/**
 * A minimal simulator for orbital phyics
 */

#include "Engine.h"
/*
class PlanetComponent
    : public Component
{
    OBJNAME(PlanetComponent)
private:
    static inline std::vector<PlanetComponent*> s_planets {};

    double         m_mass;
    vec<double>    m_vel { 0, 0 };
    SimpleDrawable m_drawable;

public:
    PlanetComponent(double mass)
        : m_mass(mass) {
        s_planets.push_back(this);
        m_drawable = SimpleDrawable(SimpleDrawable::PrimitiveType::Quads, 4);
        report("{}", m_vel);
    }

    vec<double>& velocity() { return m_vel; }

    virtual void on_update() override {
        //m_vel                     = { 0, 0 };
        const vec<double>& my_pos = parent()->transform().position();
        //report("{}", my_pos);
        double mass_2 = m_mass;
        for (auto& planet_ptr : s_planets) {
            if (planet_ptr == this)
                continue;
            double             mass_1    = planet_ptr->m_mass;
            const vec<double>& other_pos = planet_ptr->parent()->transform().position();
            double             r_squared = vec<double>::distance_squared(my_pos, other_pos);
            //std::clamp(r_squared, 1.5 * std::sqrt(planet_ptr->m_mass), std::numeric_limits<double>::max());
            vec<double> r_hat = other_pos - my_pos;
            r_hat.normalize();
            m_vel += ((mass_1 * mass_2) / r_squared) * r_hat;
        }
        report("{}", m_vel);
        //parent()->transform().move_by(m_vel / m_mass);
        parent()->transform().move_by(m_vel / m_mass);
        auto sqrt_mass = std::sqrt(m_mass);
        auto offset    = sqrt_mass;
        auto color     = sf::Color(255, 255, 255, std::lerp(128, 220, m_mass / s_sun_mass));
        m_drawable[0]  = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(my_pos.x - offset, my_pos.y - offset), color);
        m_drawable[1]  = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(my_pos.x + offset, my_pos.y - offset), color);
        m_drawable[2]  = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(my_pos.x + offset, my_pos.y + offset), color);
        m_drawable[3]  = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(my_pos.x - offset, my_pos.y + offset), color);
        m_drawable.set_changed();
    }

    virtual void on_draw(DrawSurface& surface) override {
        m_drawable.draw(surface);
    }
};
*/

static inline std::atomic_size_t s_id_counter { 0 };

static constexpr double G = 10;

struct Body {
    Body(const vec<double>& _pos = { 0, 0 }, const vec<double>& _vel = { 0, 0 }, double _mass = 0)
        : pos(_pos), vel(_vel), mass(_mass), id(++s_id_counter) { }

    vec<double> pos { 0, 0 };
    vec<double> vel { 0, 0 };
    double      mass { 0 };
    size_t      id;
    bool        dead { false };
};

class NBodySystemComponent : public Component
{
    OBJNAME(NBodySystemComponent)
private:
    std::vector<Body> m_bodies;
    std::vector<Body> m_bodies_old;
    SimpleDrawable    m_drawable;

public:
    static vec<double> random_vector(double min, double max) {
        return vec<double>(Random::random_real<double>(min, max), Random::random_real<double>(min, max));
    }

    NBodySystemComponent(size_t count) {
        ASSERT(count > 0);
        m_bodies.resize(count);
        for (auto& body : m_bodies) {
            body = Body { random_vector(-20000, 20000), random_vector(-600, 600), Random::random_real<double>(20, 300) };
        }
        m_drawable.set_primitive(SimpleDrawable::PrimitiveType::Quads);
        m_drawable.resize(count * 4);
        m_bodies_old.reserve(count);
    }

    virtual void on_update() override {
        // copy all bodies
        m_drawable.resize(m_bodies.size() * 4);
        m_bodies_old = m_bodies;
        report("{} {}", m_bodies.size(), m_drawable.size());
        for (size_t i = 0; i < m_bodies_old.size(); ++i) {
            const Body& body_1_old = m_bodies_old[i];
            Body&       body_1     = m_bodies[i];
            if (body_1.dead)
                continue;
            for (size_t k = 0; k < m_bodies_old.size(); ++k) {
                const auto& body_2_old = m_bodies_old[k];
                auto&       body_2     = m_bodies[k];
                if (body_1_old.id == body_2.id)
                    continue;
                if (body_2.dead)
                    continue;
                auto r_squared = vec<double>::distance_squared(body_1.pos, body_2_old.pos);
                if (r_squared <= body_2_old.mass * 100 + body_1.mass * 100) {
                    // collision!
                    report("collision!");
                    body_2.dead = true;
                    if (body_1.mass < body_2_old.mass) {
                        body_1.pos = body_2_old.pos;
                    }
                    body_1.mass += body_2_old.mass;
                    body_1.vel += body_2_old.vel / body_2_old.mass;
                } else {
                    body_1.vel += ((G * body_1.mass * body_2_old.mass) / r_squared) * (body_2_old.pos - body_1_old.pos).normalized();
                }
            }
            body_1.pos += body_1.vel / body_1.mass;
            auto offset           = std::sqrt(body_1.mass * 100);
            m_drawable[i * 4 + 0] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(body_1.pos.x - offset, body_1.pos.y - offset));
            m_drawable[i * 4 + 1] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(body_1.pos.x + offset, body_1.pos.y - offset));
            m_drawable[i * 4 + 2] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(body_1.pos.x + offset, body_1.pos.y + offset));
            m_drawable[i * 4 + 3] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(body_1.pos.x - offset, body_1.pos.y + offset));
            m_drawable.set_changed();
        }

        std::erase_if(m_bodies, [&](Body& elem) -> bool { return elem.dead; });
    }

    virtual void on_draw(DrawSurface& surface) override {
        m_drawable.draw(surface);
    }
};

static void init(Application& app) {
    GameWindow& window = app.window();
    World&      world  = app.world();

    window.set_framerate_limit(400);

    auto solar_system = world.add_entity(new Entity).lock();
    solar_system->add_component(new NBodySystemComponent(400));
}

int main() {
    Application app(new GameWindow("orbits 1.0", { 1280, 720 }), new World);
    init(app);
    return app.run();
}

/**
 * A minimal simulator for orbital phyics
 */

#include "Engine.h"

static inline double s_sun_mass = 50000.0;

class PlanetComponent
    : public Component
{
    OBJECT(PlanetComponent)
private:
    static inline std::vector<PlanetComponent*> s_planets {};

    double         m_mass;
    vec<double>    m_vel;
    SimpleDrawable m_drawable;

public:
    PlanetComponent(double mass)
        : m_mass(mass) {
        s_planets.push_back(this);
        m_drawable = SimpleDrawable(SimpleDrawable::PrimitiveType::Quads, 4);
        m_vel      = { Random::random_real(-500.0, 500.0), Random::random_real(-500.0, 500.0) };
    }

    virtual void on_update() override {
        //m_vel                     = { 0, 0 };
        const vec<double>& my_pos = parent()->transform().position();
        //report("{}", my_pos);
        double mass_2 = m_mass;
        for (auto& planet_ptr : s_planets) {
            if (planet_ptr == this)
                continue;
            const vec<double>& other_pos = planet_ptr->parent()->transform().position();
            double             r_squared = vec<double>::distance_squared(my_pos, other_pos);
            std::clamp(r_squared, 1.5 * std::sqrt(planet_ptr->m_mass), std::numeric_limits<double>::max());
            double      mass_1 = planet_ptr->m_mass;
            vec<double> r_hat  = other_pos - my_pos;
            r_hat.normalize();
            m_vel += ((mass_1 * mass_2) / r_squared) * r_hat;
        }
        parent()->transform().move_by(m_vel / m_mass);
        auto sqrt_mass = std::sqrt(m_mass);
        auto offset    = 1.5 * sqrt_mass;
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

static void init(Application& app) {
    GameWindow& window = app.window();
    World&      world  = app.world();

    window.set_framerate_limit(60);

    auto& sun = world.add_entity(new Entity({ window.width<double>() / 2, window.height<double>() / 2 }));
    sun.add_component(new PlanetComponent(s_sun_mass));

    for (int i = 0; i < 200; ++i) {
        world.add_entity(new Entity({ window.width<double>() / 2 + Random::random(-6000, 6000), window.height<double>() / 2 + Random::random(-6000, 6000) }))
            .add_component(new PlanetComponent(Random::random(5, 1000)));
    }
}

int main() {
    Application app(new GameWindow("orbits 1.0", { 1280, 720 }), new World);
    init(app);
    return app.run();
}

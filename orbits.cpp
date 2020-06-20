/**
 * A minimal simulator for orbital phyics
 */

#include "Engine.h"

static inline std::atomic_size_t s_id_counter { 0 };

static constexpr double G = 15;

static constexpr size_t s_pieces = 50;
static constexpr double s_size_limit = 5;

struct Body {
    Body(const vec<double>& _pos = { 0, 0 }, const vec<double>& _vel = { 0, 0 }, double _mass = 0, sf::Color _color = sf::Color::White)
        : pos(_pos), vel(_vel), mass(_mass), id(++s_id_counter) {
        if (_color == sf::Color::White) {
            color = sf::Color(Random::random(64, 255), Random::random(64, 255), Random::random(64, 255));
        } else {
            color = _color;
        }
    }

    vec<double> pos { 0, 0 };
    vec<double> vel { 0, 0 };
    double      mass { 0 };
    size_t      id;
    bool        dead { false };
    sf::Color   color;
};

class NBodySystemComponent : public Component
{
    OBJNAME(NBodySystemComponent)
private:
    std::vector<Body>    m_bodies;
    std::vector<Body>    m_bodies_old;
    SimpleDrawable       m_drawable;
    Managed<sf::Texture> m_planet_texture;

public:
    static vec<double> random_vector(double min, double max) {
        return vec<double>(Random::random_real<double>(min, max), Random::random_real<double>(min, max));
    }

    NBodySystemComponent(size_t count) {
        ASSERT(count > 0);
        m_bodies.resize(count);
        for (auto& body : m_bodies) {
            body = Body { random_vector(-120000, 120000), random_vector(-40, 40), 2000 };
        }
        m_drawable.set_primitive(SimpleDrawable::PrimitiveType::Quads);
        m_drawable.resize(count * 4);
        m_bodies_old.reserve(count);
    }

    void load_texture() {
        auto& application = parent()->world().application();
        auto& file        = application.resource_manager().get_resource_by_name("planet.png").value().get();
        auto  result      = file.get();
        ASSERT(result.ok());
        auto& vector     = result.value().get();
        m_planet_texture = Managed<sf::Texture>(new sf::Texture);
        m_planet_texture->loadFromMemory(vector.data(), vector.size());
        m_drawable.set_texture(m_planet_texture.get());
    }

    virtual void on_update() override {
        // FIXME: dont call this every time :/
        m_drawable.resize(m_bodies.size() * 4);
        m_bodies_old = m_bodies;
        for (size_t i = 0; i < m_bodies_old.size(); ++i) {
            const Body& body_1_old  = m_bodies_old[i];
            Body&       body_1      = m_bodies[i];
            if (body_1.dead)
                continue;
            auto s1 = std::sqrt(body_1.mass * 100.0);
            for (size_t k = 0; k < m_bodies_old.size(); ++k) {
                const auto& body_2_old = m_bodies_old[k];
                auto&       body_2     = m_bodies[k];
                if (body_1_old.id == body_2.id)
                    continue;
                if (body_2.dead)
                    continue;
                auto r_squared = vec<double>::distance_squared(body_1.pos, body_2_old.pos);
                if (false && (body_1.mass * body_2_old.mass) / r_squared < body_1.vel.length() / 100.0)
                    continue;
                auto s2 = std::sqrt(body_2_old.mass * 100.0);
                if (std::sqrt(r_squared) <= s1 + s2) {
                    if (body_1.mass >= body_2_old.mass) {
                        // collision!
                        //report("collision!");
                        
                        body_1.color.r = std::lerp(body_1.color.r, body_2_old.color.r, (body_2_old.mass / body_1.mass) / 2.0);
                        body_1.color.g = std::lerp(body_1.color.g, body_2_old.color.b, (body_2_old.mass / body_1.mass) / 2.0);
                        body_1.color.b = std::lerp(body_1.color.g, body_2_old.color.b, (body_2_old.mass / body_1.mass) / 2.0);
                        body_2.dead = true;
                        auto c      = body_2_old.mass / double(s_pieces) * 0.25;
                        if (c < s_size_limit) {
                            body_1.mass += body_2_old.mass;
                        } else {
                            body_1.mass += body_2_old.mass * 0.75;
                        }
                        body_1.vel += body_2_old.vel / body_1.mass;
                    } else {
                        // ignore
                    }
                } else {
                    body_1.vel += (((G * body_1.mass * body_2_old.mass) / r_squared) * (body_2_old.pos - body_1_old.pos).normalized()) / body_1.mass;
                }
            }
            body_1.pos += body_1.vel;
            auto offset                      = s1;
            m_drawable[i * 4 + 0].position.x = body_1.pos.x - offset;
            m_drawable[i * 4 + 0].position.y = body_1.pos.y - offset;
            m_drawable[i * 4 + 1].position.x = body_1.pos.x + offset;
            m_drawable[i * 4 + 1].position.y = body_1.pos.y - offset;
            m_drawable[i * 4 + 2].position.x = body_1.pos.x + offset;
            m_drawable[i * 4 + 2].position.y = body_1.pos.y + offset;
            m_drawable[i * 4 + 3].position.x = body_1.pos.x - offset;
            m_drawable[i * 4 + 3].position.y = body_1.pos.y + offset;
            m_drawable[i * 4 + 0].color      = body_1.color;
            m_drawable[i * 4 + 1].color      = body_1.color;
            m_drawable[i * 4 + 2].color      = body_1.color;
            m_drawable[i * 4 + 3].color      = body_1.color;
            auto tex_size                    = m_planet_texture->getSize();
            m_drawable[i * 4 + 0].texCoords  = SimpleDrawable::Vector2f(0, 0);
            m_drawable[i * 4 + 1].texCoords  = SimpleDrawable::Vector2f(tex_size.x, 0);
            m_drawable[i * 4 + 2].texCoords  = SimpleDrawable::Vector2f(tex_size.x, tex_size.y);
            m_drawable[i * 4 + 3].texCoords  = SimpleDrawable::Vector2f(0, tex_size.y);
            m_drawable.set_changed();
        }

        std::vector<Body> bodies_to_add;


        for (auto& old : m_bodies) {
            if (old.dead) {
                auto c = old.mass / double(s_pieces) * 0.25;
                auto size = std::sqrt(old.mass * 100);
                if (c < s_size_limit)
                    continue;
                auto mag = old.vel.length();
                for (size_t i = 0; i < s_pieces; ++i) {
                    auto new_pos = old.pos + random_vector(-size, size);
                    auto new_body = Body(new_pos, (new_pos - old.pos).normalized() * (mag * Random::random_real<double>(0.01, 0.3)), c, old.color);
                    new_body.pos += new_body.vel;
                    bodies_to_add.push_back(std::move(new_body));
                }
            }
        }

        std::erase_if(m_bodies, [&](Body& elem) -> bool { return elem.dead; });

        for (auto& new_body : bodies_to_add) {
            m_bodies.push_back(std::move(new_body));
        }
        bodies_to_add.clear();
        
        report("we have {} n-bodies!", m_bodies.size());
    }

    virtual void on_draw(DrawSurface& surface) override {
        m_drawable.draw(surface);
    }
};

static void init(Application& app) {
    GameWindow& window = app.window();
    World&      world  = app.world();

    window.set_framerate_limit(400);

    auto  solar_system = world.add_entity(new Entity).lock();
    auto& comp         = solar_system->add_component(new NBodySystemComponent(1100));
    comp.load_texture();
}

int main() {
    Application app(new GameWindow("orbits 1.0", { 1280, 720 }), new World);
    init(app);
    return app.run();
}

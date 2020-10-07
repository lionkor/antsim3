#include "Engine.h"

#include "bitops/bitops.h"
#include <map>
#include <execution>

class WorldComponent : public Component
{
    OBJNAME(WorldComponent)

public:
    struct Tile {
        enum Type
        {
            Rock,
            Air
        } type { Rock };
    };

    template<size_t Size>
    struct Chunk {
    private:
        Tile tiles[Size * Size];

    public:
        bool loaded { true };
        bool changed { true };
        veci pos;

        Chunk(veci _pos)
            : pos(_pos) {
            // FIXME: Do we need this??
            std::fill(std::begin(tiles), std::end(tiles), Tile());
        }

        Tile& at(uint _x, uint _y) {
            size_t index = _x + _y * Size;
            ASSERT(index < sizeof(tiles));
            return tiles[index];
        }

        Tile& at(const vec<uint>& xy) {
            size_t index = xy.x + xy.y * Size;
            ASSERT(index < sizeof(tiles));
            return tiles[index];
        }

        static constexpr size_t tile_count() {
            return sizeof(tiles);
        }

        static constexpr size_t size() {
            return Size;
        }
    };

    using chunk_type = Chunk<10>;

    static constexpr size_t TILE_SIZE = 1;

private:
    std::map<veci, chunk_type> m_chunks;
    bool m_redraw { false };
    SimpleDrawable m_drawable;

    Managed<sf::Texture> m_atlas;

public:
    WorldComponent(Entity& e)
        : Component(e) {
        add_chunk({ 0, 0 });

        m_atlas = resource_manager().load_texture("atlas.png");
        ASSERT(m_atlas != nullptr);
        m_atlas->generateMipmap();

        on_mouse_move = [&](GameWindow& _window, const HID::MouseAction& _ma) -> void {
            return;
            auto chunk_pos = chunk_pos_from_world_pos(_ma.world_position(_window));
            if (!m_chunks.contains(chunk_pos)) {
                add_chunk(chunk_pos);
            }
        };

        on_mouse_down = [&](GameWindow& _window, const HID::MouseAction& _ma) -> void {
            if (_ma.button != HID::MouseButton::Left) {
                return;
            }
            auto pos = _ma.world_position(_window);
            auto chunk_pos = chunk_pos_from_world_pos(pos);
            if (m_chunks.contains(chunk_pos)) {
                report_warning("tile_pos: {} chunk_pos: {} world_pos: {}", tile_pos_from_world_pos(pos), chunk_pos, pos);
                Tile& tile = tile_at(pos);
                tile.type = Tile::Type::Air;
                chunk_at(pos).changed = true;
            } else {
                report_error("chunk {} should exist but doesn't, desync?", chunk_pos);
            }
        };
    }

    chunk_type& chunk_at(const vec<float>& _pos) {
        return m_chunks.at(chunk_pos_from_world_pos(_pos));
    }

    Tile& tile_at(const vec<float>& _pos) {
        chunk_type& chunk = chunk_at(_pos);
        Tile& tile = chunk.at(tile_pos_from_world_pos(_pos));
        return tile;
    }

    static veci chunk_pos_from_world_pos(const vec<float>& _pos) {
        /*
        auto abspos = _pos.abs();
        vec<float> new_pos = (abspos - (veci(abspos) % chunk_size())) / chunk_size();
        if (_pos.x < 0 && _pos.y < 0) {
            return -new_pos - vec<float>(1);
        } else if (_pos.x < 0) {
            return veci(-new_pos.x - 1, new_pos.y);
        } else if (_pos.y < 0) {
            return veci(new_pos.x, -new_pos.y - 1);
        }
        return new_pos;
        */
        veci chunk;
        if (_pos.x < 0 || _pos.y < 0) {
            auto mod_pos = _pos;
            mod_pos.x -= mod_pos.x < 0 ? chunk_type::size() : 0;
            mod_pos.y -= mod_pos.y < 0 ? chunk_type::size() : 0;

            chunk.x = static_cast<int>(mod_pos.x / chunk_type::size() * 1.0f);
            chunk.y = static_cast<int>(mod_pos.y / chunk_type::size() * 1.0f);
        } else {
            chunk.x = static_cast<int>(_pos.x / chunk_type::size() * 1.0f);
            chunk.y = static_cast<int>(_pos.y / chunk_type::size() * 1.0f);
        }
        return chunk;
    }

    static vec<uint> tile_pos_from_world_pos(const vec<float>& _pos) {
        /*
         * 
        veci floored_pos = _pos;
        if (_pos.x < 0) {
            floored_pos.x -= TILE_SIZE;
        }
        if (_pos.y < 0) {
            floored_pos.y -= TILE_SIZE;
        }
        report("_pos = {}, floored_pos = {}", _pos, floored_pos);
        veci tile_coords = (floored_pos - (floored_pos % TILE_SIZE)) / TILE_SIZE;
        report("tile_coords = {}", tile_coords);

        veci chunk_coords = chunk_pos_from_world_pos(_pos);
        report("chunk_coords = {}", chunk_coords);
        veci chunk_coords_as_tile_coords = chunk_coords * chunk_type::size();
        report("chunk_coords_as_tile_coords = {}", chunk_coords_as_tile_coords);

        veci local_coords = tile_coords % chunk_type::size();

        if (tile_coords.x < 0) {
            local_coords.x += chunk_type::size();
        }
        if (tile_coords.y < 0) {
            local_coords.y += chunk_type::size();
        }

        report("local_coords = {}", local_coords);
        return local_coords;
        */
        veci tile = { 0, 0 };
        veci chunk_pos = chunk_pos_from_world_pos(_pos);
        if (_pos.x != 0) {
            tile.x = static_cast<int>(_pos.x - int(chunk_type::size()) * chunk_pos.x);
        }
        if (_pos.y != 0) {
            tile.y = static_cast<int>(_pos.y - int(chunk_type::size()) * chunk_pos.y);
        }
        report("chunk pos = {}", chunk_pos);
        report("tile xy = {}", tile);
        return tile;
    }

    void add_chunk(veci _pos) {
        report("adding chunk at {}", _pos);
        m_chunks.insert(std::pair(_pos, chunk_type(_pos)));
        m_redraw = true;
    }

    static size_t chunk_size() {
        return chunk_type::size() * TILE_SIZE;
    }

    virtual void on_update(float) override {
        auto iter = std::find_if(std::execution::par_unseq, m_chunks.begin(), m_chunks.end(), WorldComponent::chunk_has_changed);
        if (iter != m_chunks.end()) {
            m_redraw = true;
            m_drawable.set_changed();
            // rendering
            m_drawable.resize(chunk_type::tile_count() * m_chunks.size() * 4);
            m_drawable.set_primitive(SimpleDrawable::PrimitiveType::Quads);
            m_drawable.set_texture(m_atlas.get());
            report("resized to {}", m_drawable.size());
            size_t i = 0;
            for (auto& pair : m_chunks) {
                auto& chunk = pair.second;
                chunk.changed = false;
                for (size_t x = 0; x < chunk.size(); ++x) {
                    for (size_t y = 0; y < chunk.size(); ++y) {
                        vec<float> pos = chunk.pos * chunk_size() + vec<float>(x, y) * TILE_SIZE;
                        sf::Color color = sf::Color::Magenta;
                        sf::Vector2f tex_coords[4];
                        bool draw = true;
                        switch (chunk.at(x, y).type) {
                        case Tile::Type::Rock:
                            color = sf::Color::White;
                            tex_coords[0] = { 0, 0 };
                            tex_coords[1] = { 32, 0 };
                            tex_coords[2] = { 32, 32 };
                            tex_coords[3] = { 0, 32 };
                            break;
                        case Tile::Type::Air:
                            draw = true;
                            tex_coords[0] = { 32, 0 };
                            tex_coords[1] = { 64, 0 };
                            tex_coords[2] = { 64, 32 };
                            tex_coords[3] = { 32, 32 };
                            break;
                        default:
                            ASSERT_NOT_REACHABLE();
                        }
                        if (draw) {
                            m_drawable[i].position = ext::sf::to_sf_vec2f(pos);
                            m_drawable[i + 1].position = ext::sf::to_sf_vec2f(pos + vec<float>(TILE_SIZE, 0));
                            m_drawable[i + 2].position = ext::sf::to_sf_vec2f(pos + vec<float>(TILE_SIZE, TILE_SIZE));
                            m_drawable[i + 3].position = ext::sf::to_sf_vec2f(pos + vec<float>(0, TILE_SIZE));
                            m_drawable[i].color = color;
                            m_drawable[i + 1].color = color;
                            m_drawable[i + 2].color = color;
                            m_drawable[i + 3].color = color;
                            m_drawable[i].texCoords = tex_coords[0];
                            m_drawable[i + 1].texCoords = tex_coords[1];
                            m_drawable[i + 2].texCoords = tex_coords[2];
                            m_drawable[i + 3].texCoords = tex_coords[3];
                            i += 4;
                        }
                    }
                }
            }
        }
    }

    static bool chunk_has_changed(const std::pair<veci, chunk_type>& _pair) {
        return _pair.second.changed;
    }

    virtual void on_draw(DrawSurface& surface) override {
        if (m_redraw) {
            report_warning("redraw!");
            m_redraw = false;
        }
        m_drawable.draw(surface);
    }
};

int main(int argc, char** argv) {
    try {
        bool fullscreen = false;
        if (argc >= 2) {
            auto argstr = std::string(argv[1]);
            if (argstr == "-f" || argstr == "--fullscreen") {
                fullscreen = true;
            }
        }

        Application app("2dgame", { 960, 540 }, fullscreen, "Data/2dgame_res.list");
        World& world = app.world();
        [[maybe_unused]] GameWindow& window = app.window();

        auto gameworld_weak = world.add_entity({ 0, 0 });
        if (!gameworld_weak.expired()) {
            auto gameworld = gameworld_weak.lock();
            gameworld->add_component<WorldComponent>();
        }

        return app.run();
    }
#if DEBUG
    catch (void*) {
    }

#else
    catch (std::exception& _e) {
        report_error("uncaught exception: \n\t{}", _e.what());
        throw;
    }
#endif // DEBUG
}

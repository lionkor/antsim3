#include "Engine.h"
#include <vector>
#include <atomic>
#include <string>
#include <map>
#include <boost/asio.hpp>
#include "Physics/vec.h"
#include "Utils/DebugTools.h"

#include "mpgame-common.h"

static inline void report_errno() {
    report_error("an error occurred: {}", strerror(errno));
}

namespace boost_ip = boost::asio::ip;

static inline boost_ip::udp::endpoint resolve_endpoint(boost::asio::io_service& ios, const std::string& host, std::uint16_t port) {
    boost_ip::udp::resolver           resolver { ios };
    boost_ip::udp::resolver::query    query { boost_ip::udp::v4(), host, std::to_string(port) };
    boost_ip::udp::resolver::iterator iter = resolver.resolve(query);
    return *iter;
}

class UDPClient
{
private:
    boost::asio::io_service& m_io_service;
    boost_ip::udp::socket    m_socket;
    boost_ip::udp::endpoint  m_endpoint;

public:
    UDPClient(boost::asio::io_service& io_service, const std::string& host, std::uint16_t port)
        : m_io_service(io_service), m_socket(m_io_service, boost_ip::udp::endpoint(boost_ip::udp::v4(), 0)) {
        m_endpoint = resolve_endpoint(m_io_service, host, port);
    }

    ~UDPClient() {
        m_socket.close();
    }

    template<size_t size>
    void send(const std::array<std::uint8_t, size>& msg) {
        m_socket.send_to(boost::asio::buffer(msg, size), m_endpoint);
    }

    [[nodiscard]] std::vector<std::string> receive() {
        std::vector<std::string> data;
        while (m_socket.available() > 0) {
            std::string str;
            str.resize(PACKET_SIZE, ' ');
            m_socket.receive_from(boost::asio::buffer(str.data(), PACKET_SIZE), m_endpoint);
            report("read {} bytes: _{}_", str.size(), str);
            data.push_back(str);
        }

        return data;
    }
};

class Client
{
private:
    boost::asio::io_service m_io_service;
    UDPClient               m_backend;
    bool                    m_is_running { true };
    std::string             m_name;
    vec<float>              m_position;

public:
    void send_packet(const UpdatePacket& packet) {
        auto array = serialize_into_array<UpdatePacket, PACKET_SIZE>(packet);
        m_backend.send(array);
        report("sent _{}_", std::string(array.begin(), array.end()));
    }

    Client(const std::string& address, std::uint16_t port, const std::string& name)
        : m_io_service(), m_backend(m_io_service, address, port), m_name(name) {
    }

    int run() {
        while (m_is_running) {
            m_position.x += 1.5;
            m_position.y += 1.5;
            UpdatePacket packet(m_name, m_position.x, m_position.y);
            send_packet(packet);
            auto vec = m_backend.receive();
            report("received {} packets", vec.size());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        return 0;
    }

    const std::string& name() const { return m_name; }
};

struct Player {
    std::string name;
    vecd        position;

    bool operator==(const Player& player) {
        return name == player.name;
    }
};

class ClientComponent : public Component
{
    OBJNAME(ClientComponent)
private:
    boost::asio::io_service m_io_service;
    UDPClient               m_backend;
    std::string             m_name;
    double                  m_speed { 0.7 };
    bool                    m_update_to_server { true };
    vecd                    m_player_position;
    std::vector<Player>     m_other_players;
    SimpleDrawable          m_simple_drawable;
    sf::Font                m_font;
    sf::Text                m_text;
    bool                    m_dir_pressed[4] { false, false, false, false };

public:
    // TODO make the player a Player ?!
    ClientComponent(Entity& e, const std::string& address, std::uint16_t port, const std::string& name)
        : Component(e)
        , m_io_service()
        , m_backend(m_io_service, address, port)
        , m_name(name)
        , m_simple_drawable(SimpleDrawable::PrimitiveType::Points, 1) {
        m_player_position = parent().transform().position();

        on_key_down = [&](GameWindow&, const HID::Key& key) {
            key_pressed(key);
        };

        on_key_up = [&](GameWindow&, const HID::Key& key) {
            key_released(key);
        };

        auto& resman    = parent().world().application().resource_manager();
        auto  res_maybe = resman.get_resource_by_name("mono.ttf");
        if (res_maybe.ok()) {
            auto* data = res_maybe.value().get().load();
            if (data) {
                bool ok = m_font.loadFromMemory(data->data(), data->size());
                if (!ok) {
                    report_error("failed to load font {}", "mono.ttf");
                }
            } else {
                report_error(res_maybe.value().get().validation_error_message());
            }
        } else {
            report_error(res_maybe.message());
        }

        m_text = sf::Text(m_name, m_font, 10);
    }

    ~ClientComponent() {
        UpdatePacket disc_packet;
        disc_packet.name = m_name;
        disc_packet.type = UpdatePacket::Disconnect;
        send_packet(disc_packet);
    }

    void key_released(HID::Key key) {
        switch (key) {
        case HID::Key::W:
            m_dir_pressed[0] = false;
            break;
        case HID::Key::A:
            m_dir_pressed[1] = false;
            break;
        case HID::Key::S:
            m_dir_pressed[2] = false;
            break;
        case HID::Key::D:
            m_dir_pressed[3] = false;
            break;
        default:
            // nothing
            break;
        }
    }

    void key_pressed(HID::Key key) {
        switch (key) {
        case HID::Key::W:
            m_dir_pressed[0] = true;
            break;
        case HID::Key::A:
            m_dir_pressed[1] = true;
            break;
        case HID::Key::S:
            m_dir_pressed[2] = true;
            break;
        case HID::Key::D:
            m_dir_pressed[3] = true;
            break;
        default:
            // nothing
            break;
        }
    }

    std::vector<Player>::iterator find_player_with_name(const std::string& name) {
        return std::find_if(m_other_players.begin(), m_other_players.end(),
            [&](auto& item) -> bool {
                return name == item.name;
            });
    }

    void update_other_players_from_server() {
        // whatever
        auto                      raw_packets = m_backend.receive();
        std::vector<UpdatePacket> packets;
        packets.reserve(raw_packets.size());
        for (auto& str : raw_packets) {
            if (str.empty()) {
                report_error("received empty packet from server (bad!)");
                continue;
            }
            // FIXME: this might throw, handle please!
            packets.push_back(deserialize_from_string<UpdatePacket>(str));
        }

        if (packets.size() != 0) {
            // we know that stuff MUST have changed, so we can already tell the drawable to redraw later
            m_simple_drawable.set_changed();
            for (auto& packet : packets) {
                if (packet.name == m_name)
                    continue;
                auto player_iter = find_player_with_name(packet.name);
                if (packet.type == UpdatePacket::Disconnect) {
                    // someone's disconnecting gracefully, apparently!
                    // TODO have some console or something that says who connects/disconnects
                    report("player {} disconnected gracefully", packet.name);
                    m_other_players.erase(player_iter);
                    continue;
                }
                // find player with packet.name as name
                if (player_iter != m_other_players.end()) {
                    player_iter->position = vecd(packet.x, packet.y);
                } else {
                    m_other_players.push_back(Player { packet.name, vecd(packet.x, packet.y) });
                }
            }
        }
    }

    const std::string& name() const { return m_name; }

    void send_packet(const UpdatePacket& packet) {
        auto array = serialize_into_array<UpdatePacket, PACKET_SIZE>(packet);
        m_backend.send(array);
        report("sent _{}_", std::string(array.begin(), array.end()));
    }

    virtual void on_update() override {
        vecd vel(0, 0);
        if (m_dir_pressed[0]) {
            vel.y -= 1;
        }
        if (m_dir_pressed[1]) {
            vel.x -= 1;
        }
        if (m_dir_pressed[2]) {
            vel.y += 1;
        }
        if (m_dir_pressed[3]) {
            vel.x += 1;
        }
        if (vel.length() != 0) {
            vel.normalize();
            m_update_to_server = true;
        }
        m_player_position += vel * m_speed;
        
        if (m_update_to_server) {
            m_update_to_server = false;
            UpdatePacket packet;
            packet.name = m_name;
            packet.x    = m_player_position.x;
            packet.y    = m_player_position.y;
            send_packet(packet);
            m_simple_drawable.set_changed();
        }

        update_other_players_from_server();

        if (m_simple_drawable.has_changed()) {
            // rebuild all
            // if we got any new players
            if (m_simple_drawable.size() != m_other_players.size() + 1) {
                // resize to fit
                m_simple_drawable.resize(m_other_players.size() + 1);
            }
            // player first
            m_simple_drawable[0] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(m_player_position.x, m_player_position.y), SimpleDrawable::Color::Green);
            // now all others
            for (size_t i = 1; i < m_simple_drawable.size(); ++i) {
                auto pos             = m_other_players.at(i - 1).position;
                m_simple_drawable[i] = SimpleDrawable::Vertex(SimpleDrawable::Vector2f(pos.x, pos.y), SimpleDrawable::Color::Red);
            }
        }
    }

    virtual void on_draw(DrawSurface& surface) override {
        sf::Vector2f real_position(m_player_position.x, m_player_position.y);
        sf::Vector2i screen_pos = surface.window().mapCoordsToPixel(real_position);
        m_text.setPosition(screen_pos.x, screen_pos.y);
        surface.draw_text(m_text);
        m_simple_drawable.draw(surface);
    }
};

int main(int argc, char** argv) {
    Application app("MPGAME Client", { 512, 512 });
    auto&       world = app.world();

    { // scope for shared_ptr lock
        auto player = world.add_entity({ 256, 256 }).lock();
        ASSERT(argc == 2);
        player->add_component<ClientComponent>("localhost", 26999, argv[1]);
    }

    return app.run();
    /*
    Client client { "localhost", 26999, argv[1] };
    report("client running");
    return client.run();
    */
}

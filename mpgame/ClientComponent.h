#ifndef CLIENTCOMPONENT_H
#define CLIENTCOMPONENT_H

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Core/World.h"
#include "Core/Object.h"
#include "Network/UDPClient.h"
#include "mpgame-common.h"

struct PlayerInfo {
    std::string name;
    vecd position;

    bool operator==(const PlayerInfo& player) {
        return name == player.name;
    }
};

class ClientComponent : public Component
{
    OBJNAME(ClientComponent)
private:
    UDPClient m_backend;
    std::string m_name;
    double m_speed { 0.7 };
    bool m_update_to_server { true };
    sf::Font m_font;

    void update_other_players_from_server();
    void handle_new_player_connected(const UpdatePacket& packet);
    void handle_player_disconnected(const UpdatePacket& packet);

public:
    ClientComponent(Entity& e, const std::string& address, std::uint16_t port, const std::string& name);
    ~ClientComponent();

    void send_packet(const UpdatePacket& packet);
    sf::Font& font() { return m_font; }

    const std::string& name() const { return m_name; }
    virtual void on_update(float) override;
    virtual void on_draw(DrawSurface& surface) override;

    std::vector<Entity*>::iterator find_player_with_name(const std::string& name);
};

#endif // CLIENTCOMPONENT_H

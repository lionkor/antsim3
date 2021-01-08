#include "ClientComponent.h"
#include "PlayerComponent.h"

ClientComponent::ClientComponent(Entity& e, const std::string& address, uint16_t port, const std::string& name)
    : Component(e)
    , m_io_service()
    , m_backend(m_io_service, address, port)
    , m_name(name) {

    auto& resman = parent().world().application().resource_manager();
    auto res_maybe = resman.get_resource_by_name("mono.ttf");
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

    // add playable player
    { // shared_ptr lock scope
        auto playable = parent().add_child().lock();
        auto& comp = playable->add_component<PlayerComponent>(name);
        comp.set_is_player_controlled(true);
    }
}

ClientComponent::~ClientComponent() {
    UpdatePacket disc_packet;
    disc_packet.name = m_name;
    disc_packet.type = UpdatePacket::Disconnect;
    send_packet(disc_packet);
}

std::vector<Entity*>::iterator ClientComponent::find_player_with_name(const std::string& name) {
    return std::find_if(parent().children().begin(), parent().children().end(),
        [&](auto* item) -> bool {
            auto* cmp = item->template fetch_component<PlayerComponent>();
            if (cmp) {
                return name == cmp->name();
            }
        });
}

void ClientComponent::update_other_players_from_server() {
    // whatever
    auto raw_packets = m_backend.receive();
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
        for (auto& packet : packets) {
            if (packet.name == m_name)
                continue;
            // find player with packet.name as name
            auto player_iter = find_player_with_name(packet.name);
            if (packet.type == UpdatePacket::Disconnect) {
                // someone's disconnecting gracefully, apparently!
                // TODO have some console or something that says who connects/disconnects
                report("player {} disconnected gracefully", packet.name);
                // m_other_players.erase(player_iter);
                if (player_iter != parent().children().end()) {
                    (*player_iter)->destroy();
                }
                continue;
            }
            if (player_iter != parent().children().end()) {
                (*player_iter)->transform().set_position(vecd(packet.x, packet.y));

            } else {
                report("got a new connection!");
                auto player = parent().add_child().lock();
                player->transform().set_position(vecd(packet.x, packet.y));
                player->add_component<PlayerComponent>(packet.name);
            }
        }
    }
}

void ClientComponent::send_packet(const UpdatePacket& packet) {
    auto array = serialize_into_array<UpdatePacket, PACKET_SIZE>(packet);
    m_backend.send(array);
    // report("sent _{}_", std::string(array.begin(), array.end()));
}

void ClientComponent::on_update() {
    update_other_players_from_server();
}

void ClientComponent::on_draw(DrawSurface& surface) {
}

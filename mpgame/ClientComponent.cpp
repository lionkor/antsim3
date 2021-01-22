#include "ClientComponent.h"
#include "Core/Application.h"
#include "PlayerComponent.h"

ClientComponent::ClientComponent(Entity& e, const std::string& address, uint16_t port, const std::string& name)
    : Component(e)
    , m_backend(address, std::to_string(port))
    , m_name(name) {
    /*auto& resman = resource_manager();
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

    */
    // add playable player
    { // shared_ptr lock scope
        auto playable = parent().add_child().lock();
        auto& comp = playable->add_component<PlayerComponent>(name);
        comp.set_is_player_controlled(true);
    }
    UpdatePacket conn_packet;
    conn_packet.name = m_name;
    conn_packet.type = UpdatePacket::Connect;
    send_packet(conn_packet);
    m_update_thread = std::thread([this] {
        while (!parent().is_marked_destroyed()) {
            update_other_players_from_server();
        }
    });
}

ClientComponent::~ClientComponent() {
    UpdatePacket disc_packet;
    disc_packet.name = m_name;
    disc_packet.type = UpdatePacket::Disconnect;
    send_packet(disc_packet);
    // TODO: is this ok?!
    m_update_thread.detach();
}

std::vector<Entity*>::iterator ClientComponent::find_player_with_name(const std::string& name) {
    return std::find_if(parent().children().begin(), parent().children().end(),
        [&](auto* item) -> bool {
            auto* cmp = item->template fetch_component<PlayerComponent>();
            if (cmp) {
                return name == cmp->name();
            }
            return false;
        });
}

void ClientComponent::update_other_players_from_server() {
    // whatever
    auto raw_packet = m_backend.recv();
    if (std::all_of(raw_packet.begin(), raw_packet.end(), [](char c) { return c == 0; })) {
        return;
    } else {
        //report("got _{}_", raw_packet);
    }
    auto packet = UpdatePacket::from_binary(raw_packet);

    if (packet.name == m_name) {
        return;
    }

    //std::scoped_lock lock(m_update_mutex);
    // find player with packet.name as name
    auto player_iter = find_player_with_name(packet.name);
    if (packet.type == UpdatePacket::Disconnect) {
        report("player {} disconnected gracefully", packet.name);
        if (player_iter != parent().children().end()) {
            (*player_iter)->destroy();
        }
    } else if (packet.type == UpdatePacket::Connect) {
        report("got a new connection!");
        auto player = parent().add_child().lock();
        player->transform().set_position(vecd(packet.x, packet.y));
        (void)player->add_component<PlayerComponent>(packet.name);
    } else if (packet.type == UpdatePacket::Update) {
        if (player_iter != parent().children().end()) {
            (*player_iter)->transform().set_position(vecd(packet.x, packet.y));
        } else {
            report_warning("got update packet from player that doesn't exist, adding player");
            //auto player = parent().add_child().lock();
            //player->transform().set_position(vecd(packet.x, packet.y));
            //(void)player->add_component<PlayerComponent>(packet.name);
        }
    } else if (packet.type == UpdatePacket::Heartbeat && player_iter == parent().children().end()) {
        // heartbeat from unknown player, must have missed something!
        report("got heartbeat from unknown player, must have missed something... adding that player now");
        auto player = parent().add_child().lock();
        player->transform().set_position(vecd(packet.x, packet.y));
        (void)player->add_component<PlayerComponent>(packet.name);
    }
}

void ClientComponent::send_packet(const UpdatePacket& packet) {
    auto raw = packet.to_binary();
    m_backend.send(std::move(raw));
}

void ClientComponent::on_update(float) {
}

void ClientComponent::on_draw(DrawSurface& surface) {
    (void)surface;
}

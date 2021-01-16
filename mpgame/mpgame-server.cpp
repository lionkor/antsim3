#include <unordered_set>
#include <string>
#include <cstddef>

#include "Physics/vec.h"
#include "Utils/DebugTools.h"

#include "Network/UDPServer.h"

#include "mpgame-common.h"

int main() {
    std::unordered_set<ServerSideUDPClient> clients;
    UDPServer server(26999);
    server.on_receive = [&](ServerSideUDPClient client, SharedPtr<std::array<char, 128>> data, size_t) -> std::optional<std::array<char, 128>> {
        UpdatePacket packet = UpdatePacket::from_binary(*data);
        ASSERT(packet.name[0] != 0);
        if (packet.type == UpdatePacket::Connect) {
            report_warning("added client!");
            clients.insert(client);
        } else if (packet.type == UpdatePacket::Disconnect) {
            report_warning("removed client!");
            clients.erase(client);
        } else {
            report_warning("got packet of type {} from \"{}\"", int(packet.type), packet.name);
        }
        auto broadcast = make_shared<std::array<char, 128>>(packet.to_binary());
        for (auto& target_client : clients) {
            if (target_client != client) {
                server.send_to(data, 128, target_client);
            }
        }
        return std::nullopt;
    };
    report("server running");
    server.run();
}

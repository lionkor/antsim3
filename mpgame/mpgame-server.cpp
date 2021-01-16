#include <unordered_set>
#include <string>
#include <algorithm>
#include <cstddef>

#include "Physics/vec.h"
#include "Utils/DebugTools.h"

#include "Network/UDPServer.h"

#include "mpgame-common.h"

int main() {
    std::mutex pending_packets_mutex;
    std::deque<std::pair<SharedPtr<std::array<char, 128>>, ServerSideUDPClient>> pending_packets;
    std::mutex clients_mutex;
    std::unordered_set<ServerSideUDPClient> clients;
    UDPServer server(26999);

    auto processing_function = [&] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            pending_packets_mutex.lock();
            if (pending_packets.empty()) {
                pending_packets_mutex.unlock();
                continue;
            }
            auto data = pending_packets.front();
            pending_packets.pop_front();
            pending_packets_mutex.unlock();
            // if its all zeroes
            if (!data.first || std::all_of(data.first->begin(), data.first->end(), [&](char c) { return c == 0; })) {
                report_warning("empty!");
                continue;
            }
            UpdatePacket packet = UpdatePacket::from_binary(*data.first);
            ASSERT(packet.name[0] != 0);
            if (packet.type == UpdatePacket::Connect) {
                report_warning("added client!");
                clients_mutex.lock();
                clients.insert(data.second);
                clients_mutex.unlock();
            } else if (packet.type == UpdatePacket::Disconnect) {
                report_warning("removed client!");
                clients_mutex.lock();
                clients.erase(data.second);
                clients_mutex.unlock();
            } else {
                report_warning("got packet of type {} from \"{}\"", int(packet.type), packet.name);
            }
        }
    };

    std::thread packet_processing_1(processing_function);
    packet_processing_1.detach();
    std::thread packet_processing_2(processing_function);
    packet_processing_2.detach();
    std::thread packet_processing_3(processing_function);
    packet_processing_3.detach();

    server.on_receive = [&](ServerSideUDPClient client, SharedPtr<std::array<char, 128>> data, size_t) -> std::optional<std::array<char, 128>> {
        pending_packets_mutex.lock();
        clients_mutex.lock();
        pending_packets.push_back({ data, client });
        for (auto& target_client : clients) {
            if (target_client != client) {
                server.send_to(data, 128, target_client);
            }
        }
        pending_packets_mutex.unlock();
        clients_mutex.unlock();
        return std::nullopt;
    };
    report("server running");
    server.run();
}

#include <vector>
#include <atomic>
#include <string>
#include <map>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Physics/vec.h"
#include "Utils/DebugTools.h"

#include "mpgame-common.h"

static inline void report_errno() {
    report_error("an error occurred: {}", strerror(errno));
}

static std::atomic_size_t s_id_counter = 0;

static size_t new_id() {
    return ++s_id_counter;
}

class Server
{
private:
    std::map<std::string, vec<float>>         m_players;
    std::map<std::string, struct sockaddr_in> m_clients;
    bool                                      m_running { true };
    bool                                      m_ok { false };
    int                                       m_socket_fd;

public:
    Server(std::uint16_t port) {
        m_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_socket_fd == -1) {
            report_errno();
            return;
        }

        struct sockaddr_in addr;
        addr.sin_family      = AF_INET; // IPv4
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port        = htons(port);

        int ret = bind(m_socket_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

        if (ret != 0) {
            report_errno();
            return;
        }

        // all good
        m_ok = true;
    }

    int run() {
        if (!m_ok) {
            report_error("not initialized, see errors above");
            return -1;
        }
        while (m_running) {
            std::string data;
            data.resize(UpdatePacket::network_size());
            struct sockaddr_in client_addr;
            socklen_t          addr_len;
            int                ret = recvfrom(m_socket_fd, data.data(), UpdatePacket::network_size(), 0, reinterpret_cast<struct sockaddr*>(&client_addr), &addr_len);
            /*if (static_cast<size_t>(ret) != UpdatePacket::network_size()) {
                report_error("invalid packet received: _{}_", data);
                report_warning("ignoring potentially fatal problems here");
                continue;
            }*/
            UpdatePacket packet = UpdatePacket::from_network_data(data);
            report("got packet: name = \"{}\", x = {}, y = {}", packet.name, packet.x, packet.y);

            if (m_players.find(packet.name) != m_players.end()) {
                // new player
                m_players.insert_or_assign(packet.name, vec<float>(packet.x, packet.y));
                report("inserted new player with name \"{}\" at x,y = {},{}", packet.name, packet.x, packet.y);
            } else {
                // update player
                vec<float>& pos = m_players.at(packet.name);
                pos.x           = packet.x;
                pos.y           = packet.y;
                report("updated player \"{}\" at x,y = {},{}", packet.name, packet.x, packet.y);
            }

            if (m_clients.find(packet.name) != m_clients.end()) {
                m_clients.insert_or_assign(packet.name, client_addr);
            }

            for (auto& client_address : m_clients) {
                sendto(m_socket_fd, data.data(), data.size(), 0, reinterpret_cast<struct sockaddr*>(&client_address.second), addr_len);
            }
        }
        return 0;
    }
};

int main() {
    UpdatePacket packet { "James", 2542334.6, -1115.9 };
    report("before: name = _{}_, x = _{}_, y = _{}_", packet.name, packet.x, packet.y);
    report("network data: _{}_", packet.to_network_data());
    packet = UpdatePacket::from_network_data(packet.to_network_data());
    report("after: name = _{}_, x = _{}_, y = _{}_", packet.name, packet.x, packet.y);
    Server server { 26999 };
    return server.run();
}

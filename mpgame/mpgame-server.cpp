// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include <vector>
#include <atomic>
#include <string>
#include <map>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "Physics/vec.h"
#include "Utils/DebugTools.h"

#include "mpgame-common.h"

class UDPServer
{
private:
    // TODO
public:
};

class Server
{
private:
    std::map<std::string, vec<double>> m_players;
    std::map<std::string, struct sockaddr_in> m_clients;
    bool m_running { true };
    bool m_ok { false };
    int m_socket_fd;

public:
    int socket_fd() { return m_socket_fd; }

    Server(std::uint16_t port) {
        m_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_socket_fd == -1) {
            report_errno();
            return;
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET; // IPv4
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        int ret = bind(m_socket_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

        if (ret != 0) {
            report_errno();
            return;
        }

        // all good
        m_ok = true;
    }

    ~Server() {
        close(m_socket_fd);
    }

    int run() {
        if (!m_ok) {
            report_error("not initialized, see errors above");
            return -1;
        }

        UpdatePacket packet { "John", 24.5, 22.1 };

        auto array = serialize_into_array<UpdatePacket, 128>(packet);
        report("array size: {}", array.size());
        std::string data_string(array.begin(), array.end());
        report("string size: {}", data_string.size());
        UpdatePacket other_packet = deserialize_from_string<UpdatePacket>(data_string);

        ASSERT(packet.name == other_packet.name);
        ASSERT(packet.x == other_packet.x);
        ASSERT(packet.y == other_packet.y);

        while (m_running) {
            std::string data;
            data.resize(PACKET_SIZE, ' '); // spaces as padding
            struct sockaddr_in client_addr;
            socklen_t addr_len;
            int ret = recvfrom(m_socket_fd, data.data(), PACKET_SIZE, 0, reinterpret_cast<struct sockaddr*>(&client_addr), &addr_len);
            static_cast<void>(ret);
            if (data.empty()) {
                report_error("empty packet received!");
                continue;
            }
            UpdatePacket packet = deserialize_from_string<UpdatePacket>(data);

            if (m_players.find(packet.name) == m_players.end()) {
                // new player
                m_players.insert_or_assign(packet.name, vec<double>(packet.x, packet.y));
                report("inserted new player with name \"{}\" at x,y = {},{}", packet.name, packet.x, packet.y);
            } else {
                // update player
                vec<double>& pos = m_players.at(packet.name);
                pos.x = packet.x;
                pos.y = packet.y;
            }

            m_clients.insert_or_assign(packet.name, client_addr);

            for (auto& client : m_clients) {
                if (client.first == packet.name) {
                    continue;
                }
                sendto(m_socket_fd, data.data(), data.size(), 0, reinterpret_cast<struct sockaddr*>(&client.second), addr_len);
            }
            if (std::filesystem::exists("kill_server")) {
                std::filesystem::remove("kill_server");
                report("killed via kill_server");
                m_running = false;
            }
        }
        return 0;
    }
};

static inline Server g_server(26999);

int main() {
    report("server running");
    return g_server.run();
}

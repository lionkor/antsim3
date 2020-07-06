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

class UDPServer
{
private:
    // TODO
public:
};

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
            socklen_t          addr_len;
            int                ret = recvfrom(m_socket_fd, data.data(), PACKET_SIZE, 0, reinterpret_cast<struct sockaddr*>(&client_addr), &addr_len);
            static_cast<void>(ret);
            report("got raw packet: _{}_", data);
            if (data.empty()) {
                report_error("empty packet received!");
                continue;
            }
            UpdatePacket packet = deserialize_from_string<UpdatePacket>(data);
            report("got packet: name = \"{}\", x = {}, y = {}", packet.name, packet.x, packet.y);

            if (m_players.find(packet.name) == m_players.end()) {
                // new player
                m_players.insert_or_assign(packet.name, vec<double>(packet.x, packet.y));
                report("inserted new player with name \"{}\" at x,y = {},{}", packet.name, packet.x, packet.y);
            } else {
                // update player
                vec<float>& pos = m_players.at(packet.name);
                pos.x           = packet.x;
                pos.y           = packet.y;
                report("updated player \"{}\" at x,y = {},{}", packet.name, packet.x, packet.y);
            }

            if (m_clients.find(packet.name) != m_clients.end()) {
            }
            m_clients.insert_or_assign(packet.name, client_addr);

            for (auto& client : m_clients) {
                sendto(m_socket_fd, data.data(), data.size(), 0, reinterpret_cast<struct sockaddr*>(&client.second), addr_len);
                if (client.first != packet.name) {
                }
            }
        }
        return 0;
    }
};

int main() {
    Server server { 26999 };
    report("server running");
    return server.run();
}

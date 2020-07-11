// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "mpgame-common.h"

class UDPClient
{
private:
    boost::asio::io_service& m_io_service;
    boost_ip::udp::socket m_socket;
    boost_ip::udp::endpoint m_endpoint;

public:
    UDPClient(boost::asio::io_service& io_service, const std::string& host, std::uint16_t port);

    ~UDPClient();

    template<size_t size>
    void send(const std::array<std::uint8_t, size>& msg) {
        m_socket.send_to(boost::asio::buffer(msg, size), m_endpoint);
    }

    [[nodiscard]] std::vector<std::string> receive();
};

class Client
{
private:
    boost::asio::io_service m_io_service;
    UDPClient m_backend;
    bool m_is_running { true };
    std::string m_name;
    vec<double> m_position;

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

#endif // UDPCLIENT_H

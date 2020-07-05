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
};

int main(int argc, char** argv) {
    ASSERT(argc == 2);
    Client client { "localhost", 26999, argv[1] };
    report("client running");
    return client.run();
}

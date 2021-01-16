#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "Utils/DebugTools.h"
#include "Utils/Managed.h"
#include "Core/Object.h"

#include <array>
#include <boost/asio.hpp>
#include <functional>
#include <optional>
#include <memory>

using boost::asio::ip::udp;

static constexpr size_t s_max_message_size = 128;

// server-side representation of a client on UDP
struct ServerSideUDPClient {
    SharedPtr<udp::endpoint> endpoint;
    ServerSideUDPClient(udp::endpoint*&& ep = nullptr) {
        ASSERT(ep);
        endpoint = SharedPtr<udp::endpoint>(ep);
    }
    bool operator==(const ServerSideUDPClient& c) const {
        return *endpoint == *c.endpoint;
    }
    bool operator!=(const ServerSideUDPClient& c) const {
        return !(*this == c);
    }
};

namespace std {
template<>
struct hash<ServerSideUDPClient> {
    std::size_t operator()(ServerSideUDPClient s) const noexcept {
        return reinterpret_cast<size_t>(s.endpoint.get());
    }
};
}

class UDPServer : public Object
{
    OBJNAME(UDPServer)
private:
    boost::asio::io_context m_io;
    udp::socket m_socket;
    std::vector<ServerSideUDPClient> m_clients;

    void insert_client(const ServerSideUDPClient& client);

    void start_receive();
    void handle_receive(SharedPtr<std::array<char, s_max_message_size>>,
        ServerSideUDPClient,
        const boost::system::error_code& error,
        size_t /*bytes_transferred*/);
    void handle_send(SharedPtr<std::array<char, s_max_message_size>>,
        const boost::system::error_code&,
        size_t);

public:
    UDPServer(uint16_t port);

    [[deprecated]] void send_to_all(SharedPtr<std::array<char, s_max_message_size>> data, size_t size);
    [[deprecated]] void send_to_all(SharedPtr<std::array<char, s_max_message_size>> data, size_t size, ServerSideUDPClient ignore);
    void send_to(SharedPtr<std::array<char, s_max_message_size>> data, size_t size, ServerSideUDPClient target);

    std::function<std::optional<std::array<char, s_max_message_size>>(ServerSideUDPClient, SharedPtr<std::array<char, s_max_message_size>>, size_t)> on_receive { nullptr };
    std::function<std::optional<std::string>(ServerSideUDPClient, const std::string&, size_t)> simple_on_receive { nullptr };

    void run();
};

#endif // UDPSERVER_H

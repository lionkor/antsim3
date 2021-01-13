#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "Utils/DebugTools.h"
#include "Utils/Managed.h"

#include <array>
#include <boost/asio.hpp>
#include <functional>
#include <optional>
#include <memory>

using boost::asio::ip::udp;

static constexpr size_t s_max_message_size = 1024;

struct UDPClient {
    SharedPtr<udp::endpoint> endpoint;
    UDPClient(udp::endpoint*&& ep) {
        ASSERT(ep);
        endpoint = SharedPtr<udp::endpoint>(ep);
    }
    bool operator==(const UDPClient& c) const {
        return *endpoint == *c.endpoint;
    }
    bool operator!=(const UDPClient& c) const {
        return !(*this == c);
    }
};

class UDPServer
{
private:
    udp::socket m_socket;
    std::vector<UDPClient> m_clients;
    std::atomic_size_t m_pps;

    void insert_client(const UDPClient& client);

    void start_receive();
    void handle_receive(SharedPtr<std::array<char, s_max_message_size>>,
        UDPClient,
        const boost::system::error_code& error,
        size_t /*bytes_transferred*/);
    void handle_send(SharedPtr<std::array<char, s_max_message_size>>,
        const boost::system::error_code&,
        size_t);

public:
    UDPServer(boost::asio::io_context& io, uint16_t port);

    void send(SharedPtr<std::array<char, s_max_message_size>> data, size_t size);
    void send(SharedPtr<std::array<char, s_max_message_size>> data, size_t size, UDPClient ignore);

    std::function<std::optional<std::array<char, s_max_message_size>>(UDPClient, SharedPtr<std::array<char, s_max_message_size>>, size_t)> on_receive { nullptr };
    std::function<std::optional<std::string>(UDPClient, const std::string&, size_t)> simple_on_receive { nullptr };
};

#endif // UDPSERVER_H

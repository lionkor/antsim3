#include "UDPClient.h"
#include "UDPServer.h"

#include "Utils/Managed.h"

UDPClient::UDPClient(const std::string& host, const std::string& port)
    : m_socket(m_io) {
    udp::resolver resolver(m_io);
    m_recv_endpoint = *resolver.resolve(udp::v4(), host, port).begin();
    m_socket.open(udp::v4());
    m_thread = std::thread([&]() { m_io.run(); });
    report("client online!");
}

UDPClient::~UDPClient() {
    m_socket.close();
    m_thread.join();
}

void UDPClient::send(std::array<char, 128>&& message) {
    m_socket.send_to(boost::asio::buffer(message), m_recv_endpoint);
}

std::array<char, 128> UDPClient::recv() {
    if (m_socket.available() > 0) {
        SharedPtr<std::array<char, s_max_message_size>> buf = make_shared<std::array<char, s_max_message_size>>();
        udp::endpoint sender_endpoint;
        m_socket.receive_from(boost::asio::buffer(*buf), sender_endpoint);
        return *buf;
    } else {
        return {};
    }
}

#include "UDPServer.h"

#include <boost/bind/bind.hpp>
#include <iostream>
#include <string>

void UDPServer::insert_client(const UDPClient& client) {
    auto iter = std::find(m_clients.begin(), m_clients.end(), client);
    if (iter == m_clients.end()) {
        m_clients.push_back(client);
    }
}

void UDPServer::start_receive() {
    SharedPtr<std::array<char, s_max_message_size>> ptr(new std::array<char, s_max_message_size> { 0 });
    UDPClient client(new udp::endpoint);
    m_socket.async_receive_from(boost::asio::buffer(*ptr),
        *client.endpoint,
        boost::bind(&UDPServer::handle_receive,
            this,
            ptr,
            client,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handle_send(SharedPtr<std::array<char, s_max_message_size>> msg,
    const boost::system::error_code& error,
    size_t) {
    ASSERT(!error);
    report("sent: _{}_", std::string(msg->data()));
}

void UDPServer::handle_receive(SharedPtr<std::array<char, s_max_message_size>> msg,
    UDPClient client,
    const boost::system::error_code& error,
    size_t size) {
    if (!error) {
        ++m_pps;
        insert_client(client);
        report("got msg from {}, {}", client.endpoint->address(), client.endpoint->port());
        SharedPtr<std::array<char, s_max_message_size>> message(new std::array<char, s_max_message_size> { 0 });
        if (on_receive) {
            auto maybe_msg = on_receive(client, msg, size);
            if (!maybe_msg.has_value()) {
                message = nullptr;
            } else {
                *message = maybe_msg.value();
            }
        } else if (simple_on_receive) {
            msg->at(size) = 0;
            std::string str(msg->data());
            auto maybe_msg_as_str = simple_on_receive(client, str, size);
            if (!maybe_msg_as_str.has_value()) {
                message = nullptr;
            } else {
                std::string MessageAsStr = maybe_msg_as_str.value();
                std::copy(MessageAsStr.begin(), MessageAsStr.end(), message->begin());
            }
        }
        if (message) {
            m_socket.async_send_to(boost::asio::buffer(*message),
                *client.endpoint,
                boost::bind(&UDPServer::handle_send, this,
                    message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
        start_receive();
    }
}

UDPServer::UDPServer(boost::asio::io_context& io, uint16_t port)
    : m_socket(io, udp::endpoint(udp::v4(), port)) {
    start_receive();
    std::thread monitor([&] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            double New = m_pps;
            m_pps = 0;
            if (!m_clients.empty()) {
                std::cout << "PPS: " << New / m_clients.size() << " @ " << m_clients.size() << " clients" << std::endl;
            }
        }
    });
    monitor.detach();
}

void UDPServer::send(SharedPtr<std::array<char, s_max_message_size>> data, size_t) {
    report("i know of {} client(s).", m_clients.size());
    for (auto& client : m_clients) {
        m_socket.async_send_to(boost::asio::buffer(*data),
            *client.endpoint,
            boost::bind(&UDPServer::handle_send, this,
                data,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
}

void UDPServer::send(SharedPtr<std::array<char, s_max_message_size>> data, size_t, UDPClient ignore) {
    report("i know of {} client(s).", m_clients.size());
    for (auto& client : m_clients) {
        if (client == ignore) {
            continue;
        }
        m_socket.async_send_to(boost::asio::buffer(*data),
            *client.endpoint,
            boost::bind(&UDPServer::handle_send, this,
                data,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
}

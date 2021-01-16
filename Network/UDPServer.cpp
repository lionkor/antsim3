#include "UDPServer.h"

#include <boost/bind/bind.hpp>
#include <iostream>
#include <string>

void UDPServer::insert_client(const ServerSideUDPClient& client) {
    auto iter = std::find(m_clients.begin(), m_clients.end(), client);
    if (iter == m_clients.end()) {
        m_clients.push_back(client);
    }
}

void UDPServer::start_receive() {
    SharedPtr<std::array<char, s_max_message_size>> ptr(new std::array<char, s_max_message_size> { 0 });
    ServerSideUDPClient client(new udp::endpoint);
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
    // report("sent: _{}_", std::string(msg->data(), strlen(msg->data())));
}

void UDPServer::handle_receive(SharedPtr<std::array<char, s_max_message_size>> msg,
    ServerSideUDPClient client,
    const boost::system::error_code& error,
    size_t size) {
    if (!error) {
        insert_client(client);
        // report("got msg from {}, {}: _{}_", client.endpoint->address(), client.endpoint->port(), std::string(msg->data()));
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

UDPServer::UDPServer(uint16_t port)
    : m_socket(m_io, udp::endpoint(udp::v4(), port)) {
    start_receive();
}

void UDPServer::send_to_all(SharedPtr<std::array<char, s_max_message_size>> data, size_t) {
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

void UDPServer::send_to_all(SharedPtr<std::array<char, s_max_message_size>> data, size_t, ServerSideUDPClient ignore) {
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

void UDPServer::send_to(SharedPtr<std::array<char, s_max_message_size>> data, size_t, ServerSideUDPClient target) {
    m_socket.async_send_to(boost::asio::buffer(*data),
        *target.endpoint,
        boost::bind(&UDPServer::handle_send, this,
            data,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::run() {
    std::thread processing([&] { m_io.run(); });
    processing.join();
}

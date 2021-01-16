#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "Core/Object.h"
#include "Utils/DebugTools.h"

#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UDPClient : public Object
{
    OBJNAME(UDPClient)
private:
    boost::asio::io_context m_io;
    udp::socket m_socket;
    udp::endpoint m_recv_endpoint;
    std::thread m_thread;

public:
    UDPClient(const std::string& host, const std::string& port);
    ~UDPClient();

    void send(std::array<char, 128>&& message);
    std::array<char, 128> recv();
};

#endif // UDPCLIENT_H

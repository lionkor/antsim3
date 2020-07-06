#ifndef MPGAMECOMMON_H
#define MPGAMECOMMON_H

#include "Engine.h"
#include <vector>
#include <atomic>
#include <string>
#include <map>
#include <boost/asio.hpp>
#include <string>
#include <sstream>
#include "fmt/format.h"
#include <arpa/inet.h>
#include <cstdint>

#include "Utils/ISerializable.h"
#include "Utils/DebugTools.h"
#include "Physics/vec.h"

namespace boost_ip = boost::asio::ip;

static inline boost_ip::udp::endpoint resolve_endpoint(boost::asio::io_service& ios, const std::string& host, std::uint16_t port) {
    boost_ip::udp::resolver           resolver { ios };
    boost_ip::udp::resolver::query    query { boost_ip::udp::v4(), host, std::to_string(port) };
    boost_ip::udp::resolver::iterator iter = resolver.resolve(query);
    return *iter;
}

static inline void report_errno() {
    report_error("an error occurred: {}", strerror(errno));
}

static constexpr size_t PACKET_SIZE = 128;

struct UpdatePacket : public ISerializable {
    UpdatePacket() { }
    UpdatePacket(const std::string& s, double x, double y)
        : name(s), x(x), y(y) { }

    enum Type
    {
        Connect,    // "i'm new"
        Update,     // "i have new information"
        Disconnect, // "im disconnecting gracefully"
        Ping,       // "you still there?"
        Pong,       // "i'm still here"
    } type { Type::Update };

    std::string name;
    double      x;
    double      y;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar& type;
        ar& name;
        ar& x;
        ar& y;
    }
};

#endif // MPGAMECOMMON_H

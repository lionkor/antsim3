#ifndef MPGAMECOMMON_H
#define MPGAMECOMMON_H

#include "Utils/ISerializable.h"
#include "Utils/DebugTools.h"
#include "Physics/vec.h"

#include <msgpack.hpp>
#include <sstream>

struct UpdatePacket {
    enum Type : uint8_t
    {
        Connect = 0x00,    // "i'm new"
        Update = 0x01,     // "i have new information"
        Disconnect = 0x02, // "im disconnecting gracefully"
        Ping = 0x03,       // "you still there?"
        Pong = 0x04,       // "i'm still here"
        Heartbeat = 0x05,  // "i'm alive"
    } type { Type::Update };

    UpdatePacket() { }
    UpdatePacket(const std::string& s, double x, double y, Type type = Type::Update)
        : type(type), name(s), x(x), y(y) { }

    std::string name;
    double x;
    double y;

    std::array<char, 128> to_binary() const {
        msgpack::type::tuple<uint8_t, std::string, double, double> src(type, name, x, y);
        std::stringstream buffer;
        msgpack::pack(buffer, src);
        auto str = buffer.str();
        std::array<char, 128> arr;
        std::move(str.begin(), str.end(), arr.begin());
        return arr;
    }

    static UpdatePacket from_binary(const std::array<char, 128>& array) {
        std::string str(array.begin(), array.end());
        std::stringstream ss(str);
        msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
        msgpack::object deserialized = oh.get();
        msgpack::type::tuple<uint8_t, std::string, double, double> dst;
        deserialized.convert(dst);
        return UpdatePacket { dst.get<1>(), dst.get<2>(), dst.get<3>(), Type(dst.get<0>()) };
    }
};

#endif // MPGAMECOMMON_H
